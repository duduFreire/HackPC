#include "Computer.h"

Computer::Computer(std::string hackPath, HDC hdc) :
	screen((COLORREF*)calloc(width * height, sizeof(COLORREF))),
	hdc(hdc)
{
	for (int i = 0; i < width*height; i++) {
		screen[i] = 0xFFFFFF;
	}

	pc = 0;
	ram.reserve(24576);
	rom.reserve(32768);

	std::ifstream hackFile(hackPath);

	std::string line;
	for (int i = 0; std::getline(hackFile, line); i++) {
		rom.push_back((word)std::stoi(line, 0, 2));
	}

	for (int i = 0; i < 24576; i++) ram.push_back((word)0);

	hackFile.close();
}

void Computer::executeCycle() {
	word instruction = rom[pc];
	if ((instruction >> 15 & 1) == 0) {
		aReg = instruction;
		pc++;
		return;
	}

	word y;
	if ((instruction >> 12) & 1) y = ram[aReg];
	else y = aReg;
	word comp = ALU(dReg, y, instruction >> 11 & 1, instruction >> 10 & 1,
		instruction >> 9 & 1, instruction >> 8 & 1, instruction >> 7 & 1, instruction >> 6 & 1);

	if ((instruction >> 5 & 1) == 1) aReg = comp;
	if ((instruction >> 4 & 1) == 1) dReg = comp;
	if ((instruction >> 3 & 1) == 1) {
		ram[aReg] = comp;
		if (aReg >= SCREEN && aReg < KBD) {
			refreshScreen(comp, aReg);
		}
	}

	pc++;

	if ((instruction >> 2 & 1) == 1 && ng) pc = aReg;
	if ((instruction >> 1 & 1) == 1 && zr) pc = aReg;
	if ((instruction >> 0 & 1) == 1 && (!ng && !zr)) pc = aReg;
}

Computer::word Computer::ALU(word x, word y, bool zx, bool nx, bool zy, bool ny, bool f, bool no) {
	word result;
	if (zx) x = 0;
	if (nx) x = ~x;
	if (zy) y = 0;
	if (ny) y = ~y;
	if (f) result = x + y;
	else result = x & y;
	if (no) result = ~result;

	zr = result == 0;
	ng = (result & (1 << 15)) != 0;

	return result;
}

void Computer::refreshScreen(word comp, word aReg)
{
	for (uint16_t i = 0; i < 16; i++) {
		if ((comp >> i) & 1) {
			screen[16 * (aReg - 16384) + i] = 0x000000;
		}
		else {
			screen[16 * (aReg - 16384) + i] = 0xFFFFFF;
		}
	}

	HBITMAP map = CreateBitmap(width, // width. 512 in my case
		height, // height
		1, // Color Planes, unfortanutelly don't know what is it actually. Let it be 1
		8 * 4, // Size of memory for one pixel in bits (in win32 4 bytes = 4*8 bits)
		(void*)screen); // pointer to array
// Temp HDC to copy picture
	HDC src = CreateCompatibleDC(hdc); // hdc - Device context for window, I've got earlier with GetDC(hWnd) or GetDC(NULL);
	SelectObject(src, map); // Inserting picture into our temp HDC
	// Copy image from temp HDC to window
	BitBlt(hdc, // Destination
		0,  // x and
		0,  // y - upper-left corner of place, where we'd like to copy
		width, // width of the region
		height, // height
		src, // source
		0,   // x and
		0,   // y of upper left corner  of part of the source, from where we'd like to copy
		SRCCOPY); // Defined DWORD to juct copy pixels. Watch more on msdn;

	DeleteObject(map);
	DeleteDC(src); // Deleting temp HDC
}