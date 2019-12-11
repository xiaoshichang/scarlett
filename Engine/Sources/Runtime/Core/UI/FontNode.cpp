#include "FontNode.h"
#include "Director.h"
#include "Runtime/Core/Font/FontMgr.h"
#include "Runtime/Core/Application/Application.h"
#include "Runtime/Core/Application/GlobalConfig.h"

using namespace scarlett;

FontNode * scarlett::FontNode::Create(const std::string & text, const std::string & style)
{
	auto font = new FontNode();
	font->SetText(text);
	font->SetStyle(style);
	font->SetColor(Vector3f(1.0f, 1.0f, 0.6f));
	return font;
}


void scarlett::FontNode::Visit(Renderer * render, const Matrix4x4f & parentTransform, uint32_t flags)
{
	UINode::Visit(render, parentTransform, flags);
}

void scarlett::FontNode::Render(Renderer * render, const Matrix4x4f & transform, uint32_t flags)
{
	if (mText.size() == 0)return;

	UpdateContent();

	auto config = GlobalConfig::GetInstance();
	float width = mTextureWidth;
	float height = mTextureHeight;
	Matrix4x4f world;
	BuildMatrixScale(world, width / config->GetScreenWidth(), height / config->GetScreenHeight() , 1);
	render->RenderFont(mTexture, world, transform, flags);
}

void scarlett::FontNode::SetText(const std::string & text)
{
	mText = text;
	mContentDirty = true;
}

std::string scarlett::FontNode::GetText()
{
	return mText;
}

void scarlett::FontNode::SetColor(const Vector3f & color)
{
	mColor = color;
	mContentDirty = true;
}

Vector3f scarlett::FontNode::GetColor()
{
	return mColor;
}

void scarlett::FontNode::SetStyle(const std::string & style)
{
	mStyle = style;
	mContentDirty = true;
}

std::string scarlett::FontNode::GetStyle()
{
	return mStyle;
}

void scarlett::FontNode::UpdateContent()
{
	if (!mContentDirty) return;

	CalculateTextureHeight();
	CopyFontBitmap();

	mContentDirty = false;
}

void scarlett::FontNode::ResizeTexture(int width, int height)
{
	mTextureWidth = width;
	mTextureHeight = height;
}

void scarlett::FontNode::CopyFontBitmap()
{
	unsigned char red = mColor.x * 255;
	unsigned char green = mColor.y * 255;
	unsigned char blue = mColor.z * 255;

	int textureSize = mTextureWidth * mTextureHeight * sizeof(unsigned char) * 4;
	unsigned char* data = (unsigned char*)GMemoryManager::GetInstance()->Allocate(textureSize);
	
	memset(data, 0, textureSize);

	int advance = 0;
	for (int idx = 0; idx < mText.size(); idx++) {
		auto character = mText[idx];
		auto characterDetail = GApp->mFontMgr->GetCharacterDetail(mStyle, character);
		int width = (characterDetail->Advance >> 6);
		int height = mTextureHeight;

		int left = characterDetail->Bearing.x;
		int right = characterDetail->Bearing.x + characterDetail->Size.x;
		int top = mTextureHeight - characterDetail->Bearing.y - mTextureOriginY;
		int bottom = top + characterDetail->Size.y;

		for (int w = 0; w < width; w++) {
			for (int h = 0; h < height; h++) {
				if (w >= left && w < right && h >= top && h < bottom) {
					unsigned char power = characterDetail->buffer[(h - top) * characterDetail->Size.x + w - characterDetail->Bearing.x];
					data[(h * mTextureWidth + advance + w) * 4] = red;
					data[(h * mTextureWidth + advance + w) * 4 + 1] = green;
					data[(h * mTextureWidth + advance + w) * 4 + 2] = blue;
					data[(h * mTextureWidth + advance + w) * 4 + 3] = power;
				}
				else {
					data[(h * mTextureWidth + advance + w) * 4] = 0;
					data[(h * mTextureWidth + advance + w) * 4 + 1] = 0;
					data[(h * mTextureWidth + advance + w) * 4 + 2] = 0;
					data[(h * mTextureWidth + advance + w) * 4 + 3] = 0;
				}
			}
		}
		advance += width;
	}
	mTexture = GApp->mGraphicsManager->CreateTexture2D(mTextureWidth, mTextureHeight, data);
}

void scarlett::FontNode::CalculateTextureHeight()
{
	int height = 0;
	int width = 0;
	int maxHeight = 0, minHeight = 0;
	
	for (int idx = 0; idx < mText.size(); idx++) {
		auto character = mText[idx];
		auto characterDetail = GApp->mFontMgr->GetCharacterDetail(mStyle, character);
		auto top = characterDetail->Bearing.y;
		auto bottom = top - characterDetail->Size.y;
		if (top > maxHeight) {
			maxHeight = top;
		}
		if (bottom < minHeight) {
			minHeight = bottom;
		}
		width += (characterDetail->Advance >> 6);
	}
	height = maxHeight - minHeight;
	mTextureOriginY = -minHeight;

	if (width > mTextureWidth || height > mTextureHeight) {
		ResizeTexture(width, height);
	}
}

scarlett::FontNode::FontNode() :
	mTextureWidth(0),
	mTextureHeight(0),
	mTextureOriginY(0),
	mContentDirty(true)
{
}
