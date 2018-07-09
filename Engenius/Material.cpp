#include "Material.h"

Material::Material(unsigned int tex_dif, unsigned int tex_spec, unsigned int tex_norm, glm::mat4 modelMatrix, float tiling) : 
	tex_diffuse(tex_dif), tex_specular(tex_spec), tex_normal(tex_norm), modelMatrix(modelMatrix), tiling(tiling){

}

unsigned int Material::getTex_diffuse() {
	return tex_diffuse;
}
unsigned int Material::getTex_specular() {
	return tex_specular;
}
unsigned int Material::getTex_normal() {
	return tex_normal;
}
float Material::getTiling() {
	return tiling;
}

void Material::setModelMatrix(glm::vec3 pos, glm::vec3 scale, glm::vec3 rotation) {
	;
}
void Material::setModelMatrix(glm::mat4 newMatrix) {
	modelMatrix = newMatrix;
}
glm::mat4* Material::getModelMatrix() {
	return &modelMatrix;
}