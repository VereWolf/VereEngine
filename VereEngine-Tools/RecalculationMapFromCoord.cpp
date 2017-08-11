#include "stdafx.h"
#include "RecalculationMapFromCoord.h"


RecalculationMapFromCoord::RecalculationMapFromCoord()
{
	SetFolderName(L"Calc_Map");
}

Float2 RecalculationMapFromCoord::GetReCalCoord(Float2 coord, std::vector<Float2> coordMap, Float2 mapSize, Float2 sourceSize)
{
	Float2 C = Float2(mapSize.x * coord.x / sourceSize.x, mapSize.y * coord.y / sourceSize.y);
	INT2 C1 = INT2(C.x, C.y);
	Float2 C2 = Float2(C.x - (float)C1.x, C.y - (float)C.y);

	Float2 R1;
	R1.x = (1.0f - C2.x) * coordMap.at(C1.y * mapSize.x + C1.x).x + C2.x * C2.x * coordMap.at(C1.y * mapSize.x + C1.x + 1).x;
	R1.y = (1.0f - C2.x) * coordMap.at(C1.y * mapSize.x + C1.x).y + C2.x * C2.x * coordMap.at(C1.y * mapSize.x + C1.x + 1).y;

	Float2 R2;
	R2.x = (1.0f - C2.x) * coordMap.at((C1.y + 1) * mapSize.x + C1.x).x + C2.x * C2.x * coordMap.at((C1.y + 1) * mapSize.x + C1.x + 1.0f).x;
	R2.y = (1.0f - C2.x) * coordMap.at((C1.y + 1) * mapSize.x + C1.x).y + C2.x * C2.x * coordMap.at((C1.y + 1) * mapSize.x + C1.x + 1.0f).y;

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
	W2 = 512 + 16;
	H2 = 512 + 16;
	S2 = W2 * H2;
	float W3, H3, S3;
	W3 = 1792;
	H3 = 1024;
	S3 = W3 * H3;
	INT2 C = INT2(0, 0);



	std::vector<Float2> M(S2);
	std::vector<Float4> M2(S2);


	std::vector<float> HM(S, 0.0f);
	std::vector<float> HM2(S2, 0.0f);
	std::vector<Float4> HM3(S2);

	std::vector<Float4> NM(S);
	std::vector<Float3> NM2(S2);

	DataStreaming::LoadFile(GetWholeFilePatch("heightmap_512.raw"), &HM[0], H, W, sizeof(float));
	DataStreaming::LoadImageFromFile(GetWholeFilePatch(L"normalmap_512.png"), &NM, H, W);

	for (int i = 0; i < 6; ++i)
	{
		std::stringstream str;
		std::stringstream sH;
		std::wstringstream sH2;
		std::stringstream sN;

		str << "coord_" << site[i] << ".txt";
		sH << "height_" << site[i] << ".raw";
		sH2 << L"height_" << wsite[i] << L".png";
		sN << "normal_" << site[i] << ".raw";

		DataStreaming::LoadFile(GetWholeFilePatch(&str.str()[0]), &M[0], H2, W2, sizeof(Float2));

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
				NM2[j].x = NM[C.x * H + C.y].x;
				NM2[j].y = NM[C.x * H + C.y].z;
				NM2[j].z = NM[C.x * H + C.y].y;
				HM3[j].x = VMath::Clamp((HM[C.x * H + C.y] + 2300.0f) / 5000.0f, 0.0, 1.0f);
				HM3[j].y = HM3[j].x;
				HM3[j].z = HM3[j].y;
				HM3[j].w = 1.0f;
			}
			else
			{
				HM2[j] = -2300.0f;
				NM2[j] = Float3(0.5f, 1.0f, 0.5f);
				HM3[j] = Float4(0.0f, 0.0f, 0.0f, 1.0f);
			}
		}
		std::wstringstream str2;
		str2 << L"coord_" << wsite[i] << L".png";
		DataStreaming::SaveImage(GetWholeFilePatch(&str2.str()[0]), &M2, H2, W2);
		DataStreaming::SaveImage(GetWholeFilePatch(&sH2.str()[0]), &HM3, H2, W2);
		DataStreaming::SaveFile(GetWholeFilePatch(&sN.str()[0]), &NM2[0], H2, W2, sizeof(Float3));
		DataStreaming::SaveFile(GetWholeFilePatch(&sH.str()[0]), &HM2[0], H2, W2, sizeof(float));
	}
}

void RecalculationMapFromCoord::GenerateTiles()
{
	std::string site[6] = { "+X", "+Y","+Z","-X","-Y","-Z" };
	std::wstring wsite[6] = { L"+X", L"+Y",L"+Z",L"-X",L"-Y",L"-Z" };
	int W, H, S, W2, H2, S2;
	Float2 offset = Float2(4096, 0);
	W = 8192;
	H = 8192;
	S = W * H;
	W2 = 512 + 2 * 512 / 64;
	H2 = 512 + 2 * 512 / 64;
	S2 = W2 * H2;
	float W3, H3, S3;
	W3 = 25735.92702f;
	H3 = 16384.0f;
	S3 = W3 * H3;
	INT2 C = INT2(0, 0);
	int lvl = 4;
	int plvl = pow(2, lvl);
	float ilvl = 1.0f / plvl;



	std::vector<Float2> M(S2);
	std::vector<Float4> M2(S2);


	std::vector<float> HM(S, 0.0f);
	std::vector<float> HM2(S2, 0.0f);
	std::vector<Float4> HM3(S2);

	std::vector<Float4> NM(S);
	std::vector<Float3> NM2(S2);

	DataStreaming::LoadFile(GetWholeFilePatch("height map.raw"), &HM[0], H, W, sizeof(float));
	DataStreaming::LoadImageFromFile(GetWholeFilePatch(L"normal map.png"), &NM, H, W);

	for (int i = 0; i < 6; ++i)
	{
		for (int n = 0; n < plvl; ++n)
		{
			for (int m = 0; m < plvl; ++m)
			{
				std::stringstream str;
				std::stringstream sH;
				std::wstringstream sH2;
				std::stringstream sN;
				str << "coord_4/" << "coord_" << site[i] << "_" << lvl << "_" << m << "_" << n << ".txt";
				sH << "coord_4/" << "height_" << site[i] << "_" << lvl << "_" << m << "_" << n << ".raw";
				sH2 << L"coord_4\\" << L"height_" << wsite[i] << L"_" << lvl << L"_" << m << L"_" << n << L".png";
				sN << "coord_4/" << "normal_" << site[i] << "_" << lvl << "_" << m << "_" << n << ".raw";

				DataStreaming::LoadFile(GetWholeFilePatch(&str.str()[0]), &M[0], H2, W2, sizeof(Float2));

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
						NM2[j].x = NM[C.x * H + C.y].x;
						NM2[j].y = NM[C.x * H + C.y].z;
						NM2[j].z = NM[C.x * H + C.y].y;
						HM3[j].x = VMath::Clamp((HM[C.x * H + C.y] + 2300.0f) / 5000.0f, 0.0, 1.0f);
						HM3[j].y = HM3[j].x;
						HM3[j].z = HM3[j].y;
						HM3[j].w = 1.0f;
					}
					else
					{
						HM2[j] = -2300.0f;
						NM2[j] = Float3(0.5f, 1.0f, 0.5f);
						HM3[j] = Float4(0.0f, 0.0f, 0.0f, 1.0f);
					}
				}
				std::wstringstream str2;
				str2 << L"coord_4\\" << L"coord_" << wsite[i] << L"_" << lvl << L"_" << m << L"_" << n << L".png";
				DataStreaming::SaveImage(GetWholeFilePatch(&str2.str()[0]), &M2, H2, W2);
				DataStreaming::SaveImage(GetWholeFilePatch(&sH2.str()[0]), &HM3, H2, W2);
				DataStreaming::SaveFile(GetWholeFilePatch(&sN.str()[0]), &NM2[0], H2, W2, sizeof(Float3));
				DataStreaming::SaveFile(GetWholeFilePatch(&sH.str()[0]), &HM2[0], H2, W2, sizeof(float));
			}
		}
	}
}