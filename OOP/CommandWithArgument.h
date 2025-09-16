#pragma once
#include "Command.h"


class CommandWithArgument : public Command {
protected:
    string argument;
    Reader* myReader;

    void argumentKeyboardInput();
    bool isFile();
    bool isFileCommand();
    bool noArgument();
    void newArgument();

    string removeQuotes(const string& input);

public:
    CommandWithArgument(const string& name, const string& arg);
    virtual ~CommandWithArgument();

    string getArgument() const;
    void setArgument(const string& newArgument); // <-- NOVA METODA
};