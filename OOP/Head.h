#pragma once
#include "CommandOption.h" // ISPRAVNO: Nasle?uje CommandOption

class Head : public CommandOption {
private:
    int lineCount; // Preimenovano iz 'counter' da bude jasnije

public:
    Head(const string& name, const string& option, const string& argument);
    void execute() override; // execute() sada radi pravi posao
};