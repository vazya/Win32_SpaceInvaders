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
}

void PainterClass::SetRightMove() {
	ship.moveLeft = false;
	ship.moveRight = true;
}

void PainterClass::SetStopMove() {
	ship.moveLeft = false;
	ship.moveRight = false;
}

void PainterClass::SetShipShoot(bool status) {
//	bool leftMove = ship.moveLeft;
//	bool rightMove = ship.moveRight;
//	ship.moveLeft = false;
//	ship.moveRight = false;
	bullet.onShoot = status;
	MoveShipShoot();
//	ship.moveLeft = leftMove;
//	ship.moveRight = rightMove;
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

	HINSTANCE hInst = GetModuleHandle(NULL);
	hBitmapInvader = (HBITMAP)LoadImage(hInst, L"invader.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
	GetObject(hBitmapInvader, sizeof(BITMAP), &bitmapInvader);//Получение информации о битмэпе
	unitProp.unitWidth = bitmapInvader.bmWidth;
	unitProp.unitHeight = bitmapInvader.bmHeight;

	matrixProp.top = matrixProp.topShift = top + (int)(0.1*(double)(height));
	matrixProp.botShift = top + (int)(0.5*(double)(height));
//	int hHeight = matrixProp.hHeight = int((double)(matrixProp.botShift - matrixProp.topShift) / (2 * m));
	int hHeight = matrixProp.hHeight = bitmapInvader.bmHeight;

	matrixProp.leftShift = left + (int)(0.1*(double)(width));
	matrixProp.rightShift = left + (int)(1.01*(double)(width));
//	matrixProp.rightShift = left + (int)(0.95*(double)(width));
	int length = matrixProp.length = (int)(0.7*(double)(matrixProp.rightShift - matrixProp.leftShift));

	matrixProp.left = matrixProp.leftShift + (int)(0.2*(double)(length));
	matrixProp.right = matrixProp.left + length;
//	int hWidth = matrixProp.hWidth = int((double)(length) / (2 * n));
	int hWidth = matrixProp.hWidth = bitmapInvader.bmWidth;
	int matrixMove = matrixProp.matrixMove = (int)(0.75*(double)(matrixProp.hWidth));

	for (int i = 0; i < m; i++) {
		for (int j = 0; j < n; j++) {
//			v[i][j] = Unit(j*(hWidth + matrixMove),2*i*hHeight, 1);
			v[i][j] = Unit(j*unitProp.unitWidth, 2*i*unitProp.unitHeight, 1);
		}
	}

//	unitProp.unitWidth = matrixProp.hWidth;
//	unitProp.unitHeight = matrixProp.hHeight;
	matrixProp.maxTime = 50;
/*
	HINSTANCE hInst = GetModuleHandle(NULL);
	hBitmapInvader = (HBITMAP)LoadImage(hInst, L"invader.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
	BitmapInvader;
	GetObject(hBitmapInvader, sizeof(BITMAP), &BitmapInvader);//Получение информации о битмэпе
	unitProp.unitWidth = BitmapInvader.bmWidth;
	unitProp.unitHeight = BitmapInvader.bmHeight;
*/
}

void PainterClass::InitShip() {
	HINSTANCE hInst = GetModuleHandle(NULL);
	hBitmapShip = (HBITMAP)LoadImage(hInst, L"ship2.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
	GetObject(hBitmapShip, sizeof(BITMAP), &bitmapShip);//Получение информации о битмэпе

	ship.x = (int)((double)(width) / 2);
//	ship.width = (int)(0.06*(double)(width));
	ship.width = bitmapShip.bmWidth;
	ship.y = (int)(0.9*(double)(height));
//	ship.height = (int)(0.1*(double)(height));
	ship.height = bitmapShip.bmHeight;
	ship.speed = (int)(0.062*(double)(ship.width));
}

void PainterClass::InitBullet() {
	bullet.x = ship.x + (ship.width - bullet.width) / 2;
	bullet.y = ship.y - bullet.height;
	bullet.width = 5;
	bullet.height = 18;
	bullet.speed = (int)(2*(double)(ship.speed));
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
/*
	PAINTSTRUCT ps;
	HDC dc = ::BeginPaint(handle, &ps);
	HDC memDC = ::CreateCompatibleDC(dc);

	HBITMAP memBitmap = ::CreateCompatibleBitmap(dc, width, height);
	HGDIOBJ oldBitmap = ::SelectObject(memDC, memBitmap);

//	DrawMatrix(memDC);
//	DrawShipBullet(memDC);
////////	DrawShipBitmap2(dc, memDC);
//	SelectObject(memDC, memBitmap);
//	::BitBlt(dc, 0, 0, width, height, memDC, 0, 0, PATCOPY);
	HANDLE hOldBitmap1 = SelectObject(memDC, hBitmapInvader);
	MoveMatrix();
	for (int i = 0; i < v.size(); i++) {
		for (int j = 0; j < v[i].size(); j++) {
			if (v[i][j].status == 1) {
				StretchBlt(dc, matrixProp.left + v[i][j].xPos, matrixProp.top + v[i][j].yPos, 
					unitProp.unitWidth, unitProp.unitHeight, memDC, 0, 0,
					BitmapInvader.bmWidth, BitmapInvader.bmHeight, SRCCOPY);
			}
		}
	}
	SelectObject(memDC, hOldBitmap1);
	MoveShip();
	HANDLE hOldBitmap = SelectObject(memDC, hBitmap);
	StretchBlt(dc,ship.x,ship.y,ship.width,ship.height,memDC,0, 0,
		Bitmap.bmWidth,Bitmap.bmHeight, SRCCOPY);
//	StretchBlt(dc, 1.5*ship.x, ship.y, ship.width, ship.height, memDC, 0, 0,
//		Bitmap.bmWidth, Bitmap.bmHeight, SRCCOPY);
	SelectObject(memDC, memBitmap);
//	DrawMatrix(memDC);
//	DrawShipBullet(memDC);
//	SelectObject(memDC, hOldBitmap);
//	DrawMatrix(memDC);
//	DrawShipBullet(memDC);
	::BitBlt(dc, 0, 0, width, height, memDC, 0, 0, SRCCOPY);


	::DeleteObject(memBitmap);
	::DeleteObject(oldBitmap);
	::DeleteDC(dc);
	::DeleteDC(memDC);

	EndPaint(handle, &ps);
//*/
/*
	PAINTSTRUCT ps;
	HDC dc = ::BeginPaint(handle, &ps);
	HDC memDC = ::CreateCompatibleDC(dc);
	HDC hCompatibleDC = ::CreateCompatibleDC(memDC);

	HBITMAP memBitmap = ::CreateCompatibleBitmap(dc, width, height);
	HGDIOBJ oldBitmap = ::SelectObject(memDC, memBitmap);


	MoveShip();
	HANDLE hOldBitmap = SelectObject(memDC, hBitmap);
	StretchBlt(dc, ship.x, ship.y, ship.width, ship.height, memDC, 0, 0,
		Bitmap.bmWidth, Bitmap.bmHeight, SRCCOPY);
	StretchBlt(dc, 1.5*ship.x, ship.y, ship.width, ship.height, memDC, 0, 0,
		Bitmap.bmWidth, Bitmap.bmHeight, SRCCOPY);
	SelectObject(memDC, oldBitmap);
	::DeleteObject(memBitmap);

	::DeleteDC(memDC);
	::DeleteDC(dc);

	EndPaint(handle, &ps);
*/

	PAINTSTRUCT ps;
	HDC hdc = BeginPaint(handle, &ps);
	HDC hdcMem = CreateCompatibleDC(hdc);


	HBITMAP memBitmap = ::CreateCompatibleBitmap(hdc, width, height);
	HBITMAP hbmOld = reinterpret_cast<HBITMAP>(SelectObject(hdcMem, memBitmap));

	//	HBITMAP hbmOld = reinterpret_cast<HBITMAP>(SelectObject(hdcMem, hBtmp));


	RECT rect;
	GetClientRect(handle, &rect);
	HBRUSH brush = (HBRUSH)CreateSolidBrush(RGB(0, 0, 0));
	FillRect(hdcMem, &rect, brush);
	DeleteObject(brush);

	MoveMatrix();
	for (int i = 0; i < v.size(); i++) {
		for (int j = 0; j < v[i].size(); j++) {
			if (v[i][j].status == 1) {
				DrawBitmap(hdcMem, matrixProp.left + v[i][j].xPos, matrixProp.top + 1.1*v[i][j].yPos, hBitmapInvader);
			}
		}
	}

	MoveShip();
	DrawBitmap(hdcMem, ship.x, ship.y, hBitmapShip);
	DrawShipBullet(hdcMem);

	BitBlt(hdc, 0, 0, width, height, hdcMem, 0, 0, SRCCOPY);

	SelectObject(hdcMem, hbmOld);
	DeleteObject(memBitmap);
	DeleteObject(hbmOld);
	DeleteDC(hdcMem);
	DeleteDC(hdc);
	EndPaint(handle, &ps);
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
		matrixProp.maxTime = 50 + (rand() % 45);			// еще немного захардкодим время движения матрицы
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
	CheckDeath();
}

void PainterClass::CheckDeath() {
	int m = v.size();										// количество строк
	int n = v[0].size();									// количество столбцов

	for (int i = 0; i < m; i++) {
		for (int j = 0; j < n; j++) {
			int bulletLeft = bullet.x;
			int bulletTop = bullet.y;
			int bulletRight = bullet.x + bullet.width;
			int bulletBot = bullet.y + bullet.height;
			int bulletCentrX = (bulletLeft + bulletRight) / 2;
			int bulletCentrY = (bulletTop + bulletBot) / 2;

			if (v[i][j].status == 1) {
				if (bulletTop >= matrixProp.top + v[i][j].yPos && bulletTop <= matrixProp.top + v[i][j].yPos + unitProp.unitHeight) {
					if (bulletCentrX >= matrixProp.left + v[i][j].xPos && bulletCentrX < matrixProp.left + v[i][j].xPos + unitProp.unitWidth) {
						v[i][j].status = 0;
						SetShipShoot(false);
					}
				}
			}
		}
	}
}

void PainterClass::OnTime() {
	matrixProp.curTime++;
	curTime++;
}

void PainterClass::DrawShip(HDC memDC) {
	MoveShip();
	int r = rand() % 256; int g = rand() % 256; int b = rand() % 256;
	HBRUSH mybrush = ::CreateSolidBrush(RGB(r, g, b));

	::SelectObject(memDC, mybrush);
	::Ellipse(memDC, ship.x, ship.y, ship.x + ship.width, ship.y + ship.height);
	::DeleteObject(mybrush);
}

void PainterClass::DrawShipBullet(HDC memDC) {
	MoveShipShoot();
//	int r = rand() % 256; int g = rand() % 256; int b = rand() % 256;
	HBRUSH mybrush = ::CreateSolidBrush(RGB(255, 0, 0));

	::SelectObject(memDC, mybrush);
	::Ellipse(memDC, bullet.x + 1, bullet.y, bullet.x + bullet.width, bullet.y + bullet.height + 1);
	::DeleteObject(mybrush);
}

void PainterClass::DrawShipBitmap(HDC dc, HDC memDC, HBITMAP hBitmap) {
	HINSTANCE hInst = GetModuleHandle(NULL);
	int shipWidth = ship.width;
	int shipHeight = ship.height;
	hBitmap = (HBITMAP)LoadImage(hInst, L"ship.bmp", IMAGE_BITMAP, shipWidth, shipHeight, LR_LOADFROMFILE);
	SelectObject(memDC, hBitmap);

	::BitBlt(dc, 0, 0, shipWidth, shipHeight, memDC, 0, 0, SRCCOPY);
}

void PainterClass::DrawShipBitmap2(HDC hDC, HDC hCompatibleDC) {
	MoveShip();
	HANDLE hOldBitmap = SelectObject(hCompatibleDC/*Хендл совместимого контекста, в котором размещается текущий элемент*/,
		hBitmapShip/*Хендл элемента, которым замещается текущий элемент*/);//Функция, возвращающая хендл замещённого элемента

	StretchBlt(
		hDC/*Хендл окна*/,
		ship.x/*Смещение по оси x*/,
		ship.y/*Смещение по оси y*/,
		ship.width/*Ширина прямоугольника*/,
		ship.height/*Высота прямоугольника*/,
		hCompatibleDC/*Хендл совместимого контекста*/,
		0, 0,
		bitmapShip.bmWidth/*Ширина отображаемой части*/,
		bitmapShip.bmHeight/*Высота отображаемой части*/,
		SRCCOPY);

	SelectObject(hCompatibleDC, hOldBitmap);
	BitBlt(hDC, 0, 0, width, height, hCompatibleDC, 0, 0, SRCCOPY);
}

void PainterClass::DrawBitmap(HDC hDC, int x, int y, HBITMAP hBtmp) {
	BITMAP bm;

	HDC hdcMem = CreateCompatibleDC(hDC);
	HBITMAP hbmOld = reinterpret_cast<HBITMAP>(SelectObject(hdcMem, hBtmp));

	GetObject(hBtmp, sizeof(bm), &bm);

	BitBlt(hDC, x, y, bm.bmWidth, bm.bmHeight, hdcMem, 0, 0, SRCCOPY);

	SelectObject(hdcMem, hbmOld);
	DeleteDC(hdcMem);
}
