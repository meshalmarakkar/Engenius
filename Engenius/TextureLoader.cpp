#include "TextureLoader.h"
#include "SOIL.h"
#include <GL/glew.h>
#include <iostream>

namespace TextureLoader{

	int loadTexture(const std::string& type, const std::string& path)
	{
		std::string directory = path.substr(0, path.find_last_of('/'));
		directory = directory + "/" + type;
		const char* pathToFile = directory.c_str();

		unsigned int texID = 0;

		int width, height;
		unsigned char* image = SOIL_load_image(pathToFile, &width, &height, 0, SOIL_LOAD_RGBA);

		if (image) {
			glGenTextures(1, &texID); // generate texture ID
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

	int loadTextureNonTransparent(const std::string& type, const std::string& path)
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
};