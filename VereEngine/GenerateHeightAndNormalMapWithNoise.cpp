#include "GenerateHeightAndNormalMapWithNoise.h"

void GenerateHeightAndNormalMapWithNoiseMessage::Use()
{
	((GenerateHeightAndNormalMapWithNoiseEffect*)m_Effect)->SetStartPos(m_StartPos);
	((GenerateHeightAndNormalMapWithNoiseEffect*)m_Effect)->SetStepSize(m_StepSize);
	((GenerateHeightAndNormalMapWithNoiseEffect*)m_Effect)->SetSpacingMap(m_SpacingMap);
	((GenerateHeightAndNormalMapWithNoiseEffect*)m_Effect)->SetSpacingWorld(m_SpacingWorld);
	((GenerateHeightAndNormalMapWithNoiseEffect*)m_Effect)->SetRangeNoise(m_RangeNoise);
	((GenerateHeightAndNormalMapWithNoiseEffect*)m_Effect)->SetIsMap1(m_IsMap1);
	((GenerateHeightAndNormalMapWithNoiseEffect*)m_Effect)->SetIsMap2(m_IsMap2);

	((GenerateHeightAndNormalMapWithNoiseEffect*)m_Effect)->SetInputHeightMap(m_inputHeightMapSRV);
	((GenerateHeightAndNormalMapWithNoiseEffect*)m_Effect)->SetInputNormalMap(m_inputNormalMapSRV);
	((GenerateHeightAndNormalMapWithNoiseEffect*)m_Effect)->SetTileMap(m_tileSRV);

	if (m_IsMap1)
	{
		((GenerateHeightAndNormalMapWithNoiseEffect*)m_Effect)->SetInputRiverWidth1Map(m_inputRiverWidth1MapSRV);
		((GenerateHeightAndNormalMapWithNoiseEffect*)m_Effect)->SetInputRiverLength1Map(m_inputRiverLength1MapSRV);
		((GenerateHeightAndNormalMapWithNoiseEffect*)m_Effect)->SetInputRiverType1Map(m_inputRiverType1MapSRV);
	}

	if (m_IsMap2)
	{
		((GenerateHeightAndNormalMapWithNoiseEffect*)m_Effect)->SetInputRiverWidth2Map(m_inputRiverWidth2MapSRV);
		((GenerateHeightAndNormalMapWithNoiseEffect*)m_Effect)->SetInputRiverLength2Map(m_inputRiverLength2MapSRV);
		((GenerateHeightAndNormalMapWithNoiseEffect*)m_Effect)->SetInputRiverType2Map(m_inputRiverType2MapSRV);
	}

	if (m_IsMap1 || m_IsMap2)
	{
		((GenerateHeightAndNormalMapWithNoiseEffect*)m_Effect)->SetInputRiverHeightMap(m_inputRiverHeightMapSRV);
	}

	((GenerateHeightAndNormalMapWithNoiseEffect*)m_Effect)->SetOutputHeightMap(m_outputHeightMapUAV);
	((GenerateHeightAndNormalMapWithNoiseEffect*)m_Effect)->SetOutputNormalMap(m_outputNormalMapUAV);
}