#pragma once

#include "pch.h"
#include "DeviceResources.h"
#include "GameBaseObjectsStack.h"
#include "GameStreamingData.h"
#include "GameDataSet.h"
#include "DataDepository.h"


class StreamingDataManager
{
public:
	StreamingDataManager();
	StreamingDataManager(DX::DeviceResources *resources,
		int levelsOfByteDepository, int levelsOfIntDepository, int levelsOfFloatDepository,
		int levelsOfByte4Depository, int levelsOfInt4Depository, int levelsOfFloat4Depository);
	~StreamingDataManager();

	void Init(DX::DeviceResources *resources,
		int levelsOfByteDepository, int levelsOfIntDepository, int levelsOfFloatDepository,
		int levelsOfByte4Depository, int levelsOfInt4Depository, int levelsOfFloat4Depository);

	int LoadData(string nameFile);
	bool SaveData(string nameFile, int id);

	int CreateStreamingData(void *data, int size);
	void DeleteStreamingData(int id);
	void  *GetStreamingData(int id);
	int GetSizeOfStreamingData(int id);

	int CreateBYTEDepository(int level, int width, int height);
	void DeleteBYTEDepository(int id);
	int CreateNewBlockInBYTEDepository(int idDepository, void *data);
	void ReleaseBlockInBYTEDepository(int idDepository, int idBlock);
	BYTE GetValueFromBYTEDepository(int idDepository, int idBlock, float x, float y);
	void SetValueFromBYTEDepository(int idDepository, int idBlock, int x, int y, BYTE value);
	BYTE *GetTextureFromBYTEDepository(int idDepository, int idBlock);
	void SetTextureFromBYTEDepository(int idDepository, int idBlock, BYTE *data);
	BYTE *CreateNewMapFromBYTEDepository(int idDepository, int idBlock, float lvl, XMFLOAT2 offset, float seam, float height, float width);
	int GetWidthFromBYTEDepository(int idDepository);
	int GetHeightFromBYTEDepository(int idDepository);

	int CreateINTDepository(int level, int width, int height);
	void DeleteINTDepository(int id);
	int CreateNewBlockInINTDepository(int idDepository, void *data);
	void ReleaseBlockInINTDepository(int idDepository, int idBlock);
	int GetValueFromINTDepository(int idDepository, int idBlock, float x, float y);
	void SetValueFromINTDepository(int idDepository, int idBlock, int x, int y, int value);
	int *GetTextureFromINTDepository(int idDepository, int idBlock);
	void SetTextureFromINTDepository(int idDepository, int idBlock, int *data);
	int *CreateNewMapFromINTDepository(int idDepository, int idBlock, float lvl, XMFLOAT2 offset, float seam, float height, float width);
	int GetWidthFromINTDepository(int idDepository);
	int GetHeightFromINTDepository(int idDepository);

	int CreateFLOATDepository(int level, int width, int height);
	void DeleteFLOATDepository(int id);
	int CreateNewBlockInFLOATDepository(int idDepository, void *data);
	void ReleaseBlockInFLOATDepository(int idDepository, int idBlock);
	float GetValueFromFLOATDepository(int idDepository, int idBlock, float x, float y);
	void SetValueFromFLOATDepository(int idDepository, int idBlock, int x, int y, float value);
	float *GetTextureFromFLOATDepository(int idDepository, int idBlock);
	void SetTextureFromFLOATDepository(int idDepository, int idBlock, float *data);
	float *CreateNewMapFromFLOATDepository(int idDepository, int idBlock, float lvl, XMFLOAT2 offset, float seam, float height, float width);
	int GetWidthFromFLOATDepository(int idDepository);
	int GetHeightFromFLOATDepository(int idDepository);

	int CreateBYTE4Depository(int level, int width, int height);
	void DeleteBYTE4Depository(int id);
	int CreateNewBlockInBYTE4Depository(int idDepository, void *data);
	void ReleaseBlockInBYTE4Depository(int idDepository, int idBlock);
	VBYTE4 GetValueFromBYTE4Depository(int idDepository, int idBlock, float x, float y);
	void SetValueFromBYTE4Depository(int idDepository, int idBlock, int x, int y, VBYTE4 value);
	VBYTE4 *GetTextureFromBYTE4Depository(int idDepository, int idBlock);
	void SetTextureFromBYTE4Depository(int idDepository, int idBlock, VBYTE4 *data);
	VBYTE4 *CreateNewMapFromBYTE4Depository(int idDepository, int idBlock, float lvl, XMFLOAT2 offset, float seam, float height, float width);
	int GetWidthFromBYTE4Depository(int idDepository);
	int GetHeightFromBYTE4Depository(int idDepository);

	int CreateINT4Depository(int level, int width, int height);
	void DeleteINT4Depository(int id);
	int CreateNewBlockInINT4Depository(int idDepository, void *data);
	void ReleaseBlockInINT4Depository(int idDepository, int idBlock);
	XMINT4 GetValueFromINT4Depository(int idDepository, int idBlock, float x, float y);
	void SetValueFromINT4Depository(int idDepository, int idBlock, int x, int y, XMINT4 value);
	XMINT4 *GetTextureFromINT4Depository(int idDepository, int idBlock);
	void SetTextureFromINT4Depository(int idDepository, int idBlock, XMINT4 *data);
	XMINT4 *CreateNewMapFromINT4Depository(int idDepository, int idBlock, float lvl, XMFLOAT2 offset, float seam, float height, float width);
	int GetWidthFromINT4Depository(int idDepository);
	int GetHeightFromINT4Depository(int idDepository);

	int CreateFLOAT4Depository(int level, int width, int height);
	void DeleteFLOAT4Depository(int id);
	int CreateNewBlockInFLOAT4Depository(int idDepository, void *data);
	void ReleaseBlockInFLOAT4Depository(int idDepository, int idBlock);
	XMFLOAT4 GetValueFromFLOAT4Depository(int idDepository, int idBlock, float x, float y);
	void SetValueFromFLOAT4Depository(int idDepository, int idBlock, int x, int y, XMFLOAT4 value);
	XMFLOAT4 *GetTextureFromFLOAT4Depository(int idDepository, int idBlock);
	void SetTextureFromFLOAT4Depository(int idDepository, int idBlock, XMFLOAT4 *data);
	XMFLOAT4 *CreateNewMapFromFLOAT4Depository(int idDepository, int idBlock, float lvl, XMFLOAT2 offset, float seam, float height, float width);
	int GetWidthFromFLOAT4Depository(int idDepository);
	int GetHeightFromFLOAT4Depository(int idDepository);
private:
	DX::DeviceResources *m_resources;

	StreamingDataAssetsStacks m_streamingData;

	IDRegistr *m_dataByteDepositoryReg;
	std::vector<DataDepositoryBYTE*> m_dataByteDepository;

	IDRegistr *m_dataIntDepositoryReg;
	std::vector<DataDepositoryINT*> m_dataIntDepository;

	IDRegistr *m_dataFloatDepositoryReg;
	std::vector<DataDepositoryFLOAT*> m_dataFloatDepository;

	IDRegistr *m_dataByte4DepositoryReg;
	std::vector<DataDepositoryBYTE4*> m_dataByte4Depository;

	IDRegistr *m_dataInt4DepositoryReg;
	std::vector<DataDepositoryINT4*> m_dataInt4Depository;

	IDRegistr *m_dataFloat4DepositoryReg;
	std::vector<DataDepositoryFLOAT4*> m_dataFloat4Depository;

	Windows::Foundation::IAsyncAction^ m_Worker;
};

__declspec(selectany) StreamingDataManager *GameStreamingDataHandle = 0;

