#include "pch.h"
#include "StreamingDataManager.h"
#include "GameStreamingData.h"
#include "GameDataSet.h"
#include "DataDepository.h"

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
using namespace Windows::ApplicationModel;
using namespace Windows::Storage;
using namespace Windows::Storage::Streams;

StreamingDataManager::StreamingDataManager()
{
	GameStreamingDataHandle = this;

	m_isOpenFileOutput = false;
}

StreamingDataManager::StreamingDataManager(DX::DeviceResources *resources,
	int levelsOfByteDepository, int levelsOfIntDepository, int levelsOfFloatDepository,
	int levelsOfByte4Depository, int levelsOfInt4Depository, int levelsOfFloat4Depository)
{
	GameStreamingDataHandle = this;

	Init(resources,
		levelsOfByteDepository, levelsOfIntDepository, levelsOfFloatDepository,
		levelsOfByte4Depository, levelsOfInt4Depository, levelsOfFloat4Depository);

	m_isOpenFileOutput = false;
}

StreamingDataManager::~StreamingDataManager()
{

}

void StreamingDataManager::Init(DX::DeviceResources *resources,
	int levelsOfByteDepository, int levelsOfIntDepository, int levelsOfFloatDepository,
	int levelsOfByte4Depository, int levelsOfInt4Depository, int levelsOfFloat4Depository)
{
	m_resources = resources;

	m_streamingData.m_gameData.Init(resources);
	m_streamingData.m_gameDataSet.Init(resources);

	m_dataByteDepositoryReg = new IDRegistr(levelsOfByteDepository);
	m_dataByteDepository.resize(pow(2, levelsOfByteDepository));

	m_dataIntDepositoryReg = new IDRegistr(levelsOfIntDepository);
	m_dataIntDepository.resize(pow(2, levelsOfIntDepository));

	m_dataFloatDepositoryReg = new IDRegistr(levelsOfFloatDepository);
	m_dataFloatDepository.resize(pow(2, levelsOfFloatDepository));

	m_dataByte4DepositoryReg = new IDRegistr(levelsOfByte4Depository);
	m_dataByte4Depository.resize(pow(2, levelsOfByte4Depository));

	m_dataInt4DepositoryReg = new IDRegistr(levelsOfInt4Depository);
	m_dataInt4Depository.resize(pow(2, levelsOfInt4Depository));

	m_dataFloat4DepositoryReg = new IDRegistr(levelsOfFloat4Depository);
	m_dataFloat4Depository.resize(pow(2, levelsOfFloat4Depository));
}

int StreamingDataManager::LoadData(string nameFile)
{
	std::ifstream fin(&nameFile[0], std::ios::binary);

	if (!fin)
	{
		return -1;
	}

	fin.seekg(0, std::ios_base::end);
	int size = (int)fin.tellg();
	fin.seekg(0, std::ios_base::beg);

	if (size <= 0)
	{
		return -1;
	}

	char *dataBuffer = new char[size];

	fin.read(&dataBuffer[0], size);
	fin.close();

	return CreateStreamingData(dataBuffer, size);
}

bool StreamingDataManager::SaveData(string nameFile, int id)
{

	Platform::String^ localfolder = Windows::Storage::ApplicationData::Current->LocalFolder->Path;

	std::wstring folderNameW(localfolder->Begin());
	std::string folderNameA(folderNameW.begin(), folderNameW.end());

	stringstream ss;

	ss << folderNameA << "\\" << nameFile;

	std::ofstream fout(ss.str(), std::ios::binary | ios::out);

	if (!fout)
	{
		return false;
	}

	fout.write((char*)GetStreamingData(id), GetSizeOfStreamingData(id));

	fout.close();

	return true;
}

bool StreamingDataManager::OpenFileOutput(std::string nameFile)
{

	Platform::String^ localfolder = Windows::Storage::ApplicationData::Current->LocalFolder->Path;

	std::wstring folderNameW(localfolder->Begin());
	std::string folderNameA(folderNameW.begin(), folderNameW.end());

	stringstream ss;

	ss << folderNameA << "\\" << nameFile;

	m_fileOutput.open(ss.str(), std::ios::binary | ios::out);

	if (!m_fileOutput)
	{
		m_isOpenFileOutput = false;
		return m_isOpenFileOutput;
	}

	m_isOpenFileOutput = true;
	return m_isOpenFileOutput;
}

void StreamingDataManager::CloseFileOutput()
{
	if (m_fileOutput)
	{
		m_fileOutput.close();
	}

	m_isOpenFileOutput = false;
}

bool StreamingDataManager::WriteToFileOutput(std::string text)
{
	if (!m_fileOutput)
	{
		m_isOpenFileOutput = false;
		return false;
	}

	m_fileOutput.write(text.c_str(), text.length());
}

int  StreamingDataManager::CreateStreamingData(void *data, int size)
{
	GameStreamingData *gameStreamingData = new GameStreamingData;
	gameStreamingData->PreInit(m_resources);
	gameStreamingData->SetStreamingData(data, size);

	return m_streamingData.m_gameData.CreateGameObject(gameStreamingData);
}

void StreamingDataManager::DeleteStreamingData(int id)
{
	m_streamingData.m_gameData.DeleteGameObject(id);
}

void* StreamingDataManager::GetStreamingData(int id)
{
	if (id >= 0)
	{
		return ((GameStreamingData*)m_streamingData.m_gameData.GetGameObject(id))->GetStreamingData();
	}

	return NULL;
}

int StreamingDataManager::GetSizeOfStreamingData(int id)
{
	if (id >= 0)
	{
		return ((GameStreamingData*)m_streamingData.m_gameData.GetGameObject(id))->GetSize();
	}

	return NULL;
}

int StreamingDataManager::CreateBYTEDepository(int level, int width, int height)
{
	int id = m_dataByteDepositoryReg->TakeElement();
	m_dataByteDepository[id] = new DataDepositoryBYTE(level, width, height);

	return id;
}

void StreamingDataManager::DeleteBYTEDepository(int id)
{
	m_dataByteDepositoryReg->ReturnElement(id);
	delete m_dataByteDepository[id];
}

int StreamingDataManager::CreateNewBlockInBYTEDepository(int idDepository, void *data)
{
	int id = m_dataByteDepository[idDepository]->CreateNewBlock(data);

	return id;
}

void StreamingDataManager::ReleaseBlockInBYTEDepository(int idDepository, int idBlock)
{
	m_dataByteDepository[idDepository]->ReleaseBlock(idBlock);
}

BYTE StreamingDataManager::GetValueFromBYTEDepository(int idDepository, int idBlock, float x, float y)
{
	return m_dataByteDepository[idDepository]->GetValue(idBlock, x, y);
}

void StreamingDataManager::SetValueFromBYTEDepository(int idDepository, int idBlock, int x, int y, BYTE value)
{
	m_dataByteDepository[idDepository]->SetValue(idBlock, x, y, value);
}

BYTE *StreamingDataManager::GetTextureFromBYTEDepository(int idDepository, int idBlock)
{
	return (BYTE*)m_dataByteDepository[idDepository]->GetTexture(idBlock);
}

void StreamingDataManager::SetTextureFromBYTEDepository(int idDepository, int idBlock, BYTE *data)
{
	m_dataByteDepository[idDepository]->SetTexture(idBlock, data);
}

BYTE *StreamingDataManager::CreateNewMapFromBYTEDepository(int idDepository, int idBlock, float lvl, XMFLOAT2 offset, float seam, float height, float width)
{
	return m_dataByteDepository[idDepository]->CreateNewMapFrom(idBlock, lvl, offset, seam, height, width);
}

int StreamingDataManager::GetWidthFromBYTEDepository(int idDepository)
{
	return m_dataByteDepository[idDepository]->GetWidth();
}

int StreamingDataManager::GetHeightFromBYTEDepository(int idDepository)
{
	return m_dataByteDepository[idDepository]->GetHeight();
}

int StreamingDataManager::CreateINTDepository(int level, int width, int height)
{
	int id = m_dataIntDepositoryReg->TakeElement();
	m_dataIntDepository[id] = new DataDepositoryINT(level, width, height);

	return id;
}

void StreamingDataManager::DeleteINTDepository(int id)
{
	m_dataIntDepositoryReg->ReturnElement(id);
	delete m_dataIntDepository[id];
}

int StreamingDataManager::CreateNewBlockInINTDepository(int idDepository, void *data)
{
	int id = m_dataIntDepository[idDepository]->CreateNewBlock(data);

	return id;
}

void StreamingDataManager::ReleaseBlockInINTDepository(int idDepository, int idBlock)
{
	m_dataIntDepository[idDepository]->ReleaseBlock(idBlock);
}

int StreamingDataManager::GetValueFromINTDepository(int idDepository, int idBlock, float x, float y)
{
	return m_dataIntDepository[idDepository]->GetValue(idBlock, x, y);
}

void StreamingDataManager::SetValueFromINTDepository(int idDepository, int idBlock, int x, int y, int value)
{
	m_dataIntDepository[idDepository]->SetValue(idBlock, x, y, value);
}

int *StreamingDataManager::GetTextureFromINTDepository(int idDepository, int idBlock)
{
	return (int*)m_dataIntDepository[idDepository]->GetTexture(idBlock);
}

void StreamingDataManager::SetTextureFromINTDepository(int idDepository, int idBlock, int *data)
{
	m_dataIntDepository[idDepository]->SetTexture(idBlock, data);
}

int *StreamingDataManager::CreateNewMapFromINTDepository(int idDepository, int idBlock, float lvl, XMFLOAT2 offset, float seam, float height, float width)
{
	return m_dataIntDepository[idDepository]->CreateNewMapFrom(idBlock, lvl, offset, seam, height, width);
}

int StreamingDataManager::GetWidthFromINTDepository(int idDepository)
{
	return m_dataIntDepository[idDepository]->GetWidth();
}

int StreamingDataManager::GetHeightFromINTDepository(int idDepository)
{
	return m_dataIntDepository[idDepository]->GetHeight();
}

int StreamingDataManager::CreateFLOATDepository(int level, int width, int height)
{
	int id = m_dataFloatDepositoryReg->TakeElement();
	m_dataFloatDepository[id] = new DataDepositoryFLOAT(level, width, height);

	return id;
}

void StreamingDataManager::DeleteFLOATDepository(int id)
{
	m_dataFloatDepositoryReg->ReturnElement(id);
	delete m_dataFloatDepository[id];
}

int StreamingDataManager::CreateNewBlockInFLOATDepository(int idDepository, void *data)
{
	int id = m_dataFloatDepository[idDepository]->CreateNewBlock(data);

	return id;
}

void StreamingDataManager::GetTextureInputFromFloat(int idDepository, int idBlock, ID3D11ShaderResourceView **SRV)
{
	m_dataFloatDepository[idDepository]->CreateTexturesForInput(m_resources, idBlock, SRV, DXGI_FORMAT_R32_FLOAT);
}

void StreamingDataManager::GetTextureOutputFromFloat(int idDepository, ID3D11ShaderResourceView **SRV, ID3D11UnorderedAccessView **UAV)
{
	m_dataFloatDepository[idDepository]->CreateTexturesForOutput(m_resources, SRV, UAV, DXGI_FORMAT_R32_FLOAT);
}

void StreamingDataManager::ReleaseBlockInFLOATDepository(int idDepository, int idBlock)
{
	m_dataFloatDepository[idDepository]->ReleaseBlock(idBlock);
}

float StreamingDataManager::GetValueFromFLOATDepository(int idDepository, int idBlock, float x, float y)
{
	return m_dataFloatDepository[idDepository]->GetValue(idBlock, x, y);
}

void StreamingDataManager::SetValueFromFLOATDepository(int idDepository, int idBlock, int x, int y, float value)
{
	m_dataFloatDepository[idDepository]->SetValue(idBlock, x, y, value);
}

float *StreamingDataManager::GetTextureFromFLOATDepository(int idDepository, int idBlock)
{
	return (float*)m_dataFloatDepository[idDepository]->GetTexture(idBlock);
}

void StreamingDataManager::SetTextureFromFLOATDepository(int idDepository, int idBlock, float *data)
{
	m_dataFloatDepository[idDepository]->SetTexture(idBlock, data);
}

VBYTE4 *StreamingDataManager::GetTextureFromBYTE4Depository(int idDepository, int idBlock)
{
	return (VBYTE4*)m_dataByte4Depository[idDepository]->GetTexture(idBlock);
}

void StreamingDataManager::SetTextureFromBYTE4Depository(int idDepository, int idBlock, VBYTE4 *data)
{
	m_dataByte4Depository[idDepository]->SetTexture(idBlock, data);
}

float *StreamingDataManager::CreateNewMapFromFLOATDepository(int idDepository, int idBlock, float lvl, XMFLOAT2 offset, float seam, float height, float width)
{
	return m_dataFloatDepository[idDepository]->CreateNewMapFrom(idBlock, lvl, offset, seam, height, width);
}

int StreamingDataManager::GetWidthFromFLOATDepository(int idDepository)
{
	return m_dataFloatDepository[idDepository]->GetWidth();
}

int StreamingDataManager::GetHeightFromFLOATDepository(int idDepository)
{
	return m_dataFloatDepository[idDepository]->GetHeight();
}

int StreamingDataManager::CreateBYTE4Depository(int level, int width, int height)
{
	int id = m_dataByte4DepositoryReg->TakeElement();
	m_dataByte4Depository[id] = new DataDepositoryBYTE4(level, width, height);

	return id;
}

void StreamingDataManager::DeleteBYTE4Depository(int id)
{
	m_dataByte4DepositoryReg->ReturnElement(id);
	delete m_dataByte4Depository[id];
}

int StreamingDataManager::CreateNewBlockInBYTE4Depository(int idDepository, void *data)
{
	int id = m_dataByte4Depository[idDepository]->CreateNewBlock(data);

	return id;
}

void StreamingDataManager::GetTextureInputFromBYTE4(int idDepository, int idBlock, ID3D11ShaderResourceView **SRV)
{
	m_dataByte4Depository[idDepository]->CreateTexturesForInput(m_resources, idBlock, SRV, DXGI_FORMAT_R8G8B8A8_UNORM);
}

void StreamingDataManager::GetTextureOutputFromBYTE4(int idDepository, ID3D11ShaderResourceView **SRV, ID3D11UnorderedAccessView **UAV)
{
	m_dataByteDepository[idDepository]->CreateTexturesForOutput(m_resources, SRV, UAV, DXGI_FORMAT_R8G8B8A8_UNORM);
}

void StreamingDataManager::ReleaseBlockInBYTE4Depository(int idDepository, int idBlock)
{
	m_dataByte4Depository[idDepository]->ReleaseBlock(idBlock);
}

VBYTE4 StreamingDataManager::GetValueFromBYTE4Depository(int idDepository, int idBlock, float x, float y)
{
	return m_dataByte4Depository[idDepository]->GetValue(idBlock, x, y);
}

void StreamingDataManager::SetValueFromBYTE4Depository(int idDepository, int idBlock, int x, int y, VBYTE4 value)
{
	m_dataByte4Depository[idDepository]->SetValue(idBlock, x, y, value);
}

VBYTE4 *StreamingDataManager::CreateNewMapFromBYTE4Depository(int idDepository, int idBlock, float lvl, XMFLOAT2 offset, float seam, float height, float width)
{
	return m_dataByte4Depository[idDepository]->CreateNewMapFrom(idBlock, lvl, offset, seam, height, width);
}

int StreamingDataManager::GetWidthFromBYTE4Depository(int idDepository)
{
	return m_dataByte4Depository[idDepository]->GetWidth();
}

int StreamingDataManager::GetHeightFromBYTE4Depository(int idDepository)
{
	return m_dataByte4Depository[idDepository]->GetHeight();
}

int StreamingDataManager::CreateINT4Depository(int level, int width, int height)
{
	int id = m_dataInt4DepositoryReg->TakeElement();
	m_dataInt4Depository[id] = new DataDepositoryINT4(level, width, height);

	return id;
}

void StreamingDataManager::DeleteINT4Depository(int id)
{
	m_dataInt4DepositoryReg->ReturnElement(id);
	delete m_dataInt4Depository[id];
}

int StreamingDataManager::CreateNewBlockInINT4Depository(int idDepository, void *data)
{
	int id = m_dataInt4Depository[idDepository]->CreateNewBlock(data);

	return id;
}

void StreamingDataManager::ReleaseBlockInINT4Depository(int idDepository, int idBlock)
{
	m_dataInt4Depository[idDepository]->ReleaseBlock(idBlock);
}

XMINT4 StreamingDataManager::GetValueFromINT4Depository(int idDepository, int idBlock, float x, float y)
{
	return m_dataInt4Depository[idDepository]->GetValue(idBlock, x, y);
}

void StreamingDataManager::SetValueFromINT4Depository(int idDepository, int idBlock, int x, int y, XMINT4 value)
{
	m_dataInt4Depository[idDepository]->SetValue(idBlock, x, y, value);
}

XMINT4 *StreamingDataManager::GetTextureFromINT4Depository(int idDepository, int idBlock)
{
	return (XMINT4*)m_dataInt4Depository[idDepository]->GetTexture(idBlock);
}

void StreamingDataManager::SetTextureFromINT4Depository(int idDepository, int idBlock, XMINT4 *data)
{
	m_dataInt4Depository[idDepository]->SetTexture(idBlock, data);
}

XMINT4 *StreamingDataManager::CreateNewMapFromINT4Depository(int idDepository, int idBlock, float lvl, XMFLOAT2 offset, float seam, float height, float width)
{
	return m_dataInt4Depository[idDepository]->CreateNewMapFrom(idBlock, lvl, offset, seam, height, width);
}

int StreamingDataManager::GetWidthFromINT4Depository(int idDepository)
{
	return m_dataInt4Depository[idDepository]->GetWidth();
}

int StreamingDataManager::GetHeightFromINT4Depository(int idDepository)
{
	return m_dataInt4Depository[idDepository]->GetHeight();
}

int StreamingDataManager::CreateFLOAT4Depository(int level, int width, int height)
{
	int id = m_dataFloat4DepositoryReg->TakeElement();
	m_dataFloat4Depository[id] = new DataDepositoryFLOAT4(level, width, height);

	return id;
}

void StreamingDataManager::DeleteFLOAT4Depository(int id)
{
	m_dataFloat4DepositoryReg->ReturnElement(id);
	delete m_dataFloat4Depository[id];
}

int StreamingDataManager::CreateNewBlockInFLOAT4Depository(int idDepository, void *data)
{
	int id = m_dataFloat4Depository[idDepository]->CreateNewBlock(data);

	return id;
}

void StreamingDataManager::ReleaseBlockInFLOAT4Depository(int idDepository, int idBlock)
{
	m_dataFloat4Depository[idDepository]->ReleaseBlock(idBlock);
}

XMFLOAT4 StreamingDataManager::GetValueFromFLOAT4Depository(int idDepository, int idBlock, float x, float y)
{
	return m_dataFloat4Depository[idDepository]->GetValue(idBlock, x, y);
}

void StreamingDataManager::SetValueFromFLOAT4Depository(int idDepository, int idBlock, int x, int y, XMFLOAT4 value)
{
	m_dataFloat4Depository[idDepository]->SetValue(idBlock, x, y, value);
}

XMFLOAT4 *StreamingDataManager::GetTextureFromFLOAT4Depository(int idDepository, int idBlock)
{
	return (XMFLOAT4*)m_dataFloat4Depository[idDepository]->GetTexture(idBlock);
}
void StreamingDataManager::SetTextureFromFLOAT4Depository(int idDepository, int idBlock, XMFLOAT4 *data)
{
	m_dataFloat4Depository[idDepository]->SetTexture(idBlock, data);
}

XMFLOAT4 *StreamingDataManager::CreateNewMapFromFLOAT4Depository(int idDepository, int idBlock, float lvl, XMFLOAT2 offset, float seam, float height, float width)
{
	return m_dataFloat4Depository[idDepository]->CreateNewMapFrom(idBlock, lvl, offset, seam, height, width);
}

int StreamingDataManager::GetWidthFromFLOAT4Depository(int idDepository)
{
	return m_dataFloat4Depository[idDepository]->GetWidth();
}

int StreamingDataManager::GetHeightFromFLOAT4Depository(int idDepository)
{
	return m_dataFloat4Depository[idDepository]->GetHeight();
}