#include "Foundation/Assert.h"
#include "FontMgr.h"
#include "Runtime/Core/Application/AssetFinder.h"
#include "Runtime/Core/Memory/MemoryManager.h"

int scarlett::FontMgr::Initialize() noexcept
{
	auto error = FT_Init_FreeType(&mLibrary);
	if (error) {
		SCARLETT_ASSERT(false);
	}

	LoadFont("arial.ttf");

	return 0;
}

void scarlett::FontMgr::Finalize() noexcept
{
	for (auto pair : mFonts) {
		FT_Done_Face(pair.second);
	}
	FT_Done_FreeType(mLibrary);
}

void scarlett::FontMgr::LoadFont(const std::string & style)
{
	if (mFonts[style]) {
		return;
	}
	auto path = GAssetFinder::GetInstance()->GetRealPath("Fonts/" + style);

	FT_Face font;
	auto error = FT_New_Face(mLibrary, path.c_str(), 0, &font);
	if (error) {
		SCARLETT_ASSERT(false);
	}
	FT_Set_Pixel_Sizes(font, 0, 16);
	mFonts[style] = font;
	mCharacterSet[style] = std::make_shared<FontCharacterSet>();
}

std::shared_ptr<scarlett::FontCharacter> scarlett::FontMgr::GetCharacterDetail(const std::string & style, const char c)
{
	auto characterSet = mCharacterSet[style];
	if (characterSet->mCharacters.find(c) == characterSet->mCharacters.end()) {
		auto font = mFonts[style];
		if (FT_Load_Char(font, c, FT_LOAD_RENDER)) {
			SCARLETT_ASSERT(false);
		}

		auto glyph = font->glyph;
		auto character = std::make_shared<FontCharacter>();
		character->Size = Vector2Type<int>(glyph->bitmap.width, glyph->bitmap.rows);
		character->Bearing = Vector2Type<int>(glyph->bitmap_left, glyph->bitmap_top);
		character->Advance = glyph->advance.x;
		character->buffer = (unsigned char*)GMemoryManager::GetInstance()->Allocate(character->Size.x * character->Size.y);
		memcpy(character->buffer, glyph->bitmap.buffer, character->Size.x * character->Size.y);
		characterSet->mCharacters[c] = character;
	}
	return characterSet->mCharacters[c];

}

FT_Face scarlett::FontMgr::GetFont(const std::string& font) {
	return mFonts[font];
}