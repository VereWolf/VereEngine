#pragma once
#include "pch.h"
#include "GameComponent.h"

enum
{
	KT_TERRAIN_LOD0 = 0,
	KT_TERRAIN_LOD1,
	KT_TERRAIN_LOD2
};

struct GameComponentThread
{
	VereQueue<int> m_proccessQueue;
	IDRegistr *m_gameComponentReg;
	std::vector<GameComponent*> m_gameComponents;
};

class GameComponentsManager
{
public:
	GameComponentsManager(DX::DeviceResources *resources, int numOfThread);

	void Update();
	void DeleteUselessElement(int fromQues, int toQues);

	bool InsertToProccess(GameComponent * C,int kindOfThread);
private:

	std::vector<GameComponentThread> m_componentThread;

	DX::DeviceResources *m_resources;

	std::vector<int> m_idCurrentProcces;
	std::vector<bool> m_isFinishProcces;
};


__declspec(selectany) GameComponentsManager *GameComponentsManagerHandle = 0;
