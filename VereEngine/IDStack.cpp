#include "pch.h"
#include "IDStack.h"

IDStack::IDElement::IDElement(int itsID, IDElement *itsNextElem)
{
	id = itsID;
	nextElem = itsNextElem;
}

void IDStack::IDElement::DeleteAllElements()
{
	if (this != NULL &&nextElem != NULL)
	{
		nextElem->DeleteAllElements();

		delete nextElem;
		nextElem = NULL;
	}
}

IDStack::StackOfMark::StackOfMark()
{
	firstMark = NULL;
	markCounter = 0;
	occupancyRate = 0;
}

void IDStack::StackOfMark::DeleteAllIDElements()
{
	firstMark->DeleteAllElements();
	delete firstMark;
	firstMark = NULL;
	markCounter = 0;
	occupancyRate = 0;
}

IDStack::IDStack()
{
	m_sizeOfStep = 32;
	m_idMarks.resize(1);
	m_suitableMark = 0;
}

void IDStack::ReturnElement(int id, bool saveoccupancyRate)
{
	int M = id / m_sizeOfStep;

	if (M < m_suitableMark)
	{
		m_suitableMark = M;
	}

	if (M > m_idMarks.size())
	{
		m_idMarks.resize(M);
	}

	if (m_idMarks[M].occupancyRate == 0)
	{
		IDElement *e = new IDElement(id, NULL);
		m_idMarks[M].firstMark = e;
	}
	else
	{
		IDElement *e = new IDElement(id, m_idMarks[M].firstMark);
		m_idMarks[M].firstMark = e;
	}

	if (saveoccupancyRate == false)
	{
		--m_idMarks[M].occupancyRate;
	}

	/*if (m_idMarks[M].markCounter == m_sizeOfStep)
	{
		m_idMarks[M].DeleteAllIDElements();
	}*/

	int S = m_idMarks.size();

	if (m_idMarks[S - 1].markCounter == 0)
	{
		if (S > 0)
		{
			m_idMarks.resize(S + 1);
		}
	}

	return;
}

int IDStack::GetElement()
{
	if (m_idMarks[m_suitableMark].firstMark == NULL)
	{
		ReturnElement(m_suitableMark * m_sizeOfStep + m_idMarks[m_suitableMark].markCounter, true);
		++m_idMarks[m_suitableMark].markCounter;
	}

	int id = m_idMarks[m_suitableMark].firstMark->id;
	IDElement *e = m_idMarks[m_suitableMark].firstMark;
	m_idMarks[m_suitableMark].firstMark = e->nextElem;
	delete e;
	e = NULL;
	++m_idMarks[m_suitableMark].occupancyRate;

	if (!(m_idMarks[m_suitableMark].occupancyRate < m_sizeOfStep))
	{
		int S = m_idMarks.size();

		for (int i = m_suitableMark; i < S; ++i)
		{
			if (m_idMarks[i].occupancyRate < m_sizeOfStep)
			{
				m_suitableMark = i;
				return id;
			}
		}
		S += 1;
		m_idMarks.resize(S);
		m_suitableMark = S;
	}

	return id;
}