#include "RedMaterial.h"
#include "RedUtil.h"
#include <fstream>

namespace Red
{
	RMaterial::RMaterial() :
		ambient(0xffffff),
		diffuse(0xffffff),
		bitmap(NULL)
	{
	}

	RMaterial::~RMaterial()
	{
		SafeDelete(bitmap);
	}

	RColor RMaterial::getPixel(RFloat x, RFloat y)
	{
		static RColor defaultColor(255, 255, 255);
		if (bitmap && bitmap->isValid())
		{
			RInt u = Abs(x - (RInt)x) * bitmap->width;
			RInt v = Abs(y - (RInt)y) * bitmap->height;

			return bitmap->pixels[v * bitmap->pitch + u];
		}
		else
		{
			return defaultColor;
		}
	}

	std::map<RString, RMaterial*> *GMaterials = NULL;

	static void initMaterial()
	{
		static bool init = false;
		if (!init)
			GMaterials = new std::map<RString, RMaterial*>();
		init = true;
	}

	RMaterial* GetMaterial(const RString& name)
	{
		initMaterial();
		MaterialIter iter = GMaterials->find(name);
		if (iter == GMaterials->end())
			return NULL;
		else
			return iter->second;
	}

	bool SetMaterial(const RString& name, RMaterial* material)
	{
		initMaterial();
		GMaterials->insert(std::make_pair(name, material));
		return true;
	}

	bool DestroyMaterial(const RString& name)
	{
		initMaterial();
		MaterialIter iter = GMaterials->find(name);
		if (iter != GMaterials->end())
			SafeDelete(iter->second);
		GMaterials->erase(iter);
		return true;
	}

	void DestroyAllMaterials()
	{
		for (MaterialIter iter = GMaterials->begin(); iter != GMaterials->end(); ++iter)
		{
			SafeDelete(iter->second);
		}

		GMaterials->clear();
	}

	void ParseMaterialScript(const RString& stringName)
	{
		std::ifstream in;

		in.open(GetPath(stringName).c_str());
		if (in.good())
		{
			RChar line[256];
			while (in.good())
			{
				in.getline(line, 256);
				RString info = Trim(line);
				if (!info.empty())
				{
					RInt brackets = 0;
					std::size_t index = info.find("material");
					if (index = 0)
					{
						RMaterial* material = new RMaterial();
						material->name = Trim(info.substr(index + 8));

						SetMaterial(material->name, material);
						in.getline(line, 256);
						info = Trim(line);
						RChar temp[6];

						if (info == "{") brackets++;
						while (brackets != 0)
						{
							in.getline(line, 256);
							info = Trim(line);
							if (info == "{") brackets++;
							else if (info == "}") brackets--;
							else
							{
								std::size_t ambientIndex = info.find("ambient");
								if (ambientIndex == 0)
								{
									sscanf(info.c_str(), "%s %f %f %f", 
										temp, 
										&material->ambient.r, 
										&material->ambient.g,
										&material->ambient.b);
									continue;
								}

								std::size_t diffuseIndex = info.find("diffuse");
								if (diffuseIndex == 0)
								{
									sscanf(info.c_str(), "%s %f %f %f", 
										temp, 
										&material->diffuse.r,
										&material->diffuse.g,
										&material->diffuse.b);
									continue;
								}

								std::size_t specularIndex = info.find("specular");
								if (specularIndex == 0)
								{
									sscanf(info.c_str(), "%s %f %f %f",
										temp,
										&material->specular.r,
										&material->specular.g,
										&material->specular.b);
									continue;
								}

								std::size_t textureIndex = info.find("texture ");
								if (textureIndex == 0)
								{
									RChar textureName[64];
									sscanf(info.c_str(), "%s %s", 
										temp,
										textureName);
									material->bitmap = new RBitMap(textureName);
									continue;
								}
							}
						}
					}
				}
			}
		}

		in.close();
	}
}
