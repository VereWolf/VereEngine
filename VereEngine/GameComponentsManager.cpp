#include "pch.h"
#include "GameComponent.h"
#include "GameComponentsManager.h"

GameComponentsManager::GameComponentsManager(DX::DeviceResources *resources, int numOfThread)
{
	m_componentThread.resize(numOfThread);

	for (int i = 0; i < numOfThread; ++i)
	{
		m_componentThread[i].m_gameComponentReg = new IDRegistr(15);
		m_componentThread[i].m_gameComponents.resize(pow(2, 15), NULL);
	}

	m_resources = resources;
	GameComponentsManagerHandle = this;
}

void GameComponentsManager::Update()
{
	for (int i = 0; i < m_componentThread.size(); ++i)
	{
		if (m_componentThread[i].m_proccessQueue.GetSize() <= 0) return;

		int id = m_componentThread[i].m_proccessQueue.TakeElement();

		m_componentThread[i].m_gameComponentReg->ReturnElement(id);

		if (!m_componentThread[i].m_gameComponents[id]->IsDestroy())
		{
			m_componentThread[i].m_gameComponents[id]->ComponentProccess();

			m_componentThread[i].m_proccessQueue.SetCurrentElementToBegin();

			/*while (m_componentThread[i].m_proccessQueue.GetCurrentElement())
			{
				int id2 = m_componentThread[i].m_proccessQueue.GetCurrentElement();

				if (m_componentThread[i].m_gameComponents[id2]->IsDestroy() == true)
				{
					m_componentThread[i].m_gameComponents[id2]->SetProccessed(2);
					m_componentThread[i].m_gameComponents[id2] = NULL;
					if (m_componentThread[i].m_proccessQueue.DeleteCurrentElementAndSetToFront() == false) return;
				}
				else
				{
					if (m_componentThread[i].m_proccessQueue.SetCurrentElementToFront() == false) return;
				}
			}*/
		}

		m_componentThread[i].m_gameComponents[id]->SetProccessed(2);
		m_componentThread[i].m_gameComponents[id] = NULL;
	}
}

void GameComponentsManager::DeleteUselessElement(int fromQues, int toQues)
{
}

bool GameComponentsManager::InsertToProccess(GameComponent * C, int kindOfThread)
{
	int id = m_componentThread[kindOfThread].m_gameComponentReg->TakeElement();

	if (id < 0) return false;

	C->SetComponentID(id);
	C->SetProccessed(1);

	m_componentThread[kindOfThread].m_gameComponents[id] = C;
	m_componentThread[kindOfThread].m_proccessQueue.GiveElement(id);

	return true;
}