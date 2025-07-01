#include "Simulation.h"

Simulation::Simulation() {
	
	myParser = new Parser(new ConsoleReader());
}

Simulation::~Simulation() {
	if (myParser) {
		delete myParser;
		myParser = nullptr; 
	}
}


bool Simulation::nextCommand() {
	//cout << '$'; 
	cout << myParser->getLeadingCharacter();
	Command* command = nullptr;
	command = myParser->getNextCommand();
	if (command) {
		command->execute();
		return true;
		
	}
	/* if(command.getCommandName()=="Exit"){
	return false;}*/
	return false;
}

