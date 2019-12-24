#include <Windows.h>
#include <iostream>
#include <vector>
#include <time.h>
#include <fstream>

HWND GameHWND = NULL;
HDC GameDC = NULL;
RECT GameRect;
int WindowWidth = 1600;
int WindowHeight = 900;

static DWORDLONG random_number(DWORDLONG min, DWORDLONG max)
{
	srand(time(0));
	DWORDLONG out = min + rand() % (max - min + 1);
	return out;
}

struct RGBPixel
{
	int r;
	int g;
	int b;
};

int angle = 4;
int mincolor = 253;

bool HexMode = false;
bool Scanning = false;
bool Wiggling = false;
bool Struggling = false;

bool ShouldBeep = false;
bool ShouldMonitor = false;
bool ShouldMonitor2 = false;
bool ShouldMonitor3 = false;
bool ShouldMonitor4 = false;

int Radius = 54;
int PixelX = 0;
int PixelY = 0;
int PixelX2 = 0;
int PixelY2 = 0;
int Radius3 = 55;
int PixelX3 = 0;
int PixelY3 = 0;
int Radius4 = 56;
int PixelX4 = 0;
int PixelY4 = 0;

INPUT input;
void SendSpace()
{
	input.ki.wScan = 0x39;
	input.ki.dwFlags = KEYEVENTF_SCANCODE;
	SendInput(1, &input, sizeof(INPUT));

	Sleep(random_number(70, 100));

	input.ki.wScan = 0x39;
	input.ki.dwFlags = KEYEVENTF_KEYUP | KEYEVENTF_SCANCODE;
	SendInput(1, &input, sizeof(INPUT));

	std::cout << "FIRING\n";
}
void MonitorTimer()
{
	Sleep(1800);
	ShouldMonitor = false;
	if(Scanning) std::cout << "SCANNING...\n";
}
void MonitorTimer3()
{
	Sleep(1800);
	ShouldMonitor3 = false;
}
void MonitorTimer4()
{
	Sleep(1800);
	ShouldMonitor4 = false;
}
void SecondThread()
{
	while (true)
	{
		if (Scanning)
		{
			if (GameHWND)
			{
				if (ShouldMonitor)
				{
					HDC GameDC2 = GetDC(GameHWND);
					COLORREF color = GetPixel(GameDC2, WindowWidth * 0.501 + PixelX, (WindowHeight * 0.5 + PixelY));
					RGBPixel Pixel = { GetRValue(color), GetGValue(color),GetBValue(color) };
					if (Pixel.r > 200 && Pixel.g < 150 && Pixel.b < 150) SendSpace();
					ReleaseDC(GameHWND, GameDC2);
				}
			}
		}
	}
}
void ThirdThread()
{
	while (true)
	{
		if (Scanning)
		{
			if (GameHWND)
			{
				HDC GameDC3 = GetDC(GameHWND); //GameDC = GetDC(Desktop);
				if (GameDC3)
				{
					if (!ShouldMonitor3)
					{
						for (int j = 0; j < 360; j = j + angle)
						{
							int x = Radius3 * cos(j * 3.14159265 / 180);
							int y = Radius3 * sin(j * 3.14159265 / 180);

							COLORREF color = GetPixel(GameDC3, WindowWidth * 0.501 + x, (WindowHeight * 0.5 + y));
							RGBPixel Pixel = { GetRValue(color), GetGValue(color),GetBValue(color) };
							if (Pixel.r > mincolor && Pixel.g > mincolor && Pixel.b > mincolor)
							{
								ShouldMonitor3 = true;
								PixelX3 = x;
								PixelY3 = y;
								CreateThread(0, 0, (LPTHREAD_START_ROUTINE)MonitorTimer3, 0, 0, 0);
								break;
							}
						}
					}
					ReleaseDC(GameHWND, GameDC3);
				}

				if (ShouldMonitor3)
				{
					GameDC3 = GetDC(GameHWND);
					COLORREF color = GetPixel(GameDC3, WindowWidth * 0.501 + PixelX3, (WindowHeight * 0.5 + PixelY3));
					RGBPixel Pixel = { GetRValue(color), GetGValue(color),GetBValue(color) };
					if (Pixel.r > 200 && Pixel.g < 150 && Pixel.b < 150) SendSpace();
					ReleaseDC(GameHWND, GameDC3);
				}
			}
		}
	}
}
void FourthThread()
{
	while (true)
	{
		if (Scanning)
		{
			if (GameHWND)
			{
				HDC GameDC3 = GetDC(GameHWND); //GameDC = GetDC(Desktop);
				if (GameDC3)
				{
					if (!ShouldMonitor4)
					{
						for (int j = 0; j < 360; j = j + angle)
						{
							int x = Radius4 * cos(j * 3.14159265 / 180);
							int y = Radius4 * sin(j * 3.14159265 / 180);

							COLORREF color = GetPixel(GameDC3, WindowWidth * 0.501 + x, (WindowHeight * 0.5 + y));
							RGBPixel Pixel = { GetRValue(color), GetGValue(color),GetBValue(color) };
							if (Pixel.r > mincolor&& Pixel.g > mincolor&& Pixel.b > mincolor)
							{
								ShouldMonitor4 = true;
								PixelX4 = x;
								PixelY4 = y;
								CreateThread(0, 0, (LPTHREAD_START_ROUTINE)MonitorTimer4, 0, 0, 0);
								break;
							}
						}
					}
					ReleaseDC(GameHWND, GameDC3);
				}

				if (ShouldMonitor4)
				{
					GameDC3 = GetDC(GameHWND);
					COLORREF color = GetPixel(GameDC3, WindowWidth * 0.501 + PixelX4, (WindowHeight * 0.5 + PixelY4));
					RGBPixel Pixel = { GetRValue(color), GetGValue(color),GetBValue(color) };
					if (Pixel.r > 200 && Pixel.g < 150 && Pixel.b < 150) SendSpace();
					ReleaseDC(GameHWND, GameDC3);
				}
			}
		}
	}
}
int main()
{
	memset(&input, 0, sizeof(input));
	input.type = INPUT_KEYBOARD;
	input.ki.time = 0;
	input.ki.dwExtraInfo = 0;
	MoveWindow(GetConsoleWindow(), 0, 0, 155, 100, TRUE);
	SetWindowPos(GetConsoleWindow(), HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);

	GameHWND = FindWindowA("UnrealWindow", NULL);
	if (GameHWND)
	{
		GetWindowRect(GameHWND, &GameRect);
		WindowWidth = GameRect.right - GameRect.left;
		WindowHeight = GameRect.bottom - GameRect.top;
		DWORD dwStyle = GetWindowLong(GameHWND, GWL_STYLE);
		if (dwStyle & WS_BORDER)
		{
			GameRect.top += 23;
			WindowHeight -= 23;
		}
		Radius = WindowWidth * 0.03375;
		Radius3 = Radius +1;
		Radius4 = Radius +1;
	}
	else
	{
		std::cout << "Game not found\n";
		getchar();
		exit(1);
	}

	CreateThread(0, 0, (LPTHREAD_START_ROUTINE)SecondThread, 0, 0, 0);
	Sleep(100);
	CreateThread(0, 0, (LPTHREAD_START_ROUTINE)ThirdThread, 0, 0, 0);
	Sleep(100);
	CreateThread(0, 0, (LPTHREAD_START_ROUTINE)FourthThread, 0, 0, 0);
	Sleep(100);

	while (true)
	{
		if (GetAsyncKeyState(VK_CAPITAL))
		{
			Struggling = false;
			Wiggling = false;
			Scanning = !Scanning;
			if (Scanning) std::cout << "SCANNING: ON\n";
			else  std::cout << "SCANNING: OFF\n";
			while (GetAsyncKeyState(VK_CAPITAL)) Sleep(20);
		}
		if (GetAsyncKeyState(VK_XBUTTON1))
		{
			Struggling = false;
			Wiggling = !Wiggling;
			if (Wiggling)
			{
				std::cout << "WIGGLING: ON\n";
				Beep(500, 25);
				Sleep(25);
				Beep(500, 25);
			}
			else
			{
				std::cout << "WIGGLING: OFF\n";
				Beep(100, 150);
			}
			while (GetAsyncKeyState(VK_XBUTTON1)) Sleep(50);
		}
		if (GetAsyncKeyState(VK_XBUTTON2))
		{
			Wiggling = false;
			Struggling = !Struggling;
			if (Struggling)
			{
				std::cout << "STRUGGLING: ON\n";
				Beep(500, 25);
				Sleep(25);
				Beep(500, 25);
			}
			else
			{
				std::cout << "STRUGGLING: OFF\n";
				Beep(100, 150);
			}
			while (GetAsyncKeyState(VK_XBUTTON2)) Sleep(50);
		}
		if (GetForegroundWindow() == GameHWND)
		{
			if (Struggling)
			{
				input.ki.wScan = 0x39;
				input.ki.dwFlags = KEYEVENTF_SCANCODE;
				SendInput(1, &input, sizeof(INPUT));
				Sleep(random_number(50, 60));

				input.ki.wScan = 0x39;
				input.ki.dwFlags = KEYEVENTF_KEYUP | KEYEVENTF_SCANCODE;
				SendInput(1, &input, sizeof(INPUT));
				Sleep(random_number(50, 60));
			}
			if (Wiggling)
			{
				input.ki.wScan = 0x1E;
				input.ki.dwFlags = KEYEVENTF_SCANCODE;
				SendInput(1, &input, sizeof(INPUT));
				Sleep(random_number(25, 50));

				input.ki.wScan = 0x1E;
				input.ki.dwFlags = KEYEVENTF_KEYUP | KEYEVENTF_SCANCODE;
				SendInput(1, &input, sizeof(INPUT));
				Sleep(75);

				input.ki.wScan = 0x20;
				input.ki.dwFlags = KEYEVENTF_SCANCODE;
				SendInput(1, &input, sizeof(INPUT));
				Sleep(random_number(25, 50));

				input.ki.wScan = 0x20;
				input.ki.dwFlags = KEYEVENTF_KEYUP | KEYEVENTF_SCANCODE;
				SendInput(1, &input, sizeof(INPUT));
				Sleep(75);
			}
			if (Scanning)
			{
				if (GameHWND)
				{
					GameDC = GetDC(GameHWND); //GameDC = GetDC(Desktop);
					if (GameDC)
					{
						ShouldBeep = false;
						if (!ShouldMonitor)
						{
							for (int j = 0; j < 360; j = j + angle)
							{
								int x = Radius * cos(j * 3.14159265 / 180);
								int y = Radius * sin(j * 3.14159265 / 180);

								COLORREF color = GetPixel(GameDC, WindowWidth * 0.501 + x, (WindowHeight * 0.5 + y));
								RGBPixel Pixel = { GetRValue(color), GetGValue(color),GetBValue(color) };
								if (!HexMode)
								{
									if (Pixel.r > mincolor&& Pixel.g > mincolor&& Pixel.b > mincolor)
									{
										std::cout << "MONITORING\n";
										ShouldMonitor = true;
										PixelX = x;
										PixelY = y;
										CreateThread(0, 0, (LPTHREAD_START_ROUTINE)MonitorTimer, 0, 0, 0);
										break;
									}
								}
								else
								{
									if (Pixel.r > 160 && Pixel.r < 200 && Pixel.g < 30 && Pixel.b < 30)
									{
										std::cout << "MONITORING\n";
										ShouldMonitor = true;
										PixelX = x;
										PixelY = y;
										CreateThread(0, 0, (LPTHREAD_START_ROUTINE)MonitorTimer, 0, 0, 0);
										break;
									}
								}
							}
						}
						ReleaseDC(GameHWND, GameDC);
					}

					if (ShouldMonitor)
					{
						GameDC = GetDC(GameHWND);
						COLORREF color = GetPixel(GameDC, WindowWidth * 0.501 + PixelX, (WindowHeight * 0.5 + PixelY));
						RGBPixel Pixel = { GetRValue(color), GetGValue(color),GetBValue(color) };
						if (Pixel.r > 200 && Pixel.g < 150 && Pixel.b < 150)
						{
							SendSpace();
						}
						ReleaseDC(GameHWND, GameDC);
					}
				}
			}
		}
		else
		{
			GameHWND = FindWindowA("UnrealWindow", NULL);
			Wiggling = false;
			Struggling = false;
		}
	}
}