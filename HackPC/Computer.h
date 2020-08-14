#pragma once
#include <vector>
#include <fstream>
#include <string>
#include <iostream>
#include <Windows.h>

class Computer
{
public:
	typedef unsigned short word;
	static constexpr int width = 512;
	static constexpr int height = 256;
	static constexpr int SCREEN = 16384;
	static constexpr int KBD = 24576;
	bool ng;
	bool zr;
	word pc;
	word dReg;
	word aReg;
	std::vector<word> ram;
	std::vector<word> rom;
	HBITMAP map;
	COLORREF *screen;
	HDC hdc;

public:
	Computer(std::string hackPath, HDC hdc);
	void executeCycle();
	word ALU(word x, word y, bool zx, bool nx, bool zy, bool ny, bool f, bool no);
	void refreshScreen(word comp, word aReg);
};
