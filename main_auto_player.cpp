
#include <iostream>
#include "wordle_solver.h"
#include "wordle.h"
#include <time.h>

void help(char *argv[]) {
    std::cout << "Use:" << std::endl;
    std::cout << argv[0] << " wordlist [word_length=5]" << std::endl;
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        help(argv);
        return 1;
    }

    const std::string fname_wordlist(argv[1]);

    int word_length = 5;
    if (argc >= 3) {
        word_length = atoi(argv[2]);
    }

    Wordle game;
    game.AddWordsFromFile(fname_wordlist, word_length);
    srand(time(NULL));
    game.ChooseRandomWord();

    WordleSolver solver(word_length);
    solver.AddWordsFromFile(fname_wordlist);
    
    std::cout << "Correct word: " << game.GetWord() << std::endl;
    tiny_utf8::string hint;
    for (int i = 0; i < 6; ++i) {
        const auto guess = solver.GetRecommendedGuess();
        game.MakeGuess(guess, hint);

        std::cout << "Iter: " << i << " Guess: " << guess << " Hint: " << hint << std::endl;

        solver.AddHint(hint);
    }

    std::cout << "Remaining possible solutions:" << std::endl;
    solver.PrintPossibleSolutions();
}
