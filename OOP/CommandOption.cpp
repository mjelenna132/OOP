#include "CommandOption.h"


CommandOption::CommandOption(const std::string& name, const std::string& opt, const std::string& arg)
    : CommandWithArgument(name,arg), option(opt) {
   

}

std::string CommandOption::getOption() const {
    return option;
}