#include "stdafx.h"
#include "BaseForTool.h"

std::wstring BaseForTool::GetWholeFilePatch(WCHAR* nameFile)
{
	std::wstring S = m_folderName;
	S += L"//";
	S += nameFile;

	return S;
}

std::string BaseForTool::GetWholeFilePatch(char* nameFile)
{
	int CH;
	std::wstring str = std::wstring(m_folderName);
	int size = str.length();
	std::string P;
	P.resize(str.length());
	for (int i = 0; i < str.length(); ++i)wctomb_s(&CH, &P[i], sizeof(char), str[i]);
	std::string S = P;
	S += "/";
	S += nameFile;

	return S;
}