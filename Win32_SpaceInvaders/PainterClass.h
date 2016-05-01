#pragma once
#include <Windows.h>
#include <vector>
using namespace std;

struct point {
	int x;
	int y;
};

class PainterClass {
public:
	void Draw(HWND handle);
private:
	vector <point> v;
	vector <bool> liveOrDead;
};