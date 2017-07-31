#include "pch.h"
#include "Atmosphere.h"
#include "RenderDevice.h"

void AtmosphereRenderMessage::Use()
{
	RenderMessage::Use();

	btScalar S = m_Spacing1;
	btScalar S2 = m_Spacing2;

	btTransform mesh = btTransform(btMatrix3x3(S * S2, 0.0, 0.0, 0.0, S * S2, 0.0, 0.0, 0.0, S * S2), m_Position + btVector3(0.0, 0.5 * S2, 0.0));
	mesh = btTransform(m_AngleMatrix) * mesh;
	btTransform meshCentre = m_CameraOffset * m_Transform;
	mesh = meshCentre * mesh;

	btTransform meshN = btTransform(btMatrix3x3(S * S2, 0.0, 0.0, 0.0, S * S2, 0.0, 0.0, 0.0, S * S2), m_Position + btVector3(0.0, 0.5 * S2, 0.0));
	meshN = btTransform(m_Transform.getBasis()) * btTransform(m_AngleMatrix) * meshN;

	XMFLOAT3 EyePos = { 0.0f, 0.0f, 0.0f };

	((AtmosphereEffect*)m_BaseEffect)->SetWorld(XMLoadFloat4x4(&VereMath::ConvertToXMFLOAT4X4(mesh)));
	((AtmosphereEffect*)m_BaseEffect)->SetWorldN(XMLoadFloat4x4(&VereMath::ConvertToXMFLOAT4X4(meshN)));
	((AtmosphereEffect*)m_BaseEffect)->SetSpacing(((float)S2));
	((AtmosphereEffect*)m_BaseEffect)->SetLevel(1.0f / pow(2, m_Level));
	((AtmosphereEffect*)m_BaseEffect)->SetOffset(XMFLOAT2(m_Coord.x, m_Coord.y));
}