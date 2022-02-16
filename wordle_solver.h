#pragma once

#include <tinyutf8/tinyutf8.h>
#include <set>
#include <vector>
#include <string>

class WordleSolver {
 public:
    WordleSolver(const int word_length = 5);
    void AddHint(const std::string &hint);
    void AddHintsFromFile(const std::string &fname);
    void AddWordsFromFile(const std::string &fname);
    std::vector<tiny_utf8::string> GetPossibleSolutions();
    tiny_utf8::string GetRecommendedGuess();
    void PrintPossibleSolutions();
    bool IsPossibleSolution(const tiny_utf8::string &word);
 private:
    bool AddHintParts(const std::vector<tiny_utf8::string> &hint_parts);
 private:
    using charT = tiny_utf8::string::value_type;
    std::vector<tiny_utf8::string> words_;

    std::set<charT> contains_;
    std::set<charT> does_not_contain_;
    std::vector< std::tuple<int, charT> > known_;
    std::vector< std::tuple<int, charT> > not_here_but_somewhere_else_;
    int word_length_;
};
