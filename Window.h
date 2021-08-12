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
	// 单例模式， 管理窗口类的注册和清理（registration/cleanup）
	// 这个的运行早于WinMain中的trycatch，所以无法捕获异常，所以也没写相关的判断
	// 如果要加的话那么wndClass就不能一开始创建，得有个函数，通关函数创建，然后再catch那个
	// 不过教程作者说他从未遇到这里报错。。。所以就算了
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
		static WindowClass wndClass;  // 只需要一个窗口
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
	// 系统调用不应该带类的this指针（用不了），
	// 用static可以指定调用方式，就不会传this过去了
	// 像这种的异常由系统处理，我们不应该catch
	static LRESULT WINAPI HandleMsgSetup(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept;
	static LRESULT WINAPI HandleMsgThunk(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept;
	LRESULT HandleMsg(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept;

	int width;
	int height;
	HWND hWnd;
	std::unique_ptr<Graphics> pGfx; //这个构造需要hWnd（但这里没有），通过指针而非变量的形式来讲它构造的时机推迟，见window构造函数

};


#endif // !WINDOW_H