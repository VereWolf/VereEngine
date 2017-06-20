

#include "pch.h"
#include "GameObject.h"
#include "GameObjectsStackManager.h"
#include "StaticMesh.h"
#include "BodyMesh.h"
//#include "BillboardMesh.h"
//#include "ClothingMesh.h"
#include "GenerateMesh.h"

#ifndef TEST_OBJECT
#define TEST_OBJECT

class TestObject : public GameObject
{
public:
	TestObject()
	{
	}

	TestObject(TestObject & other)
	{
	}

	GameObject *Clone() { return new TestObject(*this); }

	void Init()
	{
		//SetScaling(100.0);//Test new universal render device

		std::vector<Vertex::PosNormalTexTan> vertices;
		std::vector<UINT> indices;
		UINT size = sizeof(Vertex::PosNormalTexTan);

		GenerateMesh::GenerateSphere(1.0f, 5, XMFLOAT3(0.0, 0.0, 0.0), vertices, indices);

		int idMB = GameRenderDeviceHandle->CreateMeshBuffer(&vertices[0], size, vertices.size(), &indices);

		BaseEffect *baseEffect = new BaseEffect(m_resources, "FX/PosNormalTexTan.fxo");
		int idE = GameRenderDeviceHandle->CreateEffect(baseEffect);

		std::vector<D3D11_INPUT_ELEMENT_DESC> vertex =
		{
			{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "TANGENT", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 24, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 36, D3D11_INPUT_PER_VERTEX_DATA, 0 }
		};

		int idV = GameRenderDeviceHandle->CreateVertex(&vertex);

		int idIL = GameRenderDeviceHandle->CreateInputLayouts(idV, idE);

		Model *model = new Model;
		model->material.Ambient = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
		model->material.Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
		model->material.Specular = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
		model->material.Reflect = XMFLOAT4(0.0f, 0.0f, 0.0f, 0.0f);
		model->faceCount = indices.size() / 3;
		model->faceStart = 0;
		model->sizeOfVertex = size;
		model->idEffect = idE;
		model->idMeshBuffer = idMB;
		model->idVertex = idV;
		model->idInputLayouts = idIL;
		model->topology = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
		m_RenderId = GameRenderDeviceHandle->CreateModel(model);
	}

	void Update()
	{
	}

	void Render(btTransform camOffset, XMMATRIX camView, XMMATRIX camProj,
		float camFarZ, btScalar heightFar, btScalar aspect,
		float camFarRangeMod, float camModifier)
	{
		btVector3 S = btVector3(GetScaling().getRow(0).getX(), GetScaling().getRow(1).getY(), GetScaling().getRow(2).getZ());
		RenderMesage message;
		message.m_ModelID = m_RenderId;
		message.m_Scaling = btTransform(btMatrix3x3(-S.getX(), 0.0, 0.0, 0.0, -S.getY(), 0.0, 0.0, 0.0, -S.getZ()), btVector3(0.0, 0.0, 0.0));
		message.m_Transform = GetWorldTransform();
		message.m_CameraOffset = camOffset;
		message.m_View = camView;
		message.m_Proj = camProj;
		message.m_FarZ = camFarZ;
		message.m_FarRangeMod = camFarRangeMod;
		message.m_FarModifier = camModifier;
		message.m_RasterizeState = RenderStates::SolidRS;
		GameRenderDeviceHandle->Render(&message);
	}

private:
}; 

/*class BoxObject : public GameObject
{
public:
	void Init()
	{
		//mModel.CreateSphere(2.5f, 8);
		m_position = GameObjectStackHandle->GetMainCamera()->GetPosition();

	}

	void Update()
	{
		/*m_relativeAngle.y += 0.000f;
		if (m_relativeAngle.y > 2.0f * PI)
		{
			m_relativeAngle.y -= 2.0f*PI;
		}
		if (m_relativeAngle.y < 0.0f)
		{
			m_relativeAngle.y += 2.0f*PI;
		}
	}

	void Render()
	{
		double S = 30.0;

		if (VereMath::GetDistance(GameObjectStackHandle->GetMainCamera()->GetPosition(), m_position) <= S)
		{
			//mModel.Draw(m_position, m_angle);
			RenderStaticMesh(0);
		}
	}

private:
	//StaticMesh mModel;
};

class TestSphereClothing : public GameObject
{
public:
	TestSphereClothing()
	{
	};

	~TestSphereClothing()
	{
	};

	void Init()
	{
		mModel.CreateSphere(m_resources->GetD3DDevice(), 12.5f, 4);
		mChangeModel.CreateSphere(m_resources->GetD3DDevice(), 12.5f, 4);
		m_position = GameObjectStackHandle->GetMainCamera()->GetPosition();

	}

	void Update()
	{

		mChangeModel.Draw(m_resources->GetD3DDevice(), m_resources->GetD3DDeviceContext(), m_cam, m_position, m_angle);
	}

	StaticMesh *GetMesh() { return &mModel; }
	StaticMesh *GetChangeMesh() { return &mChangeModel; }

private:
	StaticMesh mModel;
	StaticMesh mChangeModel;
};

class TestSphereBody : public GameObject
{
public:
	TestSphereBody()
	{
		mClothes.resize(1);
		mClothes.at(0) = new TestSphereClothing;
	};

	~TestSphereBody()
	{
		delete mClothes.at(0);
	};


	void Init()
	{
		mModel.CreateSphere(m_resources->GetD3DDevice(), 10, 3);

		m_position = GameObjectStackHandle->GetMainCamera()->GetPosition();

		mClothes.at(0)->PreInit(m_resources->GetD3DDevice(),m_resources->GetD3DDeviceContext(),m_cam);
		mClothes.at(0)->Init();

		PutOn();
	}

	void Update()
	{
		//mModel.Draw(m_resources->GetD3DDevice(), m_resources->GetD3DDeviceContext(), m_cam, m_position, m_angle);
		mClothes.at(0)->Update();
	}

	void PutOn()
	{
		Vertex::PosNormalTexTan *V;
		XMDOUBLE3 R;
		XMDOUBLE3 RC;

		double D = -1.0;
		double DN = 0.0;

		for (UINT i = 0; i < mClothes.size(); ++i)
		{
			for (UINT j = 0; j < mClothes.at(i)->GetChangeMesh()->GetVertices()->size(); ++j)
			{
				D = -1.0;
				for (UINT k = 0; k < mModel.GetIndices()->size() / 3; ++k)
				{
					V = &mClothes.at(i)->GetChangeMesh()->GetVertices()->at(j);

					if (VereMath::PenetrationOfTriangle(XMDOUBLE3(mModel.GetVertices()->at(mModel.GetIndices()->at(3 * k)).Position.x,
						mModel.GetVertices()->at(mModel.GetIndices()->at(3 * k)).Position.y,
						mModel.GetVertices()->at(mModel.GetIndices()->at(3 * k)).Position.z),
						XMDOUBLE3(mModel.GetVertices()->at(mModel.GetIndices()->at(3 * k + 1)).Position.x,
						mModel.GetVertices()->at(mModel.GetIndices()->at(3 * k + 1)).Position.y,
						mModel.GetVertices()->at(mModel.GetIndices()->at(3 * k + 1)).Position.z),
						XMDOUBLE3(mModel.GetVertices()->at(mModel.GetIndices()->at(3 * k + 2)).Position.x,
						mModel.GetVertices()->at(mModel.GetIndices()->at(3 * k + 2)).Position.y,
						mModel.GetVertices()->at(mModel.GetIndices()->at(3 * k + 2)).Position.z),
						XMDOUBLE3(V->Position.x, V->Position.y, V->Position.z), XMDOUBLE3(-V->Normal.x, -V->Normal.y, -V->Normal.z), &R))
					{
						DN = VereMath::GetDistance(V->Position, XMFLOAT3(R.x, R.y, R.z));

						if (D < 0.0 || DN < D)
						{
							D = DN;
							RC = R;
						}
					}*/

					/*VereMath::PutOn(mChangeModel.GetVertices()->at(3 * k),
						mChangeModel.GetVertices()->at(3 * k) + 1,
						mChangeModel.GetVertices()->at(3 * k)+2,
						V->Position, XMFLOAT3(-V->Normal.x, -V->Normal.y, -V->Normal.z))*/
				/*}

				if (D >= 0.0)
				{
					mClothes.at(i)->GetChangeMesh()->GetVertices()->at(j).Position.x = RC.x;
					mClothes.at(i)->GetChangeMesh()->GetVertices()->at(j).Position.y = RC.y;
					mClothes.at(i)->GetChangeMesh()->GetVertices()->at(j).Position.z = RC.z;
				}
				else
				{
					UINT a = 0;
				}
			}
			mClothes.at(i)->GetChangeMesh()->BuildVerticesBuffer(m_resources->GetD3DDevice());
		}
	}

private:
	StaticMesh mModel;

	std::vector <TestSphereClothing*> mClothes;

};

class TestBody : public GameObject
{
public:
	void Init()
	{
		mModel.CreateObject(m_resources->GetD3DDevice(), "..\\Models\\", L"..\\Models\\", "Body.txt");
		m_position = GameObjectStackHandle->GetMainCamera()->GetPosition();

	}

	void Update()
	{
		m_angle.y += 0.01f;
		if (m_angle.y > 2.0f * PI)
		{
			m_angle.y -= 2.0f * PI;
		}
		if (m_angle.y < 0.0f)
		{
			m_angle.y += 2.0f * PI;
		}

		mModel.Draw(m_resources->GetD3DDevice(), m_resources->GetD3DDeviceContext(), m_cam, m_position, m_angle);
	}

private:
	BodyMesh mModel;
};*/

/*class TestClothing : public GameObject
{
public:
	void Init()
	{
		mModel.CreateObject(m_resources->GetD3DDevice(), "..\\Models\\", L"..\\Models\\", "Body.txt");
		position = GameObjectStackHandle->GetMainCamera()->GetPosition();

		mModelC.CreateObject(m_resources->GetD3DDevice());
		position = GameObjectStackHandle->GetMainCamera()->GetPosition();

	}

	void Update()
	{
		angle.y += 0.01f;
		if (angle.y > 2.0f * PI)
		{
			angle.y -= 2.0f*PI;
		}
		if (angle.y < 0.0f)
		{
			angle.y += 2.0f*PI;
		}

		mModel.Draw(m_resources->GetD3DDevice(), m_resources->GetD3DDeviceContext(), m_cam, position, angle);
		mModelC.Draw(m_resources->GetD3DDevice(), m_resources->GetD3DDeviceContext(), m_cam, position, angle);
	}

private:
	BodyMesh mModel;
	ClothingMesh mModelC;
};*/

/*class TestBillboard : public GameObject
{
public:
	TestBillboard()
	{
		mColor = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	}
	void Init()
	{
		mModel.BuildVerticesBuffer(m_resources->GetD3DDevice());
		mModel.BuildIndicesBuffer(m_resources->GetD3DDevice());
		mModel.Init(mColor);
		m_position = GameObjectStackHandle->GetMainCamera()->GetPosition();
		//position.y += 1000.0f;

	}

	void SetColor(XMFLOAT4 itsColor){ mColor = itsColor; }

	void Update()
	{
		mModel.Draw(m_resources->GetD3DDevice(), m_resources->GetD3DDeviceContext(), m_cam, mScaling, m_position, m_angle);
	}

private:
	BillboardMesh mModel;
	XMFLOAT4 mColor;
};*/



#endif //TEST_OBJECT