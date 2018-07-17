#pragma once

#include <glm/gtc/type_ptr.hpp>
#include <vector>
#include <string>

class Uniforms {
public:
	Uniforms();
	~Uniforms();

	void addUniform(const std::string& name, const float* info);
	void addUniform(const std::string& name, const bool* info);
	void addUniform(const std::string& name, const int* info);
	void addUniform(const std::string& name, const glm::vec3* info);
	void addUniform(const std::string& name, const glm::vec4* info);
	void addUniform(const std::string& name, const glm::mat3* info);
	void addUniform(const std::string& name, const glm::mat4* info);

	const std::vector<std::pair<const std::string, const float*>>* getUniforms_float() const;
	const std::vector<std::pair<const std::string, const bool*>>* getUniforms_bool() const;
	const std::vector<std::pair<const std::string, const int*>>* getUniforms_int() const;
	const std::vector<std::pair<const std::string, const glm::vec3*>>* getUniforms_vec3() const;
	const std::vector<std::pair<const std::string, const glm::vec4*>>* getUniforms_vec4() const;
	const std::vector<std::pair<const std::string, const glm::mat3*>>* getUniforms_mat3() const;
	const std::vector<std::pair<const std::string, const glm::mat4*>>* getUniforms_mat4() const;

private:
	std::vector<std::pair<const std::string, const float*>> uniforms_float;
	std::vector<std::pair<const std::string, const bool*>> uniforms_bool;
	std::vector<std::pair<const std::string, const int*>> uniforms_int;
	std::vector<std::pair<const std::string, const glm::vec3*>> uniforms_vec3;
	std::vector<std::pair<const std::string, const glm::vec4*>> uniforms_vec4;
	std::vector<std::pair<const std::string, const glm::mat3*>> uniforms_mat3;
	std::vector<std::pair<const std::string, const glm::mat4*>> uniforms_mat4;
};