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
#include "lexicon.h"
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
 * Read in the file and compile the information into a Map.
 */

static void readFile(Map<string, Lexicon> & grammar, string filename) {
    ifstream infile;
    infile.open(filename.c_str());
    string currentLine;
    string signalLine = " ";
    string key;
    while (getline(infile, currentLine)) {
        if (isSpace(signalLine)) {
            key = currentLine;
            signalLine = currentLine;
            Lexicon entries;
            grammar.put(currentLine, entries);
        } else if (isDigit(signalLine)) {
            grammar[key].add(currentLine);
        } else if (isSpace(currentLine) || isDigit(currentLine)) {
            signalLine = currentLine;
        }
    }
    infile.close();
}

int main() {
    while (true) {
        string filename = getFileName();
        if (filename.empty()) break;
        Map<string, Lexicon> grammar;
        readFile(grammar, filename);
        generateSentence(grammar);
        grammar.clear();
    }
    
    cout << "Thanks for playing!" << endl;
    return 0;
}
