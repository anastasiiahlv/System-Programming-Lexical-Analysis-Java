#include <iostream>
#include <string>
#include "Regex.h"
using namespace std;

string getInt() {
    return R"(\b[0-9]+([lL])?\b)";
}
string getFloat() {
    return R"(\b[0-9]*\.?[0-9]+([eE][-+]?[0-9]+)?([fFdD])?\b)";
}

string getHexNumbers() {
    return R"(0x[0-9A-Fa-f]+[Ll]?)";
}

string getBinaryNumbers() {
    return R"(0b[01]+[Ll]?)";
}

string getOctalNumbers() {
    return R"(0[0-7]+[Ll]?)";
}

string getString() {
    return R"("((\\")|[^"])*")";
}

string getChar() {
    return R"('([^\\']|\\.)')";
}

string getBoolean() {
    return R"(\b(false|true)\b)";
}

string getMultilineComments() {
    return R"(\/\*(.|\r|\n)*\*\/)";
}

string getSingleLineComment() {
    return R"(//.*)";
}

string getVariable() {
    return R"(\b[A-Za-z_][A-Za-z0-9_]*\b)";
}

string getTypeNames() {
    return R"(\b(byte|short|int|long|float|double|char|boolean|void|[A-Z][a-zA-Z0-9]*)\b)";
}

string getReservedWords() {
    return R"(\b(abstract|assert|break|case|class|const|continue|default|do|else|enum|extends|final|finally|for|if|implements|import|instanceof|interface|new|null|package|private|protected|public|return|static|super|switch|synchronized|this|throw|throws|try|volatile|while)\b)";
}

string getFunctionDefinition() {
    return R"(\b[A-Za-z_][A-Za-z0-9_]*\s*\(([^()]*)\)\s*(\{)?)";
}

string getDelimiters() {
    return R"([\(\)\{\}\;\[\]\.,:])";
}

string getOperators() {
    return R"([\+\-\*\/\=\&\|\<\>\!\?]+|(\+\+|--|\+=|-=|\*=|\/=|==|!=|<=|>=|&&|\|\||->|\?))";
}

string getLinesAndSpaces() {
    return R"([\t\n\r]+)";
}

string getSystemIO() {
    return R"(\b(out|in)\b)";
}

string getError() {
    return R"(.*)";
}