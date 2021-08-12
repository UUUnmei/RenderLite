#include "Window.h"
#include <sstream>
#include "resource.h"
#include "Exception.h"

// ����static
Window::WindowClass Window::WindowClass::wndClass;

Window::WindowClass::WindowClass() noexcept
	: hInst(GetModuleHandle(nullptr))
{
	// register window class
	WNDCLASSEX wc;
	wc.cbSize = sizeof(WNDCLASSEX);
	wc.style = CS_OWNDC;	// ÿ�����ڶ�����Ⱦ
	wc.lpfnWndProc = HandleMsgSetup;  // ����������Ϣ�ĺ���
	wc.cbClsExtra = 0;   // ��������
	wc.cbWndExtra = 0;
	wc.hInstance = GetInstance();
	wc.hIcon = static_cast<HICON>(LoadImage(
		GetInstance(), MAKEINTRESOURCE(IDI_ICON1),
		IMAGE_ICON, 32, 32, 0
	));
	wc.hCursor = nullptr;
	wc.hbrBackground = nullptr;
	wc.lpszMenuName = nullptr;   // ����Ҫmenu
	wc.lpszClassName = GetName();  // ����ʵ�����ĸ��ഴ��
	wc.hIconSm = static_cast<HICON>(LoadImage(
		GetInstance(), MAKEINTRESOURCE(IDI_ICON1),
		IMAGE_ICON, 16, 16, 0
	));
	RegisterClassEx(&wc);


}

Window::WindowClass::~WindowClass()
{
	UnregisterClass(GetName(), GetInstance());
}

const char* Window::WindowClass::GetName() noexcept
{
	// ��ȡ��������
	return wndClassName;
}

HINSTANCE Window::WindowClass::GetInstance() noexcept
{
	// ��ȡ����ʵ��
	return wndClass.hInst;
}

Window::Window(int width, int height, const char* name)
	:width(width), height(height)
{
	// calculate window size based on desired client region size
	RECT wr;
	wr.left = 100;
	wr.right = width + wr.left;
	wr.top = 100;
	wr.bottom = height + wr.top;
	// һ�����ô�С���������ڴ�С����������߿��ܹ���640*480
	// �����adjust����client�����������640*480��������������Զ������������ڴ�С
	// ��������createʱ���Ĵ�С����Ҫ������wr
	if (AdjustWindowRect(&wr, WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU, FALSE) == 0) {
		THROW_ERROR("AdjustWindowRect failed");
	}

	// create window & get hWnd
	hWnd = CreateWindow(
		WindowClass::GetName(),		// ��������
		name,   // ����
		WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU,  // style
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		wr.right - wr.left, // width
		wr.bottom - wr.top,  // height
		nullptr,   // ����handle
		nullptr,  // menu handle
		WindowClass::GetInstance(),
		this  // �Զ������ handle
	);
	if (hWnd == nullptr) {
		THROW_ERROR("CreateWindow failed");
	}

	// show
	ShowWindow(hWnd, SW_SHOW);  //���ھ�� �� ��ʾ��ʽ

	// ����ͼ��
	pGfx = std::make_unique<Graphics>(hWnd, width, height);

	
}

Window::~Window()
{
	DestroyWindow(hWnd);
}

void Window::SetTitle(const std::string& title)
{
	if (SetWindowText(hWnd, title.c_str()) == 0)
	{
		THROW_ERROR("SetWindowText failed");
	}
}



std::optional<int> Window::ProcessMessages() noexcept
{
	MSG msg;
	// while queue has messages, remove and dispatch them (but do not block on empty queue)
	while (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
	{
		// check for quit because peekmessage does not signal this via return val
		if (msg.message == WM_QUIT)
		{
			// return optional wrapping int (arg to PostQuitMessage is in wparam) signals quit
			return msg.wParam;
		}

		// TranslateMessage will post auxilliary WM_CHAR messages from key msgs
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	// return empty optional when not quitting app
	return {};
}

int Window::GetWidth(void) const noexcept
{
	return width;
}

int Window::GetHeight(void) const noexcept
{
	return height;
}

Graphics& Window::Gfx()
{
	if (!pGfx)
	{
		THROW_ERROR("no graphics");
	}
	return *pGfx;
}

LRESULT WINAPI Window::HandleMsgSetup(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept
{
	// use create parameter passed in from CreateWindow() to store window class pointer at WinAPI side

	// ��һ�δ�������ʱ
	// WM_NCCREATE ������ WM_CREATE
	// ����ʼ��
	if (msg == WM_NCCREATE)
	{
		// extract ptr to window class from creation data
		const CREATESTRUCTW* const pCreate = reinterpret_cast<CREATESTRUCTW*>(lParam);
		// ������õ���֮ǰcreateʱ�����Զ������
		Window* const pWnd = static_cast<Window*>(pCreate->lpCreateParams);
		// set WinAPI-managed user data to store ptr to window instance  
		// �Ѵ�����ʹ��ڱ�����ϵ����
		SetWindowLongPtr(hWnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(pWnd));
		// set message proc to normal (non-setup) handler now that setup is finished
		//�Դ���Ϣ��������HandleMsgThunk
		SetWindowLongPtr(hWnd, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(&Window::HandleMsgThunk));
		// forward message to window instance handler
		return pWnd->HandleMsg(hWnd, msg, wParam, lParam);
	}
	// if we get a message before the WM_NCCREATE message, handle with default handler
	return DefWindowProc(hWnd, msg, wParam, lParam);
}

LRESULT WINAPI Window::HandleMsgThunk(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept
{
	// �õ�����ָ�벢������������Ϣ���������
	// retrieve ptr to window instance
	Window* const pWnd = reinterpret_cast<Window*>(GetWindowLongPtr(hWnd, GWLP_USERDATA));
	// forward message to window instance handler
	return pWnd->HandleMsg(hWnd, msg, wParam, lParam);
}

LRESULT Window::HandleMsg(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept
{
	//// ����յ�����Ϣ
	//static WindowsMessageMap mm;
	//OutputDebugString(mm(msg, lParam, wParam).c_str());

	switch (msg) {
	case WM_CLOSE:	// �ı䡰�رա���Ĭ����Ϊ
		PostQuitMessage(0);  // ���˳���Ϣ������Ϣ���У��������˳�ʱ�ķ���ֵ����Ҫwinmain�е�return���
		return 0;  // ����ֱ�ӷ��أ����������def�ˣ������౻����2�Ρ���������ֻ��quit��Ϣ����ʵ�����ٴ��ڣ���������������

	

	// clear keystate when window loses focus to prevent input getting "stuck"
	// ���㲻�ڴ�����ʱ�������е�����������ռ���״̬���������һ������һֱ����
	case WM_KILLFOCUS:
		kbd.ClearState();
		break;
		/*********** KEYBOARD MESSAGES ***********/
	case WM_KEYDOWN:
		// syskey commands need to be handled to track ALT key (VK_MENU) and F10
	case WM_SYSKEYDOWN:
		// filter autorepeat
		if (!(lParam & 0x40000000) || kbd.AutorepeatIsEnabled()) {
			kbd.OnKeyPressed(static_cast<unsigned char>(wParam));
		}
		break;
	case WM_KEYUP:
	case WM_SYSKEYUP:
		kbd.OnKeyReleased(static_cast<unsigned char>(wParam));
		break;
	case WM_CHAR:
		kbd.OnChar(static_cast<unsigned char>(wParam));
		break;
		/*********** END KEYBOARD MESSAGES ***********/

		/************* MOUSE MESSAGES ****************/
	case WM_MOUSEMOVE:
	{
		const POINTS pt = MAKEPOINTS(lParam);
		// in client region -> log move, and log enter + capture mouse (if not previously in window)
		if (pt.x >= 0 && pt.x < width && pt.y >= 0 && pt.y < height)
		{
			mouse.OnMouseMove(pt.x, pt.y);
			if (!mouse.IsInWindow())
			{
				SetCapture(hWnd);
				mouse.OnMouseEnter();
			}
		}
		// not in client -> log move / maintain capture if button down
		else
		{
			if (wParam & (MK_LBUTTON | MK_RBUTTON))
			{
				mouse.OnMouseMove(pt.x, pt.y);
			}
			// button up -> release capture / log event for leaving
			else
			{
				ReleaseCapture();
				mouse.OnMouseLeave();
			}
		}
		break;
	}
	case WM_LBUTTONDOWN:
	{
		const POINTS pt = MAKEPOINTS(lParam);
		mouse.OnLeftPressed(pt.x, pt.y);
		break;
	}
	case WM_RBUTTONDOWN:
	{
		const POINTS pt = MAKEPOINTS(lParam);
		mouse.OnRightPressed(pt.x, pt.y);
		break;
	}
	case WM_LBUTTONUP:
	{
		const POINTS pt = MAKEPOINTS(lParam);
		mouse.OnLeftReleased(pt.x, pt.y);
		// release mouse if outside of window
		if (pt.x < 0 || pt.x >= width || pt.y < 0 || pt.y >= height)
		{
			ReleaseCapture();
			mouse.OnMouseLeave();
		}
		break;
	}
	case WM_RBUTTONUP:
	{
		const POINTS pt = MAKEPOINTS(lParam);
		mouse.OnRightReleased(pt.x, pt.y);
		// release mouse if outside of window
		if (pt.x < 0 || pt.x >= width || pt.y < 0 || pt.y >= height)
		{
			ReleaseCapture();
			mouse.OnMouseLeave();
		}
		break;
	}
	case WM_MOUSEWHEEL:
	{
		const POINTS pt = MAKEPOINTS(lParam);
		const int delta = GET_WHEEL_DELTA_WPARAM(wParam);
		mouse.OnWheelDelta(pt.x, pt.y, delta);
		break;
	}
	/************** END MOUSE MESSAGES **************/


	}
	return DefWindowProc(hWnd, msg, wParam, lParam);  // Ĭ����Ϣ����
}
