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
		case WM_CREATE:
		{
			window->OnCreate(handle);
			break;
		}
		case WM_SIZE:
		{
			window->OnSize();
			break;
		}
		case WM_DESTROY:
		{
			window->OnDestroy();
			break;
		}
		default:
			break;
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
	windowClass.hCursor = LoadCursor(NULL, IDC_CROSS);

	return ::RegisterClassEx(&windowClass) != 0;
}

bool COverlappedWindow::Create(HINSTANCE instance) {
	hInst = instance;

	COverlappedWindow::handle = ::CreateWindowEx(0,
		L"COverlappedWindow",
		L"Task2",
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		0,
		0,
		instance,
		this);

	return COverlappedWindow::handle != 0;
}


void COverlappedWindow::OnCreate(HWND handle) {
	RECT rect;
	::GetClientRect(handle, &rect);
}

void COverlappedWindow::OnSize() {
	HDC hDC = GetDC(handle);
	RECT rect;
	::GetClientRect(handle, &rect);

	::ReleaseDC(handle, hDC);
}

void COverlappedWindow::OnDestroy() {
	::PostQuitMessage(0);
}

void COverlappedWindow::Show(int cmdShow) {
	::ShowWindow(handle, cmdShow);
}
