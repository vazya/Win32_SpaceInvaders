#include "PainterClass.h"
#include <Windows.h>

void PainterClass::Init(HWND _handle) {
	// инициализация liveOrDead - массива состояния юнита
	handle = _handle;
	RECT rect;
	::GetClientRect(handle, &rect);
//	LeftTop = Point(rect.left, rect.top);
//	width = rect.right - rect.left;
//	height = rect.bottom - rect.top;
	unitProp.unitWidth = 50; // тут захардкоженные размеры объектов
	unitProp.unitHeight = 50; // тут захардкоженные размеры объектов
}

void PainterClass::SetWidth(int newWidth) {
	width = newWidth;
}

void PainterClass::SetHeight(int newHeight) {
	height = newHeight;
}

void PainterClass::SetLeftTop(int left, int top) {
	LeftTop.x = left;
	LeftTop.y = top;
}

void PainterClass::Draw() {
	PAINTSTRUCT ps;
	HDC dc = ::BeginPaint(handle, &ps);
	HDC memDC = ::CreateCompatibleDC(dc);

	HBITMAP memBitmap = ::CreateCompatibleBitmap(dc, width, height);
	HGDIOBJ oldBitmap = ::SelectObject(memDC, memBitmap);

	for (int i = 0; i < 20; i++) {
		for (int j = 0; j < 10; j++) {
			DrawTestUnit(memDC, LeftTop.x + 2*i*unitProp.unitWidth, LeftTop.y + 2*j*unitProp.unitWidth);
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