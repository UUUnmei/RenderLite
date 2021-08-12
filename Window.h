#pragma once
#ifndef WINDOW_H
#define WINDOW_H


#include "ChiliWin.h"
#include "Keyboard.h"
#include "Mouse.h"
#include "Graphics.h"

#include <optional>
#include <memory>

class Window {

public:

private:
	// ����ģʽ�� ���������ע�������registration/cleanup��
	// �������������WinMain�е�trycatch�������޷������쳣������Ҳûд��ص��ж�
	// ���Ҫ�ӵĻ���ôwndClass�Ͳ���һ��ʼ���������и�������ͨ�غ���������Ȼ����catch�Ǹ�
	// �����̳�����˵����δ�������ﱨ���������Ծ�����
	class WindowClass {
	public:
		static const char* GetName() noexcept;
		static HINSTANCE GetInstance() noexcept;
	private:
		WindowClass() noexcept;
		~WindowClass();
		WindowClass(const WindowClass&) = delete;
		WindowClass& operator=(const WindowClass&) = delete;
		static constexpr const char* wndClassName = "Render Window";
		static WindowClass wndClass;  // ֻ��Ҫһ������
		HINSTANCE hInst;
	};

public:
	Window(int width, int height, const char* name);
	~Window();
	Window(const Window&) = delete;
	Window& operator=(const Window&) = delete;
	void SetTitle(const std::string& title);
	static std::optional<int> ProcessMessages() noexcept;
	int GetWidth(void) const noexcept;
	int GetHeight(void) const noexcept;
	Graphics& Gfx(); 

	Keyboard kbd;
	Mouse mouse;

private:
	// ϵͳ���ò�Ӧ�ô����thisָ�루�ò��ˣ���
	// ��static����ָ�����÷�ʽ���Ͳ��ᴫthis��ȥ��
	// �����ֵ��쳣��ϵͳ�������ǲ�Ӧ��catch
	static LRESULT WINAPI HandleMsgSetup(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept;
	static LRESULT WINAPI HandleMsgThunk(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept;
	LRESULT HandleMsg(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept;

	int width;
	int height;
	HWND hWnd;
	std::unique_ptr<Graphics> pGfx; //���������ҪhWnd��������û�У���ͨ��ָ����Ǳ�������ʽ�����������ʱ���Ƴ٣���window���캯��

};


#endif // !WINDOW_H