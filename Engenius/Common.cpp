#include "Common.h"

namespace Common {
	int loadTexture(const std::string type, const std::string path)
	{
		std::string directory = path.substr(0, path.find_last_of('/'));
		directory = directory + "/" + type;
		const char* pathToFile = directory.c_str();

		unsigned int texID;
		glGenTextures(1, &texID); // generate texture ID

		int width, height;
		unsigned char* image = SOIL_load_image(pathToFile, &width, &height, 0, SOIL_LOAD_RGBA);

		if (image) {
			glBindTexture(GL_TEXTURE_2D, texID);
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);

			glGenerateMipmap(GL_TEXTURE_2D);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); //or GL_CLAMP_TO_EDGE
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_LOD_BIAS, -0.4f);
			glBindTexture(GL_TEXTURE_2D, 0);

			SOIL_free_image_data(image);
		}
		else
		{
			std::cout << "Texture failed to load at path: " << pathToFile << std::endl;
			SOIL_free_image_data(image);
		}
		return texID;	// return value of texture ID
	}

	int loadTextureNonTransparent(const std::string type, const std::string path)
	{
		std::string directory = path.substr(0, path.find_last_of('/'));
		directory = directory + "/" + type;
		const char* pathToFile = directory.c_str();

		unsigned int texID;
		glGenTextures(1, &texID); // generate texture ID

		int width, height;
		unsigned char* image = SOIL_load_image(pathToFile, &width, &height, 0, SOIL_LOAD_RGB);

		if (image) {
			glBindTexture(GL_TEXTURE_2D, texID);
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);

			glGenerateMipmap(GL_TEXTURE_2D);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); //or GL_CLAMP_TO_EDGE
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_LOD_BIAS, -0.4f);
			glBindTexture(GL_TEXTURE_2D, 0);

			SOIL_free_image_data(image);
		}
		else
		{
			std::cout << "Texture failed to load at path: " << pathToFile << std::endl;
			SOIL_free_image_data(image);
		}
		return texID;	// return value of texture ID
	}

	unsigned int loadCubeMap(const char *faces[6]) {
		unsigned int texID;
		glGenTextures(1, &texID); // generate texture ID
		glBindTexture(GL_TEXTURE_CUBE_MAP, texID);

		int width, height;
		for (unsigned int i = 0; i < 6; i++)
		{
			unsigned char *image = SOIL_load_image(faces[i], &width, &height, 0, SOIL_LOAD_RGB);
			if (image)
			{
				glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
					0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image
				); //GL_FLOAT for more precision?
				SOIL_free_image_data(image);
			}
			else
			{
				std::cout << "Cubemap texture failed to load at path: " << faces[i] << std::endl;
				SOIL_free_image_data(image);
			}
		}
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

		glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
		return texID;
	}

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

	void bindTexture(const unsigned int shader, const unsigned int texture, const GLchar* name) {
		/*glActiveTexture(GL_TEXTURE0 + num_nextAvailableTex);
		glBindTexture(GL_TEXTURE_2D, texture);
		glUniform1i(glGetUniformLocation(shader, name), num_nextAvailableTex);
		num_nextAvailableTex++;*/
	}

	void bindCubeMapTexture(const unsigned int shader, const unsigned int texture, const GLchar* name) {
		/*	glActiveTexture(GL_TEXTURE0 + num_nextAvailableTex);
		glBindTexture(GL_TEXTURE_CUBE_MAP, texture);
		glUniform1i(glGetUniformLocation(shader, name), num_nextAvailableTex);
		num_nextAvailableTex++;*/
	}

	void unbindTextures(unsigned int num) {
		glActiveTexture(GL_TEXTURE0 + num);
		glBindTexture(GL_TEXTURE_2D, 0);

	}

	void unbindTextures(unsigned int first, unsigned int last) {
		for (unsigned int i = first; i < last + 1; i++) {
			glActiveTexture(GL_TEXTURE0 + i);
			glBindTexture(GL_TEXTURE_2D, 0);
		}
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

