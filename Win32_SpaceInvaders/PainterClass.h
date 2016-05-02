#pragma once
#include <Windows.h>
#include <vector>
using namespace std;

struct Point {
	int x;
	int y;
	Point() : x(0), y(0) {};
	Point(int _x, int _y) : x(_x), y(_y) {};
};

struct UnitProperties {
	int unitWidth;
	int unitHeight;

};

struct Unit {
	int xPos;
	int yPos;
	int status;
	/*  
		0 - ��� = ���� / �� ������ ����
		1 - ���, ����� � ������
		2 - �� ������
	*/
	Unit() : xPos(0), yPos(0), status(0) {};
	Unit(int x, int y, int s) : xPos(x), yPos(y), status(s) {};
};

struct MatrixProperties {
	int left;
	int top;

	int leftShift;
	int rightShift;
	int topShift;
	int botShift;
	int hWidth;
	int hHeight;

	int curTime;
	int maxTime;
};

class PainterClass {
public:
	void Init(HWND handle);
	void Draw();
	void DrawTestUnit(HDC memDC, int xPos, int yPos);
	void SetWidth(int newWidth);
	void SetHeight(int newHeight);
	void SetLeft(int newLeft);
	void SetTop(int newTop);
	void InitMatrix();
	void DrawMatrix(HDC memDC);
	void Move();

private:
	HWND handle;

	// ��� ����� ����� ������ ����� ������ ���������
	int width;					// �������� ���������� ������� ����
	int height;					// �������� ���������� ������� ����

	MatrixProperties matrixProp;

	UnitProperties unitProp;	// ����� ������ �������� ���� ���������� 
								// �.�. ��� ���������� � �� �����
	vector < vector <Unit> > v;	// ���� ��������� � �� ��������� � �������


};