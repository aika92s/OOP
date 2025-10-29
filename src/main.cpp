#include <iostream>
#include "FileReader.h"
#include "WordCounter.h"
#include "CSVWriter.h"

int main(int argc, char* argv[]) {
    if (argc != 3) {
        return 1;
    }

   	WordCounter wordCounter;
    FileReader inFileList(argv[1], wordCounter);
    wordCounter.CountProcentFreq();
    
    auto sortedWords = wordCounter.getSortedWordList();
    CSVWriter::writeToFile(argv[2], sortedWords);
    
    return 0;
}
