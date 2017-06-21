#include "pch.h"
#include "GameBaseObject.h"

#ifndef GAME_RESOURCE_OBJECT
#define GAME_RESOURCE_OBJECT


class GameResourceObject: public GameBaseObject
{
public:
	void PreInit(DX::DeviceResources *resources)
	{
		GameBaseObject::PreInit(resources);
	}
	virtual void Render(btTransform transform, btTransform scaling) = 0;
	void PerformancePersistence(bool onlyGetSize, bool isSaveMod);
	virtual void LoadResource() = 0;
	void SetNameFile(std::string nameFile) { m_nameFile = nameFile; }
	void SetPathFile(std::string pathFile) { m_pathFile = pathFile; }
private:
	std::string m_nameFile;
	std::string m_pathFile;
};

#endif //GAME_RESOURCE_OBJECT