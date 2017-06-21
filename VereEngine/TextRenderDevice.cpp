#include "pch.h"
#include "TextRenderDevice.h"

#include "DirectXHelper.h"

using namespace Microsoft::WRL;

TextRenderDevice::TextRenderDevice()
{
	GameTextRenderDeviceHandle = this;
}

// Initializes D2D resources used for text rendering.
TextRenderDevice::TextRenderDevice(DX::DeviceResources* deviceResources) : 
	m_resources(deviceResources)
{
	GameTextRenderDeviceHandle = this;

	ZeroMemory(&m_textMetrics, sizeof(DWRITE_TEXT_METRICS));

	DX::ThrowIfFailed(
		m_resources->GetD2DFactory()->CreateDrawingStateBlock(&m_stateBlock)
		);

	m_renderTextAssetsStacks.m_gameColorBrushs.Init(deviceResources);
	m_renderTextAssetsStacks.m_gameTextFormats.Init(deviceResources);
	m_renderTextAssetsStacks.m_gameTextLayouts.Init(deviceResources);
}

// Renders a frame to the screen.
void TextRenderDevice::Render(TextMessage* message)
{
	DX::ThrowIfFailed(
		((GameTextLayout*)m_renderTextAssetsStacks.m_gameTextLayouts.GetGameObject(message->textLayoutID))->GetTextLayout()->GetMetrics(&m_textMetrics)
	);

	ID2D1DeviceContext* context = m_resources->GetD2DDeviceContext();
	Windows::Foundation::Size logicalSize = m_resources->GetOutputSize();
	logicalSize.Width -= m_textMetrics.layoutWidth;
	logicalSize.Height -= m_textMetrics.height;

	context->SaveDrawingState(m_stateBlock.Get());
	context->BeginDraw();

	// Position on the bottom right corner
	D2D1::Matrix3x2F screenTranslation = D2D1::Matrix3x2F::Translation(
		logicalSize.Width,
		logicalSize.Height
	);

	context->SetTransform(screenTranslation * m_resources->GetOrientationTransform2D());

	DX::ThrowIfFailed(
		((GameTextFormat*)m_renderTextAssetsStacks.m_gameTextFormats.GetGameObject(message->textFormatID))->GetTextFormat()->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_TRAILING)
		);

	D2D1_POINT_2F coord = D2D1::Point2F(
		logicalSize.Width * (message->position.x - 1.0f),
		logicalSize.Height * (message->position.y - 1.0f)
	);

	context->DrawTextLayout(
		coord,
		((GameTextLayout*)m_renderTextAssetsStacks.m_gameTextLayouts.GetGameObject(message->textLayoutID))->GetTextLayout(),
		((GameColorBrush*)m_renderTextAssetsStacks.m_gameColorBrushs.GetGameObject(message->textLayoutID))->GetColorBrush()
	);

	// Ignore D2DERR_RECREATE_TARGET here. This error indicates that the device
	// is lost. It will be handled during the next call to Present.
	HRESULT hr = context->EndDraw();
	if (hr != D2DERR_RECREATE_TARGET)
	{
		DX::ThrowIfFailed(hr);
	}

	context->RestoreDrawingState(m_stateBlock.Get());
}

int TextRenderDevice::CreateColorBrush(const D2D1_COLOR_F color)
{
	ID2D1SolidColorBrush* solidColorBrush;
	m_resources->GetD2DDeviceContext()->CreateSolidColorBrush(color, &solidColorBrush);

	GameColorBrush *colorBrush = new GameColorBrush;
	colorBrush->PreInit(m_resources);
	colorBrush->SetColorBrush(solidColorBrush);

	return m_renderTextAssetsStacks.m_gameColorBrushs.CreateGameObject(colorBrush);
}

void TextRenderDevice::DeleteColorBrush(int id)
{
	m_renderTextAssetsStacks.m_gameColorBrushs.DeleteGameObject(id);
}

ID2D1SolidColorBrush* TextRenderDevice::GetColorBrush(int id)
{
	return ((GameColorBrush*)m_renderTextAssetsStacks.m_gameColorBrushs.GetGameObject(id))->GetColorBrush();
}

int TextRenderDevice::CreateTextLayout(wstring text, int textFormatId, FLOAT maxWidth, FLOAT maxHeight)
{
	IDWriteTextLayout *writeTextLayout;
	m_resources->GetDWriteFactory()->CreateTextLayout(text.c_str(), text.length(), GetTextFormat(textFormatId), maxWidth, maxHeight, &writeTextLayout);

	GameTextLayout *textLayout = new GameTextLayout;
	textLayout->PreInit(m_resources);
	textLayout->SetTextLayout(writeTextLayout);

	return  m_renderTextAssetsStacks.m_gameTextLayouts.CreateGameObject(textLayout);
}

void TextRenderDevice::DeleteTextLayout(int id)
{
	m_renderTextAssetsStacks.m_gameTextLayouts.DeleteGameObject(id);
}

IDWriteTextLayout* TextRenderDevice::GetTextLayout(int id)
{
	return ((GameTextLayout*)m_renderTextAssetsStacks.m_gameTextLayouts.GetGameObject(id))->GetTextLayout();
}

int TextRenderDevice::CreateTextFormat(
	wstring fontFamilyName,
	IDWriteFontCollection* fontCollection,
	DWRITE_FONT_WEIGHT fontWeight,
	DWRITE_FONT_STYLE fontStyle,
	DWRITE_FONT_STRETCH fontStretch,
	FLOAT fontSize,
	_In_z_ WCHAR const* localeName)
{
	IDWriteTextFormat* writeTextFormat;
	m_resources->GetDWriteFactory()->CreateTextFormat(fontFamilyName.c_str(), fontCollection, fontWeight, fontStyle, fontStretch, fontSize, localeName, &writeTextFormat);

	GameTextFormat * textFormat = new GameTextFormat;
	textFormat->PreInit(m_resources);
	textFormat->SetTextFormat(writeTextFormat);

	return m_renderTextAssetsStacks.m_gameTextFormats.CreateGameObject(textFormat);
}

void TextRenderDevice::DeleteTextFormat(int id)
{
	m_renderTextAssetsStacks.m_gameTextFormats.DeleteGameObject(id);
}

IDWriteTextFormat* TextRenderDevice::GetTextFormat(int id)
{
	return ((GameTextFormat*)m_renderTextAssetsStacks.m_gameTextFormats.GetGameObject(id))->GetTextFormat();
}