#include "FileRead.h"

FileRead::FileRead(const char* file_name, std::ios_base::openmode mode) {
    std::ifstream i_file(file_name, mode);
    std::string line;
    if (i_file.is_open()) {
        while (std::getline(i_file, line)) {
            strings.push_back(line);
        }
        i_file.close();
    }
}

std::list<std::string> FileRead::getStrings() const { 
    return strings;
}
