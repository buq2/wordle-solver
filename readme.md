# Wordle / Sanuli solver / helper

Solves word game problems seen on 
- https://www.powerlanguage.co.uk/wordle/
- https://www.nytimes.com/games/wordle/index.html
- https://sanuli.fi/

Can help to solve different length words.

# Word lists

World list should contain one word per line. File should be UTF-8 encoded.

## English

https://raw.githubusercontent.com/dwyl/english-words/master/words.txt


## Finnish

https://github.com/hugovk/everyfinnishword/blob/master/kaikkisanat.txt

# Hint file

While playing Wordle/Sanuli, fill in hints file based on the game output.
Hints are formed as follows in the text file:
- One tried word per line.
- Each tried character separated by space.
- Capital letter = this character belongs to this position.
- Lower case letter = this character does not belong here, but is in the word.
- Character prefixed with '!' = character not in the word, or character used too many times and not in this position.

For example
```
!h !e !i !n !ä
!p !u !l !j A
!r !a S !t A
```

# Missing features

- Does not use hints for number of times character appears. For example "E !e" means that letter 'e' only appears ones.
- Does not sort output based on word probability.

# Build

Conan packages are fetched automatically.

```
cmake -S . -B build
cmake --build build --parallel 12 --config Release
```

# Running

```
# Might be needed on Power shell to enable utf-8 encoding for prints
chcp 65001

.\build\bin\wordle_solver.exe words.txt hints.txt 5
```
