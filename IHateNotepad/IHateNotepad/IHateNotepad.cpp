#include<windows.h>
// #include "resource.h"
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
HINSTANCE g_hInst;
LPCTSTR lpszClass = TEXT("I hate NotePad");
HWND hWndMain;

int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpszCmdParam, int nCmdShow)
{
	HWND hWnd;
	MSG Message;
	WNDCLASS WndClass;
	g_hInst = hInstance;

	WndClass.cbClsExtra = 0;
	WndClass.cbWndExtra = 0;
	WndClass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	WndClass.hCursor = LoadCursor(NULL, IDC_ARROW);
	WndClass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	WndClass.hInstance = hInstance;
	WndClass.lpfnWndProc = WndProc;
	WndClass.lpszMenuName = NULL;
	WndClass.lpszClassName = lpszClass;
	WndClass.style = CS_HREDRAW | CS_VREDRAW | CS_NOCLOSE;
	RegisterClass(&WndClass);

	hWnd = CreateWindow(lpszClass, lpszClass, WS_POPUP, 10, 10, 200, 100, NULL, (HMENU)NULL, hInstance, NULL);
	ShowWindow(hWnd, nCmdShow);

	while (GetMessage(&Message, NULL, 0, 0))
	{
		TranslateMessage(&Message);
		DispatchMessage(&Message);
	}
	return (int)Message.wParam;
}


LRESULT CALLBACK WndProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam)
{
	HDC hdc;
	PAINTSTRUCT ps;
	HWND hNote;
	const TCHAR* Mes = TEXT("I hate NotePad!");
	UINT nHit;
	static int i = 0;
	static int ExitFlag[5] = { 0,0,0,0,0 };

	switch (iMessage)
	{
	case WM_CREATE:
		hWndMain = hWnd;
		SetTimer(hWnd, 1, 1000, NULL);
		return 0;
	case WM_TIMER:
		switch (wParam)
		{
		case 1:
			hNote = FindWindow(TEXT("NotePad"), NULL);
			if (hNote)
			{
				SendMessage(hNote, WM_CLOSE, 0, 0);
			}
			break;
		case 2:
			for (int j = 0; j < 5; j++)
			{
				ExitFlag[j] = 10;
			}
			i = 0;
			KillTimer(hWnd, 2);
		}
		return 0;
	case WM_NCHITTEST:
		nHit = DefWindowProc(hWnd, iMessage, wParam, lParam);
		if (nHit == HTCLIENT)
			nHit = HTCAPTION;
		return nHit;
	case WM_KEYDOWN:
		switch (wParam)
		{
		case VK_SPACE:
			SetTimer(hWnd, 2, 5000, NULL);
			ExitFlag[i] = 0;
			i++;
			break;
		case VK_LEFT:
			ExitFlag[i] = 1;
			i++;
			break;
		case VK_RIGHT:
			ExitFlag[i] = 2;
			i++;
			break;
		case VK_UP:
			ExitFlag[i] = 3;
			i++;
			break;
		case VK_DOWN:
			ExitFlag[i] = 4;
			i++;
			for (int j = 0; j < 5; j++)
			{
				if (ExitFlag[j] != j)
					break;
				else if (j == 4)
				{
					KillTimer(hWnd, 1);
					KillTimer(hWnd, 2);
					DestroyWindow(hWnd);
				}
			}
			break;
		}
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		TextOut(hdc, 10, 10, Mes, lstrlen(Mes));
		EndPaint(hWnd, &ps);
		return 0;
	case WM_DESTROY:
		KillTimer(hWnd, 1);
		KillTimer(hWnd, 2);
		PostQuitMessage(0);
		return 0;
	}
	return(DefWindowProc(hWnd, iMessage, wParam, lParam));
}