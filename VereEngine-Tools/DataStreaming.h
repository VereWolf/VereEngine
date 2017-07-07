#pragma once

#include "stdafx.h"

struct DataStreaming
{
	static void SaveImage(std::wstring nameFile, std::vector<Float4> *dataBuffer, int height, int width);
	static void LoadImageFromFile(std::wstring nameFile, std::vector<Float4> *dataBuffer, int &outHeight, int &outWidth);
	static bool LoadFile(const std::string& filename, void *dataBuffer, UINT height, UINT width, UINT stride);
	static bool SaveFile(std::string nameFile, void *dataBuffer, UINT height, UINT width, UINT stride);
};
