#pragma once
#include <boost/log/trivial.hpp>
#include <boost/log/sources/basic_logger.hpp>
#include <boost/log/sources/severity_feature.hpp>
#include <boost/log/sources/severity_logger.hpp>
#include <boost/log/sources/record_ostream.hpp>
#include <boost/log/attributes/attribute.hpp>
#include <boost/log/attributes/attribute_cast.hpp>
#include <boost/log/attributes/attribute_value.hpp>
#include <boost/log/attributes/mutable_constant.hpp>
#include <boost/log/attributes/counter.hpp>

namespace logging = boost::log;
namespace sinks = boost::log::sinks;
namespace src = boost::log::sources;
namespace expr = boost::log::expressions;
namespace attrs = boost::log::attributes;
namespace keywords = boost::log::keywords;

enum severity_level
{
	debug,
	info,
	warning,
	error
};
static src::severity_logger< severity_level > slg;
#define SCARLETT_LOG(LEVEL) \
	 BOOST_LOG_SEV(slg, LEVEL) << "[" << __FILE__ << "][line:" << __LINE__ << "] "