#pragma once
#include<iostream>
#include <string>
#include"Reader.h"
using namespace std;

class Command {
protected:
    string commandName;

public:
    Command(const string& name);
    virtual ~Command();

    string getCommandName() const;

    virtual void execute() = 0; // Apstraktna metoda
};



