#include "Common.h"

namespace Common {
	glm::vec3 moveForwardOrBack(const glm::vec3 pos, const GLfloat angle, const GLfloat d) {
		return glm::vec3(pos.x + d * std::sin(angle*DEG_TO_RADIAN), pos.y, pos.z - d * std::cos(angle*DEG_TO_RADIAN));
		//	return glm::vec3(pos.x + d*std::sin(angle*DEG_TO_RADIAN), pos.y - d*std::sin(angle), pos.z - d*std::cos(angle*DEG_TO_RADIAN));
	}

	glm::vec3 moveLateral(const glm::vec3 pos, const GLfloat angle, const GLfloat d) {
		return glm::vec3(pos.x + d * std::cos(angle*DEG_TO_RADIAN), pos.y, pos.z + d * std::sin(angle*DEG_TO_RADIAN));
	}

	void createModelMatrix(glm::mat4 &model, glm::vec3 position) {
		model = glm::translate(model, position);
	}
	void createModelMatrix(glm::mat4 &model, glm::vec3 position, float yaw) {
		model = glm::translate(model, position);
		model = glm::rotate(model, float(yaw * DEG_TO_RADIAN), glm::vec3(0.0f, 1.0f, 0.0f));
	}
	void createModelMatrix(glm::mat4 &model, glm::vec3 position, glm::vec3 scale) {
		model = glm::translate(model, position);
		model = glm::scale(model, scale);
	}
	void createModelMatrix(glm::mat4 &model, glm::vec3 position, float yaw, glm::vec3 scale) {
		model = glm::translate(model, position);
		model = glm::rotate(model, float(yaw * DEG_TO_RADIAN), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, scale);
	}
	void createModelMatrix(glm::mat4 &model, glm::vec3 position, float pitch, float yaw, glm::vec3 scale) {
		model = glm::translate(model, position);
		model = glm::rotate(model, float(yaw * DEG_TO_RADIAN), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::rotate(model, float(pitch * DEG_TO_RADIAN), glm::vec3(1.0f, 0.0f, 0.0f));
		//model = glm::rotate(model, float(objectsInWorld.at(iter)->getRoll() * DEG_TO_RADIAN), glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::scale(model, scale);
	}
	void createModelMatrix(glm::mat4 &model, glm::vec3 position, glm::vec3 scale, glm::vec3 rotation) {
		model = glm::translate(model, position);
		model = glm::rotate(model, float(rotation.y * DEG_TO_RADIAN), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::rotate(model, float(rotation.x * DEG_TO_RADIAN), glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::rotate(model, float(rotation.z * DEG_TO_RADIAN), glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::scale(model, scale);
	}

	void enableVertexAttribArray(unsigned int first, unsigned int last) {
		for (unsigned int i = first; i < last + 1; i++)
			glEnableVertexAttribArray(i);
	}
	void enableVertexAttribArray(unsigned int num) {
		glEnableVertexAttribArray(num);
	}

	void disableVertexAttribArray(unsigned int first, unsigned int last) {
		for (unsigned int i = first; i < last + 1; i++)
			glDisableVertexAttribArray(i);
	}
	void disableVertexAttribArray(unsigned int num) {
		glDisableVertexAttribArray(num);
	}


	void coutVector(std::string name, const glm::vec3 i) {
		std::cout << name << ": " << i.x << ", " << i.y << ", " << i.z << std::endl;
	}
	void coutVector(std::string name, const glm::vec4 i) {
		std::cout << name << ": " << i.x << ", " << i.y << ", " << i.z << std::endl;
	}
	void coutTransformOfMatrix(std::string name, const glm::mat4 i) {
		std::cout << name << ": " << i[3][0] << ", " << i[3][1] << ", " << i[3][2] << std::endl;
	}
}

