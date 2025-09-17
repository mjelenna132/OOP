#include "Prompt.h"
#include "Parser.h"

Prompt::Prompt(const string& name, const string& argument)
    : CommandWithArgument(name, argument) {}

void Prompt::execute() {
    // Change the shell prompt by updating the Parser leading character
    Parser::setLeadingCharacter(argument);


}
