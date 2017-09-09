#pragma once

class GameComponent : public GameBaseObject
{
public:
	GameComponent();

	virtual bool ComponentProccess() = 0;

	int GetComponentID() { return m_componentID; }
	void SetComponentID(int id) { m_componentID = id; }

	int GetProccessed() { return m_isProccessed; }
	void SetProccessed(int P) { m_isProccessed = P; }
private:
	int m_componentID;
	int m_isProccessed;

protected:
	XMFLOAT4 m_progress;
};
