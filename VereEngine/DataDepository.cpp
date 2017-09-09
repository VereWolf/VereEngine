#include "pch.h"
#include "DataDepository.h"

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
	memcpy(&((char*)m_data)[id * m_stride], data, m_stride);
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
