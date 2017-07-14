#include "pch.h"
#include "WaterLOD.h"
#include "RenderDevice.h"

void WaterRenderMessage::Use()
{
	btScalar S = m_Spacing1;
	btScalar S2 = m_Spacing2;

	btTransform mesh = btTransform(btMatrix3x3(S * S2, 0.0, 0.0, 0.0, S * S2, 0.0, 0.0, 0.0, S * S2), m_Position + btVector3(0.0, 0.5 * S2, 0.0));
	mesh = btTransform(m_AngleMatrix) * mesh;
	btTransform meshCentre = m_CameraOffset * m_Transform;
	mesh = meshCentre * mesh;

	XMFLOAT3 EyePos = { 0.0f, 0.0f, 0.0f };

	((WaterLODEffect*)m_BaseEffect)->SetViewProj(m_View * m_Proj);
	((WaterLODEffect*)m_BaseEffect)->SetWorld(XMLoadFloat4x4(&VereMath::ConvertToXMFLOAT4X4(mesh)));
	((WaterLODEffect*)m_BaseEffect)->SetWorldN(XMLoadFloat3x3(&VereMath::ConvertToXMFLOAT3X3(m_Transform.getBasis())));
	((WaterLODEffect*)m_BaseEffect)->SetCentrePos(VereMath::ConvertToXMFLOAT3(meshCentre.getOrigin()));
	((WaterLODEffect*)m_BaseEffect)->SetSpacing(((float)S2));
	((WaterLODEffect*)m_BaseEffect)->SetRadius(((float)m_Radius));
	((WaterLODEffect*)m_BaseEffect)->SetLevel(1.0f / pow(2, m_Level));
	((WaterLODEffect*)m_BaseEffect)->SetOffset(XMFLOAT2(m_Coord.x, m_Coord.y));
	((WaterLODEffect*)m_BaseEffect)->SetFarZ(m_FarZ);
	((WaterLODEffect*)m_BaseEffect)->SetFarRangeMod(m_FarRangeMod);
	((WaterLODEffect*)m_BaseEffect)->SetFarModifier(m_FarModifier);
	((WaterLODEffect*)m_BaseEffect)->SetCenterOfPlanet(VereMath::ConvertToXMFLOAT3(meshCentre.getOrigin()));
	((WaterLODEffect*)m_BaseEffect)->SetRadiusOfPlanet(1.2f * m_Radius);
	((WaterLODEffect*)m_BaseEffect)->SetFogStart(500.0f);
	((WaterLODEffect*)m_BaseEffect)->SetFogRange(10000.0f);
	((WaterLODEffect*)m_BaseEffect)->SetFogColor(XMFLOAT3(0.1f, 0.3f, 0.9f));
}