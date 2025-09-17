#pragma once
#include <fstream>
#include <string>
using namespace std;

// Base class for all input sources (console, file, etc.)
class Reader {
public:
    Reader();
    virtual ~Reader();

    // Read the next line (pure virtual -> must be implemented by subclasses)
    virtual string getNextLine() = 0;

    // Check if end of input has been reached
    virtual bool endOfRead() = 0;

protected:
    bool eofDetected = false; // Flag to track EOF
};

// Reads input from a file
class FileReader : public Reader {
public:
    FileReader(string filepath);
    ~FileReader();

    string getNextLine() override;
    bool endOfRead() override;

private:
    ifstream* myInput; // pointer to file input stream
};

// Reads input from the console (keyboard)
class ConsoleReader : public Reader {
public:
    ConsoleReader();
    ~ConsoleReader();

    string getNextLine() override;
    bool endOfRead() override;

    // Reset EOF flag so console can continue reading
    void reset();
};
