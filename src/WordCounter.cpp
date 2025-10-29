#include "WordCounter.h"
#include <cctype>
#include <algorithm>

void WordCounter::addWord(const std::string& word) {
	wordFreq_[word][0]++;
}

void WordCounter::CountProcentFreq() {
    float totalCount = 0.0f;
    for (const auto& pair : wordFreq_) {
        totalCount += pair.second[0];
    }  
    
    for (auto& pair : wordFreq_) {
        if (totalCount > 0) {
            pair.second[1] = (pair.second[0] / totalCount) * 100.0f;
        } else {
            pair.second[1] = 0.0f;
        }
    }
}
ListOfWords WordCounter::getSortedWordList() const {
    ListOfWords sortedWords(wordFreq_.begin(), wordFreq_.end());
    
    std::sort(sortedWords.begin(), sortedWords.end(),
        [](const auto& a, const auto& b) {
            if (a.second[0] != b.second[0]) {
                return a.second[0] > b.second[0];
            }
            return a.first < b.first;
        });
    
    return sortedWords;
}
