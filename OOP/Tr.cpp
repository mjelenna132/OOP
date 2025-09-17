#include "Tr.h"
#include <iostream>
#include "MyExceptions.h"
#include "CommandFactory.h"

Tr::Tr(const string& name,
       const string& arg,
       const string& what,
       const string& with)
    : CommandWithArgument(name, arg) 
{
    // Store arguments (remove quotes if present)
    whatToReplace = removeQuotes(what);
    replaceWith   = removeQuotes(with);
    string whatTemp, replaceTemp,argTemp;
    
    whatTemp = whatToReplace;
    replaceTemp = replaceWith;
    argTemp = argument;

    // Prepare arguments depending on pipeline or manual input
    initializeArguments(argTemp, whatTemp, replaceTemp);
}

void Tr::initializeArguments(const string& arg,
                             const string& what,
                             const string& with)
{
    // If running inside a pipeline, reuse arguments directly
    if (CommandFactory::pipelineActive) {
        whatToReplace = arg;
        replaceWith   = what;
        return;
    }
    if(arg.empty() && !what.empty() && with.empty()){
        this->argumentKeyboardInput();
        return;
    }
    // If only some arguments are provided, read missing part from keyboard
    else if (!arg.empty() && !what.empty() && with.empty()) {
        this->argumentKeyboardInput();

        if (this->noArgument()) {
            this->argument     = arg;
            whatToReplace      = what;
        }
        else {
            whatToReplace = arg;
            replaceWith   = what;
        }
    }
}

void Tr::execute() {
    if (whatToReplace.empty()) {
        throw ArgumentException("The 'tr' command requires a 'what' argument.");
    }

    // Get current input text and replace substrings
    string inputText  = getArgument();
    string outputText = replaceAll(inputText, whatToReplace, replaceWith);

    *output << outputText << endl;
}

std::string Tr::replaceAll(const string& input,
                           const string& what,
                           const string& with) const 
{
    if (what.empty()) return input;

    std::string result = input;
    size_t pos = 0;

    // Replace all occurrences of 'what' with 'with'
    while ((pos = result.find(what, pos)) != std::string::npos) {
        result.replace(pos, what.length(), with);
        pos += with.length(); // move index forward
    }

    return result;
}
