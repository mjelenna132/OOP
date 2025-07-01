#pragma once
#include"Command.h"
class CommandWithArgument : public Command{
protected:
    string argument;
    Reader* myReader;
    void argumentFromFile();
    void argumentKeyboardInput();
    bool isFile();
    bool noArgument();
    void newArgument();
public:
    CommandWithArgument(const string& name, const string& arg);
    ~CommandWithArgument();
    string getArgument() const;

   
};

