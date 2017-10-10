#pragma comment(linker,"\"/manifestdependency:type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")

#include <windows.h>
#include "resource.h"

LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	static BOOL bCapture;
	static HBITMAP hBitmap1, hBitmap2;
	static HCURSOR hCursor;
	switch (msg)
	{
	case WM_CREATE:
		CreateWindow(TEXT("STATIC"), 0, WS_CHILD | WS_VISIBLE | SS_BITMAP | SS_NOTIFY, 10, 10, 42, 42, hWnd, (HMENU)1, ((LPCREATESTRUCT)(lParam))->hInstance, 0);
		CreateWindow(TEXT("STATIC"), TEXT("幅(&W):"), WS_CHILD | WS_VISIBLE | SS_RIGHT, 10, 90, 64, 32, hWnd, (HMENU)1, ((LPCREATESTRUCT)(lParam))->hInstance, 0);
		CreateWindowEx(WS_EX_CLIENTEDGE, TEXT("EDIT"), TEXT("1024"), WS_CHILD | WS_VISIBLE | WS_TABSTOP | ES_NUMBER | ES_AUTOHSCROLL, 74, 90, 128, 32, hWnd, (HMENU)2, ((LPCREATESTRUCT)(lParam))->hInstance, 0);
		CreateWindow(TEXT("STATIC"), TEXT("高さ(&H):"), WS_CHILD | WS_VISIBLE | SS_RIGHT, 10, 130, 64, 42, hWnd, (HMENU)1, ((LPCREATESTRUCT)(lParam))->hInstance, 0);
		CreateWindowEx(WS_EX_CLIENTEDGE, TEXT("EDIT"), TEXT("768"), WS_CHILD | WS_VISIBLE | WS_TABSTOP | ES_NUMBER | ES_AUTOHSCROLL, 74, 130, 128, 32, hWnd, (HMENU)3, ((LPCREATESTRUCT)(lParam))->hInstance, 0);
		hBitmap1 = LoadBitmap(((LPCREATESTRUCT)(lParam))->hInstance, MAKEINTRESOURCE(IDB_BITMAP_FINDER_FILLED));
		hBitmap2 = LoadBitmap(((LPCREATESTRUCT)(lParam))->hInstance, MAKEINTRESOURCE(IDB_BITMAP_FINDER_EMPTY));
		hCursor = LoadCursor(((LPCREATESTRUCT)(lParam))->hInstance, MAKEINTRESOURCE(IDC_CURSOR_SEARCH_WINDOW));
		SendDlgItemMessage(hWnd, 1, (UINT)STM_SETIMAGE, (WPARAM)IMAGE_BITMAP, (LPARAM)hBitmap1);
		break;
	case WM_COMMAND:
		if (LOWORD(wParam) == 1)
		{
			SendDlgItemMessage(hWnd, 1, (UINT)STM_SETIMAGE, (WPARAM)IMAGE_BITMAP, (LPARAM)hBitmap2);
			bCapture = TRUE;
			SetCursor(hCursor);
			SetCapture(hWnd);
		}
		break;
	case WM_LBUTTONUP:
		if (bCapture)
		{
			ReleaseCapture();
			SetCursor(LoadCursor(0, IDC_ARROW));
			bCapture = FALSE;
			SendDlgItemMessage(hWnd, 1, (UINT)STM_SETIMAGE, (WPARAM)IMAGE_BITMAP, (LPARAM)hBitmap1);
			POINT point;
			GetCursorPos(&point);
			const HMONITOR hMonitor = MonitorFromPoint(point, MONITOR_DEFAULTTONULL);
			if (hMonitor)
			{
				MONITORINFO	MonitorInfo;
				MonitorInfo.cbSize = sizeof(MonitorInfo);
				GetMonitorInfo(hMonitor, &MonitorInfo);
				const int nWidth = GetDlgItemInt(hWnd, 2, 0, 0);
				const int nHeight = GetDlgItemInt(hWnd, 3, 0, 0);
				const HWND hWndTarget = GetAncestor(WindowFromPoint(point), GA_ROOT);
				if (hWndTarget != hWnd)
				{
					if (IsZoomed(hWndTarget))
					{
						ShowWindow(hWndTarget, SW_RESTORE);
					}
					SetWindowPos(hWndTarget, 0, (MonitorInfo.rcWork.right - MonitorInfo.rcWork.left - nWidth) / 2, (MonitorInfo.rcWork.bottom - MonitorInfo.rcWork.top - nHeight) / 2, GetDlgItemInt(hWnd, 2, 0, 0), GetDlgItemInt(hWnd, 3, 0, 0), SWP_NOZORDER | SWP_NOSENDCHANGING);
					SetForegroundWindow(hWndTarget);
				}
			}
		}
		break;
	case WM_CLOSE:
		DestroyWindow(hWnd);
		break;
	case WM_DESTROY:
		DeleteObject(hBitmap1);
		DeleteObject(hBitmap2);
		PostQuitMessage(0);
		break;
	default:
		return DefDlgProc(hWnd, msg, wParam, lParam);
	}
	return 0;
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPreInst, LPSTR pCmdLine, int nCmdShow)
{
	TCHAR szClassName[] = TEXT("Window");
	MSG msg;
	WNDCLASS wndclass = {
		0,
		WndProc,
		0,
		DLGWINDOWEXTRA,
		hInstance,
		0,
		LoadCursor(0,IDC_ARROW),
		0,
		0,
		szClassName
	};
	RegisterClass(&wndclass);
	RECT rect = { 0, 0, 320, 240 };
	const DWORD dwStyle = WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX;
	AdjustWindowRect(&rect, dwStyle, 0);
	HWND hWnd = CreateWindow(
		szClassName,
		TEXT("指定したウィンドウサイズを調整"),
		dwStyle,
		CW_USEDEFAULT,
		0,
		rect.right - rect.left,
		rect.bottom - rect.top,
		0,
		0,
		hInstance,
		0
	);
	ShowWindow(hWnd, SW_SHOWDEFAULT);
	UpdateWindow(hWnd);
	while (GetMessage(&msg, 0, 0, 0))
	{
		if (!IsDialogMessage(hWnd, &msg))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}
	return (int)msg.wParam;
}
