#pragma once

#include <unordered_map>
#include <string>
#include "Runtime/Interface/IModule.h"
#include "Runtime/Core/Math/Vector.h"
#include <freetype2/ft2build.h>
#include FT_FREETYPE_H

namespace scarlett {


	struct FontCharacter {
	public:
		Vector2Type<int> Size;       // font size
		Vector2Type<int> Bearing;    // left top offset
		int   Advance;				// offset to next font
		unsigned char* buffer;		// bitmap buffer
	};

	class FontCharacterSet {
	public:
		std::unordered_map<char, std::shared_ptr<FontCharacter>>	mCharacters;
	};

	class FontMgr : public IModule {
	public:
		virtual int		Initialize() noexcept;
		virtual void	Finalize() noexcept;

	public:
		void	LoadFont(const std::string& font);
		FT_Face	GetFont(const std::string& font);

		std::shared_ptr<FontCharacter>	GetCharacterDetail(const std::string& style, const char c);

	private:
		FT_Library	mLibrary;

		std::unordered_map<std::string, FT_Face>	mFonts;
		std::unordered_map<std::string, std::shared_ptr<FontCharacterSet>>			mCharacterSet;
	};

}