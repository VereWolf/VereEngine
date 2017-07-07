#include "pch.h"
#include "IDStack.h"



IDStack::IDStack(int l)
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
			m_reg[size + j] = XMINT4(j, 2 * j + size + m, 1 + 2 * j + size + m, 0);
		}

		size += pow(2, i);
	}
}

void IDStack::ReturnElement(int id)
{
	std::bitset<sizeof(id) * CHAR_BIT> bs(id);

	std::vector<int> buffer(m_level);

	if (bs[0] == 0)
	{
		buffer[0] = m_reg[0].y;
	}
	else
	{
		buffer[0] = m_reg[0].z;
	}

	for (int i = 1; i < m_level; ++i)
	{
		if (bs[i] == 0)
		{
			buffer[i] = m_reg[buffer[i - 1]].y;
		}
		else
		{
			buffer[i] = m_reg[buffer[i - 1]].z;
		}
	}

	m_reg[buffer[m_level - 1]].w = 0;

	for (int i = m_level - 2; i >= 0; --i)
	{
		if (m_reg[m_reg[buffer[i]].y].w == 1 && m_reg[m_reg[buffer[i]].z].w == 1)
		{
			m_reg[buffer[i]].w = 1;
		}
		else
		{
			m_reg[buffer[i]].w = 0;
		}
	}
}

int IDStack::TakeElement()
{
	std::vector<int> buffer(m_level);

	if (m_reg[0].w == 0)
	{
		if (m_reg[m_reg[0].y].w == 0)
		{
			buffer[0] = m_reg[0].y;
		}
		else
		{
			buffer[0] = m_reg[0].z;
		}

		for (int i = 1; i < m_level; ++i)
		{
			if (m_reg[m_reg[buffer[i - 1]].y].w == 0)
			{
				buffer[i] = m_reg[buffer[i - 1]].y;
			}
			else if(m_reg[m_reg[buffer[i - 1]].z].w == 0)
			{
				buffer[i] = m_reg[buffer[i - 1]].z;
			}
		}

		m_reg[buffer[m_level - 1]].w = 1;

		for (int i = m_level - 2; i >= 0; --i)
		{
			if (m_reg[m_reg[buffer[i]].y].w == 1 && m_reg[m_reg[buffer[i]].z].w == 1)
			{
				m_reg[buffer[i]].w = 1;
			}
			else
			{
				m_reg[buffer[i]].w = 0;
			}
		}


		return m_reg[buffer[m_level - 1]].x;
	}

	return -1;
}