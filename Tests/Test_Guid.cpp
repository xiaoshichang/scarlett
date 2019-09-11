//
// Created by gzxiaoshichang on 2019/2/16.
//

#include <iostream>
#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_generators.hpp>
#include <boost/uuid/uuid_io.hpp>

using namespace std;

int main(int argc, char* argv[]) {
	boost::uuids::uuid tag;
	tag = boost::uuids::random_generator()();
	std::cout << tag;
	return 0;
}
