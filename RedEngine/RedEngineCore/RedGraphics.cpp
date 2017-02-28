#include "RedGraphics.h"
#include "RedUtil.h"

namespace Red
{
	RedBitMap::RedBitMap(const RString& filename) :name(filename), pixels(NULL),
		pitch(0), width(0), height(0), valid(false)
	{
		hBitmap = (HBITMAP)::LoadImage(GHInstance, GetPath(filename).c_str(), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
		if (hBitmap != NULL)
		{
			bitmapHDC = ::CreateCompatibleDC(NULL);
			::SelectObject(bitmapHDC, (HGDIOBJ)hBitmap);
			::GetObject(hBitmap, sizeof(BITMAP), &bitmap);

			width = bitmap.bmWidth;
			height = bitmap.bmHeight;
			pitch = bitmap.bmHeight;
			valid = true;

			pixels = new RColor(width * height);
			for (RInt i = 0; i < getHeight(); i++)
			{
				for (RInt j = 0; j < getWidth(); j++)
				{
					COLORREF color = ::GetPixel(bitmapHDC, i, j);
					pixels[j * width + i] = RColor(GetRValue(color), GetBValue(color));
				}
			}
		}
	}

	RedBitMap::~RedBitMap()
	{
		DeleteObject(hBitmap);
		DeleteDC(bitmapHDC);
		SafeDeleteArray(pixels);
	}

	RColor RedBitMap::getPixel(RInt x, RInt y)
	{
		return pixels[y * pitch + x];
	}

	HINSTANCE	GHinstance;
	HBITMAP		RedGraphics::GBufferHandle;
	HDC			RedGraphics::GBufferedHDC;
	HBRUSH		RedGraphics::GBgBrush;
	HPEN		RedGraphics::GPen;
	HINSTANCE	RedGraphics::GInstance;
	RECT		RedGraphics::GBufferSize;
	DIBSECTION	RedGraphics::GDIBSection;
	BYTE		*RedGraphics::GDatas;
	RInt		RedGraphics::GPitch;

	RFloat		*RedGraphics::GZBuffer;

	bool RedGraphics::initGraphics(HINSTANCE hinstance)
	{
		GBufferedHDC = ::CreateCompatibleDC(NULL);

		BITMAPINFO	info = { 0 };
		info.bmiHeader.biSize = sizeof(info.bmiHeader);
		info.bmiHeader.biWidth = SCREEN_WIDTH;
		info.bmiHeader.biHeight = -SCREEN_HEIGHT;
		info.bmiHeader.biPlanes = 1;
		info.bmiHeader.biBitCount = 2;
		info.bmiHeader.biCompression = BI_RGB;
		info.bmiHeader.biSizeImage = SCREEN_WIDTH * SCREEN_HEIGHT * 32 / 8;

		void* pBits = NULL;
		GBufferHandle = ::CreateDIBSection(GBufferedHDC, &info, DIB_RGB_COLORS, &pBits, NULL, 0);
		::SelectObject(GBufferedHDC,GBufferHandle);

		::GetObject(GBufferHandle, sizeof(DIBSECTION), &GDIBSection);
		GDatas = (BYTE*)GDIBSection.dsBm.bmBits;
		GPitch = GDIBSection.dsBm.bmWidthBytes;

		::SetRect(&GBufferSize, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);

		GPen = ::CreatePen(PS_SOLID, 1, RGB(255, 255, 255));
		::SelectObject(GBufferedHDC, GPen);

		HFONT hfnt = (HFONT)::GetStockObject(OEM_FIXED_FONT);
		::SelectObject(GBufferedHDC, hfnt);

		::SetBkMode(GBufferedHDC, TRANSPARENT);

		GZBuffer = new RFloat[SCREEN_WIDTH * SCREEN_HEIGHT];
		memset(GZBuffer, 0, sizeof(RFloat) * SCREEN_WIDTH * SCREEN_HEIGHT);

		return true;
	}

	void RedGraphics::shutdownGraphics()
	{
		::DeleteObject(GPen);
		::DeleteObject(GBgBrush);
		::DeleteObject(GBufferHandle);
		::DeleteObject(GBufferedHDC);
	}

	void RedGraphics::clearBuffer(const RColor& c)
	{
		::FillRect(GBufferedHDC, &GBufferSize, GBgBrush);
		::memset(GZBuffer, 0, sizeof(RFloat) * SCREEN_WIDTH * SCREEN_HEIGHT);
	}

	void RedGraphics::drawLine(RInt x0, RInt y0, RInt x1, RInt y1, const RColor& c)
	{
		::SelectObject(GBufferedHDC, GetStockObject(DC_PEN));
		::SetDCPenColor(GBufferedHDC, RGB(c.r, c.g, c.b));
		::MoveToEx(GBufferedHDC, x0, y0, NULL);
		::LineTo(GBufferedHDC, x1, y1);
	}

	void RedGraphics::drawString(const RString& str, RInt x, RInt y, const RColor& c/* = RColor(255, 255, 255)*/)
	{
		::SetTextColor(GBufferedHDC, RGB(c.r, c.g, c.b));
		::TextOut(GBufferedHDC, x, y, str.c_str(), str.length());
	}

	void RedGraphics::fillTriangle(RInt x0, RInt y0, RInt x1, RInt y1, RInt x2, RInt y2, const RColor& c/* = RColor(255, 255, 255)*/)
	{

	}

	void RedGraphics::enableSmoothingMode(RBool enable)
	{

	}

	RBool RedGraphics::checkZ(RInt x, RInt y, RFloat z)
	{
		RInt index = y * SCREEN_WIDTH + x;
		RFloat divZ = 1.0f / z;

		if (GZBuffer[index] > divZ)
			return false;
		GZBuffer[index] = divZ;
		return true;
	}

	void RedGraphics::setPixel(RInt x, RInt y, const RColor& c)
	{
		BYTE* pSrcPix = GDatas + (GPitch * y);
		pSrcPix += x * 4;
		pSrcPix[0] = c.b;
		pSrcPix[1] = c.g;
		pSrcPix[2] = c.r;
		pSrcPix[3] = c.a;
	}

	RColor RedGraphics::getPixel(RInt x, RInt y)
	{
		BYTE* pSrcPix = GDatas + (GPitch * y);
		pSrcPix += x * 4;
		return RColor(pSrcPix[2], pSrcPix[1], pSrcPix[0]);
	}

	void RedGraphics::fillBuffer(HDC hdc)
	{
		::BitBlt(hdc, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, GBufferedHDC, 0, 0, SRCCOPY);
	}
}
