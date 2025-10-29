#include "FileReader.h"
#include "WordCounter.h"

FileReader::FileReader(const std::string& filename, WordCounter& counter) {
    std::ifstream i_file(filename);

    if (!i_file.is_open()) {
		return;
	}
	std::string word;
	while (std::getline(i_file, line)) {
        for (char c : line) {
            if (std::isalnum(static_cast<unsigned char>(c))) {
                word += static_cast<char>(std::tolower(static_cast<unsigned char>(c)));
            } else {
                if (!word.empty()) {
					counter.addWord(word);
                   word.clear();
                }
            }
        }
        if (!word.empty()) {
			counter.addWord(word);
            word.clear();
        }
    }
    i_file.close();
}

