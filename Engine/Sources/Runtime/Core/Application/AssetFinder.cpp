#include "AssetFinder.h"
#include "Foundation/Assert.h"

scarlett::AssetFinder::AssetFinder()
{
	SetSearchPath("./Asset");
	SetPatchPath("./Patch");
}

void scarlett::AssetFinder::SetSearchPath(const std::string & searchPath)
{	
	path p(searchPath);
	if (exists(p) && is_directory(p)) {
		mSearchPath = p;
	}
	else {
		SCARLETT_ASSERT(false);
	}
}

void scarlett::AssetFinder::SetPatchPath(const std::string & patchPath)
{
	path p(patchPath);
	if (exists(p) && is_directory(p)) {
		mPatchPath = p;
	}
}

const std::string scarlett::AssetFinder::GetRealPath(const std::string & assetPath)
{
	path asset = path(assetPath);

	path patch = mPatchPath / asset;
	if (exists(patch)) {
		return patch.string();
	}

	path full = mSearchPath / asset;
	if (exists(full)) {
		return full.string();
	}

	SCARLETT_ASSERT(false);
	return std::string();
}

const std::string scarlett::AssetFinder::GetRealPath(const char * assetPath)
{
	path asset = path(assetPath);

	path patch = mPatchPath / asset;
	if (exists(patch)) {
		return patch.string();
	}

	path full = mSearchPath / asset;
	if (exists(full)) {
		return full.string();
	}

	SCARLETT_ASSERT(false);
	return std::string();
}
