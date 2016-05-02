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

class PainterClass {
public:
	void Init(HWND handle);
	void Draw();
	void DrawTestUnit(HDC memDC, int xPos, int yPos);
	void SetWidth(int newWidth);
	void SetHeight(int newHeight);
	void SetLeftTop(int left, int top);
	void InitMatrix();
	void DrawMatrix(HDC memDC);

private:
	HWND handle;
	int width;
	int height;
	Point LeftTop;

	UnitProperties unitProp;
	vector < vector <Point> > v;
	vector < vector<int> > liveOrDead;
};