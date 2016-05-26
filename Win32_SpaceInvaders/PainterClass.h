#pragma once
#include <Windows.h>
#include <vector>
using namespace std;

struct UnitProperties {
	int unitWidth;
	int unitHeight;

};

struct Unit {
	int xPos;
	int yPos;
	int status;
	/*  
		0 - нет = умер / не должно быть
		1 - жив, готов к вылету
		2 - на вылете
	*/
	Unit() : xPos(0), yPos(0), status(0) {};
	Unit(int x, int y, int s) : xPos(x), yPos(y), status(s) {};
};

struct MatrixProperties {
	int left;
	int right;
	int top;

	int leftShift;
	int rightShift;
	int topShift;
	int botShift;
	int hWidth;
	int hHeight;
	int length;

	int curTime;
	int maxTime;

	int matrixMove;
};

struct Ship {
	int x;
	int y;
	int speed;
	int width;
	int height;
	bool moveLeft;
	bool moveRight;
	Ship() : x(0), y(0), speed(0), width(0), height(0), moveLeft(false), moveRight(false) {};
	Ship(int newX, int newY, int newSpeed, int newWidth, int newHeight) :
		x(newX), y(newY), speed(newSpeed), width(newWidth), height(newHeight), moveLeft(false), moveRight(false) {};
};

struct Bullet {
	int x;
	int y;
	int speed;
	int width;
	int height;
	bool onShoot;
	Bullet() : x(0), y(0), speed(0), width(0), height(0), onShoot(false) {};
	Bullet(int xPos, int yPos, int newSpeed, int newWidth, int newHeight, bool flag) :
		x(xPos), y(yPos), speed(newSpeed), width(newWidth), height(newHeight), onShoot(flag) {};
};

class PainterClass {
public:
	void Init(HWND handle, HINSTANCE hInst);
	void InitMatrix();
	void InitShip();
	void InitBullet();

	void SetWidth(int newWidth);
	void SetHeight(int newHeight);
	void SetLeft(int newLeft);
	void SetRight(int newRight);
	void SetTop(int newTop);
	void SetLeftMove();
	void SetRightMove();
	void SetStopMove();
	void SetShipShoot(bool status);

	void OnTime();
	void OnSize();

	void Draw();
	void DrawMatrix(HDC memDC);
	void DrawTestUnit(HDC memDC, int xPos, int yPos);
	void DrawShip(HDC memDC);

	void DrawShipBullet(HDC memDC);
//	void DrawInvader1(HDC memDC);
//	void DrawInvader2(HDC memDC);

	void DrawBitmap(HDC hDC, int x, int y, HBITMAP hBtmp);

	void MoveMatrix();
	void MoveShip();
	void MoveShipShoot();

	void CheckDeath();

private:
	HWND handle;
	HINSTANCE hInstance;
	HBITMAP hBitmapShip;
	HBITMAP hBitmapInvader;
	HBITMAP hBitmapBullet;
	BITMAP bitmapShip;
	BITMAP bitmapInvader;
	BITMAP bitmapBullet;

	int width;					// свойства клиентской области игры
	int height;					// свойства клиентской области игры

	int curTime;

	MatrixProperties matrixProp;

	Ship ship;
	Bullet bullet;

	UnitProperties unitProp;	// здесь храним свойства злых инвейдеров 
								// т.к. они одинаковые и их много
	vector < vector <Unit> > v;	// сами инвейдеры и их состояние и позиция
};