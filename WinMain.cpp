#include "App.h"

#include "Draw2d.h"


int WINAPI WinMain(   // winapi stdcall宏 指定编译器处理参数的方式，包括存的位置和顺序等
	HINSTANCE hInstance,
	HINSTANCE hPrevInstance,
	LPSTR lpCmdline,
	int nCmdShow)
{
	try {
		//App{}.Go();
		Draw2d{}.Go();
	}
	catch (const std::exception& e) {
		MessageBox(nullptr, e.what(), "Standard Exception", MB_OK | MB_ICONEXCLAMATION);
	}
	catch (...) {
		MessageBox(nullptr, "No details available", "Unknown Exception", MB_OK | MB_ICONEXCLAMATION);
	}
	return -1;
}
