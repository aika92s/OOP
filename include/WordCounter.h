#ifndef WORDCOUNTER_H
#define WORDCOUNTER_H

#include <string>
#include <list>
#include <map>
#include <array>
#include <vector>
#include "FileRead.h"

class WordCounter {
private: 
    std::map<std::string, std::array<float, 2>> wordStats;
    std::list<std::string> lines;

public:
    WordCounter() = default;
    void setString(const FileRead &strings);
    void splitAndCount();
    std::vector<std::pair<std::string, std::array<float, 2>>> getSortedWordStats() const;
};

#endif
