#include "pch.h"
#include "Clouds.h"
#include "RenderDevice.h"

void CloudsRenderMessage::Use()
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

	((CloudsEffect*)m_Effect)->SetWorld(XMLoadFloat4x4(&VereMath::ConvertToXMFLOAT4X4(mesh)));
	((CloudsEffect*)m_Effect)->SetWorldN(XMLoadFloat4x4(&VereMath::ConvertToXMFLOAT4X4(meshN)));
	((CloudsEffect*)m_Effect)->SetSpacing(((float)S2));
	((CloudsEffect*)m_Effect)->SetLevel(1.0f / pow(2, m_Level));
	((CloudsEffect*)m_Effect)->SetOffset(XMFLOAT2(m_Coord.x, m_Coord.y));
}