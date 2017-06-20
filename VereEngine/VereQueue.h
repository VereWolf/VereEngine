
#ifndef VEREQUEUE_H
#define VEREQUEUE_H


template<class T>
class VereQueue
{
public:
	~VereQueue();

	T TakeElement();
	void GiveElement(T E);
	int GetSize() { return m_size; }

private:
	struct CellForTemplate
	{
		T element;
		CellForTemplate *front;
		CellForTemplate *back;
	};

	CellForTemplate *m_begin;
	CellForTemplate *m_end;
	int m_size;
};

template<class T>
VereQueue<T>::~VereQueue()
{
	while (m_size > 0)
	{
		TakeElement();
	}
}

template<class T>
T VereQueue<T>::TakeElement()
{
	CellForTemplate *thisCell = m_begin;
	T E = thisCell->element;
	if (m_size > 0)
	{
		m_begin = thisCell->back;
		delete thisCell;

		--m_size;
	}

	return E;
}

template<class T>
void VereQueue<T>::GiveElement(T E)
{
	CellForTemplate *newCell = new CellForTemplate;
	newCell->element = E;

	if (m_size > 0)
	{
		m_end->back = newCell;
		m_end = newCell;

	}
	else
	{
		m_begin = newCell;
		m_end = newCell;
	}

	++m_size;
}

#endif //VEREQUEUE_H