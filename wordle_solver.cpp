#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <set>
#include <sstream>
#include <tinyutf8/tinyutf8.h>

using charT = tiny_utf8::string::value_type;

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

struct Hints {
    std::set<charT> contains;
    std::set<charT> does_not_contain;
    std::vector< std::tuple<int, charT> > known;
    std::vector< std::tuple<int, charT> > not_here_but_somewhere_else;
    int word_length = 5;
};

/// Read hints from a text file and populate Hints/rules.
/// Hints are formed as follows in the text file:
/// - One tried word per line
/// - Each tried character separated by space
/// - Capital letter = this character belongs to this position
/// - Lower case letter = this character does not belong here, but is in the word
/// - Character prefixed with '!' character not in the word, or character used too many times and not in this position
Hints read_hints(const std::string &fname, const int word_length = 5) {
    Hints out;
    out.word_length = word_length;
    std::ifstream file(fname);
    std::string line;
    while(std::getline(file, line)) {
        std::istringstream line_stream(line);
        std::string hint_std;
        int location = -1;
        while (std::getline(line_stream, hint_std, ' ')) {
            tiny_utf8::string hint(hint_std);
            ++location;

            // isalpha does not work for utf-8 characters
            if (hint.length() == 1 /*&& isalpha(hint[0])*/) {
                auto c = hint[0];
                if (islower(c)) {
                    // This character is in the word, but this is not a correct location for it
                    out.contains.insert(c);
                    out.not_here_but_somewhere_else.push_back({location, c});
                } else {
                    // This character is in correct location
                    c = tolower(c);
                    out.contains.insert(c);
                    out.known.push_back({location, c});

                    // TODO: Fix properly case where we have hint "E !e"
                    out.does_not_contain.erase(c);
                }
            } else if (hint.length() == 2 && hint[0] == '!' /*&& isalpha(hint[1])*/) {
                const auto c = tolower(hint[1]);

                // TODO: Fix properly case where we have hint "E !e" -> only single "E"
                if (!out.contains.count(c)) {
                    out.does_not_contain.insert(c);
                } else {
                    out.not_here_but_somewhere_else.push_back({location, c});
                }
            } else {
                std::cout << "Hint '" << hint << "' is malformed" << std::endl;
                continue;
            }
        }
    }

    return out;
}

/// Reads word list (one word per line) and prints out words that
/// pass the hints.
void check_words(const std::string &fname, const Hints &hints) {
    std::ifstream file(fname);
    std::string line_std;
    while(std::getline(file, line_std)) {
        tiny_utf8::string line(line_std);
        if (line.length() == hints.word_length) {
            bool fail = false;
            for (size_t i = 0; i < line.length(); ++i) {
                auto &c = line[i];
                c = tolower(c);
                if (hints.does_not_contain.count(c)) {
                    fail = true;
                    break;
                }
            }
            for (const auto &[loc, c] : hints.known) {
                if (line[loc] != c) {
                    // This location should have this character, but it is not here
                    fail = true;
                    break;
                }
            }
            for (const auto &[loc, c] : hints.not_here_but_somewhere_else) {
                if (line[loc] == c) {
                    // This character should not be here
                    fail = true;
                    break;
                }
            }

            if (!fail) {
                // Possible solution
                std::cout << line << std::endl;
            }
        }
    }
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
    const auto hints = read_hints(fname_hintlist, word_length);
    check_words(fname_wordlist, hints);
}
