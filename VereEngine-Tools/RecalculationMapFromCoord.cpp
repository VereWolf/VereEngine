#include "stdafx.h"
#include "RecalculationMapFromCoord.h"


RecalculationMapFromCoord::RecalculationMapFromCoord()
{
	SetFolderName(L"Calc_Map");
}

Float2 RecalculationMapFromCoord::GetReCalCoord(Float2 coord, std::vector<Float2> *coordMap, Float2 mapSize, Float2 sourceSize)
{
	Float2 C = Float2((mapSize.x - 1) * coord.x / (sourceSize.x - 1), (mapSize.y - 1) * coord.y / (sourceSize.y - 1));
	INT2 C1 = INT2(C.x, C.y);
	INT2 C11 = C1;
	INT2 C12 = INT2(C1.x + 1, C1.y + 1);

	while (C11.x < 0.0f) { C11.x += (mapSize.x - 1.0f); }
	while (C11.x >= (mapSize.x - 1.0f)) { C11.x -= (mapSize.x - 1.0f); }
	while (C11.y < 0.0f) { C11.y += (mapSize.y - 1.0f); }
	while (C11.y >= (mapSize.y - 1.0f)) { C11.y -= (mapSize.y - 1.0f); }

	while (C12.x < 0.0f) { C12.x += (mapSize.x - 1.0f); }
	while (C12.x >= (mapSize.x - 1.0f)) { C12.x -= (mapSize.x - 1.0f); }
	while (C12.y < 0.0f) { C12.y += (mapSize.y - 1.0f); }
	while (C12.y >= (mapSize.y - 1.0f)) { C12.y -= (mapSize.y - 1.0f); }

	Float2 C2 = Float2(C.x - (float)C1.x, C.y - (float)C1.y);

	Float2 R1;
	R1.x = (1.0f - C2.x) * coordMap->at(C11.y * mapSize.x + C11.x).x + C2.x * coordMap->at(C11.y * mapSize.x + C12.x).x;
	R1.y = (1.0f - C2.x) * coordMap->at(C11.y * mapSize.x + C11.x).y + C2.x * coordMap->at(C11.y * mapSize.x + C12.x).y;

	Float2 R2;
	R2.x = (1.0f - C2.x) * coordMap->at((C12.y) * mapSize.x + C1.x).x + C2.x * coordMap->at((C12.y) * mapSize.x + C12.x).x;
	R2.y = (1.0f - C2.x) * coordMap->at((C12.y) * mapSize.x + C1.x).y + C2.x * coordMap->at((C12.y) * mapSize.x + C12.x).y;

	Float2 R;
	R.x = (1.0f - C2.y) * R1.x + C2.y * R2.x;
	R.y = (1.0f - C2.y) * R1.y + C2.y * R2.y;

	return R;
}

void RecalculationMapFromCoord::GenerateResize()
{
	int H, W, S;
	int H2, W2, S2;
	H = 8192;
	W = 8192;
	S = H * W;

	H2 = 512;
	W2 = 512;
	S2 = H2 * W2;

	int SPC = H / H2;

	std::vector<float> HM(S, 0.0f);
	std::vector<float> HM2(S2, 0.0f);

	std::vector<Float4> NM(S);
	std::vector<Float4> NM2(S2);

	DataStreaming::LoadFile(GetWholeFilePatch("height map.raw"), &HM[0], H, W, sizeof(float));
	DataStreaming::LoadImageFromFile(GetWholeFilePatch(L"normal map.png"), &NM, H, W);

	for (int y = 0; y < H2; ++y)
	{
		for (int x = 0; x < W2; ++x)
		{
			HM2[y * W2 + x] = HM[y * SPC * W + x * SPC];
			NM2[y * W2 + x] = NM[y * SPC * W + x * SPC];
		}
	}

	HM.resize(0);
	NM.resize(0);

	DataStreaming::SaveFile(GetWholeFilePatch("heightmap_512.raw"), &HM2[0], H2, W2, sizeof(float));
	DataStreaming::SaveImage(GetWholeFilePatch(L"normalmap_512.png"), &NM2, H2, W2);
}

void RecalculationMapFromCoord::Generate()
{
	std::string site[6] = { "+X", "+Y","+Z","-X","-Y","-Z" };
	std::wstring wsite[6] = { L"+X", L"+Y",L"+Z",L"-X",L"-Y",L"-Z" };
	int W, H, S, W2, H2, S2;
	Float2 offset = Float2(256, 0);
	W = 512;
	H = 512;
	S = W * H;
	W2 = 512 + 1 + 16;
	H2 = 512 + 1 + 16;
	S2 = W2 * H2;
	float W3, H3, S3;
	W3 = 1792;
	H3 = 1024;
	S3 = W3 * H3;
	INT2 C = INT2(0, 0);



	std::vector<Float2> M(S2);
	std::vector<Float4> M2(S2);


	std::vector<float> HM(S, 0.0f);
	std::vector<float> HM2(S2 + 2, 0.0f);
	std::vector<Float4> HM3(S2);

	std::vector<Float4> NM(S);
	std::vector<BYTE4> NM2(S2);

	std::vector<Float4> TM(S);
	std::vector<Float4> WM(S);
	std::vector<Float4> TRM(S);
	std::vector<Float4> EM(S2);
	std::vector<BYTE4> EM2(S2);
	std::vector<BYTE4> TRM2(S2);
	std::vector<Float4> TRM3(S2);

	DataStreaming::LoadFile(GetWholeFilePatch("heightmap_512.raw"), &HM[0], H, W, sizeof(float));
	DataStreaming::LoadImageFromFile(GetWholeFilePatch(L"normalmap_512.png"), &NM, H, W);
	DataStreaming::LoadImageFromFile(GetWholeFilePatch(L"warmmap_512.png"), &TM, H, W);
	DataStreaming::LoadImageFromFile(GetWholeFilePatch(L"watermap_512.png"), &WM, H, W);
	DataStreaming::LoadImageFromFile(GetWholeFilePatch(L"trees_512.png"), &TRM, H, W);

	for (int i = 0; i < 6; ++i)
	{
		std::stringstream str;
		std::stringstream sH;
		std::wstringstream sH2;
		std::stringstream sN;
		std::stringstream sE;
		std::stringstream sT;
		std::wstringstream sE2;
		std::wstringstream sT2;

		str << "coord_" << site[i] << ".txt";
		sH << "height_" << site[i] << ".raw";
		sH2 << L"height_" << wsite[i] << L".png";
		sN << "normal_" << site[i] << ".raw";
		sE << "enviroment_" << site[i] << ".raw";
		sT << "trees_" << site[i] << ".raw";
		sE2 << L"enviroment_" << wsite[i] << L".png";
		sT2 << L"trees_" << wsite[i] << L".png";

		DataStreaming::LoadFile(GetWholeFilePatch(&str.str()[0]), &M[0], H2, W2, sizeof(Float2));

		float min = 9999.0f;
		INT2 CMIN;
		INT2 CMIN2;
		float max = -9999.0f;
		INT2 CMAX;
		INT2 CMAX2;

		for (int j = 0; j < S2; ++j)
		{
			M2[j].x = M[j].x;
			M2[j].y = M[j].y;
			M2[j].z = 0.0f;
			M2[j].w = 0.0f;

			C = INT2((float)M[j].x * H3 - offset.x, (float)M[j].y *W3 - offset.y);

			if (C.x >= 0 && C.x < H && C.y >= 0 && C.y < W)
			{
				HM2[j] = HM[C.x * H + C.y];
				NM2[j].x = NM[C.x * H + C.y].x * 255.0f + 0.5f;
				NM2[j].y = NM[C.x * H + C.y].z * 255.0f + 0.5f;
				NM2[j].z = NM[C.x * H + C.y].y * 255.0f + 0.5f;
				NM2[j].w = 0;
				HM3[j].x = VMath::Clamp((HM[C.x * H + C.y] + 2300.0f) / 5000.0f, 0.0, 1.0f);
				HM3[j].y = HM3[j].x;
				HM3[j].z = HM3[j].y;
				HM3[j].w = 1.0f;
				EM[j].x = VMath::Clamp(TM[C.x * H + C.y].x - 0.0001625f * VMath::Clamp(HM2[j], 0.0f, HM2[j]), 0.0f, 1.0f);
				EM[j].y = WM[C.x * H + C.y].x;
				TRM2[j] = BYTE4(TRM[C.x * H + C.y].x * 255, 0, 0, 0);

				if (HM2[j] < 0.0f)
				{
					EM[j].y = 1.0f;
				}
				else if (HM2[j] < 60.0f)
				{
					EM[j].y = 1.0f + 0.01666f * HM2[j] * (EM[j].y - 1.0f);
				}

				if (EM[j].x < 0.25f && EM[j].y < 0.31f)
				{
					TRM2[j].x = 0;
				}

				if ((EM[j].x < 0.25f || EM[j].y < 0.31f) || HM2[j] < 20.0f)
				{
					TRM2[j].x = 0;
				}

				TRM3[j] = Float4((float)TRM2[j].x * 0.00392156f, 0.0f, 0.0f, 0.0f);
			}
			else
			{
				HM2[j] = -2300.0f;
				NM2[j] = BYTE4(127, 255, 127, 0);
				HM3[j] = Float4(0.0f, 0.0f, 0.0f, 1.0f);
				EM[j].x = 0.0f;
				EM[j].y = 0.0f;
				TRM2[j] = BYTE4(0, 0, 0, 0);
				TRM3[j] = Float4(0.0f, 0.0f, 0.0f, 0.0f);
			}

			if (HM2[j] > max) max = HM2[j];
			if (HM2[j] < min) min = HM2[j];

			EM[j].z = 0.0f;
			EM[j].w = 1.0f;

			EM2[j].x = EM[j].x * 255.0f;
			EM2[j].y = EM[j].y * 255.0f;
			EM2[j].z = 0.0f;
			EM2[j].w = 0.0f;
		}
		std::wstringstream str2;
		std::wstringstream strT2;
		str2 << L"coord_" << wsite[i] << L".png";

		HM2[S2] = min;
		HM2[S2 + 1] = max;

		DataStreaming::SaveImage(GetWholeFilePatch(&str2.str()[0]), &M2, H2, W2);
		DataStreaming::SaveImage(GetWholeFilePatch(&sH2.str()[0]), &HM3, H2, W2);
		DataStreaming::SaveImage(GetWholeFilePatch(&sE2.str()[0]), &EM, H2, W2);
		DataStreaming::SaveImage(GetWholeFilePatch(&sT2.str()[0]), &TRM3, H2, W2);
		DataStreaming::SaveFile(GetWholeFilePatch(&sN.str()[0]), &NM2[0], H2, W2, sizeof(BYTE4));
		DataStreaming::SaveFile(GetWholeFilePatch(&sH.str()[0]), &HM2[0], S2 + 2, 1, sizeof(float));
		DataStreaming::SaveFile(GetWholeFilePatch(&sE.str()[0]), &EM2[0], H2, W2, sizeof(BYTE4));
		DataStreaming::SaveFile(GetWholeFilePatch(&sT.str()[0]), &TRM2[0], H2, W2, sizeof(BYTE4));
	}
}

void RecalculationMapFromCoord::GenerateTiles()
{
	std::string site[6] = { "+X", "+Y","+Z","-X","-Y","-Z" };
	std::wstring wsite[6] = { L"+X", L"+Y",L"+Z",L"-X",L"-Y",L"-Z" };
	int W, H, S, W2, H2, S2, WE2, HE2;
	Float2 offset = Float2(4096, 0);
	W = 8192;
	H = 8192;
	S = W * H;
	W2 = 512 + 1 + 2 * 512 / 64;
	H2 = 512 + 1 + 2 * 512 / 64;
	WE2 = (8192 - 512) / 64;
	HE2 = (8192 - 512) / 64;
	S2 = W2 * H2;
	float W3, H3, S3;
	W3 = 28672.0f;
	H3 = 16384.0f;
	S3 = W3 * H3;
	INT2 C = INT2(0, 0);
	int lvl = 4;
	int plvl = pow(2, lvl);
	float ilvl = 1.0f / plvl;

	std::vector<Float2> M(S2);
	std::vector<Float4> M2(S2);

	std::vector<float> HM(S, 0.0f);
	std::vector<float> HM2(S2 + 2, 0.0f);
	std::vector<Float4> HM3(S2);

	std::vector<Float4> NM(S);
	std::vector<BYTE4> NM2(S2);

	std::vector<float> RW1(S);
	std::vector<float> RL1(S);
	std::vector<float> RT1(S);
	std::vector<float> RW2(S);
	std::vector<float> RL2(S);
	std::vector<float> RT2(S);
	std::vector<float> RH(S);

	std::vector<float> RW12(S2);
	std::vector<float> RL12(S2);
	std::vector<float> RT12(S2);
	std::vector<float> RW22(S2);
	std::vector<float> RL22(S2);
	std::vector<float> RT22(S2);
	std::vector<float> RH2(S2);

	std::vector<Float4> RW13(S2);
	/*std::vector<Float4> RL13(S2);
	std::vector<Float4> RT13(S2);*/
	std::vector<Float4> RW23(S2);
	/*std::vector<Float4> RL23(S2);
	std::vector<Float4> RT23(S2);
	std::vector<Float4> RH3(S2);*/

	DataStreaming::LoadFile(GetWholeFilePatch("height map.raw"), &HM[0], H, W, sizeof(float));
	DataStreaming::LoadImageFromFile(GetWholeFilePatch(L"normal map.png"), &NM, H, W);

	DataStreaming::LoadFile(GetWholeFilePatch("river_width1.raw"), &RW1[0], H, W, sizeof(float));
	DataStreaming::LoadFile(GetWholeFilePatch("river_width2.raw"), &RW2[0], H, W, sizeof(float));
	DataStreaming::LoadFile(GetWholeFilePatch("river_length1.raw"), &RL1[0], H, W, sizeof(float));
	DataStreaming::LoadFile(GetWholeFilePatch("river_length2.raw"), &RL2[0], H, W, sizeof(float));
	DataStreaming::LoadFile(GetWholeFilePatch("river_type1.raw"), &RT1[0], H, W, sizeof(float));
	DataStreaming::LoadFile(GetWholeFilePatch("river_type2.raw"), &RT2[0], H, W, sizeof(float));
	DataStreaming::LoadFile(GetWholeFilePatch("river_height.raw"), &RH[0], H, W, sizeof(float));

	for (int i = 0; i < 6; ++i)
	{
		if (i == 3 || i == 5)
		{
			std::stringstream str;

			str << "coord_" << site[i] << ".txt";

			DataStreaming::LoadFile(GetWholeFilePatch(&str.str()[0]), &M[0], H2, W2, sizeof(Float2));

			for (int n = 0; n < plvl; ++n)
			{
				for (int m = 0; m < plvl; ++m)
				{
					std::stringstream sH;
					std::wstringstream sH2;
					std::stringstream sN;
					std::stringstream sRW1;
					std::stringstream sRW2;
					std::stringstream sRL1;
					std::stringstream sRL2;
					std::stringstream sRT1;
					std::stringstream sRT2;
					std::stringstream sRH;
					std::wstringstream sR1;
					std::wstringstream sR2;
					sH << "coord_4/" << "height_" << site[i] << "_" << lvl << "_" << m << "_" << n << ".raw";
					sH2 << L"coord_4\\" << L"height_" << wsite[i] << L"_" << lvl << L"_" << m << L"_" << n << L".png";
					sN << "coord_4/" << "normal_" << site[i] << "_" << lvl << "_" << m << "_" << n << ".raw";
					sRW1 << "coord_4/" << "river_width1_" << site[i] << "_" << lvl << "_" << m << "_" << n << ".raw";
					sRW2 << "coord_4/" << "river_width2_" << site[i] << "_" << lvl << "_" << m << "_" << n << ".raw";
					sRL1 << "coord_4/" << "river_length1_" << site[i] << "_" << lvl << "_" << m << "_" << n << ".raw";
					sRL2 << "coord_4/" << "river_length2_" << site[i] << "_" << lvl << "_" << m << "_" << n << ".raw";
					sRT1 << "coord_4/" << "river_type1_" << site[i] << "_" << lvl << "_" << m << "_" << n << ".raw";
					sRT2 << "coord_4/" << "river_type2_" << site[i] << "_" << lvl << "_" << m << "_" << n << ".raw";
					sRH << "coord_4/" << "river_height_" << site[i] << "_" << lvl << "_" << m << "_" << n << ".raw";
					sR1 << L"coord_4\\" << L"river1_" << wsite[i] << L"_" << lvl << L"_" << m << L"_" << n << L".png";
					sR2 << L"coord_4\\" << L"river2_" << wsite[i] << L"_" << lvl << L"_" << m << L"_" << n << L".png";

					int j = 0;
					Float2 CC;

					bool isTile = false;
					bool isTile1 = false;
					bool isTile2 = false;

					float min = 9999.0f;
					float max = -9999.0f;

					for (int l = 0; l < H2; ++l)
					{
						for (int k = 0; k < W2; ++k)
						{
							j = l * W2 + k;

							CC = GetReCalCoord(Float2(WE2 + m * 512 + k, HE2 + n * 512 + l), &M, Float2(W2, H2), Float2((W2 - 1) * plvl + 1, (H2 - 1) * plvl + 1));

							M2[j].x = CC.x;
							M2[j].y = CC.y;
							M2[j].z = 0.0f;
							M2[j].w = 0.0f;

							C = INT2(CC.x * H3 - offset.x, CC.y * W3 - offset.y);

							if (C.x >= 0 && C.x < H && C.y >= 0 && C.y < W && HM[C.x * H + C.y] > -2000.0f)
							{
								isTile = true;
								HM2[j] = HM[C.x * H + C.y];
								NM2[j].x = NM[C.x * H + C.y].x * 255.0f + 0.5f;
								NM2[j].y = NM[C.x * H + C.y].z * 255.0f + 0.5f;
								NM2[j].z = NM[C.x * H + C.y].y * 255.0f + 0.5f;
								NM2[j].w = 0;
								HM3[j].x = VMath::Clamp((HM[C.x * H + C.y] + 2300.0f) / 5000.0f, 0.0, 1.0f);
								HM3[j].y = HM3[j].x;
								HM3[j].z = HM3[j].y;
								HM3[j].w = 1.0f;

								RW12[j] = RW1[C.x * H + C.y];
								RW22[j] = RW2[C.x * H + C.y];
								RL12[j] = RL1[C.x * H + C.y];
								RL22[j] = RL2[C.x * H + C.y];
								RT12[j] = RT1[C.x * H + C.y];
								RT22[j] = RT2[C.x * H + C.y];
								RH2[j] = RH[C.x * H + C.y];

								if (RW1[C.x * H + C.y] > -1000.0f)
								{
									RW13[j] = Float4(1.0f, 1.0f, 1.0f, 1.0f);
									isTile1 = true;
								}
								else
								{
									RW13[j] = Float4(0.0f, 0.0f, 0.0f, 1.0f);
								}
								if (RW2[C.x * H + C.y] > -1000.0f)
								{
									RW23[j] = Float4(1.0f, 1.0f, 1.0f, 1.0f);
									isTile2 = true;
								}
								else
								{
									RW23[j] = Float4(0.0f, 0.0f, 0.0f, 1.0f);
								}
							}
							else
							{
								HM2[j] = -2300.0f;
								NM2[j] = BYTE4(127, 255, 127, 0);
								HM3[j] = Float4(0.0f, 0.0f, 0.0f, 1.0f);

								RW12[j] = -9999.0f;
								RW22[j] = -9999.0f;
								RL12[j] = -9999.0f;
								RL22[j] = -9999.0f;
								RT12[j] = -9999.0f;
								RT22[j] = -9999.0f;
								RH2[j] = -9999.0f;
								RW13[j] = Float4(0.0f, 0.0f, 0.0f, 1.0f);
								RW23[j] = Float4(0.0f, 0.0f, 0.0f, 1.0f);
							}

							if (HM2[j] > max) max = HM2[j];
							if (HM2[j] < min) min = HM2[j];
						}
					}

					std::wstringstream str2;
					str2 << L"coord_4\\" << L"coord_" << wsite[i] << L"_" << lvl << L"_" << m << L"_" << n << L".png";
					DataStreaming::SaveImage(GetWholeFilePatch(&str2.str()[0]), &M2, H2, W2);

					if (isTile == true)
					{
						HM2[S2] = min;
						HM2[S2 + 1] = max;

						DataStreaming::SaveImage(GetWholeFilePatch(&sH2.str()[0]), &HM3, H2, W2);
						DataStreaming::SaveFile(GetWholeFilePatch(&sN.str()[0]), &NM2[0], H2, W2, sizeof(BYTE4));
						DataStreaming::SaveFile(GetWholeFilePatch(&sH.str()[0]), &HM2[0], S2 + 2, 1, sizeof(float));
						DataStreaming::SaveImage(GetWholeFilePatch(&sH2.str()[0]), &HM3, H2, W2);

						if (isTile1 == true)
						{
							DataStreaming::SaveFile(GetWholeFilePatch(&sRW1.str()[0]), &RW12[0], H2, W2, sizeof(float));
							DataStreaming::SaveFile(GetWholeFilePatch(&sRL1.str()[0]), &RL12[0], H2, W2, sizeof(float));
							DataStreaming::SaveFile(GetWholeFilePatch(&sRT1.str()[0]), &RT12[0], H2, W2, sizeof(float));
							DataStreaming::SaveImage(GetWholeFilePatch(&sR1.str()[0]), &RW13, H2, W2);
						}


						if (isTile2 == true)
						{
							DataStreaming::SaveFile(GetWholeFilePatch(&sRW2.str()[0]), &RW22[0], H2, W2, sizeof(float));
							DataStreaming::SaveFile(GetWholeFilePatch(&sRL2.str()[0]), &RL22[0], H2, W2, sizeof(float));
							DataStreaming::SaveFile(GetWholeFilePatch(&sRT2.str()[0]), &RT22[0], H2, W2, sizeof(float));
							DataStreaming::SaveImage(GetWholeFilePatch(&sR2.str()[0]), &RW23, H2, W2);
						}

						if (isTile1 == true || isTile2 == true)
						{
							DataStreaming::SaveFile(GetWholeFilePatch(&sRH.str()[0]), &RH2[0], H2, W2, sizeof(float));
						}
					}
				}
			}
		}
	}
}

void RecalculationMapFromCoord::ConvertNormalFromPNGtoRAW()
{
	int H = 512;
	int W = 512;
	int S = H * W;
	std::vector<Float4> NM(S);
	std::vector<Float4> NM2(S);

	DataStreaming::LoadImageFromFile(GetWholeFilePatch(L"Tile_Normal.png"), &NM, H, W);

	for (int i = 0; i < NM.size(); ++i)
	{
		NM2[0].x = NM[0].x;
		NM2[0].y = NM[0].z;
		NM2[0].z = NM[0].y;
	}

	DataStreaming::SaveFile(GetWholeFilePatch("Tile_Normal.RAW"), &NM2[0], H, W, sizeof(Float4));
}