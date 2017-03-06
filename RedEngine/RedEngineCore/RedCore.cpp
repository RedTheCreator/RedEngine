#include "RedCore.h"
#include "RedFrustum.h"
#include "RedGraphics.h"
#include "RedLight.h"
#include "RedMaterial.h"

namespace Red
{
	RVertex4::RVertex4() :x(0.0f), y(0.0f), z(0.0f), w(1.0f),
		u(0.0f), v(0.0f) {}

	RVertex4::RVertex4(const RVector4& v) : x(v.x), y(v.y), z(v.z), w(v.w),
		u(0.0f), v(0.0f) {}

	RVertex4::RVertex4(RFloat xf, RFloat yf, RFloat zf) : x(xf), y(yf), z(zf), w(1),
		u(0.0f), v(0.0f) {}

	RVertex4 RVertex4::operator *(const RVector4& right)
	{
		RVertex4 temp = *this;
		temp.x *= right.x, temp.y *= right.y, temp.z *= right.z;
		return temp;
	}

	RVertex4 RVertex4::operator -(const RVertex4& right)
	{
		RVertex4 temp = *this;
		temp.x -= right.x, temp.y -= right.y, temp.z -= right.z;
		return temp;
	}

	RVertex4 RVertex4::operator +(const RVertex4& right)
	{
		RVertex4 temp = *this;
		temp.x += right.x, temp.y += right.y, temp.z += right.z;
		return temp;
	}

	RVertex4 RVertex4::operator /(RFloat factor)
	{
		RVertex4 temp = *this;
		temp.x /= factor, temp.y /= factor, temp.z /= factor;
		return temp;
	}

	RVertex4& RVertex4::operator =(const RVertex4& right)
	{
		x = right.x; y = right.y; z = right.z; w = right.w;
		u = right.u; v = right.v;

		return *this;
	}

	RVector4 RVertex4::toVector4() const
	{
		return RVector4(x, y, z, w);
	}

	RPlane3::RPlane3(const RVector4& p, const RVector4& nor) :point(p)
	{
		normal = nor;
		normal.normalize();
	}

	RPlane3& RPlane3::operator =(const RPlane3& right)
	{
		point = right.point;
		normal = right.normal;
		return *this;
	}

	RPolygon4::RPolygon4() :state(0), attribute(0), color(0xffffff), verlist(NULL) {}

	RPolygonF4::RPolygonF4() : state(0), attribute(0), color(0xffffff) {}

	RObject4::RObject4() : nextObject(NULL), materialName(DEFAULT_NAME), scale(1, 1, 1), direction(RVector4::UNIT_X), needCull(true) {}

	void Transform_Object4(RObject4* object, const RMatrix4x4& mat, OBJ_TRANSFORM_TYPE transformType)
	{
		RObject4* obj = object;
		switch (transformType)
		{
		case TRANSFORM_LOCAL:
		{
			while (obj)
			{
				for (RInt i = 0; i < obj->vertexNumber; i++)
				{
					GetVertex4MulMatrix4x4(obj->localList[i], mat, obj->localList[i]);
				}
				obj = obj->nextObject;
			}
		}
		break;
		case TRANSFORM_TRANS:
		{
			while (obj)
			{
				for (RInt i = 0; i < obj->vertexNumber;i++)
				{
					GetVertex4MulMatrix4x4(obj->transformList[i], mat, obj->transformList[i]);
				}
				obj = obj->nextObject;
			}
		}
			break;
		case TRANSFORM_LOCAL_TO_TRANS:
		{
			while (obj)
			{
				for (RInt i = 0; i < obj->vertexNumber;i++)
				{
					GetVertex4MulMatrix4x4(obj->localList[i], mat, obj->transformList[i]);
				}
				obj = obj->nextObject;
			}
		}
			break;
		default:
			break;
		}
	}

	void Transform_Renderlist4(RRenderList4* renderList, const RMatrix4x4& mat, OBJ_TRANSFORM_TYPE transformType)
	{
		RPolygonF4* poly = NULL;
		switch (transformType)
		{
		case TRANSFORM_LOCAL:
		{

		}
			break;
		case TRANSFORM_TRANS:
		{

		}
			break;
		case TRANSFORM_LOCAL_TO_TRANS:
		{

		}
			break;
		default:
			break;
		}
	}
}