#include "pch.h"
#include "TerrainLOD.h"
#include "RenderDevice.h"


void TerrainRenderMessage::Use()
{
	btScalar S = m_Spacing1;
	btScalar S2 = m_Spacing2;

	btTransform mesh = btTransform(btMatrix3x3(S * S2, 0.0, 0.0, 0.0, S * S2, 0.0, 0.0, 0.0, S * S2), m_Position + btVector3(0.0, 0.5 * S2, 0.0));
	mesh = btTransform(m_AngleMatrix) * mesh;
	btTransform meshCentre = m_CameraOffset * m_Transform;
	mesh = meshCentre * mesh;

	XMFLOAT3 EyePos = { 0.0f, 0.0f, 0.0f };
	XMFLOAT3 T = XMFLOAT3(m_Tangent.getX(), m_Tangent.getY(), m_Tangent.getZ());

	((TerrainLODEffect*)m_BaseEffect)->SetViewProj(m_View * m_Proj);
	((TerrainLODEffect*)m_BaseEffect)->SetWorld(XMLoadFloat4x4(&VereMath::ConvertToXMFLOAT4X4(mesh)));
	((TerrainLODEffect*)m_BaseEffect)->SetWorldN(XMLoadFloat3x3(&VereMath::ConvertToXMFLOAT3X3(m_Transform.getBasis())));
	((TerrainLODEffect*)m_BaseEffect)->SetCentrePos(VereMath::ConvertToXMFLOAT3(meshCentre.getOrigin()));
	((TerrainLODEffect*)m_BaseEffect)->SetSpacing(((float)S2));
	((TerrainLODEffect*)m_BaseEffect)->SetRadius(((float)m_Radius));
	((TerrainLODEffect*)m_BaseEffect)->SetLevel(1.0f / pow(2, m_Level));
	((TerrainLODEffect*)m_BaseEffect)->SetOffset(XMFLOAT2(m_Coord.x, m_Coord.y));
	((TerrainLODEffect*)m_BaseEffect)->SetHeightMap(m_HeightSRV);
	((TerrainLODEffect*)m_BaseEffect)->SetNormalMap(m_NormalSRV);
	((TerrainLODEffect*)m_BaseEffect)->SetTang(T);
	((TerrainLODEffect*)m_BaseEffect)->SetFarZ(m_FarZ);
	((TerrainLODEffect*)m_BaseEffect)->SetFarRangeMod(m_FarRangeMod);
	((TerrainLODEffect*)m_BaseEffect)->SetFarModifier(m_FarModifier);
	((TerrainLODEffect*)m_BaseEffect)->SetCenterOfPlanet(VereMath::ConvertToXMFLOAT3(meshCentre.getOrigin()));
	((TerrainLODEffect*)m_BaseEffect)->SetFogStart(500.0f);
	((TerrainLODEffect*)m_BaseEffect)->SetFogRange(10000.0f);
	((TerrainLODEffect*)m_BaseEffect)->SetFogColor(XMFLOAT3(0.1f, 0.3f, 0.9f));
}