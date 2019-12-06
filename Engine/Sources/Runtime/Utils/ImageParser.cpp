#include "ImageParser.h"
#include "Runtime/Utils/stb_image.h"

unsigned char * scarlett::ImageParser::Parse(const std::string & file, int * width, int * height, int * channels, int reqeryChannel)
{
	auto data = stbi_load(file.c_str(), width, height, channels, reqeryChannel);
	return data;
}

void scarlett::ImageParser::Free(void * p)
{
	STBI_FREE(p);
}
