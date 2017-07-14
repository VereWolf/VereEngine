#include "pch.h"
#include "GameComponent.h"
#include "GameComponentsManager.h"

GameComponentsManager::GameComponentsManager(DX::DeviceResources *resources)
{
	m_gameComponentReg = new IDRegistr(15);
	m_gameComponents.resize(pow(2, 15), NULL);
	m_resources = resources;
	GameComponentsManagerHandle = this;
}

void GameComponentsManager::Update()
{
	if (m_proccessQueue.GetSize() <= 0) return;

	int id = m_proccessQueue.TakeElement();

	m_gameComponentReg->ReturnElement(id);

	m_gameComponents[id]->ComponentProccess();
	m_gameComponents[id]->SetProccessed(2);
	m_gameComponents[id] = NULL;
}

bool GameComponentsManager::InsertToProccess(GameComponent * C)
{
	int id = m_gameComponentReg->TakeElement();

	if (id < 0) return false;

	C->SetComponentID(id);
	//C->SetProccessed(0);

	m_gameComponents[id] = C;
	m_proccessQueue.GiveElement(id);

	return true;
}