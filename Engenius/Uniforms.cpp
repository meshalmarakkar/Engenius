#include "Uniforms.h"

Uniforms::Uniforms() {

}

Uniforms::~Uniforms() {

}

void Uniforms::addUniform(const std::string& name, const float* info) {
	uniforms_float.push_back(std::pair<const std::string, const float*>(name, info));
}
void Uniforms::addUniform(const std::string& name, const bool* info) {
	uniforms_bool.push_back(std::pair<const std::string, const bool*>(name, info));
}
void Uniforms::addUniform(const std::string& name, const int* info) {
	uniforms_int.push_back(std::pair<const std::string, const int*>(name, info));
}
void Uniforms::addUniform(const std::string& name, const glm::vec3* info) {
	uniforms_vec3.push_back(std::pair<const std::string, const glm::vec3*>(name, info));
}
void Uniforms::addUniform(const std::string& name, const glm::vec4* info) {
	uniforms_vec4.push_back(std::pair<const std::string, const glm::vec4*>(name, info));
}
void Uniforms::addUniform(const std::string& name, const glm::mat3* info) {
	uniforms_mat3.push_back(std::pair<const std::string, const glm::mat3*>(name, info));
}
void Uniforms::addUniform(const std::string& name, const glm::mat4* info) {
	uniforms_mat4.push_back(std::pair<const std::string, const glm::mat4*>(name, info));
}

const std::vector<std::pair<const std::string, const float*>>* Uniforms::getUniforms_float() const {
	return &uniforms_float;
}
const std::vector<std::pair<const std::string, const bool*>>* Uniforms::getUniforms_bool() const {
	return &uniforms_bool;
}
const std::vector<std::pair<const std::string, const int*>>* Uniforms::getUniforms_int() const {
	return &uniforms_int;
}
const std::vector<std::pair<const std::string, const glm::vec3*>>* Uniforms::getUniforms_vec3() const {
	return &uniforms_vec3;
}
const std::vector<std::pair<const std::string, const glm::vec4*>>* Uniforms::getUniforms_vec4() const {
	return &uniforms_vec4;
}
const std::vector<std::pair<const std::string, const glm::mat3*>>* Uniforms::getUniforms_mat3() const {
	return &uniforms_mat3;
}
const std::vector<std::pair<const std::string, const glm::mat4*>>* Uniforms::getUniforms_mat4() const {
	return &uniforms_mat4;
}