#include "Reader.h"
#include "MyExceptions.h"
#include <iostream>
using namespace std;

Reader::Reader() {}
Reader::~Reader() {}

// Reads input from a file
FileReader::FileReader(string filepath) {
    myInput = new ifstream(filepath);
    if (!myInput->is_open()) {
        delete myInput;
        throw FileException("Could not open file: " + filepath);
    }
}

FileReader::~FileReader() {
    myInput->close();
    delete myInput;
}

string FileReader::getNextLine() {
    string line;
    if (!getline(*myInput, line)) {
        eofDetected = true;  // same as in ConsoleReader
        return "";
    }
    return line;
}

// same getNextLine for FileReader and ConsoleReader

bool FileReader::endOfRead() {
    return eofDetected;
}

// Reads input from console (keyboard)
ConsoleReader::ConsoleReader() {}
ConsoleReader::~ConsoleReader() {}

string ConsoleReader::getNextLine() {
    string line;
    if (!getline(cin, line)) {
        eofDetected = true;  // set EOF flag if input ends
        return "";
    }
    return line; // read a line from standard input
}

bool ConsoleReader::endOfRead() {
    bool eof = eofDetected || cin.eof();
    cin.clear(); // reset error flags for further reading
    return eof;
}

void ConsoleReader::reset() {
    eofDetected = false;
    cin.clear();
}
