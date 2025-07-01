#pragma once

#include "Parser.h"
#include <vector>
using namespace std;

class Simulation {
public:

	Simulation();
	~Simulation();

	bool nextCommand();


private:
	Parser* myParser;

};

