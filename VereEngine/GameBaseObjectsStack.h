#ifndef GAME_BASE_OBJECTS_STACK_H
#define GAME_BASE_OBJECTS_STACK_H

#include "pch.h"
#include "IDStack.h"
#include "GameBaseObject.h"

class GameBaseObjectsStack
{
public:
	GameBaseObjectsStack();
	~GameBaseObjectsStack()
	{
		delete idObjectStack;

		for (int i = 0; i < m_gameObjects.size(); ++i)
		{
			DeleteGameObject(i);
		}
	}

	void Init(DX::DeviceResources *resources);

	int CreateGameObject(GameBaseObject *object);
	void DeleteGameObject(int id);
	GameBaseObject *GetGameObject(int id);
private:
	DX::DeviceResources *m_resources;

	IDStack *idObjectStack;
	std::vector<GameBaseObject*> m_gameObjects;
};

struct AssetsStacks
{
	GameBaseObjectsStack m_gameSounds;
	GameBaseObjectsStack m_gameCollisionShapes;
};

struct StreamingDataAssetsStacks
{
	GameBaseObjectsStack  m_gameDataSet;
	GameBaseObjectsStack  m_gameData;
};

struct RenderAssetsStacks
{
	GameBaseObjectsStack m_gameInputLayouts;
	GameBaseObjectsStack m_gameEffects;
	GameBaseObjectsStack m_gameVertices;
	GameBaseObjectsStack m_gameModels;
	GameBaseObjectsStack m_gameTextures;
	GameBaseObjectsStack m_gameMeshBuffers;
	GameBaseObjectsStack m_gameAnimations;
	GameBaseObjectsStack m_gameAmbientLight;
	GameBaseObjectsStack m_gameDirectionalLight;
	GameBaseObjectsStack m_gamePointLight;
	GameBaseObjectsStack m_gameSpotLight;
	GameBaseObjectsStack m_gameTilePlanetData;
};

struct RenderTextAssetsStacks
{
	GameBaseObjectsStack  m_gameColorBrushs;
	GameBaseObjectsStack m_gameTextLayouts;
	GameBaseObjectsStack m_gameTextFormats;
};

#endif //GAME_BASE_OBJECTS_STACK_H