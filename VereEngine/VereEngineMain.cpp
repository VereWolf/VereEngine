
#include "pch.h"
#include "VereEngineMain.h"
#include "DirectXHelper.h"
#include "RenderState.h"
#include "Effects.h"
#include "Vertex.h"
#include "GenerateMesh.h"

using namespace VereEngine;
using namespace Windows::Foundation;
using namespace Windows::System::Threading;
using namespace Windows::System;
using namespace Windows::UI::Core;
using namespace Windows::UI::Xaml;
using namespace Concurrency;

// Loads and initializes application assets when the application is loaded.
VereEngineMain::VereEngineMain(const std::shared_ptr<DX::DeviceResources>& deviceResources) :
	m_deviceResources(deviceResources), m_pointerLocation(0.0f, 0.0f)
{
	Effects::InitAll(deviceResources.get());
	InputLayouts::InitAll(deviceResources.get());
	RenderStates::InitAll(deviceResources.get());

	// Register to be notified if the Device is lost or recreated
	m_deviceResources->RegisterDeviceNotify(this);

	// TODO: Replace this with your app's content initialization.
	m_gameStreamingData = std::unique_ptr<StreamingDataManager>(new StreamingDataManager(m_deviceResources.get(), 0, 0, 1, 3, 0, 0));

	m_gameComponentsManager = std::unique_ptr<GameComponentsManager>(new GameComponentsManager(m_deviceResources.get(), 3));

	m_gameRenderDevice = std::unique_ptr<RenderDevice>(new RenderDevice(m_deviceResources.get(), 1.0f));

	m_gameTextRenderDevice = std::unique_ptr<TextRenderDevice>(new TextRenderDevice(m_deviceResources.get()));

	m_gameObjects = std::unique_ptr<GameObjectsStackManager>(new GameObjectsStackManager(m_deviceResources.get()));

<<<<<<< HEAD
=======
	m_gamePlanetHelper = std::unique_ptr<GamePlanetHelper>(new GamePlanetHelper(m_deviceResources.get()));

>>>>>>> VereEngine-Planet
	int idC1 = GameTextRenderDeviceHandle->CreateColorBrush(D2D1::ColorF(D2D1::ColorF::Purple));

	int idF1 = GameTextRenderDeviceHandle->CreateTextFormat(
		L"Segoe UI",
		nullptr,
		DWRITE_FONT_WEIGHT_LIGHT,
		DWRITE_FONT_STYLE_NORMAL,
		DWRITE_FONT_STRETCH_NORMAL,
		16.0f,
		L"en-US");

	GamePlanetHelperHandle->CreatePlanetTile("planet/Tiles/Tile_Normal.raw", "planet/Tiles/Tile_Height.raw",
		"planet/Tiles/Tile_Normal.raw", "planet/Tiles/Tile_Height.raw",
		0.1f, 0.8f, 512, 512);
	
	std::wstring text = L" BlaBla";

	int idT1 = GameTextRenderDeviceHandle->CreateTextLayout(text, idF1, 240.0f, 50.0f);

	GameObjectStackHandle->GetMainCamera()->SetLocalPosition(btVector3(10.0, 100.0, 0.0));

	Planet *likeE = new Planet;

	int id1 = GameObjectStackHandle->CreateObjectToReg(likeE);

	TestObject *likeJ = new TestObject;

	int id2 = GameObjectStackHandle->CreateObjectToReg(likeJ);

<<<<<<< HEAD
	//Planet *planet = new Planet;

	//int id2 = GameObjectStackHandle->CreateObjectToReg(planet);

=======
>>>>>>> VereEngine-Planet
	int id1a = GameObjectStackHandle->CreateObject(id2, btVector3(0.0, 0.0, 0.0), btVector3(0.0, 0.0, 0.0), btVector3(7150000.0, 7150000.0, 7150000.0), 1000.0, btVector3(1.0, 1.0, 1.0), 715000000.0, __nullptr);


	int id2a = GameObjectStackHandle->CreateObject(id1, btVector3(0.0, 0.0, 0.0), btVector3(0.0, 0.0, 0.0), btVector3(250000.0, 250000.0, 250000.0), 1000.0, btVector3(1.0, 1.0, 1.0), 25000000.0, __nullptr);
<<<<<<< HEAD

	((Planet*)GameObjectStackHandle->GetGameObjectByID(id2a))->BuildPlanet(64, 4, 4, 13, 1, XMFLOAT3(0.0f, 0.698f, 0.894f), XMFLOAT3(0.0f, 0.506f, 0.725f), 200.0f);
	((Planet*)GameObjectStackHandle->GetGameObjectByID(id2a))->GenerateCoord(512, 512, 0);

	//GameObjectStackHandle->CreateObject(id2, btVector3(0.0, 0.0, 0.0), btVector3(0.0, 0.0, 0.0), btVector3(7150000, 7150000, 7150000), 1000.0, btVector3(1.0, 1.0, 1.0), 715000000, __nullptr);
=======
	((Planet*)GameObjectStackHandle->GetGameObjectByID(id2a))->BuildPlanet("Vaelhad/", 64, 13, 5, 4, 4, XMFLOAT3(0.0f, 0.698f, 0.894f), XMFLOAT3(0.0f, 0.506f, 0.725f), 200.0f,
		528, 9, 7);
>>>>>>> VereEngine-Planet

	GameObjectSpace *circleMoonSpace = new GameObjectSpace;

	int id3 = GameObjectStackHandle->CreateObjectToReg(circleMoonSpace);
	int id3a = GameObjectStackHandle->CreateObject(id3, btVector3(34000000.0, 0.0, 0.0), btVector3(0.0, 0.0, 0.0), btVector3(1.0, 1.0, 1.0), 1000.0, btVector3(1.0, 1.0, 1.0), 715000000.0, __nullptr);
	((GameObjectSpace*)GameObjectStackHandle->GetGameObjectByID(id3a))->CreateGameObject(id2a);
	((Planet*)GameObjectStackHandle->GetGameObjectByID(id3a))->CreateGameObject(GameObjectStackHandle->GetMainCamera()->GetId());
	GameObjectStackHandle->GetMainCamera()->SetLocalPosition(btVector3(350000.0, 100.0, 0.0));

	GameObjectSpace *circlePlanetSpace = new GameObjectSpace;

	int id4 = GameObjectStackHandle->CreateObjectToReg(circlePlanetSpace);
	int id4a = GameObjectStackHandle->CreateObject(id4, btVector3(150000000.0, 0.0, 0.0), btVector3(0.0, 0.0, 0.0), btVector3(1.0, 1.0, 1.0), 1000.0, btVector3(1.0, 1.0, 1.0), 715000000.0, __nullptr);
	((GameObjectSpace*)GameObjectStackHandle->GetGameObjectByID(id4a))->CreateGameObject(id3a);
	((GameObjectSpace*)GameObjectStackHandle->GetGameObjectByID(id4a))->CreateGameObject(id1a);

	// TODO: Change the timer settings if you want something other than the default variable timestep mode.
	// e.g. for 60 FPS fixed timestep update logic, call:
	/*
	m_timer.SetFixedTimeStep(true);
	m_timer.SetTargetElapsedSeconds(1.0 / 60);
	*/
}

VereEngineMain::~VereEngineMain()
{
	// Deregister device notification
	m_deviceResources->RegisterDeviceNotify(nullptr);
}

// Updates application state when the window size changes (e.g. device orientation change)
void VereEngineMain::CreateWindowSizeDependentResources()
{
}

void VereEngineMain::StartRenderLoop()
{
	// If the animation render loop is already running then do not start another thread.
	if (m_renderLoopWorker != nullptr && m_renderLoopWorker->Status == AsyncStatus::Started)
	{
		return;
	}

	// Create a task that will be run on a background thread.
	auto workItemHandler = ref new WorkItemHandler([this](IAsyncAction ^ action)
	{
		// Calculate the updated frame and render once per vertical blanking interval.
		while (action->Status == AsyncStatus::Started)
		{
			critical_section::scoped_lock lock(m_criticalSection);
			Update();
			if (Render())
			{
				m_deviceResources->Present();
			}
		}
	});

	// Run task on a dedicated high priority background thread.
	m_renderLoopWorker = ThreadPool::RunAsync(workItemHandler, WorkItemPriority::High, WorkItemOptions::None);
}

void VereEngineMain::StartExpensiveLoop()
{
	// If the expensive loop is already running then do not start another thread.
	if (m_expensiveLoopWorker != nullptr && m_expensiveLoopWorker->Status == AsyncStatus::Started)
	{
		return;
	}

	// Create a task that will be run on a background thread.
	auto workItemHandler = ref new WorkItemHandler([this](IAsyncAction ^ action)
	{
		// Calculate the updated expensive proccess.
		while (action->Status == AsyncStatus::Started)
		{
			critical_section::scoped_lock lock(m_criticalSection);
			UpdateExpensive();
		}
	});

	// Run task on a dedicated normal priority background thread.
	m_expensiveLoopWorker = ThreadPool::RunAsync(workItemHandler, WorkItemPriority::Low, WorkItemOptions::TimeSliced);
}

void VereEngineMain::StopRenderLoop()
{
	m_renderLoopWorker->Cancel();
}

void VereEngineMain::StopExpensiveLoop()
{
	m_expensiveLoopWorker->Cancel();
}

// Updates the application state once per frame.
void VereEngineMain::Update()
{
	ProcessInput();

	// Update scene objects.
	m_timer.Tick([&]()
	{
		// TODO: Replace this with your app's content update functions.
		m_gameObjects->Update();
	});
}

// Updates the application for expensive proccess.
void VereEngineMain::UpdateExpensive()
{
	m_gameComponentsManager->Update();
}

// Process all input from the user before updating game state
void VereEngineMain::ProcessInput()
{

}

// Renders the current frame according to the current application state.
// Returns true if the frame was rendered and is ready to be displayed.
bool VereEngineMain::Render()
{
	// Don't try to render anything before the first Update.
	if (m_timer.GetFrameCount() == 0)
	{
		return false;
	}

	auto context = m_deviceResources->GetD3DDeviceContext();

	// Reset the viewport to target the whole screen.
	auto viewport = m_deviceResources->GetScreenViewport();
	//context->RSSetViewports(1, &viewport);

	// Reset render targets to the screen.
	ID3D11RenderTargetView *const targets[1] = { m_deviceResources->GetBackBufferRenderTargetView() };
	context->OMSetRenderTargets(1, targets, m_deviceResources->GetDepthStencilView());

	// Clear the back buffer and depth stencil view.
	context->ClearRenderTargetView(m_deviceResources->GetBackBufferRenderTargetView(), DirectX::Colors::Black);
	context->ClearDepthStencilView(m_deviceResources->GetDepthStencilView(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);

	// Render the scene objects.
	m_gameObjects->DrawScene();

	return true;
}

// Notifies renderers that device resources need to be released.
void VereEngineMain::OnDeviceLost()
{
}

// Notifies renderers that device resources may now be recreated.
void VereEngineMain::OnDeviceRestored()
{
	CreateWindowSizeDependentResources();
}
