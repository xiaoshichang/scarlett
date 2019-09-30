#include "Runtime/Utils/Singleton.h"
#include <boost/filesystem.hpp>
#include <string>

using namespace boost::filesystem;

namespace scarlett {

	class AssetFinder {
	private:
		AssetFinder();

		void SetSearchPath(const std::string& path);
		void SetPatchPath(const std::string& path);

		path		mSearchPath;
		path		mPatchPath;

	public:
		const std::string GetRealPath(const std::string& assetPath);
		const std::string GetRealPath(const char* assetPath);

		friend class Singleton<AssetFinder>;

	};
	typedef Singleton<AssetFinder> GAssetFinder;

}