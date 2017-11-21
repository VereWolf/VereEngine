#include "stdafx.h"
#include "DataStreaming.h"

void DataStreaming::SaveImage(std::wstring nameFile, std::vector<BYTE4> *dataBuffer, int height, int width)
{
	GdiplusStartupInput gdiplusStartupInput;
	ULONG_PTR gdiplusToken;
	GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);

	int S = height * width;

	std::vector<BYTE> vImageData(4 * S);

	for (int i = 0; i < S; ++i)
	{
		vImageData[4 * i] = dataBuffer->at(i).z;
		vImageData[4 * i + 1] = dataBuffer->at(i).y;
		vImageData[4 * i + 2] = dataBuffer->at(i).x;
		vImageData[4 * i + 3] = 0xFF;
	}

	Bitmap *bmp = new Bitmap(width, height, 4 * width, PixelFormat32bppARGB, &vImageData[0]);
	Gdiplus::Graphics *g = Gdiplus::Graphics::FromImage(bmp);

	CLSID bmpClsid = { 0x557cf400, 0x1a04, 0x11d3,{ 0x9a, 0x73, 0x00, 0x00, 0xf8, 0x1e, 0xf3, 0x2e } };

	int b = bmp->Save(nameFile.c_str(), &bmpClsid, NULL);

	delete bmp;

	GdiplusShutdown(gdiplusToken);
};

void DataStreaming::SaveImage(std::wstring nameFile, std::vector<Float4> *dataBuffer, int height, int width)
{
	GdiplusStartupInput gdiplusStartupInput;
	ULONG_PTR gdiplusToken;
	GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);

	int S = height * width;

	std::vector<BYTE> vImageData(4 * S);

	for (int i = 0; i < S; ++i)
	{
		vImageData[4 * i] = (BYTE)(dataBuffer->at(i).z * 255);
		vImageData[4 * i + 1] = (BYTE)(dataBuffer->at(i).y * 255);
		vImageData[4 * i + 2] = (BYTE)(dataBuffer->at(i).x * 255);
		vImageData[4 * i + 3] = 0xFF;
	}

	Bitmap *bmp = new Bitmap(width, height, 4 * width, PixelFormat32bppARGB, &vImageData[0]);
	Gdiplus::Graphics *g = Gdiplus::Graphics::FromImage(bmp);

	CLSID bmpClsid = { 0x557cf400, 0x1a04, 0x11d3,{ 0x9a, 0x73, 0x00, 0x00, 0xf8, 0x1e, 0xf3, 0x2e } };

	int b = bmp->Save(nameFile.c_str(), &bmpClsid, NULL);

	delete bmp;

	GdiplusShutdown(gdiplusToken);
};

void DataStreaming::SaveImage(std::wstring nameFile, std::vector<float> *dataBuffer, int height, int width)
{
	GdiplusStartupInput gdiplusStartupInput;
	ULONG_PTR gdiplusToken;
	GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);

	int S = height * width;

	std::vector<BYTE> vImageData(4 * S);

	for (int i = 0; i < S; ++i)
	{
		vImageData[4 * i] = (BYTE)(dataBuffer->at(i) * 255);
		vImageData[4 * i + 1] = (BYTE)(dataBuffer->at(i) * 255);
		vImageData[4 * i + 2] = (BYTE)(dataBuffer->at(i) * 255);
		vImageData[4 * i + 3] = 0xFF;
	}

	Bitmap *bmp = new Bitmap(width, height, 4 * width, PixelFormat32bppARGB, &vImageData[0]);
	Gdiplus::Graphics *g = Gdiplus::Graphics::FromImage(bmp);

	CLSID bmpClsid = { 0x557cf400, 0x1a04, 0x11d3,{ 0x9a, 0x73, 0x00, 0x00, 0xf8, 0x1e, 0xf3, 0x2e } };

	int b = bmp->Save(nameFile.c_str(), &bmpClsid, NULL);

	delete bmp;

	GdiplusShutdown(gdiplusToken);
};

void DataStreaming::SaveImageNorm(std::wstring nameFile, std::vector<float> *dataBuffer, int height, int width)
{
	float max = 0.0f;

	for (int i = 0; i < dataBuffer->size(); i += 1)
	{
		if (dataBuffer->at(i) > max)
		{
			max = dataBuffer->at(i);
		}

		if (dataBuffer->at(i) < 0.0f)
		{
			dataBuffer->at(i) = 0.0f;
		}
		/*else
		{
			dataBuffer->at(i) = 1.0f;
		}*/
	}

	for (int i = 0; i < dataBuffer->size(); i += 1)
	{
		dataBuffer->at(i) /= max;
	}

	SaveImage(nameFile, dataBuffer, height, width);
}

void DataStreaming::LoadImageFromFile(std::wstring nameFile, std::vector<BYTE4> *dataBuffer, int &height, int &width)
{
	GdiplusStartupInput gdiplusStartupInput;
	ULONG_PTR gdiplusToken;
	GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);

	Bitmap *bmp = new Bitmap(nameFile.c_str());
	height = bmp->GetHeight();
	width = bmp->GetWidth();
	int S = height * width;

	dataBuffer->resize(S);

	Color color;

	for (int j = 0; j < height; ++j)
	{
		for (int i = 0; i < width; ++i)
		{
			bmp->GetPixel(i, height - j - 1, &color);

			dataBuffer->at(j * width + i).x = color.GetRed();
			dataBuffer->at(j * width + i).y = color.GetGreen();
			dataBuffer->at(j * width + i).z = color.GetBlue();
			dataBuffer->at(j * width + i).w = color.GetAlpha();
		}
	}

	delete bmp;

	GdiplusShutdown(gdiplusToken);
};

void DataStreaming::LoadImageFromFile(std::wstring nameFile, std::vector<Float4> *dataBuffer, int &height, int &width)
{
	GdiplusStartupInput gdiplusStartupInput;
	ULONG_PTR gdiplusToken;
	GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);

	Bitmap *bmp = new Bitmap(nameFile.c_str());
	height = bmp->GetHeight();
	width = bmp->GetWidth();
	int S = height * width;

	dataBuffer->resize(S);

	Color color;

	for (int j = 0; j < height; ++j)
	{
		for (int i = 0; i < width; ++i)
		{
			bmp->GetPixel(i, height - j - 1, &color);

			dataBuffer->at(j * width + i).x = (float)color.GetRed() / 255.0f;
			dataBuffer->at(j * width + i).y = (float)color.GetGreen() / 255.0f;
			dataBuffer->at(j * width + i).z = (float)color.GetBlue() / 255.0f;
			dataBuffer->at(j * width + i).w = (float)color.GetAlpha() / 255.0f;
		}
	}

	delete bmp;

	GdiplusShutdown(gdiplusToken);
};

bool DataStreaming::LoadFile(const std::string& filename, void *dataBuffer, UINT height, UINT width, UINT stride)
{
	std::ifstream fin;
	fin.open(filename, std::ios::in | std::ios::binary);

	fin.seekg(0, std::ios_base::end);
	int size = (int)fin.tellg();
	fin.seekg(0, std::ios_base::end);

	if (size != stride * height * width)
	{
		return false;
	}

	int S = height * width;

	fin.seekg(0, std::ios_base::beg);
	std::vector<char> HM(size);

	fin.read(&HM[0], size);
	fin.close();

	memcpy(dataBuffer, &HM[0], size);

	return true;
};

bool DataStreaming::SaveFile(std::string nameFile, void *dataBuffer, UINT height, UINT width, UINT stride)
{
	std::ofstream fout(nameFile, std::ios::binary);

	if (!fout)
	{
		return false;
	}

	/*std::vector<char> buffer(height * width * stride);

	memcpy(&buffer[0], dataBuffer, buffer.size());*/

	fout.write((char*)dataBuffer, height * width * stride);

	return true;
};