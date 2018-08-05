#ifndef SKYBOX
#define SKYBOX

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Shader.h"
#include "Model.h"
#include "Renderer.h"

class Skybox {
public:
	Skybox(Shader* skyboxProgram, Model* modelForSkybox);
	void renderSkybox(const glm::mat4& projection, const glm::mat4& view, Renderer* renderer);
	void renderSkybox(const glm::mat4& projection, const glm::mat4& view, Renderer* renderer, const unsigned int& texture);

private:
	unsigned int skyboxTexture;
	/*const char *nightSkyTexFiles[6] = {
		"../Engenius/Textures/starfield/starfield_rt.png",
		"../Engenius/Textures/starfield/starfield_lf.png",
		"../Engenius/Textures/starfield/starfield_up.png",
		"../Engenius/Textures/starfield/starfield_dn.png",
		"../Engenius/Textures/starfield/starfield_bk.png",
		"../Engenius/Textures/starfield/starfield_ft.png"
	};*/
	const char *nightSkyTexFiles[6] = {
		"../Engenius/Textures/defaultSkybox.png",
		"../Engenius/Textures/defaultSkybox.png",
		"../Engenius/Textures/defaultSkybox.png",
		"../Engenius/Textures/defaultSkybox.png",
		"../Engenius/Textures/defaultSkybox.png",
		"../Engenius/Textures/defaultSkybox.png"
	};
	Model* skyboxModel;//to render skybox
	glm::mat4 modelMatrix;

	Shader* shaderProgram;
};
#endif
