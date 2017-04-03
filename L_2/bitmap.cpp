#include "bitmap.h"
int ShowBitMap(HWND  hWnd, RECT& r, HBITMAP hBit, int x, int y)
{
	BITMAP BitMap;
	GetObject(hBit, sizeof(BITMAP), &BitMap);
	int Height = BitMap.bmHeight;
	//int Height = r.bottom;
	int Width = BitMap.bmWidth;
	//int Width = r.right;
	HDC hdc = GetDC(hWnd);
	HDC hdcMem = CreateCompatibleDC(hdc);
	HBITMAP OldBitmap = (HBITMAP)SelectObject(hdcMem, hBit);
	BitBlt(hdc, x, y, Width, Height, hdcMem, 0, 0, SRCCOPY);
	SelectObject(hdcMem, OldBitmap);
	ReleaseDC(hWnd, hdc);
	DeleteDC(hdcMem);
	return 0;
}

int ClientToBmp(HWND hWnd, RECT& r, CString Name)
{
	BITMAPFILEHEADER bmfHdr;
	BITMAPINFOHEADER bi;
	int BitToPixel = 16;
	HDC hdc = GetDC(hWnd);
	HDC hdcMem = CreateCompatibleDC(hdc);
	HBITMAP BitMap = CreateCompatibleBitmap(hdc, r.right - r.left, r.bottom - r.top);
	HBITMAP OldBitmap = (HBITMAP)SelectObject(hdcMem, BitMap);
	BitBlt(hdcMem,0 ,0, r.right,r.bottom, hdc, r.left,r.top, SRCCOPY);
	BitMap = (HBITMAP)SelectObject(hdcMem, OldBitmap);
	ZeroMemory(&bi, sizeof(BITMAPINFOHEADER));
	bi.biSize = sizeof(BITMAPINFOHEADER);
	bi.biWidth = r.right-r.left;
	bi.biHeight = r.bottom-r.top;
	bi.biPlanes = 1;
	bi.biBitCount = BitToPixel;
	bi.biSizeImage = ((r.right - r.left)*BitToPixel + 31) / 32 * 4 * (r.bottom - r.top);
	DWORD dwWritten;
	HANDLE fh = CreateFile(Name, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL | FILE_FLAG_SEQUENTIAL_SCAN, NULL);
	if (fh == INVALID_HANDLE_VALUE) return 2;
	bmfHdr.bfType = ('M' << 8) | 'B';
	bmfHdr.bfSize = bi.biSizeImage + sizeof(bmfHdr)+bi.biSize;
	bmfHdr.bfReserved1 = bmfHdr.bfReserved2 = 0;
	bmfHdr.bfOffBits = sizeof(bmfHdr)+bi.biSize;
	WriteFile(fh, (LPSTR)&bmfHdr, sizeof(bmfHdr), &dwWritten, NULL);
	WriteFile(fh, (LPSTR)&bi, sizeof(BITMAPINFOHEADER), &dwWritten, NULL);
	char *lp = (char*)GlobalAlloc(GMEM_FIXED, bi.biSizeImage);
	int err = GetDIBits(hdc, BitMap, 0, (UINT)(r.bottom), lp, (LPBITMAPINFO)&bi, DIB_RGB_COLORS);
	WriteFile(fh , lp, bi.biSizeImage, &dwWritten , NULL);
	GlobalFree(GlobalHandle(lp));
	CloseHandle(fh);
	ReleaseDC(hWnd, hdc);
	DeleteDC(hdcMem);
	if (dwWritten == 0) return 2;
	return 0;
}

//int ClientToBmp(HWND hWnd, RECT& r, CString Name)
//{
//	BITMAPFILEHEADER bmfHdr;
//	BITMAPINFOHEADER bi;
//	int BitToPixel = 16;
//	HDC hdc = GetDC(hWnd);
//	HDC hdcMem = CreateCompatibleDC(hdc);
//	HBITMAP BitMap = CreateCompatibleBitmap(hdc, r.right, r.bottom);
//	HBITMAP OldBitmap = (HBITMAP)SelectObject(hdcMem, BitMap);
//	BitBlt(hdcMem, 0, 0, r.right, r.bottom, hdc, 0, 0, SRCCOPY);
//	BitMap = (HBITMAP)SelectObject(hdcMem, OldBitmap);
//	ZeroMemory(&bi, sizeof(BITMAPINFOHEADER));
//	bi.biSize = sizeof(BITMAPINFOHEADER);
//	bi.biWidth = r.right;
//	bi.biHeight = r.bottom;
//	bi.biPlanes = 1;
//	bi.biBitCount = BitToPixel;
//	bi.biSizeImage = (r.right*BitToPixel + 31) / 32 * 4 * r.bottom;
//	DWORD dwWritten;
//	HANDLE fh = CreateFile(Name, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL | FILE_FLAG_SEQUENTIAL_SCAN, NULL);
//	if (fh == INVALID_HANDLE_VALUE) return 2;
//	bmfHdr.bfType = ('M' << 8) | 'B';
//	bmfHdr.bfSize = bi.biSizeImage + sizeof(bmfHdr)+bi.biSize;
//	bmfHdr.bfReserved1 = bmfHdr.bfReserved2 = 0;
//	bmfHdr.bfOffBits = sizeof(bmfHdr)+bi.biSize;
//	WriteFile(fh, (LPSTR)&bmfHdr, sizeof(bmfHdr), &dwWritten, NULL);
//	WriteFile(fh, (LPSTR)&bi, sizeof(BITMAPINFOHEADER), &dwWritten, NULL);
//	char *lp = (char*)GlobalAlloc(GMEM_FIXED, bi.biSizeImage);
//	int err = GetDIBits(hdc, BitMap, 0, (UINT)r.bottom, lp, (LPBITMAPINFO)&bi, DIB_RGB_COLORS);
//	WriteFile(fh, lp, bi.biSizeImage, &dwWritten, NULL);
//	GlobalFree(GlobalHandle(lp));
//	CloseHandle(fh);
//	ReleaseDC(hWnd, hdc);
//	DeleteDC(hdcMem);
//	if (dwWritten == 0) return 2;
//	return 0;
//}
