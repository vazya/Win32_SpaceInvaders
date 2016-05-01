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
	windowClass.hCursor = LoadCursor(NULL, IDC_ARROW);

	return ::RegisterClassEx(&windowClass) != 0;
}

bool COverlappedWindow::Create(HINSTANCE instance) {
	hInst = instance;

	COverlappedWindow::handle = ::CreateWindowEx(0,
		L"COverlappedWindow",
		L"Task2",
		WS_OVERLAPPEDWINDOW,
		100,
		100,
		1000,
		600,
		0,
		0,
		instance,
		this);
	x = 500; y = 250; dx = 5; dy = 5;
	::SetTimer(handle, 1, 1, NULL);
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
	DeleteObject(hDC);
}

void COverlappedWindow::OnDestroy() {
	::PostQuitMessage(0);
}

void COverlappedWindow::Show(int cmdShow) {
	::ShowWindow(handle, cmdShow);
}

void COverlappedWindow::OnTimer() {
	RECT rect;
	::GetClientRect(handle, &rect);
	::InvalidateRect(handle, &rect, 0);
}

void COverlappedWindow::OnDraw2() {

	int diam = 500;
	POINT center = POINT();
	center.x = 500; center.y = 250;
	POINT top_left_corner;
	top_left_corner.x = center.x - diam / 2;
	top_left_corner.y = center.y - diam / 2;

	PAINTSTRUCT ps;
	HDC dc = ::BeginPaint(handle, &ps);
	HDC memDC = ::CreateCompatibleDC(dc);

	RECT rect;
	::GetClientRect(handle, &rect);
	int width = rect.right - rect.left;
	int height = rect.bottom - rect.top;

	HBITMAP memBitmap = ::CreateCompatibleBitmap(dc, width, height);
	HGDIOBJ oldBitmap = ::SelectObject(memDC, memBitmap);

	HPEN whitePen = ::CreatePen(PS_SOLID, 0, RGB(255, 255, 255));
	::SelectObject(memDC, whitePen);
	::Rectangle(memDC, 0, 0, width, height);
	
	int r = rand() % 256; int g = rand() % 256; int b = rand() % 256;
	HBRUSH mybrush = ::CreateSolidBrush(RGB(r, g, b));

	::SelectObject(memDC, mybrush);
	::Ellipse(memDC, top_left_corner.x, top_left_corner.y, top_left_corner.x + diam, top_left_corner.y + diam);

	::DeleteObject(mybrush);
	::DeleteObject(whitePen);

	::DeleteObject(memBitmap);
	::DeleteObject(oldBitmap);

	::BitBlt(dc, 0, 0, width, height, memDC, 0, 0, SRCCOPY);

	::DeleteDC(dc);
	::DeleteDC(memDC);
	::EndPaint(handle, &ps);
}

void COverlappedWindow::OnDraw() {
	painter.Draw(handle);
}