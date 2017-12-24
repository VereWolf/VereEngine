#pragma once

#include "pch.h"

class GenerateBlockOfLODMessage : public ComputeMessage
{
public:
	void Use();

	float m_offset;
	float m_scaling;

	ID3D11ShaderResourceView *m_inputHeightMapSRV;
	ID3D11ShaderResourceView *m_inputNormalMapSRV;
	ID3D11ShaderResourceView *m_inputEnviromentMapSRV;
	ID3D11ShaderResourceView *m_inputTreesMapSRV;
	ID3D11ShaderResourceView *m_tileSRV;
	ID3D11UnorderedAccessView *m_outputAngleMapUAV;
	ID3D11UnorderedAccessView *m_outputEnviromentMapUAV;
	ID3D11UnorderedAccessView *m_outputTreesMapUAV;
};
