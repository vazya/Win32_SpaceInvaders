#include "PainterClass.h"
#include <Windows.h>

void PainterClass::Draw(HWND handle) {

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
}
