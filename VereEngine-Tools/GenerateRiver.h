#pragma once

#include "stdafx.h"
#include "BaseForTool.h"

struct RiverNode
{
	float id;
	float size;
	float height;
	float length;
	Float2 pos;

	RiverNode *prev;
	RiverNode *next;

};

struct RiverBlock
{
	int numNode;
	RiverNode *start;
	RiverNode *end;
	RiverNode *current;
};

class GenerateRiver : public BaseForTool
{
public:
	GenerateRiver();

	void GeneratePixelOfRiver(Float2 V1, Float2 V2, float d1, float d2, float id);
	void ClampRiver(int x, int y, float d, float id);
	void CreateRiverNode(int x, int y, float d, float id, int multiplicationForRiverNode);
	INT2 GetRiverOffset(Float2 V1, Float2 V2);
	void AddRiverNode(float id, float size, float height, Float2 pos);
	void InsertRiverNodeBetweenNodes(RiverNode *firstNode, float id, float size, float height, Float2 pos);
	void SetCurrentToStartRiverNode() { m_currentRiverNode = m_startRiverNode; }
	void SetCurrentToNextRiverNode() { m_currentRiverNode = m_currentRiverNode->next; }
	void InsertRiverNodesRecurse(RiverNode *firstNode, float minLenght);
	void AddRiverNodeToBlock(float id, float size, Float2 pos, int blockIndex);
	void GenerateRiverNode(float minLenght);
	void GenerateNodeInBlock(int widthBlockStack, int heightBlockStack, int sizeOfBlock);
	void GenerateBlock(int widthOfMap, int heightOfMap, int numBlockInRow, int numBlockInColumn);
	void GenerateRiverMap(int width, int height, int index);
	float GetIndexOfRiver(Float2 pos1, Float2 pos2, Float2 dir1, Float2 dir2);

	void Generate(int height, int width);

private:
	std::vector<Float4> m_Rivers;
	std::vector<Float4> m_Rivers2;
	std::vector<BYTE4> m_RiverMap1;
	std::vector<BYTE4> m_RiverMap2;
	std::vector<float> m_HMap;

	std::vector<INT2> m_riverID;
	std::vector<INT2> m_riverLvl;
	std::vector<float> m_riverWidth1;
	std::vector<float> m_riverWidth2;
	std::vector<float> m_riverLength1;
	std::vector<float> m_riverLength2;
	std::vector<float> m_waterHeight1;
	std::vector<float> m_waterHeight2;
	std::vector<Float2> m_riverVector1;
	std::vector<Float2> m_riverVector2;
	std::vector<float> m_riverType1;
	std::vector<float> m_riverType2;
	std::vector<float> m_riverHeight;

	std::vector<RiverBlock> m_RiverBlocks;
	RiverNode *m_startRiverNode;
	RiverNode *m_endRiverNode;
	RiverNode *m_currentRiverNode;
	int m_numRiverNode;
	int m_height;
	int m_width;
};
