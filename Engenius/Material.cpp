#include "Material.h"

Material::Material(const float& tiling) : tiling(tiling), ifSpecularMap(false), ifNormalMap(false) { }

void Material::addTexture_2D(const TextureType& tex) {
	if ((tex.name).c_str() == TextureNames::specular)
		ifSpecularMap = true;
	else if ((tex.name).c_str() == TextureNames::normal)
		ifNormalMap = true;
	tex_2D.push_back(tex);
}

void Material::addTexture_Cubemap(const TextureType& tex) {
	tex_Cubemap.push_back(tex);
}

std::vector<TextureType>* Material::getTextures_2D() {
	return &tex_2D;
}

std::vector<TextureType>* Material::getTextures_Cubemap() {
	return &tex_Cubemap;
}

float Material::getTiling() {
	return tiling;
}

bool Material::getIfSpecular() {
	return ifSpecularMap;
}

bool Material::getIfNormalMapped() {
	return ifNormalMap;
}
