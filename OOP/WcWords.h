#pragma once
#include "Wc.h"

class WcWords : public Wc {
public:
    WcWords(const string& name, const string& option, const string& argument);
protected:
    void count() override; // Counts words
};
