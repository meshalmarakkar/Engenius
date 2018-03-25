#ifndef SKYBOX
#define SKYBOX

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "SDL.h"
#include "Common.h"
#include <iostream>

#include "Model.h"

class Skybox {
public:
	Skybox(GLuint _skyboxProgram, Model* modelForSkybox);
	void renderSkybox(const glm::mat4 projection, const glm::mat4 view);
	void renderSkybox(const glm::mat4 projection, const glm::mat4 view, GLuint texture);

private:
	GLuint skyboxProgram;
	GLuint skyboxTexture;
	const char *nightSkyTexFiles[6] = {
		"../Engenius/star-skybox/sky_right.bmp", 
		"../Engenius/star-skybox/sky_left.bmp", 
		"../Engenius/star-skybox/sky_top.bmp", 
		"../Engenius/star-skybox/sky_bot.bmp", 
		"../Engenius/star-skybox/sky_back.bmp", 
		"../Engenius/star-skybox/sky_front.bmp"
	};
	Model* skyboxModel;//to render skybox
	glm::mat4 modelMatrix;
};
#endif
