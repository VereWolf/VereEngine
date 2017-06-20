#pragma once

#include "DeviceResources.h"
#include "GameBaseObjectsStack.h"
#include "Game2DColorBrush.h"
#include "Game2DTextFormat.h"
#include "Game2DTextLayout.h"
#include "..\StepTimer.h"

using namespace Game2DRenderDevice;

struct TextMessage
{
	XMFLOAT2 position;
	int colorBrushID;
	int textLayoutID;
	int textFormatID;
};

	// Renders the current FPS value in the bottom right corner of the screen using Direct2D and DirectWrite.
class TextRenderDevice
{
public:
	TextRenderDevice();
	TextRenderDevice(DX::DeviceResources* deviceResources);
	void Render(TextMessage* message);

	int CreateColorBrush(const D2D1_COLOR_F color);
	void DeleteColorBrush(int id);
	ID2D1SolidColorBrush* GetColorBrush(int id);
	int CreateTextLayout(wstring text, int textFormatId, FLOAT maxWidth, FLOAT maxHeight);
	void DeleteTextLayout(int id);
	IDWriteTextLayout* GetTextLayout(int id);
	int CreateTextFormat(
		wstring fontFamilyName,
		IDWriteFontCollection* fontCollection,
		DWRITE_FONT_WEIGHT fontWeight,
		DWRITE_FONT_STYLE fontStyle,
		DWRITE_FONT_STRETCH fontStretch,
		FLOAT fontSize,
		_In_z_ WCHAR const* localeName);
	void DeleteTextFormat(int id);
	IDWriteTextFormat* GetTextFormat(int id);

	void NextStepOfOffset(float s) { m_offsetOfText += s; }
	void SetOffset(float o) { m_offsetOfText = o; }
	float GetOffset() { return m_offsetOfText; }

private:
	// Cached pointer to device resources.
	//std::shared_ptr<DX::DeviceResources> m_deviceResources;
	DX::DeviceResources *m_resources;

	// Resources related to text rendering.
	DWRITE_TEXT_METRICS	                            m_textMetrics;
	Microsoft::WRL::ComPtr<ID2D1DrawingStateBlock1> m_stateBlock;

	RenderTextAssetsStacks m_renderTextAssetsStacks;

	float m_offsetOfText;
};

__declspec(selectany) TextRenderDevice *GameTextRenderDeviceHandle = 0;