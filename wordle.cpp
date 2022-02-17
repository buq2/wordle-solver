#include "wordle.h"
#include <fstream>
#include <iostream>

Wordle::Wordle()
{
}

void Wordle::AddWordsFromFile(const std::string &fname, const int word_length)
{
    word_length_ = word_length;
    word_chosen_ = false;
    std::ifstream file(fname);
    std::string line_std;
    while(std::getline(file, line_std)) {
        tiny_utf8::string line(line_std);
        if (line.length() != word_length_) {
            continue;
        }
        for (auto &c : line) {
            c = tolower(c);
        }
        words_.push_back(line);
    }
}

void Wordle::ChooseRandomWord()
{
    if (words_.size() == 0) {
        std::cerr << "Words list is empty. Can not choose random word.\n";
        return;
    }
    SetWord(words_[rand()%words_.size()]);
}

void Wordle::SetWord(const tiny_utf8::string &word)
{
    word_length_ = word.length();
    chosen_word_ = word;

    character_count_.clear();
    for (const auto &c : chosen_word_) {
        character_count_[c]++;
    }
    word_chosen_ = true;
}

tiny_utf8::string Wordle::GetWord()
{
    if (!word_chosen_) {
        std::cerr << "Word not chosen, can not return the word" << std::endl;
        return "";
    }
    return chosen_word_;
}

void Wordle::MakeGuess(const tiny_utf8::string &guess, tiny_utf8::string &hint)
{
    hint = "";

    if (guess.length() != word_length_) {
        return;
    }
    
    auto count = character_count_;

    // Substract correctly placed letters from counts
    for (int i = 0; i < word_length_; ++i) {
        const auto &c = guess[i];
        if (c == chosen_word_[i]) {
            count[c]--;
        }
    }

    for (int i = 0; i < word_length_; ++i) {
        if (i > 0) {
            hint += " ";
        }

        const auto &c = guess[i];
        if (c == chosen_word_[i]) {
            hint += toupper(c);
        } else if (count[c] > 0) {
            hint += c;
            count[c]--;
        } else {
            hint += "!";
            hint += c;
        }
    }
}
