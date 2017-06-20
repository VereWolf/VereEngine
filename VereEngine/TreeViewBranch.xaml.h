#pragma once

#include "TreeViewBranch.g.h"
#include "DeviceResources.h"
#include "VereEngineMain.h"


namespace VereEngine
{
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

	public ref class PopupMenuAndRename sealed
	{
	public:
		PopupMenuAndRename() :isOpen(false), isRenameFolder(false), isRenameItem(false) {}
		void NewPopupMenu(Popup^ M);
		
		String^ RenameFolder(StackPanel^ F);
		String^ RenameItem(StackPanel^ I);
		void Collapse();
	private:
		Popup^ menu;
		StackPanel^ folder;
		StackPanel^ item;
		bool isOpen;
		bool isRenameFolder;
		bool isRenameItem;
	};

	public ref class TreeViewBranch sealed
	{
	public:
		TreeViewBranch(String^ nameBranch, String^ path, PopupMenuAndRename^ openMenu);
	private:
		~TreeViewBranch();

	public:
		StackPanel^ GetBranch() { return m_branch; }
		String^ GetPath() { return m_branch->Name; }

		void OnCollapsedObjects(Platform::Object^ sender, Windows::UI::Xaml::Input::TappedRoutedEventArgs^ e);
		void OnPopupMenuFolder(Platform::Object^ sender, Windows::UI::Xaml::Input::RightTappedEventHandler^ e);
		void OnPopupMenuNewFolder(Platform::Object^ sender, Windows::UI::Xaml::Input::TappedEventHandler^ e);
		void OnPopupMenuNewItem(Platform::Object^ sender, Windows::UI::Xaml::Input::TappedEventHandler^ e);
		void OnPopupMenuRenameFolder(Platform::Object^ sender, Windows::UI::Xaml::Input::TappedEventHandler^ e);
		void OnPopupMenuDeleteFolder(Platform::Object^ sender, Windows::UI::Xaml::Input::TappedEventHandler^ e);

		void OnPopupMenuItem(Platform::Object^ sender, Windows::UI::Xaml::Input::RightTappedEventHandler^ e);
		void OnPopupMenuRenameItem(Platform::Object^ sender, Windows::UI::Xaml::Input::TappedEventHandler^ e);
		void OnPopupMenuDeleteItem(Platform::Object^ sender, Windows::UI::Xaml::Input::TappedEventHandler^ e);

		Object^ GetItem(Object^ folder, Platform::String^ name);
		Object^ GetFolder(Object^ folder, Platform::String^ name);
		Object^ CreateFolder(Platform::String^ name);
		void CreateFolder(Object^ folder, Platform::String^ name);
		Object^ CreateItem(Platform::String^ name, int id);
		void CreateItem(Object^ folder, Platform::String^ name, int id);
		void DeleteFolder(Object^ folder, Platform::String^ name);
		void DeleteItem(Object^ folder, Platform::String^ name);
		void RenameFolder(Object^ folder, Platform::String^ name);
		void RenameItem(Object^ folder, Platform::String^ name);
	private:
		StackPanel^ m_branch;
		PopupMenuAndRename^ m_openMenu;
	};
}
