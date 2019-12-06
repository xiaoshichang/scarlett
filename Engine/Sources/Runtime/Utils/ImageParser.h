#pragma once
#define STB_IMAGE_IMPLEMENTATION
#include <string>

namespace scarlett {

	class ImageParser {

	public:

		static unsigned char* Parse(const std::string& file, int* width, int* height, int* channels, int reqeryChannel);
		static void Free(void* p);
	};
}