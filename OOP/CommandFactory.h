#pragma once
#include <string>
#include "Wc.h"
#include "WcWords.h"
#include "WcChar.h"
#include "Echo.h"
#include "Time.h"
#include "Date.h"
#include "Touch.h"
#include "Prompt.h"

class CommandFactory {
public:
    static Command* createCommand(const std::string& command,std::istringstream& restOfline);
    static string readArgument(std::istringstream& stream);

};
