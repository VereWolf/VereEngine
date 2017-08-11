#include "pch.h"
#include "IDRegistr.h"



IDRegistr::IDRegistr(int l)
{
	m_level = l;

	int size = 0;
	int C1 = 0;
	int C2 = 0;

	for (int i = 0; i < m_level + 1; ++i)
	{
		size += pow(2, i);
	}

	m_reg.resize(size);

	size = 0;

	for (int i = 0; i < m_level + 1; ++i)
	{
		int m = pow(2, i);

		C1 = 0;
		C2 = 0;

		for (int j = 0; j < m; ++j)
		{
			m_reg[size + j] = IDRegistrElement(j, XMINT2(2 * j + size + m, 1 + 2 * j + size + m), false, false);
		}

		size += pow(2, i);
	}
}

void IDRegistr::ReturnElement(int id)
{
	std::bitset<sizeof(id) * CHAR_BIT> bs(id);

	std::vector<int> buffer(m_level);

	if (m_level <= 0) return;

	if (bs[m_level - 1] == 0)
	{
		buffer[0] = m_reg[0].branches.x;
	}
	else
	{
		buffer[0] = m_reg[0].branches.y;
	}

	for (int i = 1; i < m_level; ++i)
	{
		if (bs[m_level - i - 1] == 0)
		{
			buffer[i] = m_reg[buffer[i - 1]].branches.x;
		}
		else
		{
			buffer[i] = m_reg[buffer[i - 1]].branches.y;
		}
	}

	m_reg[buffer[m_level - 1]].isFree = 0;
	m_reg[buffer[m_level - 1]].isElement = 0;

	for (int i = m_level - 2; i >= 0; --i)
	{
		if (m_reg[m_reg[buffer[i]].branches.x].isFree == 1 && m_reg[m_reg[buffer[i]].branches.y].isFree == 1)
		{
			m_reg[buffer[i]].isFree = 1;
		}
		else
		{
			m_reg[buffer[i]].isFree = 0;
		}

		if (m_reg[m_reg[buffer[i]].branches.x].isElement == 1 || m_reg[m_reg[buffer[i]].branches.y].isElement == 1)
		{
			m_reg[buffer[i]].isElement = 1;
		}
		else
		{
			m_reg[buffer[i]].isElement = 0;
		}
	}
}

int IDRegistr::TakeElement()
{
	std::vector<int> buffer(m_level);

	if (m_reg[0].isFree == 0)
	{
		if (m_reg[m_reg[0].branches.x].isFree == 0)
		{
			buffer[0] = m_reg[0].branches.x;
		}
		else if (m_reg[m_reg[0].branches.y].isFree == 0)
		{
			buffer[0] = m_reg[0].branches.y;
		}
		else
		{
			return -1;
		}

		for (int i = 1; i < m_level; ++i)
		{
			if (m_reg[m_reg[buffer[i - 1]].branches.x].isFree == 0)
			{
				buffer[i] = m_reg[buffer[i - 1]].branches.x;
			}
			else if(m_reg[m_reg[buffer[i - 1]].branches.y].isFree == 0)
			{
				buffer[i] = m_reg[buffer[i - 1]].branches.y;
			}
		}

		m_reg[buffer[m_level - 1]].isFree = 1;
		m_reg[buffer[m_level - 1]].isElement = 1;

		for (int i = m_level - 2; i >= 0; --i)
		{
			if (m_reg[m_reg[buffer[i]].branches.x].isFree == 1 && m_reg[m_reg[buffer[i]].branches.y].isFree == 1)
			{
				m_reg[buffer[i]].isFree = 1;
			}
			else
			{
				m_reg[buffer[i]].isFree = 0;
			}

			if (m_reg[m_reg[buffer[i]].branches.x].isElement == 1 || m_reg[m_reg[buffer[i]].branches.y].isElement == 1)
			{
				m_reg[buffer[i]].isElement = 1;
			}
			else
			{
				m_reg[buffer[i]].isElement = 0;
			}
		}

		return m_reg[buffer[m_level - 1]].order;
	}

	return -1;
}

int IDRegistr::GetRangeOfElement()
{
	std::vector<int> buffer(m_level);

	if (m_reg[0].isElement == 0)
	{
		if (m_reg[m_reg[0].branches.y].isElement == 1)
		{
			buffer[0] = m_reg[0].branches.y;
		}
		else if(m_reg[m_reg[0].branches.x].isElement == 1)
		{
			buffer[0] = m_reg[0].branches.x;
		}

		for (int i = 1; i < m_level; ++i)
		{
			if (m_reg[m_reg[buffer[i - 1]].branches.y].isElement == 1)
			{
				buffer[i] = m_reg[buffer[i - 1]].branches.y;
			}
			else if (m_reg[m_reg[buffer[i - 1]].branches.x].isElement == 1)
			{
				buffer[i] = m_reg[buffer[i - 1]].branches.x;
			}
		}

		return m_reg[buffer[m_level - 1]].order + 1;
	}

	return -1;
}