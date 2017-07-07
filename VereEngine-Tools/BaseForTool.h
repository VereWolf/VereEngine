#pragma once

#include "stdafx.h"

class BaseForTool
{
public:
	void SetFolderName(WCHAR* folderName) { m_folderName = folderName; }
	WCHAR *GetFileName() { return m_folderName; }
	std::wstring GetWholeFilePatch(WCHAR* nameFile);
	std::string GetWholeFilePatch(char* nameFile);
private:
	WCHAR* m_folderName;
};

