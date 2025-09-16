#include "Tr.h"
#include <iostream>
#include "MyExceptions.h"

Tr::Tr(const string& name,
    const string& arg,
    const string& what,
    const string& with)
    : CommandWithArgument(name, arg)  // Ne prosleđujemo argument baznoj klasi, inicijalizovaće se kasnije
{
    //argument = removeQuotes(arg); // ne bi trebalo da se zove ovo ali ne radi bwz toga
    whatToReplace = removeQuotes(what);
    replaceWith = removeQuotes(with);

    //keyboardInput(arg, what, with); // Ili ucita argument ili prazan string
    //u arg nam ostaje inicijalna vrednost a u argument se ucita sa standardnog ulaza
    // if is input stream skip ovo 
   initializeArguments(arg, what, with);
}

void Tr::initializeArguments(const string& arg,
    const string& what,
    const string& with)
{
    
       //Argument je svakako ucitan
    if (!arg.empty() && !what.empty() && with.empty()) {
        // what i with su uneti, tekst dolazi sa tastature
        // argument i what su uneti, with izostavljeno
        this->argumentKeyboardInput();
        if (this->noArgument()) {
            this->argument = arg;
            whatToReplace = what;
            
        }
        else {
            whatToReplace = arg;
            replaceWith = what;
            
        }
    }
    return;
    
}

bool Tr::allArgumentsProvided(const string& arg, const string& what, const string& with)
{
    //proverava da li su svi kako bi omogucio ulaz sa tastature i onda razlikuje jel arg i what ili what i with 
    return !arg.empty() && !what.empty() && !with.empty();
}



void Tr::keyboardInput(const string& arg, const string& what, const string& with) {
    if (!allArgumentsProvided(arg, what, with)) {
        cout << "uslo u keyb input" << endl;
        this->argumentKeyboardInput();
    }
}

void Tr::execute() {
    if (whatToReplace.empty()) {
        throw ArgumentException("Komanda 'tr' zahteva 'what' argument.");
    }
    string inputText = getArgument();
    string outputText;
    outputText = replaceAll(inputText, whatToReplace, replaceWith);
     *output << outputText << endl;
}
std::string Tr::replaceAll(const string& input,
    const string& what,
    const string& with) const {
    if (what.empty()) return input;
    std::string result = input;
    size_t pos = 0;
    while ((pos = result.find(what, pos)) != std::string::npos) {
        result.replace(pos, what.length(), with);
        pos += with.length(); // pomeri dalje
    }
    return result;
}


