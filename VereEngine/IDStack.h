#ifndef ID_STACK
#define ID_STACK

struct IDStack
{
	struct IDElement
	{
		IDElement(int itsID, IDElement *itsNextElem);

		void DeleteAllElements();

		int id;
		IDElement *nextElem;
	};

	struct StackOfMark
	{
		StackOfMark();

		void DeleteAllIDElements();

		IDElement *firstMark;
		int markCounter;
		int occupancyRate;
	};

	IDStack();
	~IDStack()
	{
		for (int i = 0; i < m_idMarks.size(); ++i)
		{
			m_idMarks[i].DeleteAllIDElements();
		}
	}

	void ReturnElement(int id, bool saveoccupancyRate = false);
	int GetElement();

	int m_sizeOfStep;
	std::vector<StackOfMark> m_idMarks;
	int m_suitableMark;
};

#endif //ID_STACK