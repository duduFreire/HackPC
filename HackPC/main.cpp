#include <boost/algorithm/string.hpp>
#include <fstream>
#include <iostream>
#include "assembler.h"

int main()
{
	std::ifstream asmFile("PongL.asm");
	std::ofstream hackFile("PongL.hack");

	std::map<std::string, std::string> labels;
	initializeVariablesMap();

	int instructionCounter = 0;
	std::string line;
	while (std::getline(asmFile, line)) {
		line = parse(line);
		if (line.length() == 0) continue;

		if (line[0] == '(') {
			std::string label = line.substr(1, line.length() - 2);
			labels[label] = "@" + std::to_string(instructionCounter);
		}
		else {
			instructionCounter++;
		}
	}

	asmFile.clear();
	asmFile.seekg(0);
	while (std::getline(asmFile, line)) {
		line = assemble(line, labels);
		if (line.length() == 0) continue;
		hackFile << line << '\n';
	}

	hackFile.close();
	asmFile.close();
}