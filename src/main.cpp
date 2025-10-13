#include <iostream>
#include "FileRead.h"
#include "WordCounter.h"
#include "CSVWriter.h"

int main(int argc, char* argv[]) {
    if (argc != 3) {
        return 1;
    }
    
    FileRead inFileList(argv[1]);
    WordCounter wordCounter;
    wordCounter.setList(inFileList);
    wordCounter.Count();
    
    auto sortedWordList = wordCounter.getSortedWordList();
    CSVWriter::writeToFile(argv[2], sortedWordList);
    
    return 0;
}
