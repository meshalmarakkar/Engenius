#include "Shader.h"
#include "ErrorCheck.h"

Shader::Shader(GLuint program) : program(program), nextAvailableTex(0) {

}

Shader::~Shader() {

}

void Shader::uniformsToShader_INIT() {
	uniformsToShader(&uniforms_init);
}

void Shader::uniformsToShader_RUNTIME() {
	uniformsToShader(&uniforms_runtime);
}

void Shader::bind() {
	GL_ERROR_CHECK(glUseProgram(program));
	reset_texCount();
}

void Shader::unbind() {
	GL_ERROR_CHECK(glUseProgram(0));
}

GLuint Shader::getShaderProgram() {
	return program;
}

void Shader::uniform(const std::string& name, const float& info) {
	GL_ERROR_CHECK(glUniform1f(glGetUniformLocation(program, (name).c_str()), info));
}
void Shader::uniform(const std::string& name, const unsigned int& info) {
	GL_ERROR_CHECK(glUniform1ui(glGetUniformLocation(program, (name).c_str()), info));
}
void Shader::uniform(const std::string& name, const int& info) {
	GL_ERROR_CHECK(glUniform1i(glGetUniformLocation(program, (name).c_str()), info));
}
void Shader::uniform(const std::string& name, const glm::vec3& info) {
	GL_ERROR_CHECK(glUniform3fv(glGetUniformLocation(program, (name).c_str()), 1, glm::value_ptr(info)));
}
void Shader::uniform(const std::string& name, const glm::vec4& info) {
	GL_ERROR_CHECK(glUniform4fv(glGetUniformLocation(program, (name).c_str()), 1, glm::value_ptr(info)));
}
void Shader::uniform(const std::string& name, const glm::mat3& info) {
	GL_ERROR_CHECK(glUniformMatrix3fv(glGetUniformLocation(program, (name).c_str()), 1, GL_FALSE, glm::value_ptr(info)));
}
void Shader::uniform(const std::string& name, const glm::mat4& info) {
	GL_ERROR_CHECK(glUniformMatrix4fv(glGetUniformLocation(program, (name).c_str()), 1, GL_FALSE, glm::value_ptr(info)));
}

void Shader::bindTex_2D(const char* name, const GLuint& tex){
	GL_ERROR_CHECK(glActiveTexture(GL_TEXTURE0 + nextAvailableTex));
	GL_ERROR_CHECK(glBindTexture(GL_TEXTURE_2D, tex));
	GL_ERROR_CHECK(glUniform1i(glGetUniformLocation(program, name), nextAvailableTex));
	nextAvailableTex++;
}

void Shader::bindTex_Cubemap(const char* name, const GLuint& tex) {
	GL_ERROR_CHECK(glActiveTexture(GL_TEXTURE0 + nextAvailableTex));
	GL_ERROR_CHECK(glBindTexture(GL_TEXTURE_CUBE_MAP, tex));
	GL_ERROR_CHECK(glUniform1i(glGetUniformLocation(program, name), nextAvailableTex));
	nextAvailableTex++;
}

void Shader::reset_bindTex_2D(const char* name, const GLuint& tex) {
	nextAvailableTex = 0;
	bindTex_2D(name, tex);
}

void Shader::reset_texCount() {
	nextAvailableTex = 0;
}

void Shader::uniformsToShader(const Uniforms* uni) {
	auto fl = uni->getUniforms_float();
	auto bo = uni->getUniforms_bool();
	auto in = uni->getUniforms_int();
	auto v3 = uni->getUniforms_vec3();
	auto v4 = uni->getUniforms_vec4();
	auto m3 = uni->getUniforms_mat3();
	auto m4 = uni->getUniforms_mat4();

	unsigned int i = 0;
	for (i = 0; i < fl->size(); i++) {
		uniform(fl->at(i).first, *(fl->at(i).second));
	}
	for (i = 0; i < bo->size(); i++) {
		uniform(bo->at(i).first, *(bo->at(i).second));
	}
	for (i = 0; i < in->size(); i++) {
		uniform(in->at(i).first, *(in->at(i).second));
	}
	for (i = 0; i < v3->size(); i++) {
		uniform(v3->at(i).first, *(v3->at(i).second));
	}
	for (i = 0; i < v4->size(); i++) {
		uniform(v4->at(i).first, *(v4->at(i).second));
	}
	for (i = 0; i < m3->size(); i++) {
		uniform(m3->at(i).first, *(m3->at(i).second));
	}
	for (i = 0; i < m4->size(); i++) {
		uniform(m4->at(i).first, *(m4->at(i).second));
	}
}

Uniforms* Shader::getInitUniforms() {
	return &uniforms_init;
}

Uniforms* Shader::getRunTimeUniforms() {
	return &uniforms_runtime;
}