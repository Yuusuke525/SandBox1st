#include "GameDef.h"
#include "Mode.h"


LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    switch (msg) {
        case WM_DESTROY:
            PostQuitMessage(NULL);
            break;
        default:
            return (DefWindowProc(hWnd, msg, wParam, lParam));
    }
    return (0);
}

int APIENTRY _tWinMain
(HINSTANCE hInstance, HINSTANCE hPrevInstance,
 LPTSTR lpszCmdLine, int nCmdShow)
{
	timeBeginPeriod(1);

	WNDCLASS wc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	wc.hCursor = LoadCursor(hInstance, IDC_ARROW);
	wc.hIcon = NULL;
	wc.hInstance = hInstance;
	wc.lpfnWndProc = WndProc;
	wc.lpszClassName = _T("ShootingGame");
	wc.lpszMenuName = NULL;
	wc.style = NULL;
	
	if(RegisterClass(&wc) == NULL){
		return 0;
	}

    HWND mainWnd = CreateWindow(
		wc.lpszClassName, _T("ShootingGame"),
		WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX,
        CW_USEDEFAULT, CW_USEDEFAULT,
		640, 480, NULL, NULL, hInstance, NULL );

	RECT r1, r2;
	GetClientRect(mainWnd, &r1);
	GetWindowRect(mainWnd, &r2);
	MoveWindow(mainWnd, r2.left, r2.top,
		640 + ((r2.right - r2.left) - (r1.right - r1.left)),
		480 + ((r2.bottom - r2.top) - (r1.bottom - r1.top)), FALSE);

	CGameObject game;
	CoInitializeEx(NULL, COINIT_MULTITHREADED);
	game.Initialize(mainWnd, hInstance);
	CSound::CreateDirectSound(mainWnd);

	game.AppendObject(new CTaskHead(), 0, true);
	game.AppendObject(new CTaskTail(), INT_MAX, true);

	game.AppendObject(new CTitle(), 1, true);

	ShowWindow(mainWnd, nCmdShow);

	MSG msg;
	while(TRUE){
		if(PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)){
			if(msg.message == WM_QUIT) break;

			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}else{
			game.DoAllTasks();
		}
	}

	game.Uninitialize();
	CSound::ReleaseDirectSound();
	CoUninitialize();

	return 0;
}
