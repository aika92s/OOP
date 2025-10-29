#include "CSVWriter.h"

void CSVWriter::writeToFile(const std::string& fileName, 
                           const ListOfWords& sortedWordStats) {
    std::ofstream file(fileName);
    if (!file.is_open()) {
        return;
    }
    
    for (const auto& pair : sortedWordStats) {
        file << pair.first << "," << static_cast<int>(pair.second[0]) << "," << pair.second[1] << "\n";
    }
    file.close();
}
