#include <stdio.h>
#include <stdlib.h>
#include <wchar.h>
#include <time.h>

#define _WIN32_WINNT 0x0A00

#include <windows.h>

void WinShow(HDC dc, int xPos, int yPos, int color)
{
	SelectObject(dc, GetStockObject(DC_BRUSH));
	if (color == 1) { SetDCBrushColor(dc, RGB(150, 150, 150)); }
	else if (color == 2) { SetDCBrushColor(dc, RGB(255, 0, 0)); } else if(color == 3) SetDCBrushColor(dc, RGB(128, 128, 128));
	SelectObject(dc, GetStockObject(DC_PEN));
	SetDCPenColor(dc, RGB(155, 155, 155));
	Rectangle(dc, xPos, yPos, xPos+50, yPos+50);
}

int celoe(int param) 
{ 
	return ((int)param / 50) ; 
}

LRESULT WndProc(HWND hwnd, UINT message, WPARAM wparam, LPARAM lparam)
{
	if (message == WM_DESTROY)
		PostQuitMessage(0);
	else if (message == WM_KEYDOWN)
		printf("code = %d\n", wparam);
	else if (message == WM_CHAR)
		printf("%C\n", wparam);
	else if (message == WM_MOUSEMOVE)
	{
		int xPos = LOWORD(lparam);
		int yPos = HIWORD(lparam);
		printf("mouse [%d,%d]\n", xPos, yPos);
	}
	else if (message == WM_LBUTTONDOWN)
		printf("mouse Left Button Down\n");
	else if (message == WM_RBUTTONDOWN)
		printf("mouse Right Button Down\n");
	
	else return DefWindowProcA(hwnd, message, wparam, lparam);
}

int main()
{
	HFONT hFont;
	WNDCLASSA wcl;
	memset(&wcl, 0, sizeof(WNDCLASSA));
	wcl.lpszClassName = "my Window";
	wcl.lpfnWndProc = WndProc;
	RegisterClassA(&wcl);

	HWND hwnd;

	hwnd = CreateWindowA("my Window", "Minesweeper", WS_OVERLAPPEDWINDOW,
		30, 30, 517, 540, NULL, NULL, NULL, NULL);

	HDC dc = GetDC(hwnd);

	ShowWindow(hwnd, SW_SHOWNORMAL);

	MSG msg;
	int matritsa[10][10];
	CrateMatrix(matritsa);
	searchMin(matritsa);
	for (int i = 0; i < 10; i++)
	{
		for (int j = 0; j < 10; j++)
		{
			WinShow(dc, i * 50, j * 50, 1);
			
			
			
		}
	}
	int low, hig;
	boolean game = 1;
	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
		low = celoe(LOWORD(msg.lParam));
		hig = celoe(HIWORD(msg.lParam));
		if (msg.message == WM_LBUTTONDOWN && game!=0)
		{

				WinShow(dc, low * 50,  hig * 50,3);
				
				wchar_t str[20];
				swprintf(str, 20, L"%d", matritsa[low][hig]);

				if (matritsa[low][hig] == 1)
				{ 
					SetTextColor(dc, RGB(20, 20, 255)); 
				}
				else if (matritsa[low][hig] == 2)
				{
					SetTextColor(dc, RGB(50, 70, 255));
				}
				else if (matritsa[low][hig] == 3)
				{
					SetTextColor(dc, RGB(70, 100, 255));
				}
				else if (matritsa[low][hig] == 4)
				{
					SetTextColor(dc, RGB(100, 130, 255));
				}
				else if (matritsa[low][hig] == 10)
				{
					game = 0;
					SetTextColor(dc, RGB(255, 10, 10));
					MessageBox(hwnd, L"Вы проиграли", L"Вы проиграли", MB_OK | MB_ICONINFORMATION);
				}
				else 
				{ 
					SetTextColor(dc, RGB(20, 20, 255)); 
				}
				hFont = CreateFont(20, 0, 0, 0, FW_BOLD, FALSE, FALSE, FALSE, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS,
					CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_SWISS, L"Arial");
				SetBkMode(dc, TRANSPARENT);
				SelectObject(dc, hFont);
				if (matritsa[low][hig] == 10) { TextOutW(dc, low * 50 + 15, hig * 50 + 20, str, lstrlen(str)); }
				else { TextOutW(dc, low * 50 + 20, hig * 50 + 20, str, lstrlen(str)); }
				DeleteObject(hFont);
				
				
				
		}
		if (msg.message == WM_RBUTTONDOWN && game != 0)
		{
				WinShow(dc, low * 50, hig * 50, 3);

				wchar_t str[20];
				swprintf(str, 20, L"М");

				SetTextColor(dc, RGB(255, 0, 0)); 

				hFont = CreateFont(20, 0, 0, 0, FW_BOLD, FALSE, FALSE, FALSE, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS,
					CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_SWISS, L"Arial");
				SetBkMode(dc, TRANSPARENT);
				SelectObject(dc, hFont);
				TextOutW(dc, low * 50 + 20, hig * 50 + 20, str, lstrlen(str));
				DeleteObject(hFont);
		}
	}

	return 0;

}

int CrateMatrix(int matritsa[][10])
{
	for (int i = 0; i < 10; i++) {
		for (int j = 0; j < 10; j++)
		{
			matritsa[i][j] = 0;
		}
	}
	int random_number1;
	int random_number2;
	srand(time(0));

	for (int i = 0; i < 10; )
	{
		random_number1 = rand() % 10;
		random_number2 = rand() % 10;
		if (matritsa[random_number1][random_number2] == 0) 
		{
			matritsa[random_number1][random_number2] = 10;
			i++;
		}
	}


}
int searchMin(int matritsa[][10]) 
{
	for (int i = 0; i < 10; i++) {
		for (int j = 0; j < 10; j++)
		{
			if (matritsa[i][j] == 10)
			{ 
				upCount(matritsa,i, j); 
				matritsa[i][j] = 10;
			}
		}
	}
}
int upCount(int matritsa[][10], int i, int j) 
{
	if (i == 0  )
	{
		for (int h = 0; h != 2;h++) 
		{
			for (int f = -1; f != 2;f++) 
			{
				if (matritsa[i + h][j + f]!=10) { matritsa[i + h][j + f] = matritsa[i + h][j + f] + 1; }
			}
		}
	}
	else if(j == 0)
	{	
	
		for (int h = -1; h != 2;h++)
		{
			for (int f = 0; f != 2;f++)
			{
				if (matritsa[i + h][j + f] != 10) {
					matritsa[i + h][j + f] = matritsa[i + h][j + f] + 1;
				}
			}
		}
	}
	else if (i == 9)
	{

		for (int h = -1; h != 1;h++)
		{
			for (int f = -1; f != 2;f++)
			{
				if (matritsa[i + h][j + f] != 10) {
					matritsa[i + h][j + f] = matritsa[i + h][j + f] + 1;
				}
			}
		}
	}
	else if (j == 9)
	{

		for (int h = -1; h != 2;h++)
		{
			for (int f = -1; f != 1;f++)
			{
				if (matritsa[i + h][j + f] != 10) {
					matritsa[i + h][j + f] = matritsa[i + h][j + f] + 1;
				}
			}
		}
	}
	else {
		for (int h = -1; h != 2;h++)
		{
			for (int f = -1; f != 2;f++)
			{
				if (matritsa[i + h][j + f] != 10) {
					matritsa[i + h][j + f] = matritsa[i + h][j + f] + 1;
				}
			}
		}
	}
}
/*int openZero(HDC *dc, int matritsa[][10], int i, int j, wchar_t str)
{
	HFONT hFont;
	for (int h = -1; h != 2; h++)
	{
		for (int f = -1; f != 2; f++)
		{
			WinShow(dc, i + h * 50, j + f * 50, 3);
			hFont = CreateFont(20, 0, 0, 0, FW_BOLD, FALSE, FALSE, FALSE, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS,
				CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_SWISS, L"Arial");
			SetBkMode(dc, TRANSPARENT);
			SelectObject(dc, hFont);
			wchar_t strArr[2] = { str, '\0' }; // Создание массива из одного символа
			TextOutW(dc, i + h * 50 + 20, j + f * 50 + 20, strArr, lstrlen(strArr));
			if (matritsa[i + h][j + f] == 0) {

				openZero(dc, matritsa, i + h, j + f, str);
			}
			DeleteObject(hFont);
		}
	}
}*/