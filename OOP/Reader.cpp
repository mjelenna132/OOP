#include "Reader.h"
#include <iostream>
using namespace std;

Reader::Reader() {}

Reader::~Reader() {}

FileReader::FileReader(string filepath) {
	myInput = new ifstream(filepath);
	if (!myInput->is_open()) {
		cout << "Could not open file: " + filepath << endl;
		exit(5);
	}
}

FileReader::~FileReader() {
	myInput->close();
	delete myInput;
}

string FileReader::getNextLine() {
	string line;
	if (!getline(*myInput, line)) {
		eofDetected = true;  // isto kao kod ConsoleReader
		return "";
	}
	return line;
}
//iste su ti getnextline

bool FileReader::endOfRead() {
	return eofDetected;
}

ConsoleReader::ConsoleReader() {}

ConsoleReader::~ConsoleReader() {}

string ConsoleReader::getNextLine() {
	string line;
	if (!getline(cin, line)) {
		eofDetected = true;  // Ovde postavljamo da EOF jeste detektovan
		return "";
	}  // Čitanje linije sa standardnog ulaza
	return line;
}

bool ConsoleReader::endOfRead() {
	bool eof = eofDetected || cin.eof();
	cin.clear();
	return eof;
}

void ConsoleReader::reset()
{
	eofDetected = false;
	cin.clear();
}
