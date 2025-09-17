#include "Head.h"
#include "sstream"
#include "MyExceptions.h"

Head::Head(const string& name, const string& option, const string& argument)
    : CommandOption(name, option, argument), lineCount(0)
{
    // Option must start with 'n' and have a number after it (example: n10)
    if (option.empty() || option[0] != 'n' || option.length() == 1) {
        throw ArgumentException("Invalid option format for 'head'. Expected -n<number>.");
    }

    // Extract the numeric part after 'n'
    string numberPart = option.substr(1);

    try {
        this->lineCount = stoi(numberPart); // convert string to integer
    }
    catch (const std::exception& e) {
        throw ArgumentException("Invalid number in option for 'head'.");
    }

    if (lineCount <= 0) {
        throw ArgumentException("Number of lines must be greater than 0.");
    }
}

void Head::execute() {
    istringstream stream(argument);
    string line;
    int currentLine = 0;

    // Print the first 'lineCount' lines from the argument
    while (currentLine < lineCount && getline(stream, line)) {
        *output << line << endl;
        currentLine++;
    }
}
