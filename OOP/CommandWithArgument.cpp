#include "CommandWithArgument.h"
#include "CommandFactory.h"
#include "MyExceptions.h"

CommandWithArgument::CommandWithArgument(const string& name, const string& arg)
    : Command(name), argument(arg), myReader(nullptr) {
    // Clean up the raw argument (remove quotes if needed)
    this->setArgument(removeQuotes(arg));
    if (CommandFactory::isInputStream && !this->noArgument() && this->getCommandName() != "tr") {
        throw ArgumentException("Input redirection cannot be used together with an argument.");
    }
    this->setArgument(removeQuotes(arg));

    // If command is not in a pipeline, decide how to read input
    if (!CommandFactory::pipelineActive) {

        // Case 1: input is redirected with '<'
        if (CommandFactory::isInputStream) {
            string filename = CommandFactory::inputFile;
            myReader = new FileReader(filename);
            this->newArgument();
        }
        // Case 2: argument is a file name (.txt), not for file commands, no quotes
        else if (this->isFile() && !this->isFileCommand() && !isQuoted(argument)) {
            myReader = new FileReader(this->argument);
            this->newArgument();
        }
        // Case 3: no argument → read from keyboard
        else if (this->noArgument() && this->getCommandName()!= "tr") {
            this->argumentKeyboardInput();
        }
    }
}

CommandWithArgument::~CommandWithArgument() {
    delete myReader; // free memory if reader was used
}

bool CommandWithArgument::isFile() {
    // Check if argument ends with ".txt"
    if (argument.length() < 4) return false;
    return argument.substr(argument.length() - 4) == ".txt";
}

bool CommandWithArgument::noArgument() {
    return argument.empty();
}

void CommandWithArgument::newArgument() {
    // Read all lines from reader into 'argument'
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
     if (!content.empty() && content.back() == '\n') {
        content.pop_back();
    }
    argument = content;
}

string CommandWithArgument::removeQuotes(const string& input) {
    // Allow empty input
    if (input.empty()) {
        return input;
    }

    // If quoted → remove quotes
    if (isQuoted(input)) {
        return input.substr(1, input.size() - 2);
    }

    // If not quoted → allow only if it looks like a .txt file
    if (this->isFile()) {
        return input;
    }

    // Otherwise throw error
    throw SyntaxException("Argument must be in quotes: " + input);
}

string CommandWithArgument::getArgument() const {
    return argument;
}

void CommandWithArgument::argumentKeyboardInput() {
    // Read text from keyboard
    if (myReader != nullptr) {
        delete myReader;
    }
    myReader = new ConsoleReader();
    this->newArgument();
}

bool CommandWithArgument::isFileCommand() {
    // Commands that work directly on file names
    string cmdName = getCommandName();
    return (cmdName == "touch" || cmdName == "rm" || cmdName == "truncate");
}

void CommandWithArgument::setArgument(const string& newArgument) {
    argument = newArgument;
}

bool CommandWithArgument::isQuoted(const std::string& input) const
{
    return !input.empty() && input.front() == '"' && input.back() == '"';
}
