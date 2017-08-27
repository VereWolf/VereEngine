#include "pch.h"
#include "RenderState.h"

ID3D11RasterizerState* RenderStates::WireframeRS = 0;
ID3D11RasterizerState* RenderStates::SolidRS = 0;
ID3D11RasterizerState* RenderStates::BackRS = 0;
ID3D11RasterizerState* RenderStates::NoCullRS = 0;

ID3D11BlendState*      RenderStates::NoBlendBS = 0;
ID3D11BlendState*      RenderStates::AlphaToCoverageBS = 0;
ID3D11BlendState*      RenderStates::TransparentBS = 0;
ID3D11BlendState*      RenderStates::NoAccumulateBS = 0;

ID3D11DepthStencilState* RenderStates::MarkMirrowDDS = 0;
ID3D11DepthStencilState* RenderStates::DrawReflectionDDS = 0;
ID3D11DepthStencilState* RenderStates::NoDoubleBlendDDS = 0;

bool RenderStates::InitAll(DX::DeviceResources *resources)
{
	//
	// WireframeRS
	//
	D3D11_RASTERIZER_DESC wireframeDesc;
	ZeroMemory(&wireframeDesc, sizeof(D3D11_RASTERIZER_DESC));
	wireframeDesc.FillMode = D3D11_FILL_WIREFRAME;
	wireframeDesc.CullMode = D3D11_CULL_NONE;
	wireframeDesc.FrontCounterClockwise = false;
	wireframeDesc.DepthClipEnable = true;

	HR(resources->GetD3DDevice()->CreateRasterizerState(&wireframeDesc, &WireframeRS));

	//
	// SolidRS
	//
	D3D11_RASTERIZER_DESC solidDesc;
	ZeroMemory(&solidDesc, sizeof(D3D11_RASTERIZER_DESC));
	solidDesc.FillMode = D3D11_FILL_SOLID;
	solidDesc.CullMode = D3D11_CULL_FRONT;
	solidDesc.FrontCounterClockwise = false;
	solidDesc.DepthClipEnable = true;

	HR(resources->GetD3DDevice()->CreateRasterizerState(&solidDesc, &SolidRS));

	D3D11_RASTERIZER_DESC backDesc;
	ZeroMemory(&backDesc, sizeof(D3D11_RASTERIZER_DESC));
	backDesc.FillMode = D3D11_FILL_SOLID;
	backDesc.CullMode = D3D11_CULL_BACK;
	backDesc.FrontCounterClockwise = false;
	backDesc.DepthClipEnable = true;

	HR(resources->GetD3DDevice()->CreateRasterizerState(&backDesc, &BackRS));

	//
	// NoCullRS
	//
	D3D11_RASTERIZER_DESC noCullDesc;
	ZeroMemory(&noCullDesc, sizeof(D3D11_RASTERIZER_DESC));
	noCullDesc.FillMode = D3D11_FILL_SOLID;
	noCullDesc.CullMode = D3D11_CULL_NONE;
	noCullDesc.FrontCounterClockwise = false;
	noCullDesc.DepthClipEnable = true;

	HR(resources->GetD3DDevice()->CreateRasterizerState(&noCullDesc, &NoCullRS));

	//
	// AlphaToCoverageBS
	//

	D3D11_BLEND_DESC noBlendDesc = { 0 };
	noBlendDesc.AlphaToCoverageEnable = false;
	noBlendDesc.IndependentBlendEnable = false;
	noBlendDesc.RenderTarget[0].BlendEnable = false;
	noBlendDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

	HR(resources->GetD3DDevice()->CreateBlendState(&noBlendDesc, &NoBlendBS));

	D3D11_BLEND_DESC alphaToCoverageDesc = { 0 };
	alphaToCoverageDesc.AlphaToCoverageEnable = true;
	alphaToCoverageDesc.IndependentBlendEnable = true;
	alphaToCoverageDesc.RenderTarget[0].BlendEnable = false;
	alphaToCoverageDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

	HR(resources->GetD3DDevice()->CreateBlendState(&alphaToCoverageDesc, &AlphaToCoverageBS));

	//
	// TransparentBS
	//

	D3D11_BLEND_DESC transparentDesc = { 0 };
	transparentDesc.AlphaToCoverageEnable = false;
	transparentDesc.IndependentBlendEnable = false;

	transparentDesc.RenderTarget[0].BlendEnable = true;
	transparentDesc.RenderTarget[0].SrcBlend =  D3D11_BLEND_INV_SRC_ALPHA;
	transparentDesc.RenderTarget[0].DestBlend = D3D11_BLEND_SRC_ALPHA;
	transparentDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
	transparentDesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
	transparentDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
	transparentDesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
	transparentDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

	HR(resources->GetD3DDevice()->CreateBlendState(&transparentDesc, &TransparentBS));

	//
	// NoAccumulateBS
	//

	D3D11_BLEND_DESC noAccumulateDesc = { 0 };
	noAccumulateDesc.AlphaToCoverageEnable = false;

	noAccumulateDesc.RenderTarget[0].BlendEnable = true;
	noAccumulateDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
	noAccumulateDesc.RenderTarget[0].DestBlend = D3D11_BLEND_ONE;
	noAccumulateDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_MAX;
	noAccumulateDesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_SRC_ALPHA;
	noAccumulateDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_DEST_ALPHA;
	noAccumulateDesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_MAX;
	noAccumulateDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

	HR(resources->GetD3DDevice()->CreateBlendState(&noAccumulateDesc, &NoAccumulateBS));

	D3D11_DEPTH_STENCIL_DESC mirrorDesc;
	mirrorDesc.DepthEnable = true;
	mirrorDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO;
	mirrorDesc.DepthFunc = D3D11_COMPARISON_LESS;
	mirrorDesc.StencilEnable = true;
	mirrorDesc.StencilReadMask = 0xff;
	mirrorDesc.StencilWriteMask = 0xff;

	mirrorDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	mirrorDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;
	mirrorDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_REPLACE;
	mirrorDesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

	mirrorDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	mirrorDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;
	mirrorDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_REPLACE;
	mirrorDesc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

	HR(resources->GetD3DDevice()->CreateDepthStencilState(&mirrorDesc, &MarkMirrowDDS));

	D3D11_DEPTH_STENCIL_DESC drawReflectionDesc;
	drawReflectionDesc.DepthEnable = true;
	drawReflectionDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	drawReflectionDesc.DepthFunc = D3D11_COMPARISON_LESS;
	drawReflectionDesc.StencilEnable = true;
	drawReflectionDesc.StencilReadMask = 0xff;
	drawReflectionDesc.StencilWriteMask = 0xff;

	drawReflectionDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	drawReflectionDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;
	drawReflectionDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	drawReflectionDesc.FrontFace.StencilFunc = D3D11_COMPARISON_EQUAL;

	drawReflectionDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	drawReflectionDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;
	drawReflectionDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	drawReflectionDesc.BackFace.StencilFunc = D3D11_COMPARISON_EQUAL;

	HR(resources->GetD3DDevice()->CreateDepthStencilState(&drawReflectionDesc, &DrawReflectionDDS));

	D3D11_DEPTH_STENCIL_DESC noDoubleBlendDesc;
	noDoubleBlendDesc.DepthEnable = true;
	noDoubleBlendDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	noDoubleBlendDesc.DepthFunc = D3D11_COMPARISON_LESS;
	noDoubleBlendDesc.StencilEnable = true;
	noDoubleBlendDesc.StencilReadMask = 0xff;
	noDoubleBlendDesc.StencilWriteMask = 0xff;

	noDoubleBlendDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	noDoubleBlendDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;
	noDoubleBlendDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_INCR;
	noDoubleBlendDesc.FrontFace.StencilFunc = D3D11_COMPARISON_EQUAL;

	noDoubleBlendDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	noDoubleBlendDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;
	noDoubleBlendDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_INCR;
	noDoubleBlendDesc.BackFace.StencilFunc = D3D11_COMPARISON_EQUAL;

	HR(resources->GetD3DDevice()->CreateDepthStencilState(&noDoubleBlendDesc, &NoDoubleBlendDDS));

	return true;
}

void RenderStates::DestroyAll()
{
	ReleaseCOM(WireframeRS);
	ReleaseCOM(SolidRS);
	ReleaseCOM(BackRS);
	ReleaseCOM(NoCullRS);
	ReleaseCOM(NoBlendBS);
	ReleaseCOM(AlphaToCoverageBS);
	ReleaseCOM(TransparentBS);
	ReleaseCOM(NoAccumulateBS);
	ReleaseCOM(MarkMirrowDDS);
	ReleaseCOM(DrawReflectionDDS);
	ReleaseCOM(NoDoubleBlendDDS);
}