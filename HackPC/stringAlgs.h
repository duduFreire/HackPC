#pragma once
#include <string>
#include <vector>

namespace stringalgs {
	std::string removeSpaces(std::string line);
	std::string allBefore(std::string& str, std::string delimiter);
	std::string allAfter(std::string& str, std::string delimiter);
	std::vector<std::string> splitIn2(std::string& str, std::string delimiter);
}
