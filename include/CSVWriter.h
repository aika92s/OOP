#ifndef CSVWRITER_H
#define CSVWRITER_H

#include <string>
#include <vector>
#include <map>
#include <array>
#include <fstream>

class CSVWriter {
public:
    static void writeToFile(const std::string& fileName, 
                           const std::vector<std::pair<std::string, std::array<float, 2>>>& sortedWordStats);
};

#endif
