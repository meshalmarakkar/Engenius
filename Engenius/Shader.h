#pragma once

#include <glm/gtc/type_ptr.hpp>
#include <GL/glew.h>

#include "Uniforms.h"

class Shader {
public:
	Shader(GLuint program);
	~Shader();

	void uniformsToShader_INIT();
	void uniformsToShader_RUNTIME();

	void bind();
	void unbind();

	void uniform(const std::string& name, const float& info);
	void uniform(const std::string& name, const int& info);
	void uniform(const std::string& name, const glm::vec3& info);
	void uniform(const std::string& name, const glm::vec4& info);
	void uniform(const std::string& name, const glm::mat3& info);
	void uniform(const std::string& name, const glm::mat4& info);

	void bindTex_2D(const char* name, const GLuint& tex);
	void bindTex_Cubemap(const char* name, const GLuint& tex);
	void reset_bindTex_2D(const char* name, const GLuint& tex);
	void reset_texCount();

	void uniformsToShader(const Uniforms* uni);

	Uniforms* getInitUniforms();
	Uniforms* getRunTimeUniforms();
	GLuint getShaderProgram();

private:
	GLuint program;
	unsigned int nextAvailableTex;

	Uniforms uniforms_init;
	Uniforms uniforms_runtime;
};