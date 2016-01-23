/**
 * File: word-ladder.cpp
 * ---------------------
 * Implements a program to find word ladders connecting pairs of words.
 */

#include <iostream>
using namespace std;

#include "console.h"
#include "lexicon.h"
#include "vector.h"
#include "queue.h"
#include "strlib.h"
#include "simpio.h"

static string getWord(Lexicon& english, string prompt) {
    while (true) {
        string response = trim(toLowerCase(getLine(prompt)));
        if (response.empty() || english.contains(response)) return response;
        cout << "Your response needs to be an English word, so please try again." << endl;
    }
}

/*
 * Display the ladder on the screen
 */

static bool writeLadder(Vector<string> & partial) {
    cout << "Found ladder:";
    for (int i = 0; i < partial.size(); i++) {
        cout << " " << partial[i];
    }
    cout << endl << endl;
    return true;
}

/*
 * By adding one more english word to the current ladder, creating as many new ladder as possible
 */

static void extendLadder(Queue<Vector<string>> & ladder, Lexicon & english, Lexicon & usedWord, Vector<string> & partial, string end) {
    string last = partial[partial.size()-1];
    for (int i = 0; i < last.size(); i++) {
        for (char ch = 'a'; ch <= 'z'; ch++) {
            string possible = last;
            possible[i] = ch;
            if(english.contains(possible) && !usedWord.contains(possible)) {
                usedWord.add(possible);
                Vector<string> vec = partial;
                vec.add(possible);
                ladder.enqueue(vec);
            }
        }
    }
}

static void generateLadder(Lexicon & english, string start, string end) {
    //initialization
    Vector<string> original (1, start);
    Queue<Vector<string>> ladder;
    ladder.enqueue(original);
    Lexicon usedWord;
    usedWord.add(start);
    bool LadderExist;
    //generate shortest ladder
    while (!ladder.isEmpty()) {
        Vector<string> partial = ladder.dequeue();
        if(partial[partial.size()-1] == end) {
            LadderExist = writeLadder(partial);
            break;
        } else {
            extendLadder(ladder, english, usedWord, partial, end);
        }
    }
    if (!LadderExist) cout << "No word ladder between \"" << start
                               << "\" and \"" << end << "\" could be found." << endl << endl;
}

static const string kEnglishLanguageDatafile = "EnglishWords.dat";
static void playWordLadder() {
    Lexicon english(kEnglishLanguageDatafile);
    while (true) {
        string start = getWord(english, "Please enter the source word [return to quit]: ");
        if (start.empty()) break;
        string end;
        while (true) {
            end = getWord(english, "Please enter the destination word [return to quit]: ");
            if(end.size() == start.size()) break;
            cout << "The two endpoints must contain the same number of characters, "
                    "else no word ladder can event exist." << endl;
        }
        if (end.empty()) break;
        generateLadder(english, start, end);
    }
}

int main() {
    cout << "Welcome to the CS106 word ladder application!" << endl << endl;
    playWordLadder();
    cout << "Thanks for playing!" << endl;
    return 0;
}
