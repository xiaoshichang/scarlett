#pragma once
#define STB_IMAGE_IMPLEMENTATION
#include <string>
#include "Runtime/Utils/stb_image.h"

namespace scarlett {

	class ImageParser {

	public:

		static unsigned char* Parse(const std::string& file, int* width, int* height, int* channels, int reqeryChannel) {
			auto data = stbi_load(file.c_str(), width, height, channels, reqeryChannel);
			return data;
		}
	};
}