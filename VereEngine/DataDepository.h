#pragma once
#include "pch.h"
#include "DeviceResources.h"

enum
{
	e_byte,
	e_int,
	e_float,
	e_byte4,
	e_int4,
	e_float4
};

class CreateTextureFromTextureMessage : public ComputeMessage
{
public:
	void Use();

	XMFLOAT2 m_startPos;
	XMFLOAT2 m_stepSize;

	ID3D11ShaderResourceView *m_inputSRV;
	ID3D11UnorderedAccessView *m_outputUAV;
};

class DataDepository
{
public:
	DataDepository() {}
	DataDepository(int level, int width, int height) {}
	~DataDepository();

	void Init(int level, int width, int height, int elementTypeInVector);

	bool CreateTexturesForInput(DX::DeviceResources* resources, int id, ID3D11ShaderResourceView **inputSRV, DXGI_FORMAT format);
	bool CreateTexturesForOutput(DX::DeviceResources* resources, ID3D11ShaderResourceView **outputSRV, ID3D11UnorderedAccessView **outputUAV, DXGI_FORMAT format);

	int CreateNewBlock(void *data);
	void ReleaseBlock(int id) { m_idStack->ReturnElement(id); }

	void *GetTexture(int id) { return &((BYTE*)m_data)[m_stride * id]; }
	void SetTexture(int id, void *data);

	int GetWidth() { return m_width; }
	int GetHeight() { return m_height; }

protected:
	inline float GetValue(int id, float x, float y, int elementInVector);

private:
	IDRegistr *m_idStack;
	void *m_data;

	int m_level;
	int m_width;
	int m_height;
	int m_elementInVectorSize;
	int m_numElementInVector;
	int m_elementType;
	int m_elementSize;
	int m_stride;
	int m_size;
};

class DataDepositoryBYTE : public DataDepository
{
public:
	DataDepositoryBYTE(int level, int width, int height);

	BYTE GetValue(int id, float x, float y);
	void SetValue(int id, int x, int y, BYTE value) { ((BYTE*)GetTexture(id))[y * GetWidth() + x] = value; }
	BYTE *CreateNewMapFrom(int id, float lvl, XMFLOAT2 offset, float seam, float height, float width);
};

class DataDepositoryINT : public DataDepository
{
public:
	DataDepositoryINT(int level, int width, int height);

	int GetValue(int id, float x, float y);
	void SetValue(int id, int x, int y, int value) { ((int*)GetTexture(id))[y * GetWidth() + x] = value; }
	int *CreateNewMapFrom(int id, float lvl, XMFLOAT2 offset, float seam, float height, float width);
};

class DataDepositoryFLOAT : public DataDepository
{
public:
	DataDepositoryFLOAT(int level, int width, int height);

	float GetValue(int id, float x, float y);
	void SetValue(int id, int x, int y, float value) { ((float*)GetTexture(id))[y * GetWidth() + x] = value; }
	float *CreateNewMapFrom(int id, float lvl, XMFLOAT2 offset, float seam, float height, float width);
};

class DataDepositoryBYTE4 : public DataDepository
{
public:
	DataDepositoryBYTE4(int level, int width, int height);

	VBYTE4 GetValue(int id, float x, float y);
	void SetValue(int id, int x, int y, VBYTE4 value) { ((VBYTE4*)GetTexture(id))[y * GetWidth() + x] = value; }
	VBYTE4 *CreateNewMapFrom(int id, float lvl, XMFLOAT2 offset, float seam, float height, float width);
};

class DataDepositoryINT4 : public DataDepository
{
public:
	DataDepositoryINT4(int level, int width, int height);

	XMINT4 GetValue(int id, float x, float y);
	void SetValue(int id, int x, int y, XMINT4 value) { ((XMINT4*)GetTexture(id))[y * GetWidth() + x] = value; }
	XMINT4 *CreateNewMapFrom(int id, float lvl, XMFLOAT2 offset, float seam, float height, float width);
};

class DataDepositoryFLOAT4 : public DataDepository
{
public:
	DataDepositoryFLOAT4(int level, int width, int height);

	XMFLOAT4 GetValue(int id, float x, float y);
	void SetValue(int id, int x, int y, XMFLOAT4 value) { ((XMFLOAT4*)GetTexture(id))[y * GetWidth() + x] = value; }
	XMFLOAT4 *CreateNewMapFrom(int id, float lvl, XMFLOAT2 offset, float seam, float height, float width);
};
