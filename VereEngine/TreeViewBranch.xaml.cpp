#include "pch.h"
#include "TreeViewBranch.xaml.h"

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

void VereEngine::PopupMenuAndRename::NewPopupMenu(Popup^ M)
{
	if (isOpen == true)
	{
		menu->IsOpen = false;
		isOpen = false;
	}
	if (M != nullptr)
	{
		menu = M;
		isOpen = true;
	}
}

String^ VereEngine::PopupMenuAndRename::RenameFolder(StackPanel^ F)
{
	if (isRenameFolder == true)
	{
		TextBlock^ block = ((TextBlock^)folder->Children->GetAt(1));
		TextBox^ box = ((TextBox^)folder->Children->GetAt(2));

		block->Visibility = Windows::UI::Xaml::Visibility::Visible;
		box->Visibility = Windows::UI::Xaml::Visibility::Collapsed;

		block->Text = box->Text;

		if (F != nullptr)
		{
			folder = F;
			isRenameFolder = true;
		}

		return nullptr;
	}
	if (F != nullptr)
	{
		folder = F;
		isRenameFolder = true;
	}

	return nullptr;
}

String^ VereEngine::PopupMenuAndRename::RenameItem(StackPanel^ I)
{
	if (isRenameItem == true)
	{
		ListViewItem^ list = ((ListViewItem^)item->Children->GetAt(0));
		TextBox^ box = ((TextBox^)item->Children->GetAt(1));

		list->Visibility = Windows::UI::Xaml::Visibility::Visible;
		box->Visibility = Windows::UI::Xaml::Visibility::Collapsed;

		list->Content = box->Text;

		if (I != nullptr)
		{
			item = I;
			isRenameItem = true;
		}

		return box->Text;
	}
	if (I != nullptr)
	{
		item = I;
		isRenameItem = true;
	}

	return nullptr;
}

void VereEngine::PopupMenuAndRename::Collapse()
{
	NewPopupMenu(nullptr);
	RenameFolder(nullptr);
	RenameItem(nullptr);
}

VereEngine::TreeViewBranch::TreeViewBranch(String^ nameBranch, String^ path, VereEngine::PopupMenuAndRename^ openMenu)
{
	m_branch = ref new StackPanel;
	m_branch = (StackPanel^)CreateFolder(nameBranch);
	m_branch->Name = path;
	m_openMenu = openMenu;
}

VereEngine::TreeViewBranch::~TreeViewBranch()
{
}

void VereEngine::TreeViewBranch::OnCollapsedObjects(Platform::Object^ sender, Windows::UI::Xaml::Input::TappedRoutedEventArgs^ e)
{
	StackPanel^ panel = ((StackPanel^)((StackPanel^)sender)->DataContext);
	ListView^ folders = (ListView^)panel->Children->GetAt(2);
	ListView^ objects = (ListView^)panel->Children->GetAt(3);

	if (folders->Visibility == Windows::UI::Xaml::Visibility::Visible)
	{
		folders->Visibility = Windows::UI::Xaml::Visibility::Collapsed;
		objects->Visibility = Windows::UI::Xaml::Visibility::Collapsed;
	}
	else
	{
		folders->Visibility = Windows::UI::Xaml::Visibility::Visible;
		objects->Visibility = Windows::UI::Xaml::Visibility::Visible;
	}

	m_openMenu->Collapse();
}

void VereEngine::TreeViewBranch::OnPopupMenuFolder(Platform::Object^ sender, Windows::UI::Xaml::Input::RightTappedEventHandler^ e)
{
	StackPanel^ stackPanel = ((StackPanel ^)((StackPanel^)sender)->DataContext);
	Popup^ popup = ((Popup^)stackPanel->Children->GetAt(1));

	if (popup->IsOpen == true)
	{
		popup->IsOpen = false;
	}
	else
	{
		popup->IsOpen = true;
	}

	m_openMenu->Collapse();
}

void VereEngine::TreeViewBranch::OnPopupMenuNewFolder(Platform::Object^ sender, Windows::UI::Xaml::Input::TappedEventHandler^ e)
{
	StackPanel^ stackPanel = ((StackPanel ^)((StackPanel^)sender)->DataContext);
	CreateFolder(stackPanel, "New_Folder_" + ((ListView ^)stackPanel->Children->GetAt(2))->Items->Size);

	m_openMenu->Collapse();
}

void VereEngine::TreeViewBranch::OnPopupMenuNewItem(Platform::Object^ sender, Windows::UI::Xaml::Input::TappedEventHandler^ e)
{
	StackPanel^ stackPanel = ((StackPanel ^)((StackPanel^)sender)->DataContext);
	CreateItem(stackPanel, "New_Item_", 0);

	m_openMenu->Collapse();
}

void VereEngine::TreeViewBranch::OnPopupMenuRenameFolder(Platform::Object^ sender, Windows::UI::Xaml::Input::TappedEventHandler^ e)
{
	m_openMenu->Collapse();

	StackPanel^ folder = ((StackPanel^)((StackPanel^)((StackPanel^)sender)->DataContext)->Children->GetAt(0));
	TextBlock^ block = ((TextBlock^)folder->Children->GetAt(1));
	TextBox^ box = ((TextBox^)folder->Children->GetAt(2));

	block->Visibility = Windows::UI::Xaml::Visibility::Collapsed;
	box->Visibility = Windows::UI::Xaml::Visibility::Visible;

	m_openMenu->RenameFolder(folder);
	m_openMenu->NewPopupMenu(nullptr);
}

void VereEngine::TreeViewBranch::OnPopupMenuDeleteFolder(Platform::Object^ sender, Windows::UI::Xaml::Input::TappedEventHandler^ e)
{
	StackPanel^ stack = (StackPanel^)((StackPanel^)sender)->DataContext;
	String^ name = stack->Name;
	DeleteFolder(stack, name);

	m_openMenu->Collapse();
}

void VereEngine::TreeViewBranch::OnPopupMenuItem(Platform::Object^ sender, Windows::UI::Xaml::Input::RightTappedEventHandler^ e)
{
	StackPanel^ object = (StackPanel^)((StackPanel^)sender)->DataContext;
	Popup^ popup = (Popup^)object->Children->GetAt(1);

	if (popup->IsOpen == true)
	{
		popup->IsOpen = false;
	}
	else
	{
		popup->IsOpen = true;
	}

	m_openMenu->NewPopupMenu(popup);
}

void VereEngine::TreeViewBranch::OnPopupMenuRenameItem(Platform::Object^ sender, Windows::UI::Xaml::Input::TappedEventHandler^ e)
{
	m_openMenu->Collapse();

	StackPanel^ folder = ((StackPanel^)((StackPanel^)((StackPanel^)sender)->DataContext)->Children->GetAt(0));
	ListViewItem^ item = ((ListViewItem^)folder->Children->GetAt(0));
	TextBox^ box = ((TextBox^)folder->Children->GetAt(1));

	item->Visibility = Windows::UI::Xaml::Visibility::Collapsed;
	box->Visibility = Windows::UI::Xaml::Visibility::Visible;

	m_openMenu->RenameItem(folder);
	m_openMenu->NewPopupMenu(nullptr);
}

void VereEngine::TreeViewBranch::OnPopupMenuDeleteItem(Platform::Object^ sender, Windows::UI::Xaml::Input::TappedEventHandler^ e)
{
	StackPanel^ object = (StackPanel^)((StackPanel^)sender)->DataContext;

	ListView^ list = (ListView^)object->DataContext;
	for (int i = 0; i < list->Items->Size; ++i)
	{
		if (((StackPanel^)list->Items->GetAt(i))->Name->Equals(object->Name))
		{
			list->Items->RemoveAt(i);
			return;
		}
	}

	m_openMenu->NewPopupMenu(nullptr);
}

Object^ VereEngine::TreeViewBranch::GetItem(Object^ folder, Platform::String^ name)
{
	ListView^ items = (ListView^)((StackPanel^)folder)->Children->GetAt(3);
	for (int i = 0; i < items->Items->Size; ++i)
	{
		if (((ListViewItem^)items->Items->GetAt(i))->Name->Equals(name))
		{
			return items->Items->GetAt(i);
		}
	}

	return NULL;
}

Object^ VereEngine::TreeViewBranch::GetFolder(Object^ folder, Platform::String^ name)
{
	ListView^ items = (ListView^)((StackPanel^)folder)->Children->GetAt(2);
	for (int i = 0; i < items->Items->Size; ++i)
	{
		if (((ListViewItem^)items->Items->GetAt(i))->Name->Equals(name))
		{
			return items->Items->GetAt(i);
		}
	}

	return NULL;
}

Object^ VereEngine::TreeViewBranch::CreateFolder(Platform::String^ name)
{
	StackPanel^ stackPanel = ref new StackPanel;
	StackPanel^ stackFolder = ref new StackPanel;
	FontIcon^ icon = ref new FontIcon;
	TextBlock^ textBlock = ref new TextBlock;
	TextBox^ textBox = ref new TextBox;

	stackPanel->Name = name;
	stackPanel->CanDrag = true;
	stackPanel->AllowDrop = true;
	stackFolder->Orientation = Orientation::Horizontal;
	stackFolder->Height = 40;
	stackFolder->CanDrag = true;
	stackFolder->AllowDrop = true;
	stackFolder->DataContext = stackPanel;
	stackFolder->Tapped += ref new ::Windows::UI::Xaml::Input::TappedEventHandler(this, (void(::VereEngine::TreeViewBranch::*)
		(::Platform::Object^, ::Windows::UI::Xaml::Input::TappedRoutedEventArgs^))&TreeViewBranch::OnCollapsedObjects);
	stackFolder->RightTapped += ref new ::Windows::UI::Xaml::Input::RightTappedEventHandler(this, (void(::VereEngine::TreeViewBranch::*)
		(::Platform::Object^, ::Windows::UI::Xaml::Input::RightTappedRoutedEventArgs^))&TreeViewBranch::OnPopupMenuFolder);

	icon->Name = name + "_Folder";
	//icon->DataContext = "787746568445";
	icon->Glyph = "\uE188";
	icon->FontSize = 24;
	icon->Foreground = ref new SolidColorBrush(Windows::UI::ColorHelper::FromArgb(255, 255, 225, 64));
	icon->Margin = Thickness(0, 4, 0, 12);
	icon->FontFamily = ref new Windows::UI::Xaml::Media::FontFamily("Segoe MDL2 Assets");
	icon->HorizontalAlignment = Windows::UI::Xaml::HorizontalAlignment::Left;
	stackFolder->Children->InsertAt(0, icon);

	textBlock->Name = name + "_Text";
	textBlock->DataContext = stackFolder;
	textBlock->HorizontalAlignment = Windows::UI::Xaml::HorizontalAlignment::Stretch;
	textBlock->VerticalAlignment = Windows::UI::Xaml::VerticalAlignment::Center;
	textBlock->Text = name;
	stackFolder->Children->InsertAt(1, textBlock);


	textBox->Name = textBlock->Text + "_Box";
	textBox->DataContext = stackFolder;
	textBox->HorizontalAlignment = Windows::UI::Xaml::HorizontalAlignment::Stretch;
	textBox->VerticalAlignment = Windows::UI::Xaml::VerticalAlignment::Center;
	textBox->Text = textBlock->Text;
	textBox->Visibility = Windows::UI::Xaml::Visibility::Collapsed;
	stackFolder->Children->InsertAt(2, textBox);

	Border^ folderBorder = ref new Border;
	StackPanel^ folderStack = ref new StackPanel;
	StackPanel^ folderNewFolder = ref new StackPanel;
	folderNewFolder->Tapped += ref new ::Windows::UI::Xaml::Input::TappedEventHandler(this, (void(::VereEngine::TreeViewBranch::*)
		(::Platform::Object^, ::Windows::UI::Xaml::Input::TappedRoutedEventArgs^))&TreeViewBranch::OnPopupMenuNewFolder);
	folderNewFolder->DataContext = stackPanel;
	TextBlock^ textNewFolder = ref new TextBlock;
	textNewFolder->Text = "New Folder";
	folderNewFolder->Children->InsertAt(0, textNewFolder);
	StackPanel^ folderNewItem = ref new StackPanel;
	folderNewItem->Tapped += ref new ::Windows::UI::Xaml::Input::TappedEventHandler(this, (void(::VereEngine::TreeViewBranch::*)
		(::Platform::Object^, ::Windows::UI::Xaml::Input::TappedRoutedEventArgs^))&TreeViewBranch::OnPopupMenuNewItem);
	folderNewItem->DataContext = stackPanel;
	TextBlock^ textNewItem = ref new TextBlock;
	textNewItem->Text = "New Object";
	folderNewItem->Children->InsertAt(1, textNewItem);
	StackPanel^ folderRenameFolder = ref new StackPanel;
	folderRenameFolder->Tapped += ref new ::Windows::UI::Xaml::Input::TappedEventHandler(this, (void(::VereEngine::TreeViewBranch::*)
		(::Platform::Object^, ::Windows::UI::Xaml::Input::TappedRoutedEventArgs^))&TreeViewBranch::OnPopupMenuRenameFolder);
	folderRenameFolder->DataContext = stackPanel;
	TextBlock^ textRenameFolder = ref new TextBlock;
	textRenameFolder->Text = "Rename Folder";
	folderRenameFolder->Children->InsertAt(2, textRenameFolder);
	StackPanel^ folderDeleteFolder = ref new StackPanel;
	folderDeleteFolder->Tapped += ref new ::Windows::UI::Xaml::Input::TappedEventHandler(this, (void(::VereEngine::TreeViewBranch::*)
		(::Platform::Object^, ::Windows::UI::Xaml::Input::TappedRoutedEventArgs^))&TreeViewBranch::OnPopupMenuDeleteFolder);
	folderDeleteFolder->DataContext = stackPanel;
	TextBlock^ textDeleteFolder = ref new TextBlock;
	textDeleteFolder->Text = "Delete Folder";
	folderDeleteFolder->Children->InsertAt(3, textDeleteFolder);
	folderStack->Children->InsertAt(0, folderNewFolder);
	folderStack->Children->InsertAt(1, folderNewItem);
	folderStack->Children->InsertAt(2, folderRenameFolder);
	folderStack->Children->InsertAt(3, folderDeleteFolder);
	folderBorder->Child = folderStack;

	stackPanel->Children->InsertAt(0, stackFolder);

	Popup^ folderPopup = ref new Popup;
	folderPopup->Child = folderBorder;
	folderPopup->DataContext = stackPanel;
	stackPanel->Children->InsertAt(1, folderPopup);

	ListView^ FolderView = ref new ListView;
	FolderView->Name = name + "_FolderView";
	FolderView->DataContext = stackPanel;
	stackPanel->Children->InsertAt(2, FolderView);

	ListView^ ItemView = ref new ListView;
	ItemView->Name = name + "_ItemView";
	ItemView->DataContext = stackPanel;
	stackPanel->Children->InsertAt(3, ItemView);

	return (Object^)stackPanel;
}

void  VereEngine::TreeViewBranch::CreateFolder(Object^ F, Platform::String^ name)
{
	ListView^ folder = (ListView^)((StackPanel^)F)->Children->GetAt(2);
	StackPanel^ stackPanel = (StackPanel^)CreateFolder(name);
	stackPanel->DataContext = folder;
	stackPanel->Name = ((StackPanel^)folder->DataContext)->Name + "/" + "name";

	folder->Items->InsertAt(folder->Items->Size, stackPanel);
}

Object^ VereEngine::TreeViewBranch::CreateItem(Platform::String^ name, int id)
{
	StackPanel^ stackPanel = ref new StackPanel;
	StackPanel^ object = ref new StackPanel;
	object->Name += id;
	object->RightTapped += ref new ::Windows::UI::Xaml::Input::RightTappedEventHandler(this, (void(::VereEngine::TreeViewBranch::*)
		(::Platform::Object^, ::Windows::UI::Xaml::Input::RightTappedRoutedEventArgs^))&TreeViewBranch::OnPopupMenuItem);
	object->DataContext = stackPanel;

	ListViewItem^ newItem = ref new ListViewItem;
	newItem->Content = "Object " + id;
	newItem->CanDrag = true;
	newItem->Name += id;

	TextBox^ box = ref new TextBox;
	box->Text = newItem->Content->ToString();
	box->Visibility = Windows::UI::Xaml::Visibility::Collapsed;

	object->Children->InsertAt(0, newItem);
	object->Children->InsertAt(1, box);

	Border^ itemBorder = ref new Border;
	StackPanel^ itemStack = ref new StackPanel;
	StackPanel^ itemRenameItem = ref new StackPanel;
	itemRenameItem->Tapped += ref new ::Windows::UI::Xaml::Input::TappedEventHandler(this, (void(::VereEngine::TreeViewBranch::*)
		(::Platform::Object^, ::Windows::UI::Xaml::Input::TappedRoutedEventArgs^))&TreeViewBranch::OnPopupMenuRenameItem);
	itemRenameItem->DataContext = stackPanel;
	TextBlock^ textRenameItem = ref new TextBlock;
	textRenameItem->Text = "Rename Item";
	itemRenameItem->Children->InsertAt(0, textRenameItem);
	StackPanel^ itemDeleteItem = ref new StackPanel;
	itemDeleteItem->Tapped += ref new ::Windows::UI::Xaml::Input::TappedEventHandler(this, (void(::VereEngine::TreeViewBranch::*)
		(::Platform::Object^, ::Windows::UI::Xaml::Input::TappedRoutedEventArgs^))&TreeViewBranch::OnPopupMenuDeleteItem);
	itemDeleteItem->DataContext = stackPanel;
	TextBlock^ textDeleteItem = ref new TextBlock;
	textDeleteItem->Text = "Delete Item";
	itemDeleteItem->Children->InsertAt(0, textDeleteItem);
	itemStack->Children->InsertAt(0, itemRenameItem);
	itemStack->Children->InsertAt(1, itemDeleteItem);
	itemBorder->Child = itemStack;

	stackPanel->Children->InsertAt(0, object);

	Popup^ itemPopup = ref new Popup;
	itemPopup->Child = itemBorder;
	itemPopup->DataContext = stackPanel;
	stackPanel->Children->InsertAt(1, itemPopup);
	//stackPanel->Children->InsertAt(1, itemStack);

	return (Object^)stackPanel;
}

void VereEngine::TreeViewBranch::CreateItem(Object^ F, Platform::String^ name, int id)
{
	ListView^ folder = (ListView^)((StackPanel^)F)->Children->GetAt(3);
	StackPanel^ newItem = (StackPanel^)CreateItem(name, id);
	newItem->DataContext = folder;
	
	folder->Items->InsertAt(folder->Items->Size, newItem);
}

void VereEngine::TreeViewBranch::DeleteFolder(Object^ folder, Platform::String^ name)
{
	ListView^ list = (ListView^)((StackPanel^)folder)->DataContext;

	for (int i = 0; i < list->Items->Size; ++i)
	{
		if (((StackPanel^)list->Items->GetAt(i))->Name->Equals(name))
		{
			list->Items->RemoveAt(i);
			return;
		}
	}
}

void VereEngine::TreeViewBranch::DeleteItem(Object^ folder, Platform::String^ name)
{

}

void VereEngine::TreeViewBranch::RenameFolder(Object^ folder, Platform::String^ name)
{

}

void VereEngine::TreeViewBranch::RenameItem(Object^ folder, Platform::String^ name)
{

}