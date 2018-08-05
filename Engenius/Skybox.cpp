#include "Skybox.h"
#include "TextureLoader.h"

Skybox::Skybox(Shader* program, Model* modelForSkybox) : shaderProgram(program), skyboxModel(modelForSkybox) {
	skyboxTexture = TextureLoader::loadCubeMap(nightSkyTexFiles);
	modelMatrix = glm::scale(modelMatrix, glm::vec3(1.5f, 1.5f, 1.5f));
}

void Skybox::renderSkybox(const glm::mat4& projection, const glm::mat4& view, Renderer* renderer) {
	renderer->depthMask_false(); // make sure writing to update depth test is off
	renderer->setCullFace_Front(); // drawing inside of cube!

	shaderProgram->bind();
	glm::mat3 mvRotOnlyMat3 = glm::mat3(view); //removes translation part of view
	shaderProgram->uniform("view", glm::mat4(mvRotOnlyMat3));
	shaderProgram->uniform("projection", projection);
	shaderProgram->uniform("model", modelMatrix);

	shaderProgram->reset_texCount();
	shaderProgram->bindTex_Cubemap("cubeMap", skyboxTexture);
	for (unsigned int i = 0; i < skyboxModel->getMeshes()->size(); i++) {
		Mesh& m = skyboxModel->getMeshes()->at(i);
		m.getVAO()->bind();
		renderer->drawElements(m.getVAO());
	}

	renderer->setCullFace_Back();
	renderer->depthMask_true();
}

void Skybox::renderSkybox(const glm::mat4& projection, const glm::mat4& view, Renderer* renderer, const unsigned int& texture) {
	renderer->depthMask_false(); // make sure writing to update depth test is off
	renderer->setCullFace_Front(); // drawing inside of cube!

	shaderProgram->bind();
	//glUseProgram(shaderProgram->getShaderProgram());
	glm::mat3 mvRotOnlyMat3 = glm::mat3(view); //removes translation part of view
	shaderProgram->uniform("view", glm::mat4(mvRotOnlyMat3));
	shaderProgram->uniform("projection", projection);
	shaderProgram->uniform("model", modelMatrix);

	shaderProgram->reset_texCount();
	shaderProgram->bindTex_Cubemap("cubeMap", texture);
	for (unsigned int i = 0; i < skyboxModel->getMeshes()->size(); i++) {
		Mesh& m = skyboxModel->getMeshes()->at(i);
		m.getVAO()->bind();
		renderer->drawElements(m.getVAO());
	}

	renderer->setCullFace_Back();
	renderer->depthMask_true();
}
