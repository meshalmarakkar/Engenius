#pragma once

#include <glm/gtc/type_ptr.hpp>
#include "Uniforms.h"
#include "Material.h"

class RenderProperties_Basic {
public:
	RenderProperties_Basic(glm::mat4* modelMatrix);
	glm::mat4* getModelMatrix();
	void setModelMatrix(glm::mat4* newModelMat);

private:
	glm::mat4* modelMatrix;
};

class RenderProperties_Uniforms : public RenderProperties_Basic{
public:
	RenderProperties_Uniforms(glm::mat4* modelMatrix);
	Uniforms* getUniforms();

private:
	Uniforms uniforms;
};

class RenderProperties : public RenderProperties_Uniforms {
public:
	RenderProperties(Material* material, glm::mat4* modelMatrix);
	Material* getMaterial();

private:
	Material * material;
	
};
