#ifndef REGEX_H
#define REGEX_H

#include <string>
#include "Regex.cpp"
using namespace std;

string getInt();
string getFloat();
string getHexNumbers();
string getBinaryNumbers();
string getOctalNumbers();
string getString();
string getChar();
string getBoolean();
string getMultilineComments();
string getSingleLineComment();
string getVariable();
string getTypeNames();
string getReservedWords();
string getFunctionDefinition();
string getDelimiters();
string getOperators();
string getLinesAndSpaces();
string getSystemIO();
string getError();

#endif // REGEX_H
