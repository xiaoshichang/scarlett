#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>
#include "Runtime/Utils/Singleton.h"
#include <string>

namespace scarlett {

	class ApplicationConfig{

	private:
		ApplicationConfig();
		
	public:
		int				GetScreenWidth();
		int				GetScreenHeight();
		std::string		GetApplicationName();

		friend class Singleton<ApplicationConfig>;

	private:
		boost::property_tree::ptree ptree;
		int screenWidth;
		int screenHeight;
		std::string applicationName;

	};
	typedef Singleton<ApplicationConfig> GlobalConfig;

}