#pragma once
#include <string>
#include <map>
#include <vector>
#include <fstream>

class Assembler
{
public:
	Assembler(std::string asmPath);
	void assemble();

private:
	std::ifstream asmFile;
	std::ofstream hackFile;

	std::map<std::string, std::string> cInstructions;
	std::map<std::string, int> variables;
	std::map<std::string, std::string>jumps;

	int symbolsNum;
private:
	std::string parse(std::string line);
	std::string assemble(std::string line, std::map <std::string, std::string>& labels);

	std::string aInstruction(std::string line, std::map <std::string, std::string>& labels);
	std::string cInstruction(std::string line);

	std::string comp(std::string line);
	std::string dest(std::string line);
	std::string jump(std::string line);
};
