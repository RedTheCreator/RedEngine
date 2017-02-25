#pragma once

#include "RedCommon.h"
#include "RedUtil.h"
#include "RedMath.h"

namespace Red
{
	struct RVector2
	{
		static RVector2 ZERO;
		static RVector2 UNIT_X;
		static RVector2 UNIT_Y;

		RFloat x;
		RFloat y;

		RVector2() :x(0), y(0) {}
		RVector2(RFloat xf, RFloat yf) :x(xf), y(yf) {}
		RVector2(const RVector2& v) :x(v.x), y(v.y) {}

		inline RFloat normalize()
		{
			RFloat len = length();
			if (len > EPSILON_E6)
			{
				RFloat invLength = 1.0f / len;
				x *= invLength;
				y *= invLength;
			}
			return len;
		}

		inline RFloat length() const
		{
			return sqrt(x * x + y * y);
		}

		inline RFloat dotProduct(const RVector2& right) const
		{
			return x * right.x + y * right.y;
		}

		inline RFloat angleBetween(const RVector2& right) const
		{
			RFloat lenProduct = length() * right.length();
			if (lenProduct < EPSILON_E6)
				lenProduct = EPSILON_E6;
			RFloat f = dotProduct(right) / lenProduct;
			f = Clamp(f, -1.0f, 1.0f);

			return acos(f);
		}

		inline bool isZero() const
		{
			return length() < EPSILON_E6;
		}

		inline RFloat squaredLength() const
		{
			return x * x + y * y;
		}

		inline RVector2 operator * (RFloat right) const
		{
			return RVector2(x * right, y * right);
		}

		inline RVector2 operator * (RVector2& right) const
		{
			return RVector2(x * right.x, y * right.y);
		}

		inline RVector2 operator + (const RVector2& right) const
		{
			return RVector2(x + right.x, y + right.y);
		}

		inline RVector2 operator - (const RVector2& right) const
		{
			return RVector2(x - right.x, y - right.y);
		}

		inline RVector2 operator - ()
		{
			x = -x;
			y = -y;

			return *this;
		}

		inline RVector2 operator + ()
		{
			x = +x;
			y = +y;
			return *this;
		}

		inline bool operator == (const RVector2& right) const
		{
			return EqualFloat(x, right.x) && EqualFloat(y, right.y);
		}

		inline RVector2 operator = (const RVector2& right)
		{
			x = right.x;
			y = right.y;

			return *this;
		}
	};


	struct RVector3
	{
		static RVector3 ZERO;
		static RVector3 UNIT_X;
		static RVector3 UNIT_Y;
		static RVector3 UNIT_Z;

		RFloat x;
		RFloat y;
		RFloat z;

		RVector3() :x(0), y(0), z(0) {}
		RVector3(RFloat xf, RFloat yf, RFloat zf) :x(xf), y(yf), z(zf) {}
		RVector3(const RVector3& v) :x(v.x), y(v.y), z(v.z) {}

		inline RFloat normalize()
		{
			RFloat len = length();
			if (len > EPSILON_E6)
			{
				RFloat invLength = 1.0f / len;
				x *= invLength;
				y *= invLength;
				z *= invLength;
			}
			return len;
		}

		inline RFloat length() const
		{
			return sqrt(x * x + y * y + z * z);
		}

		inline RFloat dotProduct(const RVector3& right) const
		{
			return x * right.x + y * right.y + z * right.z;
		}

		inline RVector3 crossProduct(const RVector3& right) const
		{
			return RVector3(
				y * right.z - z * right.y,
				z * right.x - x * right.z,
				x * right.y - y * right.x);
		}

		inline RFloat angleBetween(const RVector3& right) const
		{
			RFloat lenProduct = length() * right.length();
			if (lenProduct < EPSILON_E6)
				lenProduct = EPSILON_E6;
			RFloat f = dotProduct(right) / lenProduct;
			return acos(f);
		}

		inline bool isZero() const
		{
			return length() < EPSILON_E6;
		}

		inline RFloat squaredLength() const
		{
			return x * x + y * y + z * z;
		}

		inline RVector3 operator * (RFloat right) const
		{
			return RVector3(x * right, y * right, z * right);
		}

		inline RVector3 operator * (RVector3& right) const
		{
			return RVector3(x * right.x, y * right.y, z * right.z);
		}

		inline RVector3 operator + (const RVector3& right) const
		{
			return RVector3(x + right.x, y + right.y, z + right.z);
		}

		inline RVector3 operator - (const RVector3& right) const
		{
			return RVector3(x - right.x, y - right.y, z - right.z);
		}

		inline RVector3 operator - ()
		{
			x = -x;
			y = -y;
			z = -z;

			return *this;
		}

		inline RVector3 operator + ()
		{
			x = +x;
			y = +y;
			z = +z;

			return *this;
		}

		inline bool operator == (const RVector3& right) const
		{
			return EqualFloat(x, right.x) && EqualFloat(y, right.y) && EqualFloat(z, right.z);
		}

		inline RVector3 operator = (const RVector3& right)
		{
			x = right.x;
			y = right.y;
			z = right.z;

			return *this;
		}
	};

	struct RVector4
	{
		static RVector4 ZERO;
		static RVector4 UNIT_X;
		static RVector4 UNIT_Y;
		static RVector4 UNIT_Z;

		RFloat x;
		RFloat y;
		RFloat z;
		RFloat w;

		RVector4() :x(0), y(0), z(0), w(1) {}
		RVector4(RFloat xf, RFloat yf, RFloat zf, RFloat wf = 1) :x(xf), y(yf), z(zf),w(wf) {}
		RVector4(const RVector4& v) :x(v.x), y(v.y), z(v.z), w(v.w) {}

		inline RFloat normalize()
		{
			RFloat len = length();
			if (len > EPSILON_E6)
			{
				RFloat invLength = 1.0f / len;
				x *= invLength;
				y *= invLength;
				z *= invLength;
				w = 1;
			}
			return len;
		}

		inline RFloat length() const
		{
			return sqrt(x * x + y * y + z * z);
		}

		inline RFloat dotProduct(const RVector4& right) const
		{
			return x * right.x + y * right.y + z * right.z;
		}

		inline RVector4 crossProduct(const RVector4& right) const
		{
			return RVector4(
				y * right.z - z * right.y,
				z * right.x - x * right.z,
				x * right.y - y * right.x, 1);
		}

		inline RFloat angleBetween(const RVector4& right) const
		{
			RFloat lenProduct = length() * right.length();
			if (lenProduct < EPSILON_E6)
				lenProduct = EPSILON_E6;
			RFloat f = dotProduct(right) / lenProduct;
			f = Clamp(f, -1.0f, 1.0f);
			return acos(f);
		}

		inline bool isZero() const
		{
			return length() < EPSILON_E6;
		}

		inline RFloat squaredLength() const
		{
			return x * x + y * y + z * z;
		}

		inline RVector4 operator * (RFloat right) const
		{
			return RVector4(x * right, y * right, z * right, 1);
		}

		inline RVector4 operator * (RVector4& right) const
		{
			return RVector4(x * right.x, y * right.y, z * right.z, 1);
		}

		inline RVector4 operator + (const RVector4& right) const
		{
			return RVector4(x + right.x, y + right.y, z + right.z, 1);
		}

		inline RVector4 operator - (const RVector4& right) const
		{
			return RVector4(x - right.x, y - right.y, z - right.z, 1);
		}

		inline RVector4 operator - ()
		{
			return RVector4(-x, -y, -z, w);
		}

		inline bool operator == (const RVector4& right) const
		{
			return EqualFloat(x, right.x) && EqualFloat(y, right.y) && EqualFloat(z, right.z), EqualFloat(w, right.w);
		}

		inline RVector4 operator = (const RVector4& right)
		{
			x = right.x;
			y = right.y;
			z = right.z;
			w = right.w;

			return *this;
		}
	};
}