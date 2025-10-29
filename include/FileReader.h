#ifndef FILEREADER_H
#define FILEREADER_H

#include <string>
#include <array>
#include <fstream>
#include <vector>
#include <map>


class WordCounter;

class FileReader {

private:
	std::string line;

public:
    FileReader(const std::string& filename, WordCounter& counter);

};
#endif
