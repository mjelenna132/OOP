#pragma once

#include "Parser.h"
#include <vector>
using namespace std;

// Simulation runs the main loop of the program.
// It keeps a Parser, asks for the next command, executes it,
// and continues until input ends or the user exits.
class Simulation {
public:
    Simulation();
    ~Simulation();

    // Get and run the next command.
    // Returns false when there are no more commands (EOF or exit).
    bool nextCommand();

private:
    // Parser that reads input and builds commands
    Parser* myParser;
};
