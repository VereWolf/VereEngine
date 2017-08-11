#include "pch.h"
#include "TerrainLOD.h"
#include "RenderDevice.h"


void TerrainRenderMessage::Use()
{

	RenderMessage::Use();
	btScalar S = m_Spacing1;
	btScalar S2 = m_Spacing2;

	btTransform mesh = btTransform(btMatrix3x3(S * S2, 0.0, 0.0, 0.0, S * S2, 0.0, 0.0, 0.0, S * S2), m_Position + btVector3(0.0, 0.5 * S2, 0.0));
	mesh = btTransform(m_AngleMatrix) * mesh;
	btTransform meshCentre = m_CameraOffset * m_Transform;
	mesh = meshCentre * mesh;

	btTransform meshN = btTransform(btMatrix3x3(S * S2, 0.0, 0.0, 0.0, S * S2, 0.0, 0.0, 0.0, S * S2), m_Position + btVector3(0.0, 0.5 * S2, 0.0));
	meshN = btTransform(m_Transform.getBasis()) * meshN;

	XMFLOAT3 EyePos = { 0.0f, 0.0f, 0.0f };

	((TerrainLODEffect*)m_BaseEffect)->SetWorld(XMLoadFloat4x4(&VereMath::ConvertToXMFLOAT4X4(mesh)));
	((TerrainLODEffect*)m_BaseEffect)->SetWorldN(XMLoadFloat4x4(&VereMath::ConvertToXMFLOAT4X4(meshN)));
	((TerrainLODEffect*)m_BaseEffect)->SetSpacing(((float)S2));
	((TerrainLODEffect*)m_BaseEffect)->SetLevel(1.0f / pow(2, m_Level));
	((TerrainLODEffect*)m_BaseEffect)->SetOffset(XMFLOAT2(m_Coord.x, m_Coord.y));
	((TerrainLODEffect*)m_BaseEffect)->SetTang(XMFLOAT3(m_Tangent.getX(), m_Tangent.getY(), m_Tangent.getZ()));

	((TerrainLODEffect*)m_BaseEffect)->SetInverseSide(XMLoadFloat3x3(&VereMath::ConvertToXMFLOAT3X3(m_InverseSide)));
	((TerrainLODEffect*)m_BaseEffect)->SetSide(m_Side);

	((TerrainLODEffect*)m_BaseEffect)->SetHeightMap(m_HeightSRV);
	((TerrainLODEffect*)m_BaseEffect)->SetNormalMap(m_NormalSRV);
}