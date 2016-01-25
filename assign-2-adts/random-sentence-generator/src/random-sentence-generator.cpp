/**
 * File: random-sentence-generator.cpp
 * -----------------------------------
 * Presents a short program capable of reading in
 * context-free grammar files and generating arbitrary
 * sentences from them.
 */

#include <iostream>
#include <fstream>
#include <cctype>
using namespace std;

#include "console.h"
#include "map.h"
#include "vector.h"
#include "simpio.h"   // for getLine
#include "strlib.h"   // for toLowerCase, trim

static const string kGrammarsDirectory = "grammars/";
static const string kGrammarFileExtension = ".g";
static string getNormalizedFilename(string filename) {
    string normalizedFileName = kGrammarsDirectory + filename;
    if (!endsWith(normalizedFileName, kGrammarFileExtension))
        normalizedFileName += kGrammarFileExtension;
    return normalizedFileName;
}

static bool isValidGrammarFilename(string filename) {
    string normalizedFileName = getNormalizedFilename(filename);
    ifstream infile(normalizedFileName.c_str());
    return !infile.fail();
}

static string getFileName() {
    while (true) {
        string filename = trim(getLine("Name of grammar file? [<return> to quit]: "));
        if (filename.empty() || isValidGrammarFilename(filename)) return filename;
        cout << "Failed to open the grammar file named \"" << filename << "\". Please try again...." << endl;
    }
}

/*
 * Detect whether the string passed in is a series of digit.
 */

static bool isDigit(string & str) {
    for (int i = 0; i < str.size(); i++) {
        if (!isdigit(str[i])) return false;
    }
    return true;
}

/*
 * Check whether certain string is space.
 */

static bool isSpace(string & str) {
    for (int i = 0; i < str.size(); i++) {
        if (!isspace(str[i])) return false;
    }
    return true;
}

/*
 * Read in the file and compile the information into a Map.
 */

static void readFile(Map<string, Vector<string>> & grammar, string filename) {
    ifstream infile;
    infile.open(filename.c_str());
    string currentLine;
    string signalLine = " ";
    string key;
    char ch;
    while (infile.get(ch)) {
        cout << ch << " ";
        /*if (isSpace(currentLine) || isDigit(currentLine)) {
            signalLine = currentLine;
        } else if (isSpace(signalLine)) {
            key = currentLine;
            signalLine = currentLine;
            Vector<string> entries;
            grammar.put(currentLine, entries);
        } else if (isDigit(signalLine)) {
            grammar[key].add(currentLine);
        }
        cout << grammar.toString() << endl;*/
    }
    infile.close();
}

/*
 * Generate three random sentences
 */

static void generateSentence(const Map<string, Vector<string>> & grammar) {
    string original;
    for (int i = 0; i < 3; i++) {
        original = grammar["<start>"][0];
        int k = 0;
        while (original.find('<', k) != string::npos) {
            k = original.find('<', k)+1;
            int keyEnd = original.find('>', k);
            string key = original.substr(k, keyEnd - k + 1);
            Vector<string> production= grammar[key];
            original.replace(k, keyEnd - k + 1, production[randomInteger(0, production.size() -1)]);
        }
        cout << i + 1 << ".)" << original << endl << endl;
    }
}

int main() {
    while (true) {
        string filename = getFileName();
        if (filename.empty()) break;
        Map<string, Vector<string>> grammar;
        readFile(grammar, filename);
        generateSentence(grammar);
        grammar.clear();
    }
    
    cout << "Thanks for playing!" << endl;
    return 0;
}
