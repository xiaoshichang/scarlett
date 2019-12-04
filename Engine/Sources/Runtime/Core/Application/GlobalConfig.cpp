#include "Foundation/Assert.h"
#include "Runtime/Utils/Logging.h"
#include "GlobalConfig.h"
#include <iostream>
#include <sstream>

scarlett::ApplicationConfig::ApplicationConfig()
{
	std::ifstream t("./Asset/Configs/application.json");
	std::string str((std::istreambuf_iterator<char>(t)), std::istreambuf_iterator<char>());
	std::stringstream ss(str);
	try {
		boost::property_tree::read_json(ss, ptree);
	}
	catch (boost::property_tree::ptree_error & e) {
		SCARLETT_LOG(error) << e.what();
		SCARLETT_ASSERT(false);
	}
	screenWidth = ptree.get_child("screen_width").get_value<int>();
	screenHeight = ptree.get_child("screen_height").get_value<int>();
	applicationName = ptree.get_child("application_name").data();
}

int scarlett::ApplicationConfig::GetScreenWidth()
{
	return screenWidth;
}

int scarlett::ApplicationConfig::GetScreenHeight()
{
	return screenHeight;
}

std::string scarlett::ApplicationConfig::GetApplicationName()
{
	return applicationName;
}
