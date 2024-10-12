#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <regex>
#include <unordered_set>
#include <windows.h>
#include "Regex.h"

using namespace std;

class RegexAndColor {
public:
    regex lexemeRegex;
    string hexColor;
    WORD ansiColor;
    string lexemeType;

    RegexAndColor(const string &regexTemplate, const string &hexColor, WORD ansiColor, const string &type) {
        this->lexemeRegex = regex(regexTemplate);
        this->hexColor = hexColor;
        this->ansiColor = ansiColor;
        this->lexemeType = type;
    }
};

struct ColoredWord {
    RegexAndColor col;
    int start;
    int end;
};

vector<RegexAndColor> getRegex() {
    return vector<RegexAndColor> {
        RegexAndColor(getString(), "#6AAB73", 2, "String"),
        RegexAndColor(getChar(), "#6AAB73", 2, "Char"),

        RegexAndColor(getInt(), "#2AACB8", 11, "Numeric Literal"),
        RegexAndColor(getFloat(), "#2AACB8", 11, "Numeric Literal"),
        RegexAndColor(getHexNumbers(), "#2AACB8", 11, "Hexadecimal Literal"),
        RegexAndColor(getBinaryNumbers(), "#2AACB8", 11, "Binary Literal"),
        RegexAndColor(getOctalNumbers(), "#2AACB8", 11, "Octal Literal"),

        RegexAndColor(getMultilineComments(), "#7A7E85", 8, "Comment"),
        RegexAndColor(getSingleLineComment(), "#7A7E85", 8, "Comment"),

        RegexAndColor(getTypeNames(), "#CF8E6D", 6, "Data Type"),
        RegexAndColor(getReservedWords(), "#B3AE60", 14, "Keyword"),
        RegexAndColor(getOperators(), "#BCBEC4", 15, "Operator"),

        RegexAndColor(getMain(), "#56A8F5", 3, "Main Function"),
        RegexAndColor(getFunctionDefinition(), "#56A8F5", 3, "Function Definition"),
        RegexAndColor(getSystemIO(), "#C77DBB", 5, "System I/O"),

        RegexAndColor(getBoolean(), "#B3AE60", 14, "Boolean"),

        RegexAndColor(getDelimiters(), "#BCBEC4", 15, "Punctuation"),
        RegexAndColor(getLinesAndSpaces(), "#BCBEC4", 15, "Whitespace"),
        RegexAndColor(getVariable(), "#BCBEC4", 15, "Identifier"),
        RegexAndColor(getDirectives(), "#CF8E6D", 6, "Directive"),

        RegexAndColor(getError(), "#FA6675", 4, "Error")
    };
}

string readFromFile(const string &fileName) {
    ifstream file(fileName);
    if (file.is_open()) {
        string content((istreambuf_iterator<char>(file)), istreambuf_iterator<char>());
        file.close();
        return content;
    } else {
        cerr << "Unable to open file: " << fileName << endl;
        return "";
    }
}

vector<int> colorWords(const string &source) {
    vector<int> coloredText(source.length(), -1);
    auto patterns = getRegex();

    for (int j = 0; j < patterns.size(); j++) {
        sregex_iterator iter(source.begin(), source.end(), patterns[j].lexemeRegex);
        sregex_iterator end;

        while (iter != end) {
            for (size_t i = iter->position(); i < iter->position() + iter->length(); i++) {
                if (coloredText[i] == -1)
                    coloredText[i] = j;
            }
            ++iter;
        }
    }
    return coloredText;
}

vector<ColoredWord> colorText(const string &source) {
    auto coloredWords = colorWords(source);
    auto patterns = getRegex();

    vector<ColoredWord> coloredText = {};

    int start = 0;

    while (start < coloredWords.size()) {
        int end = start;
        while (end < coloredWords.size() && coloredWords[start] == coloredWords[end]) end++;
        coloredText.push_back(ColoredWord{patterns[coloredWords[start]], start, end - 1});
        start = end;
    }

    return coloredText;
}

void writeToConsole(const string &source) {
    auto colors = colorText(source);

    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

    for (auto &color: colors) {
        SetConsoleTextAttribute(hConsole, color.col.ansiColor);
        cout << source.substr(color.start, color.end - color.start + 1);
    }
}

void createHtml(const string &source, const string &outFileName) {
    auto colors = colorText(source);
    ofstream outFile(outFileName);
    outFile << "<!DOCTYPE html>\n"
               "<html lang=\"en\">\n"
               "<head>\n"
               "    <meta charset=\"UTF-8\">\n"
               "    <title>Colored Java code</title>\n"
               "</head>\n"
               "<style>\n"
               "html {\n"
               "    font-family: monospace;\n"
               "    font-size: 14px;\n"
               "    line-height: 100%;\n"
               "}\n"
               "\n"
               "span {\n"
               "    white-space: pre-wrap;\n"
               "}\n"
               "</style>\n"
               "<body bgcolor=\"#282923\">\n";

    for (auto &color: colors) {
        string word = source.substr(color.start, color.end - color.start + 1);

        if (word.find('>') != string::npos || word.find('<') != string::npos) {
            for (char letter: word)
                outFile << "<span style='color:" << color.col.hexColor << ";'>"
                        << letter << "</span>";
        } else
            outFile << "<span style='color:" << color.col.hexColor << ";'>"
                    << word << "</span>";
    }

    outFile << "</body>\n"
               "</html>";
}

void outputUniqueLexemes(const vector<ColoredWord> &coloredWords, const string &source) {
    unordered_set<string> numericLiterals;
    unordered_set<string> stringLiterals;
    unordered_set<string> charLiterals;
    unordered_set<string> comments;
    unordered_set<string> keywords;
    unordered_set<string> operators;
    unordered_set<string> identifiers;
    unordered_set<string> punctuation;
    unordered_set<string> systemIO;
    unordered_set<string> directives;
    unordered_set<string> booleans;
    unordered_set<string> dataTypes;

    for (const auto &color : coloredWords) {
        string lexeme = source.substr(color.start, color.end - color.start + 1);

        // Перевірка на функції
        if (regex_match(lexeme, regex(getReservedWords()))) {
            keywords.insert(lexeme + " - Keyword");
        } else if (regex_match(lexeme, regex(getTypeNames()))) {
            dataTypes.insert(lexeme + " - Data Type");
        } else if (regex_match(lexeme, regex(getSystemIO()))) {
            systemIO.insert(lexeme + " - System I/O");
        } else if (regex_match(lexeme, regex(getBoolean()))) {
            booleans.insert(lexeme + " - Boolean");
        } else if (regex_match(lexeme, regex(getVariable()))) {
            identifiers.insert(lexeme + " - Identifier");
        } else if (regex_match(lexeme, regex(getOperators()))) {
            operators.insert(lexeme + " - Operator");
        } else if (regex_match(lexeme, regex(getInt())) ||
                   regex_match(lexeme, regex(getFloat())) ||
                   regex_match(lexeme, regex(getHexNumbers())) ||
                   regex_match(lexeme, regex(getBinaryNumbers())) ||
                   regex_match(lexeme, regex(getOctalNumbers()))) {
            numericLiterals.insert(lexeme + " - Numeric Literal");
        } else if (regex_match(lexeme, regex(getString()))) {
            stringLiterals.insert(lexeme + " - String Literal");
        } else if (regex_match(lexeme, regex(getChar()))) {
            charLiterals.insert(lexeme + " - Character Literal");
        } else if (regex_match(lexeme, regex(getSingleLineComment())) ||
                   regex_match(lexeme, regex(getMultilineComments()))) {
            comments.insert(lexeme + " - Comment");
        } else if (regex_match(lexeme, regex(getDelimiters()))) {
            punctuation.insert(lexeme + " - Punctuation");
        } else if (regex_match(lexeme, regex(getDirectives()))) {
            directives.insert(lexeme + " - Directive");
        }
    }

    auto printLexemeSet = [](const string &categoryName, const unordered_set<string> &lexemes) {
        cout << categoryName << ":" << endl;
        for (const auto &lexeme : lexemes) {
            cout << lexeme << endl;
        }
        if (!lexemes.empty()) {
            cout << "------------------" << endl;
        }
    };

    printLexemeSet("Numeric Literals", numericLiterals);
    printLexemeSet("String Literals", stringLiterals);
    printLexemeSet("Character Literals", charLiterals);
    printLexemeSet("Comments", comments);
    printLexemeSet("Keywords", keywords);
    printLexemeSet("Operators", operators);
    printLexemeSet("Punctuation", punctuation);
    printLexemeSet("Identifiers", identifiers);
    printLexemeSet("Booleans", booleans);
    printLexemeSet("Data Types", dataTypes);
}

int main() {
    SetConsoleOutputCP(CP_UTF8);

    string inputFile = R"(input.txt)";
    string outputFile = R"(output.html)";

    string sourceCode = readFromFile(inputFile);

    if(!sourceCode.empty()) {
        createHtml(sourceCode, outputFile);
        writeToConsole(sourceCode);

        auto coloredWords = colorText(sourceCode);
        cout << endl;
        outputUniqueLexemes(coloredWords, sourceCode);
    }

    return 0;
}
