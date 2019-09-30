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
	
}

int scarlett::ApplicationConfig::GetScreenWidth()
{
	int w = ptree.get_child("screen_width").get_value<int>();
	return w;
}

int scarlett::ApplicationConfig::GetScreenHeight()
{
	int h = ptree.get_child("screen_height").get_value<int>();
	return h;
}

std::string scarlett::ApplicationConfig::GetApplicationName()
{
	std::string name = ptree.get_child("application_name").data();
	return name;
}
