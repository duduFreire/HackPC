#include "stringAlgs.h"

std::string stringalgs::removeSpaces(std::string line)
{
	std::string result;
	for (uint16_t i = 0; i < line.length(); i++) {
		if (line[i] != ' ' && line[i] != '\n' && line[i] != '	') {
			result.push_back(line[i]);
		}
	}

	return result;
}

std::string stringalgs::allBefore(std::string& str, std::string delimiter)
{
	std::string result;
	if (delimiter.length() == 1) {
		for (uint16_t i = 0; str[i] != delimiter[0] && i < str.length(); i++) {
			result.push_back(str[i]);
		}
	}
	else {
		for (uint16_t i = 0; str.substr(i, delimiter.length()) != delimiter && i < str.length(); i++) {
			result.push_back(str[i]);
		}
	}

	return result;
}

std::string stringalgs::allAfter(std::string & str, std::string delimiter)
{
	std::string result;
	bool reachedDelimiter = false;
	if (delimiter.length() == 1) {
		for (int i = 0; i < str.length(); i++) {
			if (reachedDelimiter) {
				result.push_back(str[i]);
			}

			if (str[i] == delimiter[0]) reachedDelimiter = true;
		}
	}
	else {
		for (int i = 0; i < str.length(); i++) {
			if (reachedDelimiter) {
				result.push_back(str[i]);
			}

			if (str.substr(i, delimiter.length()) == delimiter) {
				i += delimiter.length() - 1;
				reachedDelimiter = true;
			}
		}
	}

	return result;
}

std::vector<std::string> stringalgs::splitIn2(std::string & str, std::string delimiter)
{
	std::vector<std::string> result = { "", "" };
	if (delimiter.length() == 1) {
		uint16_t i = 0;
		for (; str[i] != delimiter[0] && i < str.length(); i++) {
			result[0].push_back(str[i]);
		}
		i++;
		for (; i < str.length(); i++) {
			result[1].push_back(str[i]);
		}
	}
	else {
		int i = 0;
		for (; str.substr(i, delimiter.length()) != delimiter && i < str.length(); i++) {
			result[0].push_back(str[i]);
		}
		i++;
		for (; i < str.length(); i++) {
			result[1].push_back(str[i]);
		}
	}

	return result;
}