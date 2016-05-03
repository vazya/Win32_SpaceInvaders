#include <Windows.h>
#include "COverlappedWindow.h"
#include <iostream>

int initialize(COverlappedWindow *cWindow, const HINSTANCE& instance, int cmdShow) {
	if (!cWindow->RegisterClass(instance)) {
		return -1;
	}
	if (!cWindow->Create(instance)) {
		return -1;
	}
	cWindow->Show(cmdShow);

	return 0;
}

int startLoop() {
	MSG msg;
	BOOL msgResult = 0;
	while ((msgResult = ::GetMessage(&msg, 0, 0, 0)) != 0) {
		if (msgResult == -1) {
			return -1;
		}

		::TranslateMessage(&msg);
		::DispatchMessage(&msg);
	}
	return 0;
}

int WINAPI wWinMain( HINSTANCE instance, HINSTANCE prevInstance, LPWSTR commandLine, int cmdShow) {
	COverlappedWindow cWindow;
	int init_status = initialize(&cWindow, instance, cmdShow);
	if (init_status != 0) {
		return init_status;
	}

	int loop_status = startLoop();
	if (loop_status != 0) {
		return loop_status;
	}
	return 0;
}
