#include "WordCounter.h"
#include <cctype>
#include <algorithm>

void WordCounter::setString(const FileRead &strings) {
    lines = strings.getStrings();
}

void WordCounter::splitAndCount() {
    wordStats.clear();

    for (const auto& line : lines) {
        std::string word = "";
        for (char c : line) {
            if (std::isalnum(static_cast<unsigned char>(c))) {
                word += static_cast<char>(std::tolower(static_cast<unsigned char>(c)));
            } else {
                if (!word.empty()) {
                    wordStats[word][0]++;
                    word = "";
                }
            }
        }
        if (!word.empty()) {
            wordStats[word][0]++;
        }
    }
    
    float totalCount = 0.0f;
    for (const auto& pair : wordStats) {
        totalCount += pair.second[0];
    }  
    
    for (auto& pair : wordStats) {
        if (totalCount > 0) {
            pair.second[1] = (pair.second[0] / totalCount) * 100.0f;
        } else {
            pair.second[1] = 0.0f;
        }
    }
}

std::vector<std::pair<std::string, std::array<float, 2>>> WordCounter::getSortedWordStats() const {
    std::vector<std::pair<std::string, std::array<float, 2>>> sortedVector(wordStats.begin(), wordStats.end());
    
    std::sort(sortedVector.begin(), sortedVector.end(), 
        [](const auto& a, const auto& b) {
            return a.second[0] > b.second[0]; 
        });
    
    return sortedVector;
}
