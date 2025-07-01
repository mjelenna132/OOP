#include "Simulation.h"
#include <cstdlib>
#include <iostream>
using namespace std;


int main() {

	Simulation* simulator = new Simulation();
	

	while (simulator->nextCommand()) {}

	delete simulator;

	return 0;
}

//kad citam sa tastature imam novi red viska
//kad citam iz fajla deli mi rec na pola kad je novi red
//kad je iz vise redova da li treba da broji /n
//512 da li treba u parseru ili u readeru? jer iz fajla moze vise od 512?

