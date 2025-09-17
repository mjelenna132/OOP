#include "Wc.h"
#include <iostream>
#include "MyExceptions.h"

Wc::Wc(const string& name, const string& option, const string& argument)
    : CommandOption(name, option, argument), counter(0) {
    // Check if option is valid ("c" for characters or "w" for words)
    if (option != "c" && option != "w") {
        throw ArgumentException("Invalid option for wc command.");
    }
    // You can call a method here, but usually it's better in execute()
}

void Wc::execute() {
    // If the command name is "head", skip execution (special handling)
    if (getCommandName() == "head") {
        return;
    }
    // Call the counting method implemented in derived classes
    count();
    // Print the result to the current output stream
    *output << this->counter << endl;
}
