#include "COverlappedWindow.h"
#include <Windows.h>
#include <iostream>

COverlappedWindow::COverlappedWindow() {
}

COverlappedWindow::~COverlappedWindow() {
}

LRESULT __stdcall COverlappedWindow::windowProc(HWND handle, UINT message, WPARAM wParam, LPARAM lParam) {
	if (message == WM_ERASEBKGND) {
		return true;
	}
	if (message == WM_NCCREATE) {
		COverlappedWindow* window = reinterpret_cast<COverlappedWindow*>(reinterpret_cast<CREATESTRUCT*>(lParam)->lpCreateParams);
		::SetWindowLongPtr(handle, GWLP_USERDATA, reinterpret_cast<LONG>(window));
	} else {
		COverlappedWindow* window = reinterpret_cast<COverlappedWindow*>(::GetWindowLongPtr(handle, GWLP_USERDATA));
		switch (message) {
		case WM_CREATE: {
			window->OnCreate(handle);
			break;
		}
		case WM_PAINT: {
			window->OnDraw();
			break;
		}
		case WM_TIMER: {
			window->OnTimer();
			break;
		}
		case WM_KEYDOWN: {
			switch (wParam) {
			case VK_LEFT: {
				window->painter.SetLeftMove();
			}
			break;
			case VK_RIGHT: {
				window->painter.SetRightMove();
			}
			break;
			case VK_SPACE: {
				window->painter.SetShipShoot(true);
			}
			break;
			}
			break;
		}
		case WM_KEYUP: {
			switch (wParam) {
			case VK_LEFT: {
				window->painter.SetStopMove();
			}
			break;
			case VK_RIGHT: {
				window->painter.SetStopMove();
			}
			break;
			}
			break;
		}
		case WM_SIZE: {
			window->OnSize();
			break;
		}
		case WM_DESTROY: {
			window->OnDestroy();
			break;
		}
		default:
			return DefWindowProc(handle, message, wParam, lParam);
		}
	}
	return DefWindowProc(handle, message, wParam, lParam);
}

bool COverlappedWindow::RegisterClass(HINSTANCE instance) {
	WNDCLASSEX windowClass;

	::ZeroMemory(&windowClass, sizeof(windowClass));
	windowClass.cbSize = sizeof(WNDCLASSEX);
	windowClass.style = CS_DBLCLKS | CS_HREDRAW | CS_VREDRAW;
	windowClass.lpfnWndProc = COverlappedWindow::windowProc;
	windowClass.hInstance = instance;
	windowClass.lpszClassName = L"COverlappedWindow";
	windowClass.hCursor = LoadCursor(NULL, IDC_ARROW);

	return ::RegisterClassEx(&windowClass) != 0;
}

bool COverlappedWindow::Create(HINSTANCE instance) {
	hInst = instance;

	COverlappedWindow::handle = ::CreateWindowEx(0, L"COverlappedWindow", L"GALAXIAN",
		WS_OVERLAPPEDWINDOW ^ WS_THICKFRAME, 100, 100, 1000, 600, 0, 0, instance, this);
	// запрещаю менять размер окна т.к. это не имеет смысла для нашей игры
	::SetTimer(handle, 0, 1, NULL);
	return COverlappedWindow::handle != 0;
}

void COverlappedWindow::OnCreate(HWND handle) {
	RECT rect;
	::GetClientRect(handle, &rect);
	painter.Init(handle, hInst);
}

void COverlappedWindow::OnSize() {
	HDC hDC = GetDC(handle);
	RECT rect;
	::GetClientRect(handle, &rect);
	
	painter.OnSize(); // есть гипотетическая возможность менять размеры, но по смыслу менять размеры запрещено

	::ReleaseDC(handle, hDC);
	DeleteObject(hDC);
}

void COverlappedWindow::OnDestroy() {
	KillTimer(handle, 0);
	::PostQuitMessage(0);
}

void COverlappedWindow::Show(int cmdShow) {
	::ShowWindow(handle, cmdShow);
}

void COverlappedWindow::OnTimer() {
	painter.OnTime();
	RECT rect;
	::GetClientRect(handle, &rect);
	::InvalidateRect(handle, &rect, 0);
}

void COverlappedWindow::OnDraw() {
	painter.Draw();
}