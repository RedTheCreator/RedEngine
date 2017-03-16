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
				for (RInt i = 0; i < obj->vertexNumber; i++)
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
				for (RInt i = 0; i < obj->vertexNumber; i++)
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
			for (RRenderList4::Iter iter = renderList->polyData.begin(); iter != renderList->polyData.end(); ++iter)
			{
				poly = &(*iter);
				GetVertex4MulMatrix4x4(poly->localList[0], mat, poly->localList[0]);
				GetVertex4MulMatrix4x4(poly->localList[1], mat, poly->localList[1]);
				GetVertex4MulMatrix4x4(poly->localList[2], mat, poly->localList[2]);
			}
		}
		break;
		case TRANSFORM_TRANS:
		{
			for (RRenderList4::Iter iter = renderList->polyData.begin(); iter != renderList->polyData.end(); ++iter)
			{
				poly = &(*iter);
				GetVertex4MulMatrix4x4(poly->transformList[0], mat, poly->transformList[0]);
				GetVertex4MulMatrix4x4(poly->transformList[1], mat, poly->transformList[1]);
				GetVertex4MulMatrix4x4(poly->transformList[2], mat, poly->transformList[2]);
			}
		}
		break;
		case TRANSFORM_LOCAL_TO_TRANS:
		{
			for (RRenderList4::Iter iter = renderList->polyData.begin(); iter != renderList->polyData.end(); ++iter)
			{
				poly = &(*iter);
				GetVertex4MulMatrix4x4(poly->localList[0], mat, poly->transformList[0]);
				GetVertex4MulMatrix4x4(poly->localList[1], mat, poly->transformList[1]);
				GetVertex4MulMatrix4x4(poly->localList[2], mat, poly->transformList[2]);
			}
		}
		break;
		default:
			break;
		}
	}

	void Insert_Object4_To_RenderList4(RRenderList4* renderList, RObject4* object, OBJ_TRANSFORM_TYPE transformType /* = TRANSFORM_LOCAL_TO_TRANS */, bool transformScaleAndPosition /* = false */)
	{
		if (!object)
			return;
		RVector4 scale(1, 1, 1);
		RVector4 pos(0, 0, 0);

		if (transformScaleAndPosition)
		{
			scale = object->scale;
			pos = object->worldPosition;
		}

		RLight *light = NULL;
		for (LightIter iter = GLights->begin(); iter != GLights->end(); ++iter)
		{
			light = *iter;
			light->transPosition = light->position;
			light->transDirection = light->direction;
		}

		RObject4 *obj = object;
		RPolygon4 *base;
		RPolygonF4 poly;

		switch (transformType)
		{
		case TRANSFORM_LOCAL:
		{
			while (obj)
			{
				for (RInt i = 0; i < obj->polygonNumber; i++)
				{
					base = &obj->polygonList[i];
					poly.attribute = base->attribute;
					poly.state = RPOLY_STATE_ACTIVE;
					poly.color = base->color;

					poly.localList[0] = obj->localList[base->verIndex[0]];
					poly.localList[1] = obj->localList[base->verIndex[1]];
					poly.localList[2] = obj->localList[base->verIndex[2]];

					poly.material = GetMaterial(obj->materialName);
					renderList->polyData.push_back(poly);
				}
				obj = obj->nextObject;
			}
		}
		break;
		case TRANSFORM_LOCAL_TO_TRANS:
		{
			while (obj)
			{
				for (RInt i = 0; i < obj->polygonNumber; i++)
				{
					base = &obj->polygonList[i];
					poly.attribute = base->attribute;
					poly.state = RPOLY_STATE_ACTIVE;
					poly.color = base->color;

					poly.transformList[0] = obj->localList[base->verIndex[0]] * scale + pos;
					poly.transformList[1] = obj->localList[base->verIndex[1]] * scale + pos;
					poly.transformList[2] = obj->localList[base->verIndex[2]] * scale + pos;

					poly.material = GetMaterial(obj->materialName);
					renderList->polyData.push_back(poly);
				}
				obj = obj->nextObject;
			}
		}
		break;
		case TRANSFORM_TRANS:
		{
			while (obj)
			{
				for (RInt i = 0; i < obj->polygonNumber; i++)
				{
					base = &obj->polygonList[i];
					poly.attribute = base->attribute;
					poly.state = RPOLY_STATE_ACTIVE;
					poly.color = base->color;

					poly.transformList[0] = obj->transformList[base->verIndex[0]] * scale + pos;
					poly.transformList[1] = obj->transformList[base->verIndex[1]] * scale + pos;
					poly.transformList[2] = obj->transformList[base->verIndex[2]] * scale + pos;

					poly.material - GetMaterial(obj->materialName);
					renderList->polyData.push_back(poly);
				}
				obj = obj->nextObject;
			}
		}
		break;
		default:
			break;
		}
	}

	void Local_To_World_Object4(RObject4* object, OBJ_TRANSFORM_TYPE transformType /* = TRANSFORM_LOCAL_TO_TRANS */)
	{
		if (!object)
			return;
		RObject4* obj = object;
		RVector4 pos = obj->worldPosition;
		if (TRANSFORM_LOCAL_TO_TRANS == transformType)
		{
			while (obj)
			{
				for (RInt i = 0; i < obj->vertexNumber; i++)
				{
					GetVertex4AddVector4(obj->localList[i] * obj->scale, pos, obj->transformList[i]);
				}
				obj = obj->nextObject;
			}
		}
		else if (TRANSFORM_TRANS == transformType)
		{
			while (obj)
			{
				for (RInt i = 0; i < obj->vertexNumber; i++)
				{
					GetVertex4AddVector4(obj->transformList[i], pos, obj->transformList[i]);
				}
				obj = obj->nextObject;
			}
		}
	}

	void Local_To_World_RenderList4(RRenderList4* renderList, const RVector4& pos, OBJ_TRANSFORM_TYPE transformType /* = TRANSFORM_LOCAL_TO_TRANS */)
	{
		RPolygonF4 *poly = NULL;
		switch (transformType)
		{
		case TRANSFORM_LOCAL:
		{
			for (RRenderList4::Iter iter = renderList->polyData.begin(); iter != renderList->polyData.end(); ++iter)
			{
				poly = &(*iter);
				GetVertex4AddVector4(poly->localList[0], pos, poly->localList[0]);
				GetVertex4AddVector4(poly->localList[1], pos, poly->localList[1]);
				GetVertex4AddVector4(poly->localList[2], pos, poly->localList[2]);
			}
		}
		break;
		case TRANSFORM_TRANS:
		{
			for (RRenderList4::Iter iter = renderList->polyData.begin(); iter != renderList->polyData.end(); ++iter)
			{
				poly = &(*iter);
				GetVertex4AddVector4(poly->transformList[0], pos, poly->transformList[0]);
				GetVertex4AddVector4(poly->transformList[1], pos, poly->transformList[1]);
				GetVertex4AddVector4(poly->transformList[2], pos, poly->transformList[2]);
			}
		}
		break;
		case TRANSFORM_LOCAL_TO_TRANS:
		{
			for (RRenderList4::Iter iter = renderList->polyData.begin(); iter != renderList->polyData.end(); ++iter)
			{
				poly = &(*iter);
				GetVertex4AddVector4(poly->localList[0], pos, poly->transformList[0]);
				GetVertex4AddVector4(poly->localList[1], pos, poly->transformList[1]);
				GetVertex4AddVector4(poly->localList[2], pos, poly->transformList[2]);
			}
		}
		break;
		default:
			break;
		}
	}

	bool Cull_Object4(RObject4* object, RFrustum* camera, CULL_TYPE cullType /* = CULL_SPHERE */)
	{
		if (!object)
			return true;
		if (!object->needCull)
			return false;

		RVector4 centerPos;
		GetVector4MulMatrix4x4(object->worldPosition, camera->mWorldToCamera, centerPos);

		RFloat scaleRadiusZ;
		RFloat scaleRadiusX;
		RFloat scaleRadiusY;

		if (cullType == CULL_SPHERE)
		{
			scaleRadiusZ = object->maxRadius * object->scale.z;
			scaleRadiusX = object->maxRadius * object->scale.x;
			scaleRadiusY = object->maxRadius * object->scale.y;
		}
		else if (cullType == CULL_BOX)
		{
			scaleRadiusZ = object->maxBoundingBox.z * object->scale.z;
			scaleRadiusY = object->maxBoundingBox.y * object->scale.y;
			scaleRadiusX = object->maxBoundingBox.x * object->scale.x;
		}
		else
		{
			return true;
		}

		if ((centerPos.z + scaleRadiusZ) < camera->clip_z_near || (centerPos.z - scaleRadiusZ) > camera->clip_z_far)
			return true;

		RFloat x_test = 0.5f * camera->viewplane_width * centerPos.z / camera->view_dist;
		if ((centerPos.x - scaleRadiusX) > x_test || (centerPos.x + scaleRadiusX) < x_test)
			return true;
		RFloat y_test = 0.5f * camera->viewplane_height * centerPos.z / camera->view_dist;
		if ((centerPos.y - scaleRadiusY) > y_test || (centerPos.y + scaleRadiusY) < -y_test)
			return true;

		return false;
	}

	void World_To_Camera_Object4(RObject4* object, RFrustum* camera)
	{
		RObject4 *obj = object;
		while (obj)
		{
			for (RInt i = 0; i < obj->vertexNumber; i++)
			{
				GetVertex4MulMatrix4x4(obj->transformList[i], camera->mWorldToCamera, obj->transformList[i]);
			}
			obj = obj->nextObject;
		}
	}

	void World_To_Camera_RenderList4(RRenderList4* renderList, RFrustum* camera)
	{
		RMatrix4x4 mat = camera->mWorldToCamera;

		mat.m[3][0] = mat.m[3][1] = mat.m[3][2] = 0;

		RLight *light = NULL;
		for (LightIter iter = GLights->begin(); iter != GLights->end(); ++iter)
		{
			light = *iter;
			GetVector4MulMatrix4x4(light->position, mat, light->transPosition);
			GetVector4MulMatrix4x4(light->direction, mat, light->transDirection);
		}

		RPolygonF4 *poly = NULL;
		for (RRenderList4::Iter iter = renderList->polyData.begin(); iter != renderList->polyData.end(); ++iter)
		{
			poly = &(*iter);
			GetVertex4MulMatrix4x4(poly->transformList[0], camera->mWorldToCamera, poly->transformList[0]);
			GetVertex4MulMatrix4x4(poly->transformList[1], camera->mWorldToCamera, poly->transformList[1]);
			GetVertex4MulMatrix4x4(poly->transformList[2], camera->mWorldToCamera, poly->transformList[2]);
		}
	}

	void Sort_RenderList4(RRenderList4* renderList)
	{
		renderList->polyData.sort(CompRPolygonF4());
	}

	bool Remove_Backface(RPolygon4* poly, RFrustum* camera)
	{
		RInt index0 = poly->verIndex[0];
		RInt index1 = poly->verIndex[1];
		RInt index2 = poly->verIndex[2];

		RVector4 u = (poly->verlist->at(index0) - poly->verlist->at(index1)).toVector4();
		RVector4 v = (poly->verlist->at(index0) - poly->verlist->at(index2)).toVector4();

		RVector4 n = u.crossProduct(v);
		RVector4 view = camera->position - poly->verlist->at(index0).toVector4();

		RFloat dot = n.dotProduct(view);
		if (dot < 0.0f)
		{
			poly->state |= RPOLY_STATE_BACKFACE;
			return true;
		}
		return false;
	}

	bool Remove_Backface(RPolygonF4* poly, RFrustum* camera)
	{
		if (!(poly->state&RPOLY_STATE_ACTIVE) || (poly->state & RPOLY_STATE_BACKFACE) || (poly->state & RPOLY_STATE_CLIPPED))
		{
			return true;
		}

		RVector4 u = (poly->transformList[0] - poly->transformList[1]).toVector4();
		RVector4 v = (poly->transformList[0] - poly->transformList[2]).toVector4();

		RVector4 n = u.crossProduct(v);
		RVector4 view = camera->position - poly->transformList[0].toVector4();

		RFloat dot = n.dotProduct(view);
		if (dot < 0.0f)
		{
			poly->state |= RPOLY_STATE_BACKFACE;
			return true;
		}
		return false;
	}

	void Remove_Backface_Object4(RObject4* object, RFrustum* camera)
	{
		RObject4* obj = object;
		RPolygon4* poly = NULL;
		while (obj)
		{
			if (!(obj->state & ROBJECT_STATE_CULLED))
			{
				for (RInt i = 0; i < obj->polygonNumber; i++)
				{
					poly = &obj->polygonList[i];
					Remove_Backface(poly, camera);
				}
			}
			obj = obj->nextObject;
		}
	}

	void Remove_Backface_RenderList4(RRenderList4* renderList, RFrustum* camera)
	{
		RPolygonF4* poly = NULL;
		for (RRenderList4::Iter iter = renderList->polyData.begin(); iter != renderList->polyData.end();)
		{
			poly = &(*iter);

			if (Remove_Backface(poly, camera))
				renderList->polyData.erase(iter++);
			else
				++iter;
		}
	}

	void Light_PolygonF4(RPolygonF4* poly, RFrustum* camera)
	{
		RBool lightOn = false;
		RColor baseColor(poly->color);

		RInt sumColorR = 0, sumColorG = 0, sumColorB = 0;

		RVector4 u = (poly->transformList[0] - poly->transformList[1]).toVector4();
		RVector4 v = (poly->transformList[0] - poly->transformList[2]).toVector4();

		RVector4 n = u.crossProduct(v);
		RFloat length = n.length();

		RLight* light = NULL;
		for (LightIter iter = GLights->begin(); iter != GLights->end(); ++iter)
		{
			light = *iter;
			if (!light->lightOn)
				continue;

			lightOn = true;

			if (light->lightType == LIGHT_AMBIENT)
			{
				sumColorR += light->ambient.r * baseColor.r >> 8;
				sumColorG += light->ambient.g * baseColor.g >> 8;
				sumColorB += light->ambient.b * baseColor.b >> 8;
			}
			else if (light->lightType == LIGHT_DIRECTION)
			{
				RFloat dot = n.dotProduct(-light->transDirection);

				if (dot > 0.0f)
				{
					RFloat dirLength = light->transDirection.length();

					RFloat temp = light->power * dot / (length * dirLength * 256);
					sumColorR += temp * light->diffuse.r * baseColor.r;
					sumColorG += temp * light->diffuse.g * baseColor.g;
					sumColorB += temp * light->diffuse.b * baseColor.b;
				}
				else
				{
					sumColorR += baseColor.r * light->shadowFactor;
					sumColorG += baseColor.g * light->shadowFactor;
					sumColorB += baseColor.b * light->shadowFactor;
				}
			}
			else if (light->lightType == LIGHT_POINT)
			{
				RVector4 dir = light->transPosition - poly->transformList[0].toVector4();
				RFloat dot = n.dotProduct(dir);
				if (dot > 0.0f)
				{
					RFloat dist = dir.length();
					RFloat atten = light->kc + light->kl * dist + light->kq * dist * dist;
					RFloat temp = dot / (length * dist * atten * 256);

					sumColorR += light->diffuse.r * baseColor.r * temp;
					sumColorG += light->diffuse.g * baseColor.g * temp;
					sumColorB += light->diffuse.b * baseColor.b * temp;
				}
				else
				{
					sumColorR += baseColor.r * light->shadowFactor;
					sumColorG += baseColor.g * light->shadowFactor;
					sumColorB += baseColor.b * light->shadowFactor;
				}
			}
		}
		if (lightOn)
		{
			if (sumColorR > 255) sumColorR = 255;
			if (sumColorG > 255) sumColorG = 255;
			if (sumColorB > 255) sumColorB = 255;

			poly->color = RCOLOR_16BIT(sumColorR, sumColorG, sumColorB);
		}
	}

	void Light_RenderList4(RRenderList4* renderList, RFrustum* camera)
	{
		RPolygonF4* poly = NULL;

		for (RRenderList4::Iter iter = renderList->polyData.begin(); iter != renderList->polyData.end(); ++iter)
		{
			poly = &(*iter);
			Light_PolygonF4(poly, camera);
		}
	}

	void Camera_To_Perspective_Object4(RObject4* object, RFrustum* camera)
	{
		if (!object)
			return;

		RObject4* obj = object;
		RFloat d = camera->view_dist;
		RFloat temp = 0;
		while (obj)
		{
			for (RInt i = 0; i < obj->vertexNumber; i++)
			{
				temp = d / obj->transformList[i].z;
				obj->transformList[i].x = temp * obj->transformList[i].x;
				obj->transformList[i].y = camera->aspect_ratio * temp * obj->transformList[i].y;
			}
			obj = obj->nextObject;
		}
	}

	void Camera_To_Perspective_RenderList4(RRenderList4* renderList, RFrustum* camera)
	{
		RFloat d = camera->view_dist;
		RFloat temp = 0;
		RPolygonF4* poly = NULL;

		for (RRenderList4::Iter iter = renderList->polyData.begin(); iter != renderList->polyData.end(); ++iter)
		{
			poly = &(*iter);

			for (RInt i = 0; i < 3; i++)
			{
				temp = d / poly->transformList[i].z;
				poly->transformList[i].x = temp * poly->transformList[i].x;
				poly->transformList[i].y = camera->aspect_ratio * temp * poly->transformList[i].y;
			}
		}
	}

	void Perspective_To_Screen_Object4(RObject4* object, RFrustum* camera)
	{
		if (!object)
			return;

		RFloat XOffset = 0.5f * (camera->viewport_width - 1);
		RFloat YOffset = 0.5f * (camera->viewport_height - 1);

		RObject4* obj = object;

		while (obj)
		{
			for (RInt i = 0; i < obj->vertexNumber; i++)
			{
				obj->transformList[i].x = XOffset * (1 + obj->transformList[i].x);
				obj->transformList[i].y = YOffset * (1 - obj->transformList[i].y);
			}
			obj = obj->nextObject;
		}
	}

	void Perspective_To_Screen_RenderList4(RRenderList4* renderList, RFrustum* camera)
	{
		RFloat XOffset = 0.5f * (camera->viewport_width - 1);
		RFloat YOffset = 0.5f * (camera->viewport_height - 1);

		RPolygonF4* poly = NULL;
		for (RRenderList4::Iter iter = renderList->polyData.begin(); iter != renderList->polyData.end(); ++iter)
		{
			poly = &(*iter);

			for (RInt i = 0; i < 3; i++)
			{
				poly->transformList[i].x = XOffset * (1 + poly->transformList[i].x);
				poly->transformList[i].y = YOffset * (1 - poly->transformList[i].y);
			}
		}
	}

	RBool isClipped(RPolygonF4* poly, const RPlane3& plane)
	{
		RInt flag = 0;
		for (RInt i = 0; i < 3; i++)
		{
			RFloat temp = plane.normal.dotProduct(poly->transformList[i].toVector4() - plane.point);

			if (temp < 0.0f)
				flag++;
		}
		if (flag == 3)
			return true;
		else
			return false;
	}

	RBool isClipped(RPolygonF4* point, RFloat znear, RFloat zfar)
	{
		RInt num = 0;
		for (RInt i = 0; i < 3; i++)
		{
			if (point->transformList[i].z < znear || point->transformList[i].z > zfar)
				num++;
		}
		if (num == 3)
			return true;
		else
			return false;
	}

	RBool isClipped(RPolygonF4* poly, RFrustum* camera)
	{
		RVertex4 v0, v1, v2;
		GetVertex4MulMatrix4x4(poly->transformList[0], camera->mWorldToCamera, v0);
		GetVertex4MulMatrix4x4(poly->transformList[1], camera->mWorldToCamera, v1);
		GetVertex4MulMatrix4x4(poly->transformList[2], camera->mWorldToCamera, v2);

		if (camera->clip_z_near > v0.z && camera->clip_z_near > v1.z && camera->clip_z_near > v2.z ||
			camera->clip_z_far < v0.z && camera->clip_z_far < v1.z && camera->clip_z_far < v2.z ||

			camera->clip_plane_L.normal.dotProduct(v0.toVector4() - camera->clip_plane_L.point) < 0 &&
			camera->clip_plane_L.normal.dotProduct(v1.toVector4() - camera->clip_plane_L.point) < 0 &&
			camera->clip_plane_L.normal.dotProduct(v2.toVector4() - camera->clip_plane_L.point) < 0 ||

			camera->clip_plane_R.normal.dotProduct(v0.toVector4() - camera->clip_plane_R.point) < 0 &&
			camera->clip_plane_R.normal.dotProduct(v1.toVector4() - camera->clip_plane_R.point) < 0 &&
			camera->clip_plane_R.normal.dotProduct(v2.toVector4() - camera->clip_plane_R.point) < 0 ||

			camera->clip_plane_T.normal.dotProduct(v0.toVector4() - camera->clip_plane_T.point) < 0 &&
			camera->clip_plane_T.normal.dotProduct(v1.toVector4() - camera->clip_plane_T.point) < 0 &&
			camera->clip_plane_T.normal.dotProduct(v2.toVector4() - camera->clip_plane_T.point) < 0 ||

			camera->clip_plane_B.normal.dotProduct(v0.toVector4() - camera->clip_plane_B.point) < 0 &&
			camera->clip_plane_B.normal.dotProduct(v1.toVector4() - camera->clip_plane_B.point) < 0 &&
			camera->clip_plane_B.normal.dotProduct(v2.toVector4() - camera->clip_plane_B.point) < 0)
			return true;
		return false;
	}

	void Clip_RenderList4(RRenderList4* renderList, RFrustum* camera)
	{
		RPolygonF4* poly = NULL;
		for (RRenderList4::Iter iter = renderList->polyData.begin(); iter != renderList->polyData.end();)
		{
			poly = &(*iter);
			if (isClipped(poly, camera->clip_plane_L) ||
				isClipped(poly, camera->clip_plane_R) ||
				isClipped(poly, camera->clip_plane_B) ||
				isClipped(poly, camera->clip_plane_T))
			{
				renderList->polyData.erase(iter++);
				continue;
			}
			++iter;
		}
	}

	void World_To_Screen_RenderList4(RRenderList4* renderList, RFrustum* camera)
	{
		RFloat XOffset = 0.5f * (camera->viewport_width - 1);
		RFloat YOffset = 0.5f * (camera->viewport_height - 1);

		RFloat dist = camera->view_dist;
		RFloat temp = 0;

		RMatrix4x4 mat = camera->mWorldToCamera;

		mat.m[3][0] = mat.m[3][1] = mat.m[3][2] = 0.0f;
		mat.m[3][3] = 1.0f;

		RLight *light = NULL;

		for (LightIter iter = GLights->begin(); iter != GLights->end(); ++iter)
		{
			light = *iter;
			GetVector4MulMatrix4x4(light->position, mat, light->transPosition);
			GetVector4MulMatrix4x4(light->direction, mat, light->transDirection);
		}

		RPolygonF4 *poly = NULL;
		for (RRenderList4::Iter iter = renderList->polyData.begin(); iter != renderList->polyData.end();)
		{
			poly = &(*iter);
			if (Remove_Backface(poly, camera))
			{
				renderList->polyData.erase(iter++);
				continue;
			}
			GetVertex4MulMatrix4x4(poly->transformList[0], camera->mWorldToCamera, poly->transformList[0]);
			GetVertex4MulMatrix4x4(poly->transformList[1], camera->mWorldToCamera, poly->transformList[1]);
			GetVertex4MulMatrix4x4(poly->transformList[2], camera->mWorldToCamera, poly->transformList[2]);

			if (isClipped(poly, camera->clip_z_near, camera->clip_z_far) ||
				isClipped(poly, camera->clip_plane_L) ||
				isClipped(poly, camera->clip_plane_R) ||
				isClipped(poly, camera->clip_plane_T) ||
				isClipped(poly, camera->clip_plane_B))
			{
				renderList->polyData.erase(iter++);
				continue;
			}

			Light_PolygonF4(poly, camera);

			for (RInt i = 0; i < 3; i++)
			{
				temp = dist / poly->transformList[i].z;
				poly->transformList[i].x = temp * poly->transformList[i].x;
				poly->transformList[i].y = camera->aspect_ratio * temp * poly->transformList[i].y;

				poly->transformList[i].x = XOffset * (1 + poly->transformList[i].x);
				poly->transformList[i].y = YOffset * (1 + poly->transformList[i].y);
			}
			++iter;
		}
		Sort_RenderList4(renderList);
	}

	void Draw_Objcet4_Wire(RObject4* object)
	{
		if (!object)
			return;
		RObject4 *obj = object;
		while (obj)
		{
			for (RInt i = 0; i < obj->polygonNumber; i++)
			{
				if ((obj->polygonList[i].state & RPOLY_STATE_ACTIVE) &&
					!(obj->polygonList[i].state & RPOLY_STATE_CLIPPED) &&
					!(obj->polygonList[i].state & RPOLY_STATE_BACKFACE))
				{
					RInt index0 = obj->polygonList[i].verIndex[0];
					RInt index1 = obj->polygonList[i].verIndex[1];
					RInt index2 = obj->polygonList[i].verIndex[2];

					RedGraphics::drawLine(obj->transformList[index0].x, obj->transformList[index0].y, 
						obj->transformList[index1].x, obj->transformList[index1].y, RColor(obj->polygonList[i].color));
					RedGraphics::drawLine(obj->transformList[index1].x, obj->transformList[index1].y,
						obj->transformList[index2].x, obj->transformList[index2].y, RColor(obj->polygonList[i].color));
					RedGraphics::drawLine(obj->transformList[index0].x, obj->transformList[index0].y,
						obj->transformList[index2].x, obj->transformList[index2].y, RColor(obj->polygonList[i].color));
				}
				obj->polygonList[i].state = RPOLY_STATE_ACTIVE;
			}
			obj->state = ROBJECT_STATE_ACTIVE;
			obj = obj->nextObject;
		}
	}

	void Draw_RenderList4_Wire(RRenderList4* renderList)
	{
		RPolygonF4 *poly = NULL;
		for (RRenderList4::Iter iter = renderList->polyData.begin(); iter != renderList->polyData.end();++iter)
		{
			poly = &(*iter);

			RedGraphics::drawLine(poly->transformList[0].x, poly->transformList[0].y,
				poly->transformList[1].x, poly->transformList[1].y, RColor(poly->color));
			RedGraphics::drawLine(poly->transformList[1].x, poly->transformList[1].y,
				poly->transformList[2].x, poly->transformList[2].y, RColor(poly->color));
			RedGraphics::drawLine(poly->transformList[0].x, poly->transformList[0].y,
				poly->transformList[2].x, poly->transformList[2].y, RColor(poly->color));

			poly->state = RPOLY_STATE_ACTIVE;
		}
		renderList->polyData.clear();
	}

	void Draw_Object4_Solid(RObject4* object)
	{
		if (!object)
			return;
		RObject4 *obj = object;
		while (obj)
		{
			for (RInt i = 0; i < obj->polygonNumber;i++)
			{
				if ((obj->polygonList[i].state & RPOLY_STATE_ACTIVE) &&
					!(obj->polygonList[i].state & RPOLY_STATE_CLIPPED) &&
					!(obj->polygonList[i].state & RPOLY_STATE_BACKFACE))
				{
					RInt index0 = obj->polygonList[i].verIndex[0];
					RInt index1 = obj->polygonList[i].verIndex[1];
					RInt index2 = obj->polygonList[i].verIndex[2];

					RedGraphics::fillTriangle(
						obj->transformList[index0].x, obj->transformList[index0].y,
						obj->transformList[index1].x, obj->transformList[index1].y,
						obj->transformList[index2].x, obj->transformList[index2].y,
						RColor(obj->polygonList[i].color));
				}
				obj->polygonList[i].state = RPOLY_STATE_ACTIVE;
			}
			obj->state = ROBJECT_STATE_ACTIVE;
			obj = obj->nextObject;
		}
	}

	void Draw_RenderList4_Solid(RRenderList4* renderList)
	{
		RPolygonF4 *poly = NULL;
		for (RRenderList4::Iter iter = renderList->polyData.begin(); iter != renderList->polyData.end();++iter)
		{
			poly = &(*iter);
			RedGraphics::fillTriangle(
				poly->transformList[0].x, poly->transformList[0].y,
				poly->transformList[1].x, poly->transformList[1].y,
				poly->transformList[2].x, poly->transformList[2].y,
				RColor(poly->color));

			poly->state = RPOLY_STATE_ACTIVE;
		}
		renderList->polyData.clear();
	}

	//       v0
	//     /    \
	//    /      \
	//  v1 ------ v2
	void Draw_Top_Trangle(RFloat x0, RFloat y0, RFloat z0,
		RFloat u0, RFloat v0,
		RFloat x1, RFloat y1, RFloat z1,
		RFloat u1, RFloat v1,
		RFloat x2, RFloat y2, RFloat z2,
		RFloat u2, RFloat v2,
		RMaterial *mat, const RColor &color)
	{
		if (y2 < 0 || y0 > SCREEN_HEIGHT - 1)
			return;

		RFloat dxdl = (x0 - x1) / (y0 - y1);
		RFloat dxdr = (x0 - x2) / (y0 - y2);

		//---------
		RFloat dzdl = (z0 - z1) / (y0 - y1);
		RFloat dzdr = (z0 - z2) / (y0 - y2);

		RFloat startL = x0;
		RFloat startR = x0;

		RFloat begX = 0;
		RFloat endX = 0;

		if (!mat || !mat->useTexture())
		{
			if (y0 < 0)
				y0 = 0;
			if (y2 > SCREEN_HEIGHT - 1)
				y2 = SCREEN_HEIGHT - 1;

			for (RFloat y = y0; y < y2; y++)
			{
				//DrawLine(startL, y, startR, y, color);
				begX = startL;
				endX = startR;
				if (endX < begX) std::swap(begX, endX);

				if (begX < 0)
					begX = 0;
				if (endX > SCREEN_WIDTH - 1)
					endX = SCREEN_WIDTH - 1;

				for (RFloat x = begX; x < endX; x++)
				{
					if (RedGraphics::checkZ(x, y, z0))
						RedGraphics::setPixel(x, y, /*z0, */color);
				}

				startL += dxdl;
				startR += dxdr;
			}

			return;
		}

		RFloat dudl = (u0 - u1) / (y0 - y1);
		RFloat dvdl = (v0 - v1) / (y0 - y1);
		RFloat dudr = (u0 - u2) / (y0 - y2);
		RFloat dvdr = (v0 - v2) / (y0 - y2);

		RFloat startLU = u0;
		RFloat startLV = v0;
		RFloat startRU = u0;
		RFloat startRV = v0;

		RFloat begU = 0;
		RFloat endU = 0;
		RFloat begV = 0;
		RFloat endV = 0;

		RFloat dx = 0;
		RFloat ui = 0;
		RFloat vi = 0;

		RFloat zl = 0;
		RFloat zr = 0;
		RFloat z = 0;

		for (RFloat y = y0; y < y2; y++)
		{

			begX = (RInt)startL;
			endX = (RInt)startR;
			begU = startLU;
			endU = startRU;
			begV = startLV;
			endV = startRV;

			//------------
			zl = z0 + (y - y0) * dzdl;
			zr = z0 + (y - y0) * dzdr;

			if (endX < begX)
			{
				std::swap(begX, endX);
				std::swap(begU, endU);
				std::swap(begV, endV);

				//---------------
				std::swap(zl, zr);
			}

			if (begX < 0)
			{
				begU = begU - begX * (startLU - startRU) / (startL - startR);
				begX = 0;
			}
			if (endX > SCREEN_WIDTH - 1)
			{
				endU = endU - (endX - SCREEN_WIDTH) * (startLU - startRU) / (startL - startR);
				endX = SCREEN_WIDTH - 1;
			}

			dx = endX - begX;
			ui = (endU - begU) / dx;
			vi = (endV - begV) / dx;

			//----------------
			z = zl;
			RFloat zd = (zl - zr) / (begX - endX);
			for (RFloat x = begX; x <= endX; x++)
			{
				if (y > -1 && y < SCREEN_HEIGHT && RedGraphics::checkZ(x, y, z))
				{
					const RColor tex = mat->getPixel(begU, begV);
					RedGraphics::setPixel(x, y,/* z, */color * tex);
				}

				begU += ui;
				begV += vi;
				z += zd;
			}

			startL += dxdl;
			startR += dxdr;

			startLU += dudl;
			startLV += dvdl;
			startRU += dudr;
			startRV += dvdr;
		}
	}

	//  v1     v0
	//   ------
	//    \  /
	//     \/
	//     v2
	void Draw_Botton_Trangle(RFloat x0, RFloat y0, RFloat z0,
		RFloat u0, RFloat v0,
		RFloat x1, RFloat y1, RFloat z1,
		RFloat u1, RFloat v1,
		RFloat x2, RFloat y2, RFloat z2,
		RFloat u2, RFloat v2,
		RMaterial *mat, const RColor &color)
	{
		if (y2 < 0 || y0 > SCREEN_HEIGHT - 1)
			return;

		RFloat dxdl = (x1 - x2) / (y1 - y2);
		RFloat dxdr = (x0 - x2) / (y0 - y2);

		//---------
		RFloat dzdl = (z1 - z2) / (y1 - y2);
		RFloat dzdr = (z0 - z2) / (y0 - y2);

		RFloat startL = x1;
		RFloat startR = x0;

		RFloat begX = 0;
		RFloat endX = 0;

		if (!mat || !mat->useTexture())
		{
			if (y0 < 0)
				y0 = 0;
			if (y2 > SCREEN_HEIGHT - 1)
				y2 = SCREEN_HEIGHT - 1;

			for (RFloat y = y0; y < y2; y++)
			{

				begX = startL;
				endX = startR;
				if (endX < begX) std::swap(begX, endX);

				if (begX < 0)
					begX = 0;
				if (endX > SCREEN_WIDTH - 1)
					endX = SCREEN_WIDTH - 1;

				for (RFloat x = begX; x < endX; x++)
				{
					if (RedGraphics::checkZ(x, y, z0))
						RedGraphics::setPixel(x, y,/* z0, */color);
				}

				startL += dxdl;
				startR += dxdr;
			}

			return;
		}

		RFloat dudl = (u1 - u2) / (y1 - y2);
		RFloat dvdl = (v1 - v2) / (y1 - y2);
		RFloat dudr = (u0 - u2) / (y0 - y2);
		RFloat dvdr = (v0 - v2) / (y0 - y2);

		RFloat startLU = u1;
		RFloat startLV = v1;
		RFloat startRU = u0;
		RFloat startRV = v0;

		RFloat begU = 0;
		RFloat endU = 0;
		RFloat begV = 0;
		RFloat endV = 0;

		RFloat dx = 0;
		RFloat ui = 0;
		RFloat vi = 0;

		RFloat zl = 0;
		RFloat zr = 0;
		RFloat z = 0;
		for (RFloat y = y0; y < y2; y++)
		{

			begX = (RInt)startL;
			endX = (RInt)startR;
			begU = startLU;
			endU = startRU;
			begV = startLV;
			endV = startRV;

			//---------------
			zl = z1 + (y - y0) * dzdl;
			zr = z0 + (y - y0) * dzdr;

			if (endX < begX)
			{
				std::swap(begX, endX);
				std::swap(begU, endU);
				std::swap(begV, endV);

				//-------------------
				std::swap(zl, zr);
			}

			if (begX < 0)
			{
				begU = begU - begX * (startLU - startRU) / (startL - startR);
				begX = 0;
			}
			if (endX > SCREEN_WIDTH - 1)
			{
				endU = endU - (endX - SCREEN_WIDTH) * (startLU - startRU) / (startL - startR);
				endX = SCREEN_WIDTH - 1;
			}

			dx = endX - begX;
			ui = (endU - begU) / dx;
			vi = (endV - begV) / dx;

			//----------------
			z = zl;
			RFloat zd = (zl - zr) / (begX - endX);
			for (RFloat x = begX; x <= endX; x++)
			{
				if (y > -1 && y < SCREEN_HEIGHT && RedGraphics::checkZ(x, y, z))
				{
					const RColor tex = mat->getPixel(begU, begV);
					RedGraphics::setPixel(x, y,/* z, */color * tex);
				}

				begU += ui;
				begV += vi;
				z += zd;
			}

			startL += dxdl;
			startR += dxdr;

			startLU += dudl;
			startLV += dvdl;
			startRU += dudr;
			startRV += dvdr;
		}
	}

	void Draw_Triangle(RPolygonF4 *poly)
	{
		RFloat x0 = poly->transformList[0].x, y0 = poly->transformList[0].y, z0 = poly->transformList[0].z;
		RFloat u0 = poly->transformList[0].u, v0 = poly->transformList[0].v;
		RFloat x1 = poly->transformList[1].x, y1 = poly->transformList[1].y, z1 = poly->transformList[1].z;
		RFloat u1 = poly->transformList[1].u, v1 = poly->transformList[1].v;
		RFloat x2 = poly->transformList[2].x, y2 = poly->transformList[2].y, z2 = poly->transformList[2].z;
		RFloat u2 = poly->transformList[2].u, v2 = poly->transformList[2].v;

		if (y1 < y0)
		{
			std::swap(x0, x1);
			std::swap(y0, y1);
			std::swap(z0, z1);
			std::swap(u0, u1);
			std::swap(v0, v1);
		}

		if (y2 < y0)
		{
			std::swap(x0, x2);
			std::swap(y0, y2);
			std::swap(z0, z2);
			std::swap(u0, u2);
			std::swap(v0, v2);
		}

		if (y2 < y1)
		{
			std::swap(x1, x2);
			std::swap(y1, y2);
			std::swap(z1, z2);
			std::swap(u1, u2);
			std::swap(v1, v2);
		}

		RFloat factor = (y1 - y0) / (y2 - y0);
		RFloat newX = x0 + factor * (x2 - x0);
		RFloat newZ = z0 + factor * (z2 - z0);
		RFloat newU = u0 + factor * (u2 - u0);
		RFloat newV = v0 + factor * (v2 - v0);
		Draw_Top_Trangle(x0, y0, z0, u0, v0, x1, y1, z1, u1, v1, newX, y1, newZ, newU, newV, poly->material, RColor(poly->color));
		Draw_Botton_Trangle(newX, y1, newZ, newU, newV, x1, y1, z1, u1, v1, x2, y2, z2, u2, v2, poly->material, RColor(poly->color));
	}

	void Draw_RenderList4_Texture_Solid(RRenderList4* renderList)
	{
		RPolygonF4 *poly = NULL;
		for (RRenderList4::Iter iter = renderList->polyData.begin(); iter != renderList->polyData.end();++iter)
		{
			poly = &(*iter);
			
			Draw_Triangle(poly);
			poly->state = RPOLY_STATE_ACTIVE;
		}

		renderList->polyData.clear();
	}
}