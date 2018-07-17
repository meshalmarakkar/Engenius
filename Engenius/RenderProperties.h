#pragma once

#include <glm/gtc/type_ptr.hpp>
#include "Uniforms.h"
#include "Material.h"

class RenderProperties_Basic {
public:
	RenderProperties_Basic(Material* material, glm::mat4* modelMatrix);
	~RenderProperties_Basic();

	glm::mat4* getModelMatrix();

	Material* getMaterial();

private:
	Material * material;
	glm::mat4* modelMatrix;
};

class RenderProperties : public RenderProperties_Basic {
public:
	RenderProperties(Material* material, glm::mat4* modelMatrix);
	~RenderProperties();

	Uniforms* getUniforms();

private:
	Uniforms uniforms;
};
