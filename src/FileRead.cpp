#include "FileRead.h"

FileRead::FileRead(const std::string& file_name) {
    std::ifstream i_file(file_name);

    if (i_file.is_open()) {
		std::string line;

        while (std::getline(i_file, line)) {
			std::string word = "";

        	for (char c : line) {
            	if (std::isalnum(static_cast<unsigned char>(c))) {
                	word += static_cast<char>(std::tolower(static_cast<unsigned char>(c)));
            	} else {
                	if (!word.empty()) {
                    	list[word][0]++;
                    	word = "";
                	}
            	}
        	}
        	if (!word.empty()) {
            	list[word][0]++;
        	}
    	}
        i_file.close();
    }
}

MapOfWords FileRead::getList() {
	return list;
}
