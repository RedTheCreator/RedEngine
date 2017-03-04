#pragma once

#include "RedCommon.h"
#include <map>
#include "RedGraphics.h"

namespace Red
{
	struct RMaterial
	{
		RString name;
		RColor	ambient;
		RColor	diffuse;
		RColor	specular;

		RBitMap	*bitmap;

		RMaterial();
		~RMaterial();
		inline bool useTexture() { return bitmap && bitmap->isValid(); }
		RColor getPixel(RFloat x, RFloat y);
	};

	typedef std::map<RString, RMaterial*>::iterator MaterialIter;
	extern	std::map<RString, RMaterial*> *GMaterials;

	extern	RMaterial* GetMaterial(const RString& name);
	extern	bool SetMaterial(const RString& name, RMaterial *material);
	extern	bool DestroyMaterial(const RString& name);
	extern	void DestroyAllMaterials();

	extern void ParseMaterialScript(const RString& stringName);

}