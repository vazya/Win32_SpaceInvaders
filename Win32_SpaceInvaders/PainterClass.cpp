#include "PainterClass.h"
#include <Windows.h>

void PainterClass::Init(HWND _handle) {
	handle = _handle;
	curTime = 0;
//	matrixCurTime = 0;
//	_maxTime = 2;
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

	SetWidth(rect.right - rect.left);
	SetHeight(rect.bottom - rect.top);

	matrixProp.top = rect.top;
	int lShift = matrixProp.leftShift = (int)(0.1*(double)(width));
	int rShift = matrixProp.rightShift = (int)(0.9*(double)(width));

	int left = matrixProp.left = (int)(0.15*(double)(width));
	int right = matrixProp.right = (int)(0.65*(double)(width));
	int top = matrixProp.topShift = (int)(0.1*(double)(height));
	int bot = matrixProp.botShift = (int)(0.5*(double)(height));

	int m = 5;										// количество строк
	v.resize(m);
	int n = 10;										// количество столбцов
	for (int i = 0; i < m; ++i) {
		v[i].resize(n);
	}

	int hWidth = matrixProp.hWidth = int((double)(right - left) / (2 * n));
	int hHeight = matrixProp.hHeight = int((double)(bot - top) / (2 * m));
	unitProp.unitWidth = hWidth;
	unitProp.unitHeight = hHeight;
	int matrixMove = matrixProp.matrixMove = (int)(0.5*(double)(hWidth));

	for (int i = 0; i < m; i++) {
		for (int j = 0; j < n; j++) {
			v[i][j] = Unit(left + j*(hWidth + matrixMove), top + 2*i*hHeight, 1);
		}
	}
	unitProp.unitWidth = hWidth;
	unitProp.unitHeight = hHeight;
	
	matrixProp.maxTime = 2;
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
//				DrawTestUnit(memDC, _left + v[i][j].xPos, _top + v[i][j].yPos);
			}
		}
	}
}

void PainterClass::MoveMatrix() {
	if (matrixProp.curTime >= matrixProp.maxTime) {
		if (matrixProp.right >= matrixProp.rightShift) {
			matrixProp.matrixMove = -matrixProp.matrixMove;
		}
		if (matrixProp.left <= matrixProp.leftShift - abs(matrixProp.matrixMove)) {
			matrixProp.matrixMove = -matrixProp.matrixMove;
		}

		matrixProp.left += matrixProp.matrixMove;
		matrixProp.right += matrixProp.matrixMove;
		matrixProp.curTime = 0;
		matrixProp.maxTime = 2; // 50 + (rand() % 45);			// еще немного захардкодим время движения матрицы
	}
}

/*
void PainterClass::MoveMatrix2() {
	if (matrixCurTime >= _maxTime) {
		if (_left >= _rightShift) {
			_matrixMove = -_matrixMove;
		}
		if (_left < _leftShift) {
			_matrixMove = -_matrixMove;
		}

		_left += _matrixMove;
		matrixCurTime = 0;
		_maxTime = 2; // 50 + (rand() % 45);			// еще немного захардкодим время движения матрицы
	}
}
//*/

void PainterClass::OnTime() {
	matrixProp.curTime++;
	curTime++;
}