#include "pch.h"
#include "DataDepository.h"

void CreateTextureFromTextureMessage::Use()
{
	((GenerateTexturesFromTextureEffect*)m_Effect)->SetStartPos(m_startPos);
	((GenerateTexturesFromTextureEffect*)m_Effect)->SetStepSize(m_stepSize);

	((GenerateTexturesFromTextureEffect*)m_Effect)->SetInputMap(m_inputSRV);
	((GenerateTexturesFromTextureEffect*)m_Effect)->SetOutputMap(m_outputUAV);
}

DataDepository::~DataDepository()
{
	delete m_idStack;
	delete m_data;
};

void DataDepository::Init(int level, int width, int height, int elementType)
{
	m_level = level;
	m_width = width;
	m_height = height;
	if (e_byte == elementType || e_int == elementType || e_float == elementType) m_numElementInVector = 1;
	if (e_byte4 == elementType || e_int4 == elementType || e_float4 == elementType) m_numElementInVector = 4;
	m_elementType = elementType;
	if (e_byte == elementType || e_byte4 == elementType)	m_elementInVectorSize = sizeof(BYTE);
	if (e_int == elementType || e_int4 == elementType)	m_elementInVectorSize = sizeof(int);
	if (e_float == elementType || e_float4 == elementType)	m_elementInVectorSize = sizeof(float);
	m_elementSize = m_numElementInVector * m_elementInVectorSize;
	m_stride = width * height * m_elementSize;
	m_size = m_stride * pow(2, level);

	m_idStack = new IDRegistr(level);
	m_data = new char[m_size];
};

bool  DataDepository::CreateTexturesForInput(DX::DeviceResources* resources, int id, ID3D11ShaderResourceView **inputSRV, DXGI_FORMAT format)
{

	D3D11_TEXTURE2D_DESC inputTexDesc;
	ZeroMemory(&inputTexDesc, sizeof(D3D11_TEXTURE2D_DESC));
	inputTexDesc.Width = m_width;
	inputTexDesc.Height = m_height;
	inputTexDesc.MipLevels = 1;
	inputTexDesc.ArraySize = 1;
	inputTexDesc.Format = format;
	inputTexDesc.SampleDesc.Count = 1;
	inputTexDesc.SampleDesc.Quality = 0;
	inputTexDesc.Usage = D3D11_USAGE_DEFAULT;
	inputTexDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	inputTexDesc.CPUAccessFlags = 0;
	inputTexDesc.MiscFlags = 0;

	ID3D11Texture2D* inputTex = 0;

	D3D11_SUBRESOURCE_DATA sbd;
	ZeroMemory(&sbd, sizeof(D3D11_SUBRESOURCE_DATA));

<<<<<<< HEAD
	sbd.pSysMem = GetTexture(id);
=======
>>>>>>> master
	if (format == DXGI_FORMAT_R8G8B8A8_UNORM)
	{
		sbd.SysMemPitch = sizeof(VBYTE4) * m_width;
	}
	else if (format == DXGI_FORMAT_R32_FLOAT)
	{
		sbd.SysMemPitch = sizeof(float) * m_width;
	}
<<<<<<< HEAD
=======
	if (id < 0)
	{
		void *M = new char[sbd.SysMemPitch * m_height];
		ZeroMemory(M, sbd.SysMemPitch * m_height);
		sbd.pSysMem = M;
	}
	else
	{
		sbd.pSysMem = GetTexture(id);
	}
>>>>>>> master

	sbd.SysMemSlicePitch = 0;

	HR(resources->GetD3DDevice()->CreateTexture2D(&inputTexDesc, &sbd, &inputTex));

	D3D11_SHADER_RESOURCE_VIEW_DESC isrvDesc;
	ZeroMemory(&isrvDesc, sizeof(D3D11_SHADER_RESOURCE_VIEW_DESC));
	isrvDesc.Format = format;
	isrvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	isrvDesc.Texture2D.MostDetailedMip = 0;
	isrvDesc.Texture2D.MipLevels = 1;
	HR(resources->GetD3DDevice()->CreateShaderResourceView(inputTex, &isrvDesc, inputSRV));

	ReleaseCOM(inputTex);

	return true;
}

bool DataDepository::CreateTexturesForOutput(DX::DeviceResources* resources, ID3D11ShaderResourceView **outputSRV, ID3D11UnorderedAccessView **outputUAV, DXGI_FORMAT format)
{
	D3D11_TEXTURE2D_DESC outputTexDesc;
	outputTexDesc.Width = m_width;
	outputTexDesc.Height = m_height;
	outputTexDesc.MipLevels = 1;
	outputTexDesc.ArraySize = 1;
	outputTexDesc.Format = format;
	outputTexDesc.SampleDesc.Count = 1;
	outputTexDesc.SampleDesc.Quality = 0;
	outputTexDesc.Usage = D3D11_USAGE_DEFAULT;
	outputTexDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_UNORDERED_ACCESS;
	outputTexDesc.CPUAccessFlags = 0;
	outputTexDesc.MiscFlags = 0;

	ID3D11Texture2D* outputTex = 0;
	HR(resources->GetD3DDevice()->CreateTexture2D(&outputTexDesc, 0, &outputTex));

	D3D11_SHADER_RESOURCE_VIEW_DESC osrvDesc;
	osrvDesc.Format = format;
	osrvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	osrvDesc.Texture2D.MostDetailedMip = 0;
	osrvDesc.Texture2D.MipLevels = 1;
	HR(resources->GetD3DDevice()->CreateShaderResourceView(outputTex, &osrvDesc, outputSRV));

	D3D11_UNORDERED_ACCESS_VIEW_DESC uavDesc;
	uavDesc.Format = format;
	uavDesc.ViewDimension = D3D11_UAV_DIMENSION_TEXTURE2D;
	uavDesc.Texture2D.MipSlice = 0;
	HR(resources->GetD3DDevice()->CreateUnorderedAccessView(outputTex, &uavDesc, outputUAV));

	ReleaseCOM(outputTex);

	return true;
}

int DataDepository::CreateNewBlock(void *data)
{
	int id = m_idStack->TakeElement();

	if (data == NULL) return id;

	memcpy(&((char*)m_data)[id * m_stride], data, m_stride);
	delete data;

	return id;
};

void DataDepository::SetTexture(int id, void *data)
{
	if (id >= 0)
	{
		memcpy(&((char*)m_data)[id * m_stride], data, m_stride);
	}
}

float DataDepository::GetValue(int id, float x, float y, int elementInVector)
{
	int H = m_height;
	int W = m_width;
	int S = m_elementSize;
	int SIV = m_elementInVectorSize;
	int N = m_numElementInVector;
	int O = W * H * id * N;
	int T = m_elementType;

	x *= H;
	y *= W;

	int iX = x;
	int	iY = y;

	float fX = x - iX;
	float fY = y - iY;

	int v11 = O + VereMath::Wrap(iY, H) * W * N + VereMath::Wrap(iX, W) * N + elementInVector;
	int v12 = O + VereMath::Wrap(iY, H) * W * N + VereMath::Wrap((iX + 1), W) * N + elementInVector;
	int v21 = O + VereMath::Wrap((iY + 1), H) * W * N + VereMath::Wrap(iX, W) * N + elementInVector;
	int v22 = O + VereMath::Wrap((iY + 1), H) * W * N + VereMath::Wrap((iX + 1), W) * N + elementInVector;

	float f11;
	float f12;
	float f21;
	float f22;

	if (e_byte == T || e_byte4 == T)
	{
		f11 = (float)((BYTE*)m_data)[v11];
		f12 = (float)((BYTE*)m_data)[v12];
		f21 = (float)((BYTE*)m_data)[v21];
		f22 = (float)((BYTE*)m_data)[v22];
	}

	if (e_int == T || e_int4 == T)
	{
		f11 = (float)((int*)m_data)[v11];
		f12 = (float)((int*)m_data)[v12];
		f21 = (float)((int*)m_data)[v21];
		f22 = (float)((int*)m_data)[v22];
	}

	if (e_float == T || e_float4 == T)
	{
		f11 = ((float*)m_data)[v11];
		f12 = ((float*)m_data)[v12];
		f21 = ((float*)m_data)[v21];
		f22 = ((float*)m_data)[v22];
	}

	float f1 = f11 + fX * (f12 - f11);
	float f2 = f21 + fX * (f22 - f21);

	return(f1 + fY * (f2 - f1));
};

DataDepositoryBYTE::DataDepositoryBYTE(int level, int width, int height)
{
	Init(level, width, height, e_byte);
};

BYTE DataDepositoryBYTE::GetValue(int id, float x, float y)
{
	return (BYTE)DataDepository::GetValue(id, x, y, 0);
};

BYTE *DataDepositoryBYTE::CreateNewMapFrom(int id, float lvl, XMFLOAT2 offset, float seam, float height, float width)
{
	float S = (height + seam) * (width + seam);
	BYTE *M = new BYTE[S];
	float L2 = 1.0f / pow(2, lvl);
	float H2 = 1.0f / (height + seam);
	float W2 = 1.0f / (width + seam);
	float OC = (height) / (height + seam);

	XMFLOAT2 OF = XMFLOAT2(offset.x * L2 * OC, offset.y * L2 * OC);
	XMFLOAT2 INC = XMFLOAT2(L2 * W2, L2 * H2);

	XMFLOAT2 I = XMFLOAT2(OF.x + W2 - INC.x, OF.y + H2 - INC.y);

	for (int y = 0; y < (height + seam); ++y)
	{
		for (int x = 0; x < (width + seam); ++x)
		{
			M[y * (int)(width + seam) + x] = GetValue(id, I.x, I.y);

			I.x += INC.x;
		}

		I.x = OF.x + H2 - INC.y;
		I.y += INC.y;
	}

	return M;
};

DataDepositoryINT::DataDepositoryINT(int level, int width, int height)
{
	Init(level, width, height, e_int);
};

int DataDepositoryINT::GetValue(int id, float x, float y)
{
	return (int)DataDepository::GetValue(id, x, y, 0);
};

int *DataDepositoryINT::CreateNewMapFrom(int id, float lvl, XMFLOAT2 offset, float seam, float height, float width)
{
	float S = (height + seam) * (width + seam);
	int *M = new int[S];
	float L2 = 1.0f / pow(2, lvl);
	float H2 = 1.0f / (height + seam);
	float W2 = 1.0f / (width + seam);
	float OC = (height) / (height + seam);

	XMFLOAT2 OF = XMFLOAT2(offset.x * L2 * OC, offset.y * L2 * OC);
	XMFLOAT2 INC = XMFLOAT2(L2 * W2, L2 * H2);

	XMFLOAT2 I = XMFLOAT2(OF.x + W2 - INC.x, OF.y + H2 - INC.y);

	for (int y = 0; y < (height + seam); ++y)
	{
		for (int x = 0; x < (width + seam); ++x)
		{
			M[y * (int)(width + seam) + x] = GetValue(id, I.x, I.y);

			I.x += INC.x;
		}

		I.x = OF.x + H2 - INC.y;
		I.y += INC.y;
	}

	return M;
};

DataDepositoryFLOAT::DataDepositoryFLOAT(int level, int width, int height)
{
	Init(level, width, height, e_float);
};

float DataDepositoryFLOAT::GetValue(int id, float x, float y)
{
	return (float)DataDepository::GetValue(id, x, y, 0);
};

float *DataDepositoryFLOAT::CreateNewMapFrom(int id, float lvl, XMFLOAT2 offset, float seam, float height, float width)
{
	float S = (height + seam) * (width + seam);
	float *M = new float[S];
	float L2 = 1.0f / pow(2, lvl);
	float H2 = 1.0f / (height + seam);
	float W2 = 1.0f / (width + seam);
	float OC = (height) / (height + seam);

	XMFLOAT2 OF = XMFLOAT2(offset.x * L2 * OC, offset.y * L2 * OC);
	XMFLOAT2 INC = XMFLOAT2(L2 * W2, L2 * H2);

	XMFLOAT2 I = XMFLOAT2(OF.x + W2 - INC.x, OF.y + H2 - INC.y);

	for (int y = 0; y < (height + seam); ++y)
	{
		for (int x = 0; x < (width + seam); ++x)
		{
			M[y * (int)(width + seam) + x] = GetValue(id, I.x, I.y);

			I.x += INC.x;
		}

		I.x = OF.x + H2 - INC.y;
		I.y += INC.y;
	}

	return M;
};

DataDepositoryBYTE4::DataDepositoryBYTE4(int level, int width, int height)
{
	Init(level, width, height, e_byte4);
};

VBYTE4 DataDepositoryBYTE4::GetValue(int id, float x, float y)
{
	return VBYTE4(
		(BYTE)DataDepository::GetValue(id, x, y, 0),
		(BYTE)DataDepository::GetValue(id, x, y, 1),
		(BYTE)DataDepository::GetValue(id, x, y, 2),
		(BYTE)DataDepository::GetValue(id, x, y, 3));
};

VBYTE4 *DataDepositoryBYTE4::CreateNewMapFrom(int id, float lvl, XMFLOAT2 offset, float seam, float height, float width)
{
	float S = (height + seam) * (width + seam);
	VBYTE4 *M = new VBYTE4[S];
	float L2 = 1.0f / pow(2, lvl);
	float H2 = 1.0f / (height + seam);
	float W2 = 1.0f / (width + seam);
	float OC = (height) / (height + seam);

	XMFLOAT2 OF = XMFLOAT2(offset.x * L2 * OC, offset.y * L2 * OC);
	XMFLOAT2 INC = XMFLOAT2(L2 * W2, L2 * H2);

	XMFLOAT2 I = XMFLOAT2(OF.x + W2 - INC.x, OF.y + H2 - INC.y);

	for (int y = 0; y < (height + seam); ++y)
	{
		for (int x = 0; x < (width + seam); ++x)
		{
			M[y * (int)(width + seam) + x] = GetValue(id, I.x, I.y);

			I.x += INC.x;
		}

		I.x = OF.x + H2 - INC.y;
		I.y += INC.y;
	}

	return M;
};

DataDepositoryINT4::DataDepositoryINT4(int level, int width, int height)
{
	Init(level, width, height, e_int);
};

XMINT4 DataDepositoryINT4::GetValue(int id, float x, float y)
{
	return XMINT4(
		(int)DataDepository::GetValue(id, x, y, 0),
		(int)DataDepository::GetValue(id, x, y, 1),
		(int)DataDepository::GetValue(id, x, y, 2),
		(int)DataDepository::GetValue(id, x, y, 3));
};

XMINT4 *DataDepositoryINT4::CreateNewMapFrom(int id, float lvl, XMFLOAT2 offset, float seam, float height, float width)
{
	float S = (height + seam) * (width + seam);
	XMINT4 *M = new XMINT4[S];
	float L2 = 1.0f / pow(2, lvl);
	float H2 = 1.0f / (height + seam);
	float W2 = 1.0f / (width + seam);
	float OC = (height) / (height + seam);

	XMFLOAT2 OF = XMFLOAT2(offset.x * L2 * OC, offset.y * L2 * OC);
	XMFLOAT2 INC = XMFLOAT2(L2 * W2, L2 * H2);

	XMFLOAT2 I = XMFLOAT2(OF.x + W2 - INC.x, OF.y + H2 - INC.y);

	for (int y = 0; y < (height + seam); ++y)
	{
		for (int x = 0; x < (width + seam); ++x)
		{
			M[y * (int)(width + seam) + x] = GetValue(id, I.x, I.y);

			I.x += INC.x;
		}

		I.x = OF.x + H2 - INC.y;
		I.y += INC.y;
	}

	return M;
};

DataDepositoryFLOAT4::DataDepositoryFLOAT4(int level, int width, int height)
{
	Init(level, width, height, e_float);
};

XMFLOAT4 DataDepositoryFLOAT4::GetValue(int id, float x, float y)
{
	return XMFLOAT4(
		(float)DataDepository::GetValue(id, x, y, 0),
		(float)DataDepository::GetValue(id, x, y, 1),
		(float)DataDepository::GetValue(id, x, y, 2),
		(float)DataDepository::GetValue(id, x, y, 3));
};

XMFLOAT4 *DataDepositoryFLOAT4::CreateNewMapFrom(int id, float lvl, XMFLOAT2 offset, float seam, float height, float width)
{
	float S = (height + seam) * (width + seam);
	
	XMFLOAT4 *M = new XMFLOAT4[S];
	float L2 = 1.0f / pow(2, lvl);
	float H2 = 1.0f / (height + seam);
	float W2 = 1.0f / (width + seam);
	float OC = (height) / (height + seam);

	XMFLOAT2 OF = XMFLOAT2(offset.x * L2 * OC, offset.y * L2 * OC);
	XMFLOAT2 INC = XMFLOAT2(L2 * W2, L2 * H2);

	XMFLOAT2 I = XMFLOAT2(OF.x + W2 - INC.x, OF.y + H2 - INC.y);

	for (int y = 0; y < (height + seam); ++y)
	{
		for (int x = 0; x < (width + seam); ++x)
		{
			M[y * (int)(width + seam) + x] = GetValue(id, I.x, I.y);

			I.x += INC.x;
		}

		I.x = OF.x + H2 - INC.y;
		I.y += INC.y;
	}

	return M;
};
