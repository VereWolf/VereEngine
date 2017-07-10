//
// DirectXPage.xaml.cpp
// Implementation of the DirectXPage class.
//

#include "pch.h"
#include "MainRenderPage.xaml.h"

using namespace VereEngine;

using namespace Platform;
using namespace Windows::Foundation;
using namespace Windows::Foundation::Collections;
using namespace Windows::Graphics::Display;
using namespace Windows::System::Threading;
using namespace Windows::System;
using namespace Windows::UI::Core;
using namespace Windows::UI::Input;
using namespace Windows::UI::Xaml;
using namespace Windows::UI::Xaml::Controls;
using namespace Windows::UI::Xaml::Controls::Primitives;
using namespace Windows::UI::Xaml::Data;
using namespace Windows::UI::Xaml::Input;
using namespace Windows::UI::Xaml::Media;
using namespace Windows::UI::Xaml::Navigation;
using namespace concurrency;

DirectXPage::DirectXPage() :
	m_windowVisible(true),
	m_coreInput(nullptr),
	m_cursorPos(0.0f, 0.0f, 0.0f, 0.0f)
{
	InitializeComponent();

	// Register event handlers for page lifecycle.
	CoreWindow^ window = Window::Current->CoreWindow;

	window->VisibilityChanged +=
		ref new TypedEventHandler<CoreWindow^, VisibilityChangedEventArgs^>(this, &DirectXPage::OnVisibilityChanged);

	window->KeyDown +=
		ref new TypedEventHandler<CoreWindow^, KeyEventArgs^>(this, &DirectXPage::OnKeyDown);

	window->KeyUp +=
		ref new TypedEventHandler<CoreWindow^, KeyEventArgs^>(this, &DirectXPage::OnKeyUp);

	DisplayInformation^ currentDisplayInformation = DisplayInformation::GetForCurrentView();

	currentDisplayInformation->DpiChanged +=
		ref new TypedEventHandler<DisplayInformation^, Object^>(this, &DirectXPage::OnDpiChanged);

	currentDisplayInformation->OrientationChanged +=
		ref new TypedEventHandler<DisplayInformation^, Object^>(this, &DirectXPage::OnOrientationChanged);

	DisplayInformation::DisplayContentsInvalidated +=
		ref new TypedEventHandler<DisplayInformation^, Object^>(this, &DirectXPage::OnDisplayContentsInvalidated);

	swapChainPanel->CompositionScaleChanged +=
		ref new TypedEventHandler<SwapChainPanel^, Object^>(this, &DirectXPage::OnCompositionScaleChanged);

	swapChainPanel->SizeChanged +=
		ref new SizeChangedEventHandler(this, &DirectXPage::OnSwapChainPanelSizeChanged);

	// At this point we have access to the device. 
	// We can create the device-dependent resources.
	m_deviceResources = std::make_shared<DX::DeviceResources>();
	m_deviceResources->SetSwapChainPanel(swapChainPanel);

	// Register our SwapChainPanel to get independent input pointer events
	auto workItemHandler = ref new WorkItemHandler([this](IAsyncAction ^)
	{
		// The CoreIndependentInputSource will raise pointer events for the specified device types on whichever thread it's created on.
		m_coreInput = swapChainPanel->CreateCoreIndependentInputSource(
			Windows::UI::Core::CoreInputDeviceTypes::Mouse |
			Windows::UI::Core::CoreInputDeviceTypes::Touch |
			Windows::UI::Core::CoreInputDeviceTypes::Pen
		);

		// Register for pointer events, which will be raised on the background thread.
		m_coreInput->PointerPressed += ref new TypedEventHandler<Object^, PointerEventArgs^>(this, &DirectXPage::OnPointerPressed);
		m_coreInput->PointerMoved += ref new TypedEventHandler<Object^, PointerEventArgs^>(this, &DirectXPage::OnPointerMoved);
		m_coreInput->PointerReleased += ref new TypedEventHandler<Object^, PointerEventArgs^>(this, &DirectXPage::OnPointerReleased);
		m_coreInput->PointerWheelChanged += ref new TypedEventHandler<Object^, PointerEventArgs^>(this, &DirectXPage::OnPointerWheelScroll);

		// Begin processing input messages as they're delivered.
		m_coreInput->Dispatcher->ProcessEvents(CoreProcessEventsOption::ProcessUntilQuit);
	});

	// Run task on a dedicated high priority background thread.
	m_inputLoopWorker = ThreadPool::RunAsync(workItemHandler, WorkItemPriority::High, WorkItemOptions::TimeSliced);

	m_main = std::unique_ptr<VereEngineMain>(new VereEngineMain(m_deviceResources));
	m_main->StartRenderLoop();
	m_main->StartExpensiveLoop();
}

DirectXPage::~DirectXPage()
{
	// Stop rendering and processing events on destruction.
	m_main->StopRenderLoop();
	m_main->StopExpensiveLoop();
	m_coreInput->Dispatcher->StopProcessEvents();
}

// Saves the current state of the app for suspend and terminate events.
void DirectXPage::SaveInternalState(IPropertySet^ state)
{
	critical_section::scoped_lock lock(m_main->GetCriticalSection());
	m_deviceResources->Trim();

	// Stop rendering when the app is suspended.
	m_main->StopRenderLoop();
	m_main->StopExpensiveLoop();

	// Put code to save app state here.
}

// Loads the current state of the app for resume events.
void DirectXPage::LoadInternalState(IPropertySet^ state)
{
	// Put code to load app state here.

	// Start rendering when the app is resumed.
	m_main->StartRenderLoop();
	m_main->StartExpensiveLoop();
}

// Window event handlers.

void DirectXPage::OnVisibilityChanged(CoreWindow^ sender, VisibilityChangedEventArgs^ args)
{
	m_windowVisible = args->Visible;
	if (m_windowVisible)
	{
		m_main->StartRenderLoop();
		m_main->StartExpensiveLoop();
	}
	else
	{
		m_main->StopRenderLoop();
		m_main->StopExpensiveLoop();
	}
}

void DirectXPage::OnKeyDown(Windows::UI::Core::CoreWindow^ sender, Windows::UI::Core::KeyEventArgs^ args)
{
	btScalar S = GameObjectStackHandle->GetMainCamera()->GetSpeed();
	
	if (args->VirtualKey == VirtualKey::W)
	{
		GameObjectStackHandle->GetMainCamera()->Walk(S);
	}
	if (args->VirtualKey == VirtualKey::S)
	{
		GameObjectStackHandle->GetMainCamera()->Walk(-S);
	}
	if (args->VirtualKey == VirtualKey::A)
	{
		GameObjectStackHandle->GetMainCamera()->Strafe(-S);
	}
	if (args->VirtualKey == VirtualKey::D)
	{
		GameObjectStackHandle->GetMainCamera()->Strafe(S);
	}

}

void DirectXPage::OnKeyUp(Windows::UI::Core::CoreWindow^ sender, Windows::UI::Core::KeyEventArgs^ args)
{

}

// DisplayInformation event handlers.

void DirectXPage::OnDpiChanged(DisplayInformation^ sender, Object^ args)
{
	critical_section::scoped_lock lock(m_main->GetCriticalSection());
	// Note: The value for LogicalDpi retrieved here may not match the effective DPI of the app
	// if it is being scaled for high resolution devices. Once the DPI is set on DeviceResources,
	// you should always retrieve it using the GetDpi method.
	// See DeviceResources.cpp for more details.
	m_deviceResources->SetDpi(sender->LogicalDpi);
	m_main->CreateWindowSizeDependentResources();
}

void DirectXPage::OnOrientationChanged(DisplayInformation^ sender, Object^ args)
{
	critical_section::scoped_lock lock(m_main->GetCriticalSection());
	m_deviceResources->SetCurrentOrientation(sender->CurrentOrientation);
	m_main->CreateWindowSizeDependentResources();
}

void DirectXPage::OnDisplayContentsInvalidated(DisplayInformation^ sender, Object^ args)
{
	critical_section::scoped_lock lock(m_main->GetCriticalSection());
	m_deviceResources->ValidateDevice();
}

// Called when the app bar button is clicked.
void DirectXPage::AppBarButton_Click(Object^ sender, RoutedEventArgs^ e)
{
	// Use the app bar if it is appropriate for your app. Design the app bar, 
	// then fill in event handlers (like this one).
}

void DirectXPage::OnPointerPressed(Object^ sender, PointerEventArgs^ e)
{
	// When the pointer is pressed begin tracking the pointer movement.
	m_cursorPos.x = e->CurrentPoint->Position.X;
	m_cursorPos.y = e->CurrentPoint->Position.Y;
}

void DirectXPage::OnPointerMoved(Object^ sender, PointerEventArgs^ e)
{
	Windows::ApplicationModel::Core::CoreApplication::MainView->Dispatcher->RunAsync(CoreDispatcherPriority::Normal, ref new DispatchedHandler([=]()
	{
		// Update the pointer tracking code.
		XMFLOAT4 P;

		m_cursorPos.z = m_cursorPos.x;
		m_cursorPos.w = m_cursorPos.y;
		m_cursorPos.x = Windows::UI::Core::CoreWindow::GetForCurrentThread()->PointerPosition.X;
		m_cursorPos.y = Windows::UI::Core::CoreWindow::GetForCurrentThread()->PointerPosition.Y;

		P = m_cursorPos;

		XMFLOAT2 DP = XMFLOAT2(P.x - P.z, P.y - P.w);
		m_cursorPos.x = 0.5f * m_deviceResources->GetOutputSize().Width;
		m_cursorPos.y = 0.5f *  m_deviceResources->GetOutputSize().Height;
		m_cursorPos.z = m_cursorPos.x;
		m_cursorPos.w = m_cursorPos.y;

		Windows::UI::Core::CoreWindow::GetForCurrentThread()->PointerPosition =
			Point(m_cursorPos.x, m_cursorPos.y);

		//m_main->TrackingUpdate(XMFLOAT2(500, 500));

		float dx = XMConvertToRadians(0.25f*(DP.x));
		float dy = XMConvertToRadians(0.25f*(DP.y));

		GameObjectStackHandle->GetMainCamera()->Pitch(-dy);
		GameObjectStackHandle->GetMainCamera()->RotateY(-dx);
	}));
}
void DirectXPage::OnPointerReleased(Object^ sender, PointerEventArgs^ e)
{
	// Stop tracking pointer movement when the pointer is released.
}

void DirectXPage::OnPointerWheelScroll(Platform::Object^ sender, Windows::UI::Core::PointerEventArgs^ e)
{
	GameObjectStackHandle->GetMainCamera()->SetSpeed(GameObjectStackHandle->GetMainCamera()->GetSpeed()
		+ 0.001 * GameObjectStackHandle->GetMainCamera()->GetSpeed()*e->CurrentPoint->Properties->MouseWheelDelta);
}

void DirectXPage::OnCompositionScaleChanged(SwapChainPanel^ sender, Object^ args)
{
	critical_section::scoped_lock lock(m_main->GetCriticalSection());
	m_deviceResources->SetCompositionScale(sender->CompositionScaleX, sender->CompositionScaleY);
	m_main->CreateWindowSizeDependentResources();
}

void DirectXPage::OnSwapChainPanelSizeChanged(Object^ sender, SizeChangedEventArgs^ e)
{
	critical_section::scoped_lock lock(m_main->GetCriticalSection());
	m_deviceResources->SetLogicalSize(e->NewSize);
	m_main->CreateWindowSizeDependentResources();
}
