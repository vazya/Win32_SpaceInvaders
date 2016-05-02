#include "PainterClass.h"
#include <Windows.h>

void PainterClass::Init(HWND _handle) {
	// инициализация liveOrDead - массива состояния юнита
	handle = _handle;
	RECT rect;
	::GetClientRect(handle, &rect);
	topLeft = Point(rect.left, rect.top);
	width = rect.right - rect.left;
	height = rect.bottom - rect.top;
	unitProp.unitWidth = 50; // тут захардкоженные размеры объектов
	unitProp.unitHeight = 50; // тут захардкоженные размеры объектов
}

void PainterClass::Draw() {
	PAINTSTRUCT ps;
	HDC dc = ::BeginPaint(handle, &ps);
	HDC memDC = ::CreateCompatibleDC(dc);

	HBITMAP memBitmap = ::CreateCompatibleBitmap(dc, width, height);
	HGDIOBJ oldBitmap = ::SelectObject(memDC, memBitmap);

	for (int i = 0; i < 10; i++) {
		for (int j = 0; j < 10; j++) {
			DrawTestUnit(memDC, topLeft.x + 2*i*unitProp.unitWidth, topLeft.y + 2*j*unitProp.unitWidth);
		}
	}
	
	::DeleteObject(memBitmap);
	::DeleteObject(oldBitmap);

	::BitBlt(dc, 0, 0, width, height, memDC, 0, 0, SRCCOPY);

	::DeleteDC(dc);
	::DeleteDC(memDC);
}

void PainterClass::DrawTestUnit(HDC memDC, int xPos, int yPos) {

	int r = rand() % 256; int g = rand() % 256; int b = rand() % 256;
	HBRUSH mybrush = ::CreateSolidBrush(RGB(r, g, b));

	::SelectObject(memDC, mybrush);
	::Ellipse(memDC, xPos, yPos, xPos + unitProp.unitWidth, yPos + unitProp.unitHeight);

	::DeleteObject(mybrush);
}