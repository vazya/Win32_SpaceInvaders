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

class PainterClass {
public:
	void Init(HWND handle);
	void Draw();
	void DrawTestUnit(HDC memDC, int xPos, int yPos);
	void SetWidth(int newWidth);
	void SetHeight(int newHeight);
	void SetLeft(int newLeft);
	void SetRight(int newRight);
	void SetTop(int newTop);
	void InitMatrix();
	void DrawMatrix(HDC memDC);
	void MoveMatrix();
	void OnTime();
	void OnSize();

private:
	HWND handle;

	int width;					// свойства клиентской области игры
	int height;					// свойства клиентской области игры

	int curTime;

	MatrixProperties matrixProp;

	UnitProperties unitProp;	// здесь храним свойства злых инвейдеров 
								// т.к. они одинаковые и их много
	vector < vector <Unit> > v;	// сами инвейдеры и их состояние и позиция
};