#pragma once

#include "StepTimer.h"
#include "DeviceResources.h"
#include "StreamingDataManager.h"
#include "GameComponentsManager.h"
#include "RenderDevice.h"
#include "TextRenderDevice.h"
#include "GameObjectsStackManager.h"

// Renders Direct2D and 3D content on the screen.
namespace VereEngine
{
	class VereEngineMain : public DX::IDeviceNotify
	{
	public:
		VereEngineMain(const std::shared_ptr<DX::DeviceResources>& deviceResources);
		~VereEngineMain();
		void CreateWindowSizeDependentResources();
		void TrackingUpdate(XMFLOAT2 position) { m_pointerLocation = position; }
		void StartRenderLoop();
		void StartExpensiveLoop();
		void StopRenderLoop();
		void StopExpensiveLoop();
		Concurrency::critical_section& GetCriticalSection() { return m_criticalSection; }

		// IDeviceNotify
		virtual void OnDeviceLost();
		virtual void OnDeviceRestored();

	private:
		void ProcessInput();
		void Update();
		void UpdateExpensive();
		bool Render();

		// Cached pointer to device resources.
		std::shared_ptr<DX::DeviceResources> m_deviceResources;

		// TODO: Replace with your own content renderers.
		std::shared_ptr<StreamingDataManager> m_gameStreamingData;
		std::shared_ptr<GameComponentsManager> m_gameComponentsManager;
		std::shared_ptr<RenderDevice> m_gameRenderDevice;
		std::shared_ptr<TextRenderDevice> m_gameTextRenderDevice;
		std::shared_ptr<GameObjectsStackManager> m_gameObjects;

		Windows::Foundation::IAsyncAction^ m_renderLoopWorker;
		Windows::Foundation::IAsyncAction^ m_expensiveLoopWorker;
		Concurrency::critical_section m_criticalSection;

		// Rendering loop timer.
		DX::StepTimer m_timer;

		// Track current input pointer position.
		XMFLOAT2 m_pointerLocation;
	};
}