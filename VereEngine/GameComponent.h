#pragma once

class GameComponent : public GameBaseObject
{
public:
	GameComponent();

	virtual bool ComponentProccess() = 0;

	int GetComponentID() { return m_componentID; }
	void SetComponentID(int id) { m_componentID = id; }

	bool GetProccessed() { return m_isProccessed; }
	void SetProccessed(bool P) { m_isProccessed = P; }
private:
	int m_componentID;
	bool m_isProccessed;
};
