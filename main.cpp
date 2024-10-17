#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <regex>
#include <windows.h>
#include "Regex.h"

using namespace std;

class RegexAndColor {
public:
    regex lexemeRegex;
    string hexColor;
    WORD ansiColor;
    string lexemeName;

    RegexAndColor(const string &regexTemplate, const string &hexColor, WORD ansiColor, string lexemeName) {
        this->lexemeRegex = regex(regexTemplate);
        this->hexColor = hexColor;
        this->ansiColor = ansiColor;
        this->lexemeName = lexemeName;
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

        RegexAndColor(getInt(), "#2AACB8", 11, "Int"),
        RegexAndColor(getFloat(), "#2AACB8", 11, "Float"),
        RegexAndColor(getHexNumbers(), "#2AACB8", 11, "Hex Number"),

        RegexAndColor(getBinaryNumbers(), "#2AACB8", 11, "Binary Number"),
        RegexAndColor(getOctalNumbers(), "#2AACB8", 11, "Octal Number"),

        RegexAndColor(getMultilineComments(), "#7A7E85", 8, "Multiline Comment"),
        RegexAndColor(getSingleLineComment(), "#7A7E85", 8, "Single Line Comment"),

        RegexAndColor(getTypeNames(), "#CF8E6D", 6, "Data Type"),
        RegexAndColor(getReservedWords(), "#B3AE60", 14, "Reserved Word"),
        RegexAndColor(getOperators(), "#BCBEC4", 15, "Operator"),
        RegexAndColor(getDelimiters(), "#BCBEC4", 15, "Delimiters"),

        RegexAndColor(getBoolean(), "#B3AE60", 14, "Boolean"),

        RegexAndColor(getSystemIO(), "#C77DBB", 5, "System.out/in"),
        RegexAndColor(getFunctionDefinition(), "#56A8F5", 3, "Function Definition"),
        RegexAndColor(getVariable(), "#cfd3d4", 9, "Variable"),

        RegexAndColor(getLinesAndSpaces(), "#BCBEC4", 15, "Lines and Spaces"),

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

void displayLexemeColors() {
    auto patterns = getRegex();

    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

    for (auto& pattern : patterns) {
        SetConsoleTextAttribute(hConsole, pattern.ansiColor);
        cout << "Lexeme type: " << pattern.lexemeName << " — Color: " << pattern.hexColor << endl;
    }

    SetConsoleTextAttribute(hConsole, 7);
}

int main() {
    SetConsoleOutputCP(CP_UTF8);

    displayLexemeColors();
    cout << "\n\nResult: " << endl;

    string inputFile = R"(input.txt)";
    string outputFile = R"(output.html)";

    string sourceCode = readFromFile(inputFile);

    if(!sourceCode.empty()) {
        createHtml(sourceCode, outputFile);
        writeToConsole(sourceCode);
    }

    return 0;
}
