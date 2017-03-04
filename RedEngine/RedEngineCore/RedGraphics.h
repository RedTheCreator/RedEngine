#pragma once

#include "RedCommon.h"
#include <Windows.h>

namespace Red
{
	extern HINSTANCE GHInstance;

	class RBitMap
	{
	public:
		RInt width, height;
		RBitMap(const RString& filename);
		~RBitMap();

		inline RString getName() const { return name; }
		inline bool isValid() const { return valid; }
		RColor getPixel(RInt x, RInt y);
		inline RInt getHeight() const { return height; }
		inline RInt getWidth() const { return width; }

	private:
		RString	name;
		RBool	valid;
		HBITMAP	hBitmap;
		BITMAP	bitmap;
		HDC		bitmapHDC;
	public:
		RColor	*pixels;
		RInt	pitch;
	};

	class RedGraphics
	{
	public:
		static bool initGraphics(HINSTANCE hinstance);

		static void shutdownGraphics();

		static RBool checkZ(RInt x, RInt y, RFloat z);
		static void setPixel(RInt x, RInt y, const RColor& c);
		static RColor getPixel(RInt x, RInt y);

		static void drawLine(RInt x0, RInt y0, RInt x1, RInt y1, const RColor& c);
		static void drawString(const RString& str, RInt x, RInt y, const RColor& c = RColor(255,255,255));
		static void fillTriangle(RInt x0, RInt y0, RInt x1, RInt y1, RInt x2, RInt y2, const RColor& c = RColor(255, 255, 255));

		static void enableSmoothingMode(RBool enable);

		static void clearBuffer(const RColor& c = RColor());

		static void fillBuffer(HDC hdc);

		static RInt getScreenWidth() { return SCREEN_WIDTH; }
		static RInt getScreenHeigth() { return SCREEN_HEIGHT; }

	private:
		static HBITMAP	GBufferHandle;
		static HDC		GBufferedHDC;
		static HBRUSH	GBgBrush;
		static HPEN		GPen;

		static HINSTANCE	GInstance;
		static BYTE			*GDatas;
		static RInt			GPitch;
		static DIBSECTION	GDIBSection;
		static RECT			GBufferSize;
		static RFloat		*GZBuffer;
	};
}