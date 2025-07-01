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
	getline(*myInput, line);
	return line;
}

bool FileReader::endOfRead() {
	return myInput->eof();
}

ConsoleReader::ConsoleReader() {}

ConsoleReader::~ConsoleReader() {}

string ConsoleReader::getNextLine() {
	string line;
	getline(cin, line);  // Čitanje linije sa standardnog ulaza
	return line;
}

bool ConsoleReader::endOfRead() {
	return cin.eof();  // Proverava da li je standardni ulaz završen
}