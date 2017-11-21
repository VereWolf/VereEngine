#pragma once

#include "pch.h"

class GenerateHeightAndNormalMapWithNoiseMessage : public ComputeMessage
{
public:
	void Use();

	XMFLOAT2 m_StartPos;
	XMFLOAT2 m_StepSize;
	float m_SpacingMap;
	float m_SpacingWorld;
	float m_RangeNoise;
	bool m_IsMap1;
	bool m_IsMap2;

	ID3D11ShaderResourceView *m_inputHeightMapSRV;
	ID3D11ShaderResourceView *m_inputNormalMapSRV;
	ID3D11ShaderResourceView *m_inputRiverWidth1MapSRV;
	ID3D11ShaderResourceView *m_inputRiverWidth2MapSRV;
	ID3D11ShaderResourceView *m_inputRiverLength1MapSRV;
	ID3D11ShaderResourceView *m_inputRiverLength2MapSRV;
	ID3D11ShaderResourceView *m_inputRiverType1MapSRV;
	ID3D11ShaderResourceView *m_inputRiverType2MapSRV;
	ID3D11ShaderResourceView *m_inputRiverHeightMapSRV;
	ID3D11ShaderResourceView *m_tileSRV;
	ID3D11UnorderedAccessView *m_outputHeightMapUAV;
	ID3D11UnorderedAccessView *m_outputNormalMapUAV;
};