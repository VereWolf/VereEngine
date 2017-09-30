#include "GenerateBlockOfLOD.h"

void GenerateBlockOfLODMessage::Use()
{
	((GenerateBlockOfLODEffect*)m_Effect)->SetOffset(m_offset);
	((GenerateBlockOfLODEffect*)m_Effect)->SetScaling(m_scaling);

	((GenerateBlockOfLODEffect*)m_Effect)->SetInputHeightMap(m_inputHeightMapSRV);
	((GenerateBlockOfLODEffect*)m_Effect)->SetInputNormalMap(m_inputNormalMapSRV);
	((GenerateBlockOfLODEffect*)m_Effect)->SetInputEnviromentMap(m_inputEnviromentMapSRV);
	((GenerateBlockOfLODEffect*)m_Effect)->SetInputTreesMap(m_inputTreesMapSRV);
	((GenerateBlockOfLODEffect*)m_Effect)->SetInputTileMap(m_tileSRV);
	((GenerateBlockOfLODEffect*)m_Effect)->SetOutputHeightMap(m_outputHeightMapUAV);
	((GenerateBlockOfLODEffect*)m_Effect)->SetOutputNormalMap(m_outputNormalMapUAV);
	((GenerateBlockOfLODEffect*)m_Effect)->SetOutputAngleMap(m_outputAngleMapUAV);
	((GenerateBlockOfLODEffect*)m_Effect)->SetOutputEnviromentMap(m_outputEnviromentMapUAV);
	((GenerateBlockOfLODEffect*)m_Effect)->SetOutputTreesMap(m_outputTreesMapUAV);
}