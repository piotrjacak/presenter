#include "framework.h"
#define MAX_LOADSTRING 100

HINSTANCE hInst;
WCHAR szWindowClass[MAX_LOADSTRING];
WCHAR szTitle[MAX_LOADSTRING];

ATOM MyRegisterClass(HINSTANCE hInstance);
BOOL InitInstance(HINSTANCE, int);
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
void ShowErrorMessageBox();

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPWSTR lpCmdLine,
	_In_ int nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	std::wstring className(L"POINTER\0");
	className.copy(szWindowClass, className.size());
	std::wstring title(L"pointer\0");
	title.copy(szTitle, title.size());

	MyRegisterClass(hInstance);

	if (!InitInstance(hInstance, nCmdShow))
	{
		return FALSE;
	}

	MSG message;

	while (GetMessage(&message, nullptr, 0, 0))
	{
		TranslateMessage(&message);
		DispatchMessage(&message);
	}

	return static_cast <int>(message.wParam);
}

ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEXW wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;
	wcex.hIcon = nullptr;
	wcex.hCursor = nullptr;
	wcex.hbrBackground = CreateSolidBrush(RGB(255, 255, 0));
	wcex.lpszMenuName = nullptr;

	wcex.lpszClassName = szWindowClass;
	wcex.hIconSm = nullptr;

	return RegisterClassExW(&wcex);
}

BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
	hInst = hInstance;

	POINT cursorPos;
	GetCursorPos(&cursorPos);

	int posX = cursorPos.x - 50;
	int posY = cursorPos.y - 50;

	HWND hWnd = CreateWindowEx(
		WS_EX_TRANSPARENT | WS_EX_TOPMOST | WS_EX_TOOLWINDOW,
		szWindowClass,
		szTitle,
		WS_POPUP,
		posX, posY, 100, 100,
		NULL,
		NULL,
		hInstance,
		NULL
	);

	if (!hWnd)
	{
		return FALSE;
	}

	if (RegisterHotKey(hWnd, 1, MOD_ALT | MOD_SHIFT, VK_F4) == 0)
		ShowErrorMessageBox();

	SetWindowLong(hWnd, GWL_EXSTYLE,
		GetWindowLong(hWnd, GWL_EXSTYLE) | WS_EX_LAYERED);
	SetLayeredWindowAttributes(hWnd, NULL, (255 * 20) / 100, LWA_ALPHA);

	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	return TRUE;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam,
	LPARAM lParam)
{
	switch (message)
	{
	case WM_CREATE:
	{
		HRGN hRgn = CreateEllipticRgn(0, 0, 100, 100);
		SetWindowRgn(hWnd, hRgn, TRUE);
		SetTimer(hWnd, 7, 1, NULL);
	}
	break;

	case WM_TIMER:
	{
		if (wParam == 7)
		{
			POINT cursorPos;
			GetCursorPos(&cursorPos);

			int posX = cursorPos.x - 50;
			int posY = cursorPos.y - 50;

			SetWindowPos(hWnd, NULL, posX, posY, 0, 0, SWP_NOSIZE | SWP_NOZORDER);
		}
	}
	break;

	case WM_HOTKEY:
	{
		if (wParam == 1)
		{
			PostQuitMessage(0);
		}
	}
	break;

	case WM_COMMAND:
	{
		return DefWindowProc(hWnd, message, wParam, lParam);
	}

	case WM_DESTROY:
	{
		PostQuitMessage(0);
	}
	break;

	default:
	{
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	}
	return 0;
}

void ShowErrorMessageBox() {
	DWORD errorCode = GetLastError();

	LPSTR errorText = NULL;
	FormatMessageA(
		FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
		NULL,
		errorCode,
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		(LPSTR)&errorText,
		0,
		NULL
	);

	char message[256];
	snprintf(message, sizeof(message), "Failed with error %d:\n%s", errorCode, errorText);

	MessageBoxA(NULL, message, "Error", MB_ICONERROR | MB_OK);

	LocalFree(errorText);
}

