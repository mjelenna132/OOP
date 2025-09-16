#include "Prompt.h"
#include "Parser.h"
Prompt::Prompt(const string& name, const string& argument): CommandWithArgument(name,argument){}
void Prompt::execute() {
	Parser::setLeadingCharacter(argument);
	
///kako da prosledim parser da automatski prosledi znak za zamenu jer moram da saljem parser i command factory i klasi prompt
	//sta je dovoljno objektno orjentisano

}




