#include "Skybox.h"
#include "TextureLoader.h"

Skybox::Skybox(unsigned int _skyboxProgram, Model* modelForSkybox) : skyboxProgram(_skyboxProgram), skyboxModel(modelForSkybox) {
	skyboxTexture = TextureLoader::loadCubeMap(nightSkyTexFiles);
	modelMatrix = glm::scale(modelMatrix, glm::vec3(1.5f, 1.5f, 1.5f));
}

void Skybox::renderSkybox(const glm::mat4 projection, const glm::mat4 view) {
	glDepthMask(GL_FALSE); // make sure writing to update depth test is off
	glCullFace(GL_FRONT); // drawing inside of cube!

	glUseProgram(skyboxProgram);
	glm::mat3 mvRotOnlyMat3 = glm::mat3(view); //removes translation part of view
	glUniformMatrix4fv(glGetUniformLocation(skyboxProgram, "view"), 1, GL_FALSE, glm::value_ptr(glm::mat4(mvRotOnlyMat3)));
	glUniformMatrix4fv(glGetUniformLocation(skyboxProgram, "projection"), 1, GL_FALSE, glm::value_ptr(projection));

	glUniformMatrix4fv(glGetUniformLocation(skyboxProgram, "model"), 1, GL_FALSE, glm::value_ptr(modelMatrix));
	
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_CUBE_MAP, skyboxTexture);
	skyboxModel->Draw(skyboxProgram, modelMatrix); //true for shadowRender so we can add texture here
	glBindTexture(GL_TEXTURE_CUBE_MAP, 0);

	glCullFace(GL_BACK);
	glDepthMask(GL_TRUE);
}

void Skybox::renderSkybox(const glm::mat4 projection, const glm::mat4 view, unsigned int texture) {
	glDepthMask(GL_FALSE); // make sure writing to update depth test is off
	glCullFace(GL_FRONT); // drawing inside of cube!

	glUseProgram(skyboxProgram);
	glm::mat3 mvRotOnlyMat3 = glm::mat3(view); //removes translation part of view
	glUniformMatrix4fv(glGetUniformLocation(skyboxProgram, "view"), 1, GL_FALSE, glm::value_ptr(glm::mat4(mvRotOnlyMat3)));
	glUniformMatrix4fv(glGetUniformLocation(skyboxProgram, "projection"), 1, GL_FALSE, glm::value_ptr(projection));

	glUniformMatrix4fv(glGetUniformLocation(skyboxProgram, "model"), 1, GL_FALSE, glm::value_ptr(modelMatrix));

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_CUBE_MAP, texture);
	skyboxModel->Draw(skyboxProgram, modelMatrix); //true for shadowRender so we can add texture here
	glBindTexture(GL_TEXTURE_CUBE_MAP, 0);

	glCullFace(GL_BACK);
	glDepthMask(GL_TRUE);
}
