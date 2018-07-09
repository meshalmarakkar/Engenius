#ifndef COMMON
#define COMMON

#include <GL/glew.h>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <string>
#include <iostream>

//#define SCREENWIDTH 800.0f
//#define SCREENHEIGHT 600.0f
//#define SCREENWIDTH 1024.0f
//#define SCREENHEIGHT 768.0f
#define DEG_TO_RADIAN 0.017453293f
#define PI 3.14159265359f

namespace Common {
	glm::vec3 moveForwardOrBack(const glm::vec3 pos, const GLfloat angle, const GLfloat d);
	glm::vec3 moveLateral(const glm::vec3 pos, const GLfloat angle, const GLfloat d);
	void createModelMatrix(glm::mat4 &model, glm::vec3 position);
	void createModelMatrix(glm::mat4 &model, glm::vec3 position, float yaw);
	void createModelMatrix(glm::mat4 &model, glm::vec3 position, glm::vec3 scale);
	void createModelMatrix(glm::mat4 &model, glm::vec3 position, float yaw, glm::vec3 scale);
	void createModelMatrix(glm::mat4 &model, glm::vec3 position, float pitch, float yaw, glm::vec3 scale);
	void createModelMatrix(glm::mat4 &model, glm::vec3 position, glm::vec3 scale, glm::vec3 rotation);
	void enableVertexAttribArray(unsigned int first, unsigned int last);
	void disableVertexAttribArray(unsigned int first, unsigned int last);
	void enableVertexAttribArray(unsigned int num);
	void disableVertexAttribArray(unsigned int num);


	void coutVector(std::string name, const glm::vec3 i);
	void coutVector(std::string name, const glm::vec4 i);
	void coutTransformOfMatrix(std::string name, const glm::mat4 i);
};

#endif

