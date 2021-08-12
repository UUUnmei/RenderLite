#include "App.h"

int WINAPI WinMain(   // winapi stdcall�� ָ����������������ķ�ʽ���������λ�ú�˳���
	HINSTANCE hInstance,
	HINSTANCE hPrevInstance,
	LPSTR lpCmdline,
	int nCmdShow)
{
	try {
		App{}.Go();

	}
	catch (const std::exception& e) {
		MessageBox(nullptr, e.what(), "Standard Exception", MB_OK | MB_ICONEXCLAMATION);
	}
	catch (...) {
		MessageBox(nullptr, "No details available", "Unknown Exception", MB_OK | MB_ICONEXCLAMATION);
	}
	return -1;
}
