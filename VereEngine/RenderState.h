#pragma once

#include "DeviceResources.h"
#include "pch.h"

class RenderStates
{
public:
	static bool InitAll(DX::DeviceResources *resources);
	static void DestroyAll();

	static ID3D11RasterizerState* WireframeRS;
	static ID3D11RasterizerState* SolidRS;
	static ID3D11RasterizerState* BackRS;
	static ID3D11RasterizerState* NoCullRS;

	static ID3D11BlendState* AlphaToCoverageBS;
	static ID3D11BlendState* TransparentBS;
	static ID3D11BlendState* NoAccumulateBS;
	static ID3D11BlendState* NoBlendBS;

	static ID3D11DepthStencilState * MarkMirrowDDS;
	static ID3D11DepthStencilState * DrawReflectionDDS;
	static ID3D11DepthStencilState * NoDoubleBlendDDS;
};