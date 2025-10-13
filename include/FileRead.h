#ifndef FILEREAD_H
#define FILEREAD_H

#include <array>
#include <string>
#include <list>
#include <fstream>
#include <map>

using MapOfWords = std::map<std::string, std::array<float, 2>>;

class FileRead {

private:
	MapOfWords list;

public:
    FileRead(const std::string& file_name);
    MapOfWords getList();
};

#endif
