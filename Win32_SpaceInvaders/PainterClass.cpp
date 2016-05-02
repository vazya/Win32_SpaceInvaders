#include "PainterClass.h"
#include <Windows.h>

void PainterClass::Init(HWND _handle) {
	handle = _handle;

	InitMatrix();
}

void PainterClass::SetWidth(int newWidth) {
	width = newWidth;
}

void PainterClass::SetHeight(int newHeight) {
	height = newHeight;
}

void PainterClass::SetLeft(int newLeft) {
	matrixProp.left = newLeft;
}

void PainterClass::SetTop(int newTop) {
	matrixProp.top = newTop;
}

void PainterClass::InitMatrix() {
	/*
	Здесь задается матрица относительного расположения юнитов, т.е.
	отрисовка будет вестись как LeftTop.x + v[i][j].x и LeftTop.y + v[i][j].y
	и менять мы будем с теченеием времени только LeftTop
	*/
	RECT rect;
	::GetClientRect(handle, &rect);
	SetLeft(rect.left);
	SetTop(rect.top);
	SetWidth(rect.right - rect.left);
	SetHeight(rect.bottom - rect.top);

	int m = 5;										// количество строк
	v.resize(m);
	int n = 10;										// количество столбцов
	for (int i = 0; i < m; ++i) {
		v[i].resize(n);
	}

	// тут мы захардкодим границы нашей матрицы
	int leftShift = matrixProp.leftShift = (int)(0.2*(double)(width));
	int rightShift = matrixProp.rightShift = (int)(0.8*(double)(width));
	int topShift = matrixProp.topShift = (int)(0.1*(double)(height));
	int botShift = matrixProp.botShift = (int)(0.5*(double)(height));

	int hWidth = matrixProp.hWidth = int((double)(rightShift - leftShift) / (2 * n));
	int hHeight = matrixProp.hHeight = int((double)(botShift - topShift) / (2 * m));

	for (int i = 0; i < m; i++) {
		for (int j = 0; j < n; j++) {
			v[i][j] = Unit(leftShift + 2*j*hWidth, topShift + 2*i*hHeight, 1);
		}
	}
	unitProp.unitWidth = hWidth;
	unitProp.unitHeight = hHeight;
	
	matrixProp.curTime = 0;
	matrixProp.maxTime = 100;
}


void PainterClass::Draw() {
	PAINTSTRUCT ps;
	HDC dc = ::BeginPaint(handle, &ps);
	HDC memDC = ::CreateCompatibleDC(dc);

	HBITMAP memBitmap = ::CreateCompatibleBitmap(dc, width, height);
	HGDIOBJ oldBitmap = ::SelectObject(memDC, memBitmap);

	DrawMatrix(memDC);

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
			if (v[i][j].status == 1) {
				DrawTestUnit(memDC, matrixProp.left + v[i][j].xPos, matrixProp.top + v[i][j].yPos);
			}
		}
	}
}

void PainterClass::Move() {
	matrixProp.curTime++;
	if (matrixProp.curTime >= matrixProp.maxTime) {

	}
}