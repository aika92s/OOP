#ifndef WORDCOUNTER_H
#define WORDCOUNTER_H
#include <vector>
#include "FileRead.h"

using ListOfWords = std::vector<std::pair<std::string, std::array<float, 2>>>;

class WordCounter {

private: 
   MapOfWords list;

public:
    WordCounter() = default;
    void setList(FileRead &list);
    void Count();
    ListOfWords getSortedWordList() const;
};

#endif
