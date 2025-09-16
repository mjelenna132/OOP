#pragma once
#include <iostream>
#include <string>
#include "Reader.h" // GRESKA
using namespace std;
class Command {
protected:
    string commandName;
  // Podrazumevani ulaz: tastatura
    ostream* output; // Podrazumevani izlaz: konzola

public:
    Command(const string& name);
    virtual ~Command();

    string getCommandName() const;

    // Omogućavaju cevovod i redirekciju
    virtual void setOutput();

    virtual void execute() = 0; // Apstraktna metoda
};
