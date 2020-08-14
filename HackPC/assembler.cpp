#include "assembler.h"
#include <vector>
#include <boost/algorithm/string.hpp>
#include <bitset>
#include <iostream>

std::map<std::string, std::string> cInstructions =
{
{"0","101010"},{"1", "111111"},{"-1", "111010"},
{"D", "001100"},{"A", "110000"},{"!D", "001101"},
{"!A", "110001"},{"-D", "001111"},{"-A", "110011"},
{"D-1", "001110"},{"A-1", "110010"},{"D-A", "010011"},
{"A-D", "000111"},{"D+1", "011111"},{"A+1", "110111"},
{"D+A", "000010"},{"D&A", "000000"},{"D|A", "010101"}
};

std::map<std::string, int> variables;
std::map<std::string, std::string>jumps;
int symbolsNum;

void initializeVariablesMap()
{
	variables = {
		{"@SCREEN", 16384}, {"@KBD", 24576},{"@SP", 0},
		{"@LCL", 1}, {"@ARG", 2},{"@THIS", 3},
		{"@THAT", 4}
	};

	for (int i = 0; i < 16; i++) {
		variables["@R" + std::to_string(i)] = i;
		symbolsNum = variables.size();
	}

	jumps = {
		{"JGT", "001"},{"JEQ", "010"},{"JGE", "011"},
		{"JLT", "100"},{"JNE", "101"},
		{"JLE", "110"},{"JMP", "111"}
	};
}

std::string parse(std::string line)
{
	line = removeSpaces(line);
	if (line.length() == 1 || (line[0] == '/' && line[1] == '/')) return "";
	return allBefore(line, "//");
}

std::string assemble(std::string line, std::map<std::string, std::string>& labels)
{
	line = parse(line);
	if (line.length() == 0 || line[0] == '(')  return "";

	if (line[0] == '@')
		return aInstruction(line, labels);
	else
		return cInstruction(line);
}

std::string aInstruction(std::string line, std::map <std::string, std::string>& labels) {
	std::string content = line.substr(1);
	if (labels.count(content)) {
		line = labels[content];
		content = line.substr(1);
	}

	int num;
	if (0 <= line[1] - '0' && line[1] - '0' <= 9) {
		num = std::stoi(content);
	}
	else if (variables.count(line))
		num = variables[line];
	else {
		variables[line] = variables.size() + 16 - symbolsNum;
		num = variables[line];
	}

	return std::bitset<16>(num).to_string();
}

std::string cInstruction(std::string line)
{
	return "111" + comp(line) + dest(line) + jump(line);
}

std::string comp(std::string line)
{
	line = allBefore(line, ";");
	if (line.find("=") != std::string::npos) {
		line = allAfter(line, "=");
	}

	std::string aBit;
	auto mPos = line.find("M");
	if (mPos != std::string::npos) {
		aBit = "1";
		line[mPos] = 'A';
	}
	else {
		aBit = "0";
	}

	if (line.length() >= 3) {
		std::string op = std::string(1, line[1]);
		if (op == "+" || op == "&" || op == "|") {
			auto splitOp = splitIn2(line, op);
			if (splitOp[0] == "-1" or splitOp[0] == "1" || (splitOp[0] == "A" && splitOp[1] == "D")) {
				line = splitOp[1] + op + splitOp[0];
			}
			else {
				line = splitOp[0] + op + splitOp[1];
			}
		}
	}

	return aBit + cInstructions[line];
}

std::string dest(std::string line)
{
	std::string destination = allBefore(line, "=");
	if (destination == line)
		return "000";
	else {
		std::string d1 = "0";
		std::string d2 = "0";
		std::string d3 = "0";
		if (destination.find("A") != std::string::npos) d1 = "1";
		if (destination.find("D") != std::string::npos) d2 = "1";
		if (destination.find("M") != std::string::npos) d3 = "1";
		return d1 + d2 + d3;
	}
}

std::string jump(std::string line)
{
	line = allAfter(line, ";");
	if (line.length() == 0) return "000";

	return jumps[line];
}

std::string removeSpaces(std::string line)
{
	std::string result;
	for (uint16_t i = 0; i < line.length(); i++) {
		if (line[i] != ' ' && line[i] != '\n' && line[i] != '	') {
			result.push_back(line[i]);
		}
	}

	return result;
}

std::string allBefore(std::string& str, std::string delimiter)
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

std::string allAfter(std::string & str, std::string delimiter)
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

std::vector<std::string> splitIn2(std::string & str, std::string delimiter)
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