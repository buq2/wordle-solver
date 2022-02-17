#include "wordle_solver.h"
#include <sstream>
#include <iostream>
#include <fstream>

WordleSolver::WordleSolver(const int word_length) 
    : word_length_(word_length)
{
}

void WordleSolver::AddHint(const std::string &line)
{
    std::istringstream line_stream(line);
    std::string hint_std;
    std::vector<tiny_utf8::string> hints;
    while (std::getline(line_stream, hint_std, ' ')) {
        tiny_utf8::string hint(hint_std);
        hints.push_back(hint);
    }
    AddHintParts(hints);
}

void WordleSolver::AddHint(const tiny_utf8::string &hint) {
    AddHint(hint.cpp_str());
}

void WordleSolver::AddHintsFromFile(const std::string &fname)
{
    std::ifstream file(fname);
    std::string line;
    while(std::getline(file, line)) {
        AddHint(line);
    }
}

void WordleSolver::AddWordsFromFile(const std::string &fname)
{
    std::ifstream file(fname);
    std::string line_std;
    while(std::getline(file, line_std)) {
        tiny_utf8::string line(line_std);
        if (IsPossibleSolution(line)) {
            words_.push_back(line);
        }
    }
}

std::vector<tiny_utf8::string> WordleSolver::GetPossibleSolutions()
{
    std::vector<tiny_utf8::string> solutions;
    for (const auto &word : words_) {
        if (IsPossibleSolution(word)) {
            solutions.push_back(word);
        }
    }
    return solutions;
}

void WordleSolver::PrintPossibleSolutions()
{
    for (const auto &word : GetPossibleSolutions()) {
        std::cout << word << std::endl;
    }
}

tiny_utf8::string WordleSolver::GetRecommendedGuess()
{
    // Return first possible solution
    for (const auto &word : words_) {
        if (IsPossibleSolution(word)) {
            return word;
        }
    }
    return {};
}

bool WordleSolver::IsPossibleSolution(const tiny_utf8::string &word)
{
    if (word.length() != word_length_) {
        return false;
    }

    for (size_t i = 0; i < word.length(); ++i) {
        const auto &c = word[i];
        if (does_not_contain_.count(c)) {
            return false;
        }
    }

    for (const auto &[loc, c] : known_) {
        if (word[loc] != c) {
            // This location should have this character, but it is not here
            return false;
        }
    }

    for (const auto &[loc, c] : not_here_but_somewhere_else_) {
        if (word[loc] == c) {
            // This character should not be here
            return false;
        }
    }

    return true;
}

bool WordleSolver::AddHintParts(const std::vector<tiny_utf8::string> &hint_parts)
{
    auto allows_character = [&](const auto &c) {
        for (const auto &hint : hint_parts) {
            const auto op = hint[0];
            if (op == '!') {
                // We are not interested in "not allowed" hints
                continue;
            } else if (hint[1] == c) {
                // This character is allowed here, or somewhere else
                return true;
            }
        }

        // This character is not explicitly allowed in this hint
        return false;
    };
    auto disallows_character = [&](const auto &c) {
        for (const auto &hint : hint_parts) {
            const auto &op = hint[0];
            if (op == '!' && hint[1] == c) {
                return true;
            }
        }
        return false;
    };
    auto times_allowed = [&](const auto &c) {
        int count = 0;
        for (const auto &hint : hint_parts) {
            const auto &op = hint[0];
            if (op == '!') {
                // We are not interested in "not allowed" hints
                continue;
            }
            if (hint[1] == c) {
                ++count;
            }
        }
        return count;
    };
    auto contains = [](const auto &container, const auto &tuple) {
        for (const auto &t : container) {
            if (t == tuple) return true;
        }
        return false;
    };
    // TODO: Add check for number of character occurances
    for (int location = 0; location < hint_parts.size(); ++location) {
        const auto &hint_part = hint_parts[location];

        if (hint_part.length() != 2) {
            std::cerr << "Hint '" << hint_part << "' is malformed" << std::endl;
            return false;
        }

        const auto &op = hint_part[0];
        const auto &c = hint_part[1];
        if (op == '?') {
            // This character is in the word, but this is not a correct location for it
            contains_.insert(c);
            not_here_but_somewhere_else_.push_back({location, c});
        } else if (op == '.') {
            // This character is in correct location
            contains_.insert(c);
            
            const auto t = std::make_tuple(location, c);
            if (!contains(known_, t)) {
                known_.push_back({location, c});
            }
        } else if (op == '!') {
            if (!allows_character(c)) {
                does_not_contain_.insert(c);
            } else {
                const auto t = std::make_tuple(location, c);
                if (!contains(not_here_but_somewhere_else_, t)) {
                    not_here_but_somewhere_else_.push_back(t);
                }
            }
        } else {
            std::cerr << "Hint '" << hint_part << "' is malformed" << std::endl;
            return false;
        }
    }
    return true;
}

