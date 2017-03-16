#pragma once

#include <cmath>
#include <algorithm>
#include <cassert>
#include <string>
#include <vector>
#include <list>
#include <deque>
#include <iostream>

#include "RedHeader.h"

namespace Red
{
	typedef char	RChar;
	typedef short	RShort;
	typedef float	RFloat;
	typedef	int		RInt;
	typedef	bool	RBool;
	typedef double	RDouble;
	typedef	std::string		RString;
	typedef	std::wstring	RWString;

	typedef	unsigned char	RUChar;
	typedef unsigned short	RUShort;
	typedef unsigned int	RUInt;

	const RInt	SCREEN_WIDTH = 1024;
	const RInt	SCREEN_HEIGHT = 800;

	const RString	DEFAULT_NAME = "RedEngine";
	const RString	DEFAULT_PATH = "Red/";

	static RString GetPath(const RString& file) { return DEFAULT_PATH + file; }


#define	PI			3.141592654f
#define PI2			(PI * 2)
#define PI_DIV_2	(PI * 0.5f)
#define PI_DIV_4	(PI * 0.25f)
#define PI_INV		0.318309886f

#define EPSILON_E6	(RFloat)(1E-6)

#define DEG_TO_RAD(deg) (deg * 0.017453f)
#define RAD_TO_DEG(rad) (rad * 57.295780f)

#define Abs(a)	((a) > 0 ? (a):-(a))
#define KEY_DOWN(vk_code) ((GetAsyncKeyState((vk_code) & 0x8000) ? 1 : 0)
#define KEY_UP(vk_code) ((GetAsyncKeyState((vk_code) & 0x8000) ? 1 : 0)

#define SafeDelete(ptr) if(ptr) {delete ptr; ptr = 0;}
#define SafeDeleteArray(ptr) if(ptr) {delete[] ptr; ptr = 0;}

#define COUT(a) std::cout << #a << ":" << (a) << std::endl;

#define RCOLOR_16BIT(r, g, b) (((r)&0xff) << 16 | ((g)&0xff) << 8 | (b)&0xff)

	struct RColor
	{
		RUChar r, g, b, a;
		RColor(RInt color, RUChar alpha = 255)
		{
			r = (unsigned char)((color & 0xff0000) >> 16);
			g = (unsigned char)((color & 0x00ff00) >> 8);
			b = (unsigned char)(color & 0x0000ff);
			a = alpha;
		}

		RColor(RUChar ri = 0, RUChar gi = 0, RUChar bi = 0, RUChar ai = 0) : r(ri), g(gi), b(bi), a(ai) {}

		RInt ToInt() const {return RCOLOR_16BIT(r, g, b); }
		RColor operator * (const RColor &c) const
		{
			RInt ri = r * c.r >> 8;
			RInt gi = g * c.g >> 8;
			RInt bi = b * c.b >> 8;
			RInt ai = a * c.a >> 8;

			return RColor(ri, gi, bi, ai);
		}
	};


}
