#include "Echo.h"
#include "CommandFactory.h"
Echo::Echo(const string& name, const string& argument) : CommandWithArgument(name, argument){}

// Print the argument to the configured output (console or redirected file)
// Echo.cpp
void Echo::execute() {
    // If output is redirected (> or >>), don't add a newline.
    // If not redirected (i.e., printing to console), add a newline so the next prompt is on a new line.
    bool toFile = CommandFactory::isOutputStream;
    *output << getArgument();
    if (!toFile) {
        *output << std::endl;
    }
}
