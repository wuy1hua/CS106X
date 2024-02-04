/**
 * File: word-ladder.cpp
 * ---------------------
 * Implements a program to find word ladders connecting pairs of words.
 */

#include <iostream>
using namespace std;

#include "console.h"
#include "lexicon.h"
#include "strlib.h"
#include "simpio.h"
#include "vector.h"
#include "queue.h"

static string getWord(const Lexicon& english, const string& prompt) {
    while (true) {
        string response = trim(toLowerCase(getLine(prompt)));
        if (response.empty() || english.contains(response)) return response;
        cout << "Your response needs to be an English word, so please try again." << endl;
    }
}

static void generateLadder(const Lexicon& english, const string& start, const string& end) {
    cout << "Here's where you'll search for a word ladder connecting \"" << start << "\" to \"" << end << "\"." << endl;

    Vector<string> wordLadder;
    Queue<Vector<string>> pathQueue;
    Lexicon wordUsed;

    Vector<string> initialPath;
    initialPath.add(start);
    pathQueue.enqueue(initialPath);

    while(!pathQueue.isEmpty()){
        Vector<string> currentPath = pathQueue.dequeue();
        string currentWord = currentPath[currentPath.size() - 1];
        if(currentWord == end){
            wordLadder = currentPath;
            cout << wordLadder << endl;
            break;
        }
        else{
            int len = start.length();
            for(int i = 0; i < len; i++){
                for(int letter = 0; letter < 26; letter++){
                    string word_copy = currentWord;
                    word_copy[i] = 'a' + letter;
                    if(english.contains(word_copy) && !wordUsed.contains(word_copy)){
                        Vector newPath = currentPath;
                        newPath.add(word_copy);
                        pathQueue.enqueue(newPath);
                        wordUsed.add(word_copy);
                    }
                }
            }
        }
    }
    if(pathQueue.isEmpty()){
        cout << "No such wordladder exists" << endl;
    }
}

static const string kEnglishLanguageDatafile = "res/dictionary.txt";
static void playWordLadder() {
    Lexicon english(kEnglishLanguageDatafile);
    while (true) {
        string start = getWord(english, "Please enter the source word [return to quit]: ");
        if (start.empty()) break;
        string end = getWord(english, "Please enter the destination word [return to quit]: ");
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
