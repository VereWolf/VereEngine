#pragma once
#include "pch.h"
#include "GameComponent.h"

class GameComponentsManager
{
public:
	GameComponentsManager(DX::DeviceResources *resources);

	void Update();

	bool InsertToProccess(GameComponent * C);
private:
	VereQueue<int> m_proccessQueue;

	IDRegistr *m_gameComponentReg;
	std::vector<GameComponent*> m_gameComponents;

	DX::DeviceResources *m_resources;
};


__declspec(selectany) GameComponentsManager *GameComponentsManagerHandle = 0;
