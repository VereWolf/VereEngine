//
// App.xaml.h
// Declaration of the App class.
//

#pragma once

#include "App.g.h"

#ifndef VEREEDITOR
#include "MainRenderPage.xaml.h"
#else
#include "MainEditorPage.xaml.h"
#endif

namespace VereEngine
{
	/// <summary>
	/// Provides application-specific behavior to supplement the default Application class.
	/// </summary>
	ref class App sealed
	{
	public:
		App();
		virtual void OnLaunched(Windows::ApplicationModel::Activation::LaunchActivatedEventArgs^ e) override;

	private:
		void OnSuspending(Platform::Object^ sender, Windows::ApplicationModel::SuspendingEventArgs^ e);
		void OnResuming(Platform::Object ^sender, Platform::Object ^args);

#ifndef VEREEDITOR
		DirectXPage^ m_MainDirectXPage;
#else
		MainEditorPage ^ m_MainEditorPage;
#endif
	};
}
