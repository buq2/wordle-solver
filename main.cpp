
#include <iostream>
#include "wordle_solver.h"

void help(char *argv[]) {
    std::cout << "Use:" << std::endl;
    std::cout << argv[0] << " wordlist hintlist [word_length=5]" << std::endl;
    std::cout << "Worlist file contains one word per line.\n\
Hintlist contains one tried out word per line.\n\
Hints are formed as follows in the text file:\n\
- One tried word per line.\n\
- Each tried character separated by space.\n\
- Capital letter = this character belongs to this position.\n\
- Lower case letter = this character does not belong here, but is in the word.\n\
- Character prefixed with '!' character not in the word, or character used too many times and not in this position.\n";
}

int main(int argc, char *argv[]) {
    if (argc < 3) {
        help(argv);
        return 1;
    }

    const std::string fname_wordlist(argv[1]);
    const std::string fname_hintlist(argv[2]);

    
    int word_length = 5;
    if (argc >= 4) {
        word_length = atoi(argv[3]);
    }

    WordleSolver solver(word_length);
    solver.AddWordsFromFile(fname_wordlist);
    solver.AddHintsFromFile(fname_hintlist);
    solver.PrintPossibleSolutions();
}
