#pragma once
#include "CommandWithArgument.h"
class Tr: public CommandWithArgument 
{ public:
	Tr(const string& name, const string& argument, const string& what, const string& with);
	void execute() override;
private:
	string whatToReplace;
	string replaceWith;
	void initializeArguments(const string& arg, const string& what, const string& with);
	bool allArgumentsProvided(const string& arg, const string& what, const string& with);
	void keyboardInput(const string& arg, const string& what, const string& with);
	string replaceAll(const string& input,
		const string& what,
		const string& with) const;
	// Ova metoda NE MENJA članove klase (this)

	
};

