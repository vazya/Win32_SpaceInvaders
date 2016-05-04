#include "PainterClass.h"
#include <Windows.h>

void PainterClass::Init(HWND _handle) {
	handle = _handle;
	curTime = 0;

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

void PainterClass::SetRight(int newRight) {
	matrixProp.right = newRight;
}

void PainterClass::SetTop(int newTop) {
	matrixProp.top = newTop;
}

void PainterClass::InitMatrix() {
	RECT rect;
	::GetClientRect(handle, &rect);
	// тут мы захардкодим границы нашей матрицы

	int left = rect.left;
	int right = rect.right;
	int top = rect.top;
	int bot = rect.bottom;

	int width = right - left;
	int height = bot - top;
	SetWidth(width);
	SetHeight(height);

	int m = 5;										// количество строк
	v.resize(m);
	int n = 10;										// количество столбцов
	for (int i = 0; i < m; ++i) {
		v[i].resize(n);
	}

	matrixProp.top = matrixProp.topShift = top + (int)(0.1*(double)(height));
	matrixProp.botShift = top + (int)(0.5*(double)(height));
	int hHeight = matrixProp.hHeight = int((double)(matrixProp.botShift - matrixProp.topShift) / (2 * m));

	matrixProp.leftShift = left + (int)(0.1*(double)(width));
	matrixProp.rightShift = left + (int)(1.01*(double)(width));
//	matrixProp.rightShift = left + (int)(0.95*(double)(width));
	int length = matrixProp.length = (int)(0.7*(double)(matrixProp.rightShift - matrixProp.leftShift));

	matrixProp.left = matrixProp.leftShift + (int)(0.2*(double)(length));
	matrixProp.right = matrixProp.left + length;
	int hWidth = matrixProp.hWidth = int((double)(length) / (2 * n));
	int matrixMove = matrixProp.matrixMove = (int)(0.75*(double)(matrixProp.hWidth));

	for (int i = 0; i < m; i++) {
		for (int j = 0; j < n; j++) {
			v[i][j] = Unit(j*(hWidth + matrixMove),2*i*hHeight, 1);
		}
	}

	unitProp.unitWidth = matrixProp.hWidth;
	unitProp.unitHeight = matrixProp.hHeight;

	matrixProp.maxTime = 50;
}

void PainterClass::OnSize() {
	RECT rect;
	::GetClientRect(handle, &rect);
	// тут мы захардкодим границы нашей матрицы

	int left = rect.left;
	int right = rect.right;
	int top = rect.top;
	int bot = rect.bottom;

	int width = right - left;
	int height = bot - top;
	SetWidth(width);
	SetHeight(height);

	int m = v.size();										// количество строк
	int n = v[0].size();									// количество столбцов

	matrixProp.top = matrixProp.topShift = top + (int)(0.1*(double)(height));
	matrixProp.botShift = top + (int)(0.5*(double)(height));
	int hHeight = matrixProp.hHeight = int((double)(matrixProp.botShift - matrixProp.topShift) / (2 * m));

	matrixProp.leftShift = left + (int)(0.1*(double)(width));
	matrixProp.rightShift = left + (int)(1.01*(double)(width));
	//	matrixProp.rightShift = left + (int)(0.95*(double)(width));
	int length = matrixProp.length = (int)(0.7*(double)(matrixProp.rightShift - matrixProp.leftShift));

	if (matrixProp.right >= matrixProp.rightShift) {
		matrixProp.left = matrixProp.rightShift - length;
		matrixProp.matrixMove = -abs(matrixProp.matrixMove);
		matrixProp.right = matrixProp.left;
	}

	if (matrixProp.left <= matrixProp.leftShift) {
		matrixProp.left = matrixProp.leftShift;
		matrixProp.matrixMove = abs(matrixProp.matrixMove);
		matrixProp.right = matrixProp.left + length;
	}

	matrixProp.right = matrixProp.left + length;
	int hWidth = matrixProp.hWidth = int((double)(length) / (2 * n));
	int matrixMove = matrixProp.matrixMove = (int)(0.75*(double)(matrixProp.hWidth));

	for (int i = 0; i < m; i++) {
		for (int j = 0; j < n; j++) {
			v[i][j] = Unit(j*(hWidth + matrixMove), 2 * i*hHeight, 1);
		}
	}

	unitProp.unitWidth = matrixProp.hWidth;
	unitProp.unitHeight = matrixProp.hHeight;
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
	MoveMatrix();
	for (int i = 0; i < v.size(); i++) {
		for (int j = 0; j < v[i].size(); j++) {
			if (v[i][j].status == 1) {
				DrawTestUnit(memDC, matrixProp.left + v[i][j].xPos, matrixProp.top + v[i][j].yPos);
			}
		}
	}
}

void PainterClass::MoveMatrix() {
	if (matrixProp.curTime >= matrixProp.maxTime) {
		if (matrixProp.right >= matrixProp.rightShift) {// + abs(matrixProp.matrixMove)) {
			matrixProp.matrixMove = -matrixProp.matrixMove;
		}
		if (matrixProp.left <= matrixProp.leftShift) { // - abs(matrixProp.matrixMove)) {
			matrixProp.matrixMove = -matrixProp.matrixMove;
		}

		matrixProp.left += matrixProp.matrixMove;
		matrixProp.right += matrixProp.matrixMove;
		matrixProp.curTime = 0;
		matrixProp.maxTime = 50 + (rand() % 45);			// еще немного захардкодим врем€ движени€ матрицы
	}
}

void PainterClass::OnTime() {
	matrixProp.curTime++;
	curTime++;
}