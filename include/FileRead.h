#ifndef FILEREAD_H
#define FILEREAD_H

#include <string>
#include <list>
#include <fstream>

class FileRead {
private:
    std::list<std::string> strings;

public:
    FileRead(const char* file_name, std::ios_base::openmode mode = std::ios_base::in);
    std::list<std::string> getStrings() const;
};

#endif
