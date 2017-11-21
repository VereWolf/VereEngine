#include "stdafx.h"
#include "GenerateTrees.h"

GenerateTrees::GenerateTrees()
{
	SetFolderName(L"Calc_Trees");
}

void GenerateTrees::Generate(int randomValue)
{
	srand(randomValue);

	std::vector<int> TM;
	std::vector<Float4> TM2;

	TM.resize(512 * 512, 0);
	TM2.resize(512 * 512, Float4(0.0f, 0.0f, 0.0f, 0.0f));

	bool B = true;
	int S = 0;

	std::vector<int> pos(4 * 8 * 8 + 1, -1);

	for (int i = 512; i > 0; i /= 2)
	{
		for (int by = 0; by < 512 / i; by++)
		{
			for (int bx = 0; bx < 512 / i; bx++)
			{
				int x = bx * i + rand() % i;
				int y = by * i + rand() % i;

				B = true;

				for (int sy = -2 * i; sy < 3 * i; sy++)
				{
					for (int sx = -2 * i; sx < 3 * i; sx++)
					{
						int px = VMath::Wrap(sx + x, 512);
						int py = VMath::Wrap(sy + y, 512);

						if (TM[py * 512 + px] > 0)
						{
							B = false;
						}
					}
				}

				if (B == true)
				{
					TM[y * 512 + x] = pow(VMath::Clamp(i, 0, 64), 0.5f);
					++S;
				}
			}
		}
	}

	for (int i = 0; i < 512 * 512; i++)
	{
		if (TM[i] > 0)
		{

			switch (TM[i])
			{
			case 1:
				TM2[i].x = 1;
				TM2[i].y = 0;
				TM2[i].z = 0;
				TM[i] = 1;
				break;
			case 2:
				TM2[i].x = 0;
				TM2[i].y = 1;
				TM2[i].z = 0;
				TM[i] = 2;
				break;
			case 3:
				TM2[i].x = 0;
				TM2[i].y = 1;
				TM2[i].z = 0;
				TM[i] = 2;
				break;
			case 4:
				TM2[i].x = 0;
				TM2[i].y = 0;
				TM2[i].z = 1;
				TM[i] = 3;
				break;
			case 5:
				TM2[i].x = 1;
				TM2[i].y = 1;
				TM2[i].z = 1;
				TM[i] = 4;
				break;
			case 6:
				TM2[i].x = 1;
				TM2[i].y = 1;
				TM2[i].z = 1;
				TM[i] = 4;
				break;
			case 7:
				TM2[i].x = 1;
				TM2[i].y = 1;
				TM2[i].z = 1;
				TM[i] = 4;
				break;
			case 8:
				TM2[i].x = 1;
				TM2[i].y = 1;
				TM2[i].z = 1;
				TM[i] = 4;
				break;
			}
		}
	}

	std::vector<float> trees(S * 2, -1);
	int j = 0;
	int k = 0;

	for (int by = 0; by < 8; ++by)
	{
		for (int bx = 0; bx < 8; ++bx)
		{
			for (int i = 1; i < 5; ++i)
			{
				pos[k] = j;
				++k;
				for (int cy = 0; cy < 64; ++cy)
				{
					for (int cx = 0; cx < 64; ++cx)
					{
						int x = bx * 64 + cx;
						int y = by * 64 + cy;

						if (TM[y * 512 + x] == i)
						{
							trees[2 * j] = (float)x * 0.00195694716f;
							trees[2 * j + 1] = (float)y * 0.00195694716f;
							++j;
						}
					}
				}
			}
		}
	}

	pos.back() = trees.size() / 2;

	DataStreaming::SaveImage(GetWholeFilePatch(L"Trees.jpg"), &TM2, 512, 512);
	DataStreaming::SaveFile(GetWholeFilePatch("Trees_index.raw"), &pos[0], 1, pos.size(), sizeof(int));
	DataStreaming::SaveFile(GetWholeFilePatch("Trees_tiles.raw"), &trees[0], 1, trees.size(), sizeof(float));
}