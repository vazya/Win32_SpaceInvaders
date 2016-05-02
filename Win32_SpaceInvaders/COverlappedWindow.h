#include "PainterClass.h"
#include <Windows.h>

class COverlappedWindow {
public:
	COverlappedWindow();
	~COverlappedWindow();

	static bool RegisterClass(HINSTANCE instance);
	bool Create(HINSTANCE instance);
	void Show(int cmdShow);

protected:
	void OnCreate(HWND handle);
	void OnSize();
	void OnDraw();
	void OnTimer();
	void OnDestroy();

private:
	HWND handle;
	HINSTANCE hInst;

	PainterClass painter;

	static LRESULT __stdcall windowProc(HWND handle, UINT message, WPARAM wParam, LPARAM lParam);
};
