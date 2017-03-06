#include "RedLight.h"

namespace Red
{
	RLight::RLight(RInt lightId, LIGHT_TYPE type)
		:id(lightId),
		lightOn(true),
		lightType(type),
		ambient(255, 255, 255),
		diffuse(255, 255, 255),
		specular(255, 255, 255),
		position(0, 0, 0),
		direction(0, 0, 1),
		kc(1.0f),
		kl(1.0f),
		kq(0.0f),
		spot_inner(45.0f),
		spot_outer(60.0f),
		power(1.0f),
		shadowFactor(0.05f)
	{
	}

	std::vector<RLight*> *GLights = NULL;

	RInt CreateLight(LIGHT_TYPE lightType)
	{
		if (GLights == NULL)
			GLights = new std::vector<RLight*>;
		if ((RInt)GLights->size() > MAX_LIGHTS)
			return -1;
		RInt id = (RInt)GLights->size();
		GLights->push_back(new RLight(id, lightType));

		return id;
	}

	RLight* GetLight(RInt id)
	{
		if (GLights == NULL || (RInt)GLights->size() > MAX_LIGHTS || id < 0 || id>(RInt)GLights->size() - 1)
			return NULL;

		return GLights->at(id);
	}

	RInt GetLightSize()
	{
		return GLights->size();
	}
	
	void DestroyAllLights()
	{
		for (LightIter iter = GLights->begin(); iter != GLights->end(); ++iter)
		{
			SafeDelete(*iter);
		}
		GLights->clear();
		SafeDelete(GLights);
	}
}