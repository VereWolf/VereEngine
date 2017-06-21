
#include "pch.h"
#include "MainEditorPage.xaml.h"

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

MainEditorPage::MainEditorPage() :
	m_windowVisible(true),
	m_coreInput(nullptr),
	m_cursorPos(0.0f, 0.0f, 0.0f, 0.0f)
{
	InitializeComponent();

	// Register event handlers for page lifecycle.
	CoreWindow^ window = Window::Current->CoreWindow;

	window->VisibilityChanged +=
		ref new TypedEventHandler<CoreWindow^, VisibilityChangedEventArgs^>(this, &MainEditorPage::OnVisibilityChanged);

	window->KeyDown +=
		ref new TypedEventHandler<CoreWindow^, KeyEventArgs^>(this, &MainEditorPage::OnKeyDown);

	window->KeyUp +=
		ref new TypedEventHandler<CoreWindow^, KeyEventArgs^>(this, &MainEditorPage::OnKeyUp);

	DisplayInformation^ currentDisplayInformation = DisplayInformation::GetForCurrentView();

	currentDisplayInformation->DpiChanged +=
		ref new TypedEventHandler<DisplayInformation^, Object^>(this, &MainEditorPage::OnDpiChanged);

	currentDisplayInformation->OrientationChanged +=
		ref new TypedEventHandler<DisplayInformation^, Object^>(this, &MainEditorPage::OnOrientationChanged);

	DisplayInformation::DisplayContentsInvalidated +=
		ref new TypedEventHandler<DisplayInformation^, Object^>(this, &MainEditorPage::OnDisplayContentsInvalidated);

	swapChainPanel->CompositionScaleChanged +=
		ref new TypedEventHandler<SwapChainPanel^, Object^>(this, &MainEditorPage::OnCompositionScaleChanged);

	swapChainPanel->SizeChanged +=
		ref new SizeChangedEventHandler(this, &MainEditorPage::OnSwapChainPanelSizeChanged);

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
		m_coreInput->PointerPressed += ref new TypedEventHandler<Object^, PointerEventArgs^>(this, &MainEditorPage::OnPointerPressed);
		m_coreInput->PointerMoved += ref new TypedEventHandler<Object^, PointerEventArgs^>(this, &MainEditorPage::OnPointerMoved);
		m_coreInput->PointerReleased += ref new TypedEventHandler<Object^, PointerEventArgs^>(this, &MainEditorPage::OnPointerReleased);
		m_coreInput->PointerWheelChanged += ref new TypedEventHandler<Object^, PointerEventArgs^>(this, &MainEditorPage::OnPointerWheelScroll);

		// Begin processing input messages as they're delivered.
		m_coreInput->Dispatcher->ProcessEvents(CoreProcessEventsOption::ProcessUntilQuit);
	});

	// Run task on a dedicated high priority background thread.
	m_inputLoopWorker = ThreadPool::RunAsync(workItemHandler, WorkItemPriority::High, WorkItemOptions::TimeSliced);

	m_main = std::unique_ptr<VereEngineMain>(new VereEngineMain(m_deviceResources));
	m_main->StartRenderLoop();

	m_popupmenu = ref new PopupMenuAndRename;

	m_regObjectBranch = ref new TreeViewBranch("Game_Object", "Resources/Object", m_popupmenu);

	GameObjectsTree->Items->InsertAt(0, m_regObjectBranch->GetBranch());

	/*VereEngine::MainEditorPage::CreateFolder("", "bla1");
	VereEngine::MainEditorPage::CreateFolder("", "bla2");
	VereEngine::MainEditorPage::CreateFolder("", "bla3");
	VereEngine::MainEditorPage::CreateFolder("", "bla4");
	VereEngine::MainEditorPage::CreateFolder("bla2", "bla5");
	VereEngine::MainEditorPage::CreateFolder("bla2/bla5", "bla6");
	VereEngine::MainEditorPage::CreateFolder("bla2/bla5/bla6", "bla7*/
}

MainEditorPage::~MainEditorPage()
{
	// Stop rendering and processing events on destruction.
	m_main->StopRenderLoop();
	m_coreInput->Dispatcher->StopProcessEvents();
}

// Saves the current state of the app for suspend and terminate events.
void MainEditorPage::SaveInternalState(IPropertySet^ state)
{
	critical_section::scoped_lock lock(m_main->GetCriticalSection());
	m_deviceResources->Trim();

	// Stop rendering when the app is suspended.
	m_main->StopRenderLoop();

	// Put code to save app state here.
}

// Loads the current state of the app for resume events.
void MainEditorPage::LoadInternalState(IPropertySet^ state)
{
	// Put code to load app state here.

	// Start rendering when the app is resumed.
	m_main->StartRenderLoop();
}

// Window event handlers.

void MainEditorPage::OnVisibilityChanged(CoreWindow^ sender, VisibilityChangedEventArgs^ args)
{
	m_windowVisible = args->Visible;
	if (m_windowVisible)
	{
		m_main->StartRenderLoop();
	}
	else
	{
		m_main->StopRenderLoop();
	}
}

void MainEditorPage::OnKeyDown(Windows::UI::Core::CoreWindow^ sender, Windows::UI::Core::KeyEventArgs^ args)
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

void MainEditorPage::OnKeyUp(Windows::UI::Core::CoreWindow^ sender, Windows::UI::Core::KeyEventArgs^ args)
{

}

// DisplayInformation event handlers.

void MainEditorPage::OnDpiChanged(DisplayInformation^ sender, Object^ args)
{
	critical_section::scoped_lock lock(m_main->GetCriticalSection());
	// Note: The value for LogicalDpi retrieved here may not match the effective DPI of the app
	// if it is being scaled for high resolution devices. Once the DPI is set on DeviceResources,
	// you should always retrieve it using the GetDpi method.
	// See DeviceResources.cpp for more details.
	m_deviceResources->SetDpi(sender->LogicalDpi);
	m_main->CreateWindowSizeDependentResources();
}

void MainEditorPage::OnOrientationChanged(DisplayInformation^ sender, Object^ args)
{
	critical_section::scoped_lock lock(m_main->GetCriticalSection());
	m_deviceResources->SetCurrentOrientation(sender->CurrentOrientation);
	m_main->CreateWindowSizeDependentResources();
}

void MainEditorPage::OnDisplayContentsInvalidated(DisplayInformation^ sender, Object^ args)
{
	critical_section::scoped_lock lock(m_main->GetCriticalSection());
	m_deviceResources->ValidateDevice();
}

// Called when the app bar button is clicked.
void MainEditorPage::AppBarButton_Click(Object^ sender, RoutedEventArgs^ e)
{
	// Use the app bar if it is appropriate for your app. Design the app bar, 
	// then fill in event handlers (like this one).
}

void MainEditorPage::OnPointerPressed(Object^ sender, PointerEventArgs^ e)
{
	// When the pointer is pressed begin tracking the pointer movement.
	
	m_cursorPos.x = e->CurrentPoint->Position.X;
	m_cursorPos.y = e->CurrentPoint->Position.Y;

	//m_popupmenu->NewPopupMenu(nullptr);
}

void MainEditorPage::OnPointerMoved(Object^ sender, PointerEventArgs^ e)
{
	// Update the pointer tracking code.
	XMFLOAT4 P;
	m_cursorPos.z = m_cursorPos.x;
	m_cursorPos.w = m_cursorPos.y;
	m_cursorPos.x = e->CurrentPoint->Position.X;
	m_cursorPos.y = e->CurrentPoint->Position.Y;
	P = m_cursorPos;
	//m_main->TrackingUpdate(XMFLOAT2(P.x, P.y));

	float dx = XMConvertToRadians(0.25f*(P.x - P.z));
	float dy = XMConvertToRadians(0.25f*(P.y - P.w));

	GameObjectStackHandle->GetMainCamera()->Pitch(-dy);
	GameObjectStackHandle->GetMainCamera()->RotateY(-dx);
}

void MainEditorPage::OnPointerReleased(Object^ sender, PointerEventArgs^ e)
{
	// Stop tracking pointer movement when the pointer is released.
}

void MainEditorPage::OnPointerWheelScroll(Platform::Object^ sender, Windows::UI::Core::PointerEventArgs^ e)
{
	GameObjectStackHandle->GetMainCamera()->SetSpeed(GameObjectStackHandle->GetMainCamera()->GetSpeed()
		+ 0.001 * GameObjectStackHandle->GetMainCamera()->GetSpeed()*e->CurrentPoint->Properties->MouseWheelDelta);
}

void MainEditorPage::OnCompositionScaleChanged(SwapChainPanel^ sender, Object^ args)
{
	critical_section::scoped_lock lock(m_main->GetCriticalSection());
	m_deviceResources->SetCompositionScale(sender->CompositionScaleX, sender->CompositionScaleY);
	m_main->CreateWindowSizeDependentResources();
}

void MainEditorPage::OnSwapChainPanelSizeChanged(Object^ sender, SizeChangedEventArgs^ e)
{
	critical_section::scoped_lock lock(m_main->GetCriticalSection());
	m_deviceResources->SetLogicalSize(e->NewSize);
	m_main->CreateWindowSizeDependentResources();
}

void MainEditorPage::OnGameObjectDrop(Platform::Object^ sender, Windows::UI::Xaml::DragEventArgs^ e)
{
	int a = 0;
}

void MainEditorPage::OnGameObjectDragOver(Platform::Object^ sender, Windows::UI::Xaml::DragEventArgs^ e)
{
	int a = 0;
}
