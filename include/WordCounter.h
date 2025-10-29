#ifndef WORDCOUNTER_H
#define WORDCOUNTER_H
#include "FileReader.h"

using ListOfWords = std::vector<std::pair<std::string, std::array<float, 2>>>;
using MapOfWords = std::map<std::string, std::array<float, 2>>;
class WordCounter {

private: 
   MapOfWords wordFreq_;

public:
	void addWord(const std::string& word);
    void CountProcentFreq();
    ListOfWords getSortedWordList() const;
};

#endif
