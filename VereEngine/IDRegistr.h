#pragma once

struct IDRegistrElement
{
	IDRegistrElement()
	{
		order = 0;
		branches = XMINT2(0, 0);
		isFree = false;
		isElement = false;
	}

	IDRegistrElement(int O, XMINT2 B)
	{
		order = O;
		branches = B;
		isFree = false;
		isElement = false;
	}

	IDRegistrElement(int O, XMINT2 B, bool F, bool E)
	{
		order = O;
		branches = B;
		isFree = F;
		isElement = E;
	}
	int order;
	XMINT2	branches;
	bool isFree;
	bool isElement;
};

struct IDRegistr
{
	IDRegistr(int level); //range of ID's (0 .. pow(2,level)-1)
	~IDRegistr()
	{
	}

	void ReturnElement(int id);
	int TakeElement();
	int GetRangeOfElement(); // also with empty elements between

	int m_level;
	std::vector<IDRegistrElement> m_reg;
};