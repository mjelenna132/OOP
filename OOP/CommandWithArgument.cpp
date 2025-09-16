#include "CommandWithArgument.h"
#include "CommandFactory.h"
#include "MyExceptions.h"


// Finalni konstruktor koji ispravno radi i čuva myReader
CommandWithArgument::CommandWithArgument(const string& name, const string& arg)
    : Command(name), argument(removeQuotes(arg)), myReader(nullptr) {

    // Ako je komanda deo pipeline-a, ne procesuiraj ulaz.
    // myReader ostaje nullptr, a argument će biti postavljen spolja.
    if (!CommandFactory::pipelineActive) {

        // PRVI PRIORITET: Eksplicitna redirekcija ulaza ('<').
        if (CommandFactory::isInputStream) {
           
            string filename = CommandFactory::inputFile;
  
            myReader = new FileReader(filename);
            this->newArgument();
        }
        // DRUGI PRIORITET: Argument je ime fajla.
        else if (this->isFile() && !this->isFileCommand()) {
            cout << "uslo u file" << endl;
            myReader = new FileReader(this->argument);
            this->newArgument();
        }
        // TREĆI PRIORITET: Nema argumenta, čitaj sa tastature.
        else if (this->noArgument()) {
            this->argumentKeyboardInput();
        }
    }
}

CommandWithArgument::~CommandWithArgument() {
    delete myReader;
}

bool CommandWithArgument::isFile() {
    if (argument.length() < 4) {
        return false;
    }
    return argument.substr(argument.length() - 4) == ".txt";
}

bool CommandWithArgument::noArgument() {
    return argument.empty();
}

void CommandWithArgument::newArgument() {
    if (!myReader) return;

    string content;
    string line;

    while (!myReader->endOfRead()) {
        line = myReader->getNextLine();
        if (myReader->endOfRead() && line.empty()) break;
        content += line;
        if (!myReader->endOfRead()) {
            content += '\n';
        }
    }
    argument = content;
}

string CommandWithArgument::removeQuotes(const string& input) {
    // 1) prazan string je dozvoljen (npr. with može izostati)
    if (input.empty()) {
        return input;
    }

    // 2) ako je quoted → skini navodnike
    if (input.front() == '"' && input.back() == '"') {
        return input.substr(1, input.size() - 2);
    }
    else {
        return input;
    }

    // 3) nije quoted → dozvoli SAMO ako izgleda kao .txt i komanda NIJE file-komanda
    /*
    if (isFile() && !isFileCommand())
    {
        return input; // ostavi kako jeste; biće tretirano kao fajl
    }

    // 4) u svim ostalim slučajevima → greška
    throw SyntaxException("Argument mora biti pod navodnicima: " + input);
    //resi ovo ujutru 
    */
}

string CommandWithArgument::getArgument() const {
    return argument;
}

void CommandWithArgument::argumentKeyboardInput() {
    if (myReader != nullptr) {
        delete myReader;
    }
    myReader = new ConsoleReader();
    this->newArgument();
}

bool CommandWithArgument::isFileCommand() {
    string cmdName = getCommandName();
    return (cmdName == "touch" || cmdName == "rm" || cmdName == "truncate");
}

// IMPLEMENTACIJA NOVE METODE
void CommandWithArgument::setArgument(const string& newArgument) {
    this->argument = newArgument;
}