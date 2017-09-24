#include<iostream>
#include<vector>
#include <fstream>
#include <sstream>
#include <cstdlib>
#include <time.h>
#include <stdio.h>
using namespace std;

int SUCCESS_THRESHOLD = 0;

int getRandomIndex(const vector<int>& weights) {
        int index = rand() % weights.size();
        return rand() % (SUCCESS_THRESHOLD + 1) <= weights[index] ?
            index : getRandomIndex(weights);
}

const std::string currentDateTime() {
    time_t now = time(0);
    struct tm tstruct;
    char buf[80];
    tstruct = *localtime(&now);
    // Visit http://en.cppreference.com/w/cpp/chrono/c/strftime
    // for more information about date/time format
    strftime(buf, sizeof(buf), "%Y-%m-%d.%X", &tstruct);

    return buf;
}

int main(int argc, char *argv[]) {
    string line;
    string word;
    string definition;
    vector< pair<string, string> > tests;
    srand(time(NULL));

    for (int i = 1; i < argc; ++i) {
        ifstream words(argv[i]);

        while(!words.eof()) {
            getline(words, line);
            if (!line.empty()) {
                int pos = line.find_first_of(' ');
                word = line.substr(0, pos);
                definition = line.substr(pos+1),
                tests.push_back(make_pair(word, definition));
            }
        }
    }

    vector<int> weights(tests.size(), SUCCESS_THRESHOLD);

    cout << endl << endl;
    cout << "Welcome to GRE word testing, an application built by hermits and used by hermits!" << endl;
    cout << "Probability of selection following successful attempts (near the beginning) is roughly:" << endl;
    if (SUCCESS_THRESHOLD == 0) {
        cout << 1 / (float)tests.size() << endl;
    } else {
        for (int i = 0; i <= SUCCESS_THRESHOLD; ++i) {
            cout << (SUCCESS_THRESHOLD - i) / (float)(SUCCESS_THRESHOLD * tests.size()) << "  ";
        }
        cout << endl;
    }
    cout << "You must get each word successfully " << SUCCESS_THRESHOLD + 1 << " times to deplete the list" << endl;;
    cout << "Press ENTER to step through, y + ENTER if you know the word you see" << endl;
    string currentTime = currentDateTime();
    ofstream outputFile;
    outputFile.open(currentTime + ".txt");
    cout << "Writing failures to file " << currentTime << ".txt" << endl << endl << endl;

    bool valid = false;
    int currentIndex;
    char input;
    while(cin.get(input)) {
        if (valid) {
            cout << tests[currentIndex].second << endl << endl;
            valid = false;
            if (input == 'y') {
                // Decrement the weight of the current word
                --weights[currentIndex];
            } else {
                // Output the word to a file for future reference
                outputFile << tests[currentIndex].first << " " << tests[currentIndex].second << endl;
            }
        } else {
            currentIndex = getRandomIndex(weights);
            cout << tests[currentIndex].first << endl;
            valid = true;
        }
    }

    outputFile.close();

    return 0;
}
