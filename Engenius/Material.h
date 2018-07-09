#pragma once

#include <glm/gtc/type_ptr.hpp>

class Material{
public:
	Material(unsigned int tex_dif, unsigned int tex_spec, unsigned int tex_norm, glm::mat4 modelMatrix, float tiling = 1.0f);
	unsigned int getTex_diffuse();
	unsigned int getTex_specular();
	unsigned int getTex_normal();
	float getTiling();

	void setModelMatrix(glm::vec3 pos, glm::vec3 scale, glm::vec3 rotation);
	void setModelMatrix(glm::mat4 newMatrix);
	glm::mat4* getModelMatrix();

private:
	unsigned int tex_diffuse;
	unsigned int tex_specular;
	unsigned int tex_normal;
	float tiling;
	
	glm::mat4 modelMatrix;
};