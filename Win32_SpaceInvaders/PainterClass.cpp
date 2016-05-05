#include "PainterClass.h"
#include "resource.h"
#include <Windows.h>

void PainterClass::Init(HWND _handle, HINSTANCE hInst) {
	handle = _handle;
	hInstance = hInst;
	curTime = 0;

	InitMatrix();
	InitShip();
	InitBullet();
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

void PainterClass::SetLeftMove() {
	ship.moveLeft = true;
	ship.moveRight = false;
	MoveShip();
}

void PainterClass::SetRightMove() {
	ship.moveLeft = false;
	ship.moveRight = true;
	MoveShip();
}

void PainterClass::SetStopMove() {
	ship.moveLeft = false;
	ship.moveRight = false;
}

void PainterClass::SetShipShoot(bool status) {
	bullet.onShoot = status;
	MoveShipShoot();
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

void PainterClass::InitShip() {
	ship.x = (int)((double)(width) / 2);
	ship.width = (int)(0.06*(double)(width));
	ship.y = (int)(0.9*(double)(height));
	ship.height = (int)(0.1*(double)(height));
	ship.speed = (int)(0.15*(double)(ship.width));
}

void PainterClass::InitBullet() {
	bullet.x = ship.x + (ship.width - bullet.width) / 2;
	bullet.y = ship.y - bullet.height;
	bullet.width = 5;
	bullet.height = 10;
	bullet.speed = (int)(0.6*(double)(ship.speed));
	bullet.onShoot = false;
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
	DrawShip(memDC);
	DrawShipBullet(memDC);
//	DrawShipBitmap(dc, memDC);

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

void PainterClass::MoveShip() {
	if (ship.moveLeft) {
		if (ship.x > 0.3*ship.width) {
			ship.x -= ship.speed;
		}
	}
	if (ship.moveRight) {
		if (ship.x < width - 1.3*ship.width) {
			ship.x += ship.speed;
		}
	}
	if (!bullet.onShoot) {
		MoveShipShoot();
	}
	Draw();
}

void PainterClass::MoveShipShoot() {
	if (bullet.onShoot) {
		bullet.y -= bullet.speed;
		if (bullet.y < 0) {
			bullet.onShoot = false;
		}
	} 
	if(!bullet.onShoot) {
		bullet.x = ship.x + (ship.width - bullet.width) / 2;
		bullet.y = ship.y - bullet.height;
	}
}

void PainterClass::OnTime() {
	matrixProp.curTime++;
	curTime++;
}

void PainterClass::DrawShip(HDC memDC) {
	int r = rand() % 256; int g = rand() % 256; int b = rand() % 256;
	HBRUSH mybrush = ::CreateSolidBrush(RGB(r, g, b));

	::SelectObject(memDC, mybrush);
	::Ellipse(memDC, ship.x, ship.y, ship.x + ship.width, ship.y + ship.height);
	::DeleteObject(mybrush);
}

void PainterClass::DrawShipBullet(HDC memDC) {
	MoveShipShoot();
	int r = rand() % 256; int g = rand() % 256; int b = rand() % 256;
	HBRUSH mybrush = ::CreateSolidBrush(RGB(r, g, b));

	::SelectObject(memDC, mybrush);
	::Ellipse(memDC, bullet.x, bullet.y, bullet.x + bullet.width, bullet.y + bullet.height);
	::DeleteObject(mybrush);
}

void PainterClass::DrawShipBitmap(HDC dc, HDC memDC) {
	HINSTANCE hInst = GetModuleHandle(NULL);
	int width = ship.width;
	int height = ship.height;
	HBITMAP hBitmap = (HBITMAP)LoadImage(hInst, L"ship.bmp", IMAGE_BITMAP, width, height, LR_LOADFROMFILE);
	SelectObject(memDC, hBitmap);

	::BitBlt(dc, 0, 0, width, height, memDC, 0, 0, SRCCOPY);
}