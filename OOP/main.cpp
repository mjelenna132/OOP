#include "Simulation.h"
#include <cstdlib>
#include <iostream>
using namespace std;

int main() {
    // Allocate simulator instance
    Simulation* simulator = new Simulation();

    // Process commands until there are no more
    while (simulator->nextCommand()) {}

    // Clean up allocated resources
    delete simulator;

    // Normal program termination
    return 0;
}

