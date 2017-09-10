#pragma once
<<<<<<< HEAD

=======
>>>>>>> VereEngine-Planet

template<class T>
class VereQueue
{
public:
	VereQueue();
	~VereQueue();

	T TakeElement();
	void GiveElement(T E);
	int GetSize() { return m_size; }

	void SetCurrentElementToBegin() { m_currentElement = m_begin; }
	void SetCurrentElementToEnd() { m_currentElement = m_end; }

	bool SetCurrentElementToFront();
	bool SetCurrentElementToBack();
	T GetCurrentElement();
	bool DeleteCurrentElementAndSetToFront();
	bool DeleteCurrentElementAndSetToBack();

private:
	struct CellForTemplate
	{
		T element;
		CellForTemplate *front;
		CellForTemplate *back;
	};

	CellForTemplate *m_begin;
	CellForTemplate *m_end;
	CellForTemplate *m_currentElement;
	int m_size;
};

template<class T>
VereQueue<T>::VereQueue()
{
	m_begin = NULL;
	m_end = NULL;
	m_currentElement = NULL;
	m_size = 0;
}

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
		if (m_begin) m_begin->front = NULL;

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
		newCell->front = m_end;
		newCell->back = NULL;
		m_end = newCell;

	}
	else
	{
		m_begin = newCell;
		m_end = newCell;
		newCell->front = NULL;
		newCell->back = NULL;
	}

	++m_size;
<<<<<<< HEAD
=======
}

template<class T>
bool VereQueue<T>::SetCurrentElementToFront()
{
	if (!m_currentElement || !m_currentElement->front)
	{
		return false;
	}

	m_currentElement = m_currentElement->front;

	return true;
}

template<class T>
bool VereQueue<T>::SetCurrentElementToBack()
{
	if (!m_currentElement || !m_currentElement->back)
	{
		return false;
	}

	m_currentElement = m_currentElement->back;

	return true;
}

template<class T>
T VereQueue<T>::GetCurrentElement()
{
	if (m_currentElement == NULL)
	{
		return NULL;
	}

	return m_currentElement->element;
}

template<class T>
bool VereQueue<T>::DeleteCurrentElementAndSetToFront()
{
	CellForTemplate *CE = m_currentElement;
	CE->front = CE->back;
	CE->back = CE->front;

	--m_size;

	if (!CE->front)
	{
		delete CE;
		return false;
	}

	m_currentElement = CE->front;

	delete CE;

	return true;
}

template<class T>
bool VereQueue<T>::DeleteCurrentElementAndSetToBack()
{
	CellForTemplate *CE = m_currentElement;
	CE->front = CE->back;
	CE->back = CE->front;

	--m_size;

	if (!CE->back)
	{
		delete CE;
		return false;
	}

	m_currentElement = CE->back;

	delete CE;

	return true;
>>>>>>> VereEngine-Planet
}