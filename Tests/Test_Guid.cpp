//
// Created by gzxiaoshichang on 2019/2/16.
//

#include <iostream>
#include "Guid.hpp"

using namespace xg;
using namespace std;

int main(int argc, char* argv[]) {
	Guid guid = newGuid();
	cout << "Guid test:" << guid << endl;

	return 0;
}