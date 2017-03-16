#pragma once

#include "RedCommon.h"
#include "RedVector.h"
#include <list>

namespace Red
{
#define RPOLY_ATTR_2SIDE 0x0001
#define RPOLY_ATTR_TRNASPARENT 0x0002
#define RPOLY_ATTR_SHADE_MODE_PURE 0x0004
#define RPOLY_ATTR_SHADE_MODE_CONSTANT 0x0004
#define RPOLY_ATTR_SHADE_MODE_FLAT 0x0008
#define RPOLY_ATTR_SHADE_MODE_GROUND 0x0010
#define RPOLY_ATTR_SHADE_MODE_PHONG 0x0020
#define RPOLY_ATTR_SHADE_MODE_FASTPHONG 0x0020
#define RPOLY_ATTR_SHADE_MODE_TEXTURE 0x0040

#define RPOLY_ATTR_VERTEX_POSITION 0x1000
#define RPOLY_ATTR_VERTEX_NORMAL 0x2000
#define RPOLY_ATTR_VERTEX_UV 0x4000
#define RPOLY_STATE_ACTIVE 0x0100
#define RPOLY_STATE_CLIPPED 0x0200
#define RPOLY_STATE_BACKFACE 0x0400

#define ROBJECT_STATE_ACTIVE 0x0001
#define ROBJECT_STATE_VISIBLE 0x0002
#define ROBJECT_STATE_CULLED 0x0004

	enum CULL_TYPE
	{
		CULL_BOX = 0,
		CULL_SPHERE = 1,
	};

	struct RVertex4;
	typedef std::vector<RVertex4> RVertex4Array;
	typedef std::vector<RVector4> RVector4Array;
	typedef std::vector<RPolygon4> RPolygon4Array;

	struct RVertex4
	{
		RFloat x, y, z, w;
		RFloat u, v;

		RVertex4();
		RVertex4(const RVector4& v);
		RVertex4(RFloat xf, RFloat yf, RFloat zf);
		RVertex4 operator * (const RVector4& right);
		RVertex4 operator - (const RVertex4& right);
		RVertex4& operator = (const RVertex4& right);
		RVertex4 operator + (const RVector4& right);
		RVertex4 operator + (const RVertex4& right);
		RVertex4 operator / (RFloat factor);

		RVector4 toVector4() const;
	};

	struct RPlane3
	{
		RVector4 point;
		RVector4 normal;
		RPlane3();
		RPlane3(const RVector4& p, const RVector4& nor);
		RPlane3& operator = (const RPlane3& right);
	};

	struct RPolygon4
	{
		RInt state;
		RInt attribute;
		RInt color;

		RVertex4Array *verlist;
		RInt verIndex[3];

		RPolygon4();
	};

	struct RPolygonF4
	{
		RInt state;
		RInt attribute;
		RInt color;

		RVertex4 localList[3];
		RVertex4 transformList[3];
		
		struct RMaterial *material;
		RPolygonF4();
	};

	struct CompRPolygonF4
	{
		RBool operator() (const RPolygonF4& polyA, const RPolygonF4& polyB)
		{
			RFloat zA = Max(polyA.transformList[0].z, Max(polyA.transformList[1].z, polyA.transformList[2].z));
			RFloat zB = Max(polyB.transformList[0].z, Max(polyB.transformList[1].z, polyB.transformList[2].z));

			if (Abs(zA - zB) < 0.005f)
			{
				zA = (polyA.transformList[0].z + polyA.transformList[1].z + polyA.transformList[2].z) / 3.0f;
				zB = (polyB.transformList[0].z + polyB.transformList[1].z + polyB.transformList[2].z) / 3.0f;

				if (zA <= zB)
					return false;
				else
					return true;
			}
			else if (zA < zB)
			{
				return false;
			}
			else
				return true;
		}
	};

	struct RObject4
	{
		RString		name;
		RInt		state;
		RInt		attribute;
		RFloat		avgRadius;
		RFloat		maxRadius;

		RBool		needCull;
		RString		materialName;
		RVector4	minBoundingBox;
		RVector4	maxBoundingBox;

		RVector4	worldPosition;
		RVector4	scale;

		RVector4	direction;

		RInt		vertexNumber;
		RVertex4Array	localList;
		RVertex4Array	transformList;

		RInt		polygonNumber;
		RPolygon4Array	polygonList;
		RObject4	*nextObject;

		RObject4();
	};

	struct RRenderList4
	{
		RInt	state;
		RInt	attribute;

		std::list<RPolygonF4> polyData;
		typedef std::list<RPolygonF4>::iterator Iter;
	};

	enum OBJ_TRANSFORM_TYPE
	{
		TRANSFORM_LOCAL = 0,
		TRANSFORM_TRANS = 1,
		TRANSFORM_LOCAL_TO_TRANS = 2,
	};

	void Transform_Object4(RObject4* object, const RMatrix4x4& mat, OBJ_TRANSFORM_TYPE transformType);

	void Transform_Renderlist4(RRenderList4* renderList, const RMatrix4x4& mat, OBJ_TRANSFORM_TYPE transformType);

	void Insert_Object4_To_RenderList4(RRenderList4* renderList, RObject4* object, OBJ_TRANSFORM_TYPE transformType = TRANSFORM_LOCAL_TO_TRANS, bool transformScaleAndPosition = false);

	void Local_To_World_Object4(RObject4* object, OBJ_TRANSFORM_TYPE transformType = TRANSFORM_LOCAL_TO_TRANS);

	void Local_To_World_RenderList4(RRenderList4* renderList, const RVector4& pos, OBJ_TRANSFORM_TYPE transformType = TRANSFORM_LOCAL_TO_TRANS);

	bool Cull_Object4(RObject4* object, RFrustum* camera, CULL_TYPE cullType = CULL_SPHERE);

	bool Remove_Backface(RPolygon4* poly, RFrustum* camera);
	bool Remove_Backface(RPolygonF4* poly, RFrustum* camera);

	void Remove_Backface_Object4(RObject4* object, RFrustum* camera);

	void Remove_Backface_RenderList4(RRenderList4* renderList, RFrustum* camera);

	void Light_PolygonF4(RPolygonF4* poly, RFrustum* camera);
	void Light_RenderList4(RRenderList4* renderList, RFrustum* camera);

	void World_To_Camera_Object4(RObject4* object, RFrustum* camera);
	void World_To_Camera_RenderList4(RRenderList4* renderList, RFrustum* camera);

	void Sort_RenderList4(RRenderList4* renderList);

	void Camera_To_Perspective_Object4(RObject4* object, RFrustum* camera);
	void Camera_To_Perspective_RenderList4(RRenderList4* renderList, RFrustum* camera);

	void Perspective_To_Screen_Object4(RObject4* object, RFrustum* camera);
	void Perspective_To_Screen_RenderList4(RRenderList4* renderList, RFrustum* camera);

	void Clip_RenderList4(RRenderList4* renderList, RFrustum* camera);

	void World_To_Screen_RenderList4(RRenderList4* renderList, RFrustum* camera);

	void Draw_Objcet4_Wire(RObject4* object);
	void Draw_RenderList4_Wire(RRenderList4* renderList);

	void Draw_Object4_Solid(RObject4* object);
	void Draw_RenderList4_Solid(RRenderList4* renderList);

	void Draw_RenderList4_Texture_Solid(RRenderList4* renderList);
}