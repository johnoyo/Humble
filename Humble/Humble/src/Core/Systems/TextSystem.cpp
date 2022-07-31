#include "TextSystem.h"

namespace HBL {

	void TextSystem::Start()
	{
		SDF_Importer("res/textures/testFont.fnt");
	}

	void TextSystem::Run()
	{
	}

	void TextSystem::Clear()
	{
	}

	void TextSystem::SDF_Importer(const std::string& path)
	{
		std::ifstream is(path);
		std::string line;

		// Skip the first 3 lines since they dont contain usefull data
		for (int i = 0; i < 4; i++)
			std::getline(is, line);

		// In the 4th line erase all but numbers
		line.erase(std::remove_if(line.begin(), line.end(), ispunct), line.end());
		line.erase(std::remove_if(line.begin(), line.end(), isalpha), line.end());
		line.erase(std::remove_if(line.begin(), line.end(), isblank), line.end());
		line.erase(std::remove_if(line.begin(), line.end(), isspace), line.end());

		// Convert string to integer, this is the number of entries in the SDF
		int count = stoi(line);
		char space_char = ' ';

		// Parse SDF data to vector
		for (int i = 0; i < count; i++)
		{
			std::getline(is, line);

			std::vector<std::string> words{};

			std::stringstream sstream(line);
			std::string word;
			while (std::getline(sstream, word, space_char)) 
			{
				if (word.size() != 0)
				{
					// Erase unwanted characters from string and only leave the number with its sign
					word.erase(std::remove(word.begin(), word.end(), '='), word.cend());
					word.erase(std::remove_if(word.begin(), word.end(), isalpha), word.end());
					word.erase(std::remove_if(word.begin(), word.end(), isblank), word.end());
					word.erase(std::remove_if(word.begin(), word.end(), isspace), word.end());

					if (word.size() != 0)
						words.push_back(word);
				}
			}

			sdfData.push_back({
				stoi(words[0]),
				stoi(words[1]),
				stoi(words[2]),
				stoi(words[3]),
				stoi(words[4]),
				stoi(words[5]),
				stoi(words[6]),
				stoi(words[7])
			});
		}

		is.close();

		return;
	}

}
