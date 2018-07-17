#pragma once
#include <vector>
#include <string>

namespace TextureNames {
	const std::string diffuse = "texture_diffuse1";
	const std::string specular = "texture_specular1";
	const std::string normal = "texture_normal1";
};

struct TextureType {	
	std::string name;
	unsigned int tex;
	TextureType(const std::string &name, const unsigned int &tex) : name(name), tex(tex) { ; }
};

class Material{
public:
	Material(const float& tiling = 1.0f);
	void addTexture_2D(const TextureType& tex);
	void addTexture_Cubemap(const TextureType& tex);
	std::vector<TextureType>* getTextures_2D();
	std::vector<TextureType>* getTextures_Cubemap();
	float getTiling();
	bool getIfSpecular();
	bool getIfNormalMapped();

private:
	float tiling;
	std::vector<TextureType> tex_2D;
	std::vector<TextureType> tex_Cubemap;
	bool ifSpecularMap;
	bool ifNormalMap;
};

class Material_PBR : public Material{
public:
	Material_PBR(const unsigned int& tex_dif, const unsigned int& tex_spec, const unsigned int& tex_norm, const float& tiling = 1.0f);

private:

};