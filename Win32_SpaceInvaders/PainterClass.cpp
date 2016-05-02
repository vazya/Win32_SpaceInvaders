#include "PainterClass.h"
#include <Windows.h>

void PainterClass::Init(HWND _handle) {
	// инициализация liveOrDead - массива состояния юнита
	handle = _handle;

//	RECT rect;
//	::GetClientRect(handle, &rect);
//	LeftTop = Point(rect.left, rect.top);
//	width = rect.right - rect.left;
//	height = rect.bottom - rect.top;
//	unitProp.unitWidth = 50; // тут захардкоженные размеры объектов
//	unitProp.unitHeight = 50; // тут захардкоженные размеры объектов
	InitMatrix();
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

void PainterClass::InitMatrix() {
	/*
	Здесь задается матрица относительного расположения юнитов, т.е.
	отрисовка будет вестись как LeftTop.x + v[i][j].x и LeftTop.y + v[i][j].y
	и менять мы будем с теченеием времени только LeftTop
	*/
	RECT rect;
	::GetClientRect(handle, &rect);
	LeftTop = Point(rect.left, rect.top);
	width = rect.right - rect.left;
	height = rect.bottom - rect.top;


	int m = 5;										// количество строк
	v.resize(m);
	liveOrDead.resize(m);
	int n = 10;										// количество столбцов
	for (int i = 0; i < m; ++i) {
		v[i].resize(n);
		liveOrDead[i].resize(n);
	}

	// тут мы захардкодим границы нашей матрицы

	int leftShift = (int)(0.2*(double)(width));
	int rightShift = (int)(0.8*(double)(width));
	int topShift = (int)(0.1*(double)(height));
	int botShift = (int)(0.5*(double)(height));

	int hWidth = int((double)(rightShift - leftShift) / (2 * n));
	int hHeight = int((double)(botShift - topShift) / (2 * m));
	for (int i = 0; i < m; i++) {
		for (int j = 0; j < n; j++) {
			v[i][j] = Point(leftShift + 2*j*hWidth, topShift + 2*i*hHeight);
			liveOrDead[i][j] = true;
		}
	}
	unitProp.unitWidth = hWidth;
	unitProp.unitHeight = hHeight;
}


void PainterClass::Draw() {
	PAINTSTRUCT ps;
	HDC dc = ::BeginPaint(handle, &ps);
	HDC memDC = ::CreateCompatibleDC(dc);

	HBITMAP memBitmap = ::CreateCompatibleBitmap(dc, width, height);
	HGDIOBJ oldBitmap = ::SelectObject(memDC, memBitmap);

	DrawMatrix(memDC);

/*
	for (int i = 0; i < 20; i++) {
		for (int j = 0; j < 10; j++) {
			DrawTestUnit(memDC, LeftTop.x + 2*i*unitProp.unitWidth, LeftTop.y + 2*j*unitProp.unitWidth);
		}
	}
//*/

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

void PainterClass::DrawMatrix(HDC memDC) {
	for (int i = 0; i < v.size(); i++) {
		for (int j = 0; j < v[i].size(); j++) {
			if (liveOrDead[i][j]) {
				DrawTestUnit(memDC, LeftTop.x + v[i][j].x, LeftTop.y + v[i][j].y);
			}
		}
	}
}