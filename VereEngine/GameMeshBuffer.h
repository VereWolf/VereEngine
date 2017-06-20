#ifndef GAME_MESH_BUFFER_H
#define GAME_MESH_BUFFER_H

#include "pch.h"
#include "GameBaseObject.h"

class GameMeshBuffer : public GameBaseObject
{
public:
	GameMeshBuffer() {}
	~GameMeshBuffer()
	{
		ReleaseVertexBuffer();
		ReleaseIndexBuffer();
	}

	void SetVertexBuffer(ID3D11Buffer* V) { m_gameVertexBuffer = V; }
	void SetIndexBuffer(ID3D11Buffer* I) { m_gameIndexBuffer = I; }
	void SetMinVector(btVector3 minV) { m_minV; }
	void SetMaxVector(btVector3 maxV) { m_maxV; }

	ID3D11Buffer *GetVertexBuffer() { return m_gameVertexBuffer; }
	ID3D11Buffer *GetIndexBuffer() { return m_gameIndexBuffer; }
	btVector3 GetMinVector() { return m_minV; }
	btVector3 GetMaxVector() { return m_maxV; }

	inline void ReleaseVertexBuffer() { ReleaseCOM(m_gameVertexBuffer); }
	inline void ReleaseIndexBuffer() { ReleaseCOM(m_gameIndexBuffer); }
private:
	ID3D11Buffer* m_gameVertexBuffer;
	ID3D11Buffer* m_gameIndexBuffer;
	
	btVector3 m_minV;
	btVector3 m_maxV;
};

#endif //GAME_MESH_BUFFER_H