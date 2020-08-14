#include <string>
#include <map>
#include <vector>

std::string parse(std::string line);
std::string assemble(std::string line, std::map <std::string, std::string>& labels);
std::string aInstruction(std::string line, std::map <std::string, std::string>& labels);
std::string cInstruction(std::string line);

std::string comp(std::string line);
std::string dest(std::string line);
std::string jump(std::string line);

std::string removeSpaces(std::string line);
std::string allBefore(std::string& str, std::string delimiter);
std::string allAfter(std::string& str, std::string delimiter);
std::vector<std::string> splitIn2(std::string& str, std::string delimiter);

void initializeVariablesMap();