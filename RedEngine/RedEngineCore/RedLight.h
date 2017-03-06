#pragma once

#include "RedCommon.h"
#include "RedVector.h"

namespace Red
{
	enum LIGHT_TYPE
	{
		LIGHT_AMBIENT = 0,
		LIGHT_DIRECTION = 1,
		LIGHT_POINT = 2,
	};

	struct RLight
	{
		RInt		id;
		RBool		lightOn;
		LIGHT_TYPE	lightType;
		RFloat		power;
		RFloat		shadowFactor;

		RColor		ambient;
		RColor		diffuse;
		RColor		specular;

		RVector4	position;
		RVector4	direction;

		RVector4	transPosition;
		RVector4	transDirection;

		RFloat		kc, kl, kq;

		RFloat		spot_inner;
		RFloat		spot_outer;

		RFloat		pf;

		RLight(RInt lightId, LIGHT_TYPE type);
	};

#define MAX_LIGHTS 8

	extern std::vector<RLight*> *GLights;

	typedef std::vector<RLight*>::iterator LightIter;

	extern RInt		CreateLight(LIGHT_TYPE lightType);
	extern RLight*	GetLight(RInt id);
	extern RInt		GetLightSize();

	extern void		DestroyAllLights();
}