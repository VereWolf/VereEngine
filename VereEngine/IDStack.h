#ifndef ID_STACK
#define ID_STACK

struct IDStack
{
	IDStack(int level); //range of ID's (0 .. pow(2,level)-1)
	~IDStack()
	{
	}

	void ReturnElement(int id);
	int TakeElement();

	int m_level;
	std::vector<XMINT4> m_reg;
};

#endif //ID_STACK