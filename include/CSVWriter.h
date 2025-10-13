#ifndef CSVWRITER_H
#define CSVWRITER_H

#include <string>
#include <vector>
#include <map>
#include <array>
#include <fstream>

using ListOfWords = std::vector<std::pair<std::string, std::array<float, 2>>>;

class CSVWriter {

public:
    static void writeToFile(const std::string& fileName,
                           const ListOfWords& sortedWordStats);
};

#endif
