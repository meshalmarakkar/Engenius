#include "Skybox.h"
#include "TextureLoader.h"

Skybox::Skybox(Shader* program, Model* modelForSkybox) : shaderProgram(program), skyboxModel(modelForSkybox) {
	skyboxTexture = TextureLoader::loadCubeMap(nightSkyTexFiles);
	modelMatrix = glm::scale(modelMatrix, glm::vec3(1.5f, 1.5f, 1.5f));
}

void Skybox::renderSkybox(const glm::mat4& projection, const glm::mat4& view, Renderer* renderer) {
	glDepthMask(GL_FALSE); // make sure writing to update depth test is off
	glCullFace(GL_FRONT); // drawing inside of cube!

	shaderProgram->bind();
	glm::mat3 mvRotOnlyMat3 = glm::mat3(view); //removes translation part of view
	shaderProgram->uniform("view", glm::mat4(mvRotOnlyMat3));
	shaderProgram->uniform("projection", projection);
	shaderProgram->uniform("model", modelMatrix);
	/*glUniformMatrix4fv(glGetUniformLocation(skyboxProgram, "view"), 1, GL_FALSE, glm::value_ptr(glm::mat4(mvRotOnlyMat3)));
	glUniformMatrix4fv(glGetUniformLocation(skyboxProgram, "projection"), 1, GL_FALSE, glm::value_ptr(projection));

	glUniformMatrix4fv(glGetUniformLocation(skyboxProgram, "model"), 1, GL_FALSE, glm::value_ptr(modelMatrix));*/
	
	//glActiveTexture(GL_TEXTURE0);
	//glBindTexture(GL_TEXTURE_CUBE_MAP, skyboxTexture);
	//skyboxModel->Draw(skyboxProgram, modelMatrix); //true for shadowRender so we can add texture here
	//glBindTexture(GL_TEXTURE_CUBE_MAP, 0);

	shaderProgram->reset_texCount();
	shaderProgram->bindTex_Cubemap("cubeMap", skyboxTexture);
	for (unsigned int i = 0; i < skyboxModel->getMeshes()->size(); i++) {
		Mesh& m = skyboxModel->getMeshes()->at(i);
		m.getVAO()->bind();
		renderer->drawElements(m.getVAO());
	}

	glCullFace(GL_BACK);
	glDepthMask(GL_TRUE);
}

void Skybox::renderSkybox(const glm::mat4& projection, const glm::mat4& view, Renderer* renderer, const unsigned int& texture) {
	glDepthMask(GL_FALSE); // make sure writing to update depth test is off
	glCullFace(GL_FRONT); // drawing inside of cube!

	shaderProgram->bind();
	//glUseProgram(shaderProgram->getShaderProgram());
	glm::mat3 mvRotOnlyMat3 = glm::mat3(view); //removes translation part of view
	shaderProgram->uniform("view", glm::mat4(mvRotOnlyMat3));
	shaderProgram->uniform("projection", projection);
	shaderProgram->uniform("model", modelMatrix);
	//glUniformMatrix4fv(glGetUniformLocation(skyboxProgram, "view"), 1, GL_FALSE, glm::value_ptr(glm::mat4(mvRotOnlyMat3)));
	//glUniformMatrix4fv(glGetUniformLocation(skyboxProgram, "projection"), 1, GL_FALSE, glm::value_ptr(projection));

	//glUniformMatrix4fv(glGetUniformLocation(skyboxProgram, "model"), 1, GL_FALSE, glm::value_ptr(modelMatrix));

	shaderProgram->reset_texCount();
	shaderProgram->bindTex_Cubemap("cubeMap", texture);
	for (unsigned int i = 0; i < skyboxModel->getMeshes()->size(); i++) {
		Mesh& m = skyboxModel->getMeshes()->at(i);
		m.getVAO()->bind();
		renderer->drawElements(m.getVAO());
	}
	
	//skyboxModel->Draw(skyboxProgram, modelMatrix); //true for shadowRender so we can add texture here
//	glBindTexture(GL_TEXTURE_CUBE_MAP, 0);

	glCullFace(GL_BACK);
	glDepthMask(GL_TRUE);
}
