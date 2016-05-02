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

private:
	HWND handle;
	int width;
	int height;
	Point topLeft;

	UnitProperties unitProp;
	vector <Point> v;
	vector <int> liveOrDead;
};