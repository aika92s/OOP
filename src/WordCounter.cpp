#include "WordCounter.h"
#include <cctype>
#include <algorithm>

void WordCounter::setList(FileRead &wordList) {
    list = wordList.getList();
}

void WordCounter::Count() {
    float totalCount = 0.0f;
    for (const auto& pair : list) {
        totalCount += pair.second[0];
    }  
    
    for (auto& pair : list) {
        if (totalCount > 0) {
            pair.second[1] = (pair.second[0] / totalCount) * 100.0f;
        } else {
            pair.second[1] = 0.0f;
        }
    }
}

ListOfWords WordCounter::getSortedWordList() const {
    ListOfWords sortedVector(list.begin(), list.end());
    
    std::sort(sortedVector.begin(), sortedVector.end(), 
        [](const auto& a, const auto& b) {
            if (a.second[0] != b.second[0]) {
                return a.second[0] > b.second[0];
            }
            return a.first > b.first;
        });
    
    return sortedVector;
}
