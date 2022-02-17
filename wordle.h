#pragma once

#include <string>
#include <tinyutf8/tinyutf8.h>
#include <vector>
#include <unordered_map>
class Wordle {
 public:
    Wordle();
    void AddWordsFromFile(const std::string &fname, const int word_length);
    void ChooseRandomWord();
    void SetWord(const tiny_utf8::string &word);
    tiny_utf8::string GetWord();

    /// Returns a hint
    void MakeGuess(const tiny_utf8::string &guess, tiny_utf8::string &hint);
 private:
    std::vector<tiny_utf8::string> words_;
    int word_length_{5};

    bool word_chosen_{false};
    tiny_utf8::string chosen_word_;
    std::unordered_map<tiny_utf8::string::value_type, int> character_count_;
};
