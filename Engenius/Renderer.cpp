#include "Renderer.h"
#include "Common.h"
#include "Material.h"
#include <GL/glew.h>

Renderer::Renderer(WindowManager* _windowManager, ShaderManager* _shaderManager):windowManager(_windowManager), shaderManager(_shaderManager) {
	enableDepthTest();
	//glDepthMask(GL_TRUE); //write to depth buffer
	//glDepthFunc(GL_LESS);
	enableCullFace();

	enableBlend(); //blend and deferred shading....nope....
	setBlendFunction(BlendOptions::gl_one);

	VAO_screenQuad = new VertexArrayObject(6);
	VAO_screenQuad->genBuffer_andAddData(Buffer_Options::gl_array_buffer, sizeof(quadVertices), &quadVertices, Buffer_Options::gl_static_draw);
	VAO_screenQuad->enableVertexAttribArray(0, 2, Buffer_Options::gl_float, Buffer_Options::gl_false, 4 * sizeof(float), 0);
	VAO_screenQuad->enableVertexAttribArray(1, 2, Buffer_Options::gl_float, Buffer_Options::gl_false, 4 * sizeof(float), 2 * sizeof(float));

	initFBOs();
}

Renderer::~Renderer() {
	delete VAO_screenQuad;
	delete FBO_postProcess;
	delete FBO_pingpong[2];
	delete FBO_gBuffer;
}

void Renderer::initFBOs() {
	//--------POST_PROCESSING--------//
	const unsigned int width = windowManager->getScreenWidth();
	const unsigned int height = windowManager->getScreenHeight();
	FBO_postProcess = new Framebuffer(width, height, true);
	FBO_postProcess->createTexturesAndAttach(2, width, height, Tex_Options::gl_linear, Tex_Options::gl_linear, Tex_Options::gl_clamp_to_edge, Tex_Options::gl_clamp_to_edge);

	//--------BLOOM / BLURRING--------//
	FBO_pingpong[0] = new Framebuffer(width, height, false);
	FBO_pingpong[0]->createTexturesAndAttach(1, width, height, Tex_Options::gl_linear, Tex_Options::gl_linear, Tex_Options::gl_clamp_to_edge, Tex_Options::gl_clamp_to_edge);

	FBO_pingpong[1] = new Framebuffer(width, height, false);
	FBO_pingpong[1]->createTexturesAndAttach(1, width, height, Tex_Options::gl_linear, Tex_Options::gl_linear, Tex_Options::gl_clamp_to_edge, Tex_Options::gl_clamp_to_edge);

	//--------DEFERRED_RENDERING--------//
	FBO_gBuffer = new Framebuffer(width, height, true);
	//positions
	FBO_gBuffer->createSingleTexture(Tex_Options::gl_rgb16f, width, height, Tex_Options::gl_rgb, Tex_Options::gl_float, Tex_Options::gl_nearest, Tex_Options::gl_nearest);
	//normals
	FBO_gBuffer->createSingleTexture(Tex_Options::gl_rgb16f, width, height, Tex_Options::gl_rgb, Tex_Options::gl_float, Tex_Options::gl_nearest, Tex_Options::gl_nearest);
	//albedospecular
	FBO_gBuffer->createSingleTexture(Tex_Options::gl_rgba, width, height, Tex_Options::gl_rgba, Tex_Options::gl_unsigned_byte, Tex_Options::gl_nearest, Tex_Options::gl_nearest);
	//pointLight ID
	FBO_gBuffer->createSingleTexture(Tex_Options::gl_rgb16, width, height, Tex_Options::gl_rgb, Tex_Options::gl_int, Tex_Options::gl_nearest, Tex_Options::gl_nearest);
	//spotLight ID
	FBO_gBuffer->createSingleTexture(Tex_Options::gl_rgb16, width, height, Tex_Options::gl_rgb, Tex_Options::gl_int, Tex_Options::gl_nearest, Tex_Options::gl_nearest);
	//TBN_T
	FBO_gBuffer->createSingleTexture(Tex_Options::gl_rgb16f, width, height, Tex_Options::gl_rgb, Tex_Options::gl_float, Tex_Options::gl_nearest, Tex_Options::gl_nearest);
	//TBN__B
	FBO_gBuffer->createSingleTexture(Tex_Options::gl_rgb16f, width, height, Tex_Options::gl_rgb, Tex_Options::gl_float, Tex_Options::gl_nearest, Tex_Options::gl_nearest);
	//TBN__N
	FBO_gBuffer->createSingleTexture(Tex_Options::gl_rgb16f, width, height, Tex_Options::gl_rgb, Tex_Options::gl_float, Tex_Options::gl_nearest, Tex_Options::gl_nearest);

	FBO_gBuffer->attachDrawBuffers();

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void Renderer::setupFrame_Deferred() {
	FBO_gBuffer->bind();
	FBO_gBuffer->edit_viewport(windowManager->getScreenWidth(), windowManager->getScreenHeight());
	FBO_gBuffer->clearBuffers();
	FBO_gBuffer->clearScreen();
}

void Renderer::setupFrame_PostProcess() {
	FBO_postProcess->bind();
	FBO_postProcess->edit_viewport(windowManager->getScreenWidth(), windowManager->getScreenHeight());
	FBO_postProcess->clearBuffers();
	FBO_postProcess->clearScreen();
}

void Renderer::setup_Screen() {
	//glDisable(GL_DEPTH_TEST); 
	VAO_screenQuad->bind();
}

void Renderer::toShader_Buffers(GLuint shader) {
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, FBO_gBuffer->getColourBuffer(0));
	glUniform1i(glGetUniformLocation(shader, "gPosition"), 0);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, FBO_gBuffer->getColourBuffer(1));
	glUniform1i(glGetUniformLocation(shader, "gNormal"), 1);
	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, FBO_gBuffer->getColourBuffer(2));
	glUniform1i(glGetUniformLocation(shader, "gAlbedoSpecular"), 2);
	glActiveTexture(GL_TEXTURE3);
	glBindTexture(GL_TEXTURE_2D, FBO_gBuffer->getColourBuffer(3));
	glUniform1i(glGetUniformLocation(shader, "gPointLightIDs"), 3);
	glActiveTexture(GL_TEXTURE4);
	glBindTexture(GL_TEXTURE_2D, FBO_gBuffer->getColourBuffer(4));
	glUniform1i(glGetUniformLocation(shader, "gSpotLightIDs"), 4);
	glActiveTexture(GL_TEXTURE5);
	glBindTexture(GL_TEXTURE_2D, FBO_gBuffer->getColourBuffer(5));
	glUniform1i(glGetUniformLocation(shader, "gTBN_T"), 5);
	glActiveTexture(GL_TEXTURE6);
	glBindTexture(GL_TEXTURE_2D, FBO_gBuffer->getColourBuffer(6));
	glUniform1i(glGetUniformLocation(shader, "gTBN_B"), 6);
	glActiveTexture(GL_TEXTURE7);
	glBindTexture(GL_TEXTURE_2D, FBO_gBuffer->getColourBuffer(7));
	glUniform1i(glGetUniformLocation(shader, "gTBN_N"), 7);
}

void Renderer::SS_Bloom() {
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

	unsigned int amount = 10;
	bool horizontal = true, first_iteration = true;
	glUseProgram(shaderManager->get_gaussianBlur_program());

	setup_Screen();
	
	for (unsigned int i = 0; i < amount; i++)
	{
		FBO_pingpong[horizontal]->bind();
		/*glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);*/
		glUniform1i(glGetUniformLocation(shaderManager->get_gaussianBlur_program(), ("horizontal")), horizontal);

		// bind texture of other framebuffer (or scene if first iteration)
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, first_iteration ? FBO_postProcess->getColourBuffer(1)/*colourBuffer[1]*/ : FBO_pingpong[!horizontal]->getColourBuffer(0)/*pingpongColorbuffers[!horizontal]*/);
		glUniform1i(glGetUniformLocation(shaderManager->get_gaussianBlur_program(), "textureToBlur"), 0);

		draw_screenQuad();
		horizontal = !horizontal;
		if (first_iteration)
			first_iteration = false;
	}

	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, FBO_pingpong[!horizontal]->getColourBuffer(0));// pingpongColorbuffers[!horizontal]);
	glUniform1i(glGetUniformLocation(shaderManager->get_postProcessing_program(), "bloomBlur"), 1);
}

void Renderer::SS_GodRays() {
	//	//GOD RAYS
	//	glUseProgram(shaderManager->get_firstPass_program());
	//
	//	glm::vec4 clipSpace = camera->getProjection() * (camera->getView()  * glm::vec4(lightingManager->getPointLightPosition(0), 1.0));
	//
	//	glm::vec3 ndcSpace = glm::vec3(clipSpace.x, clipSpace.y, clipSpace.z) / clipSpace.w;
	//
	//	glm::vec2 windowSpace = glm::vec4((((ndcSpace.x + 1.0f) / 2.0f) * windowManager->getScreenWidth()),
	//		(((ndcSpace.y + 1.0f) / 2.0f) * windowManager->getScreenHeight()), 0.0f, 1.0f);
	//
	//	glUniform3fv(glGetUniformLocation(shaderManager->get_firstPass_program(), "screenSpace_lightPos"), 1, glm::value_ptr(ndcSpace));
	//
	////	std::cout << "screenPos: " << windowSpace.x << ", " << windowSpace.y << std::endl;
	//
	//	glActiveTexture(GL_TEXTURE0);
	//	glBindTexture(GL_TEXTURE_2D, colourBuffer[1]);
	//	glUniform1i(glGetUniformLocation(shaderManager->get_firstPass_program(), "screenTexture"), 0);
	//
	//	glDrawArrays(GL_TRIANGLES, 0, 6);
}

void Renderer::draw_screenQuad() {
	setup_Screen();
	glDrawArrays(GL_TRIANGLES, 0, 6);
	unbindAllTextures();
}

void Renderer::draw_finalDisplay(const float& exposure, const int& ifBloom, const int& effectNo, const bool& ifPause, const GLuint& tex_pause, const bool& ifEndGame, const GLuint& tex_end) {
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

	setup_Screen();

	glUseProgram(shaderManager->get_postProcessing_program());
	glUniform1f(glGetUniformLocation(shaderManager->get_postProcessing_program(), ("exposure")), exposure);
	glUniform1i(glGetUniformLocation(shaderManager->get_postProcessing_program(), ("bloom")), ifBloom);
	glUniform1i(glGetUniformLocation(shaderManager->get_postProcessing_program(), ("effect")), effectNo);

	glActiveTexture(GL_TEXTURE0);
	if (ifPause)
		glBindTexture(GL_TEXTURE_2D, tex_pause);
	else if (ifEndGame) {
		glBindTexture(GL_TEXTURE_2D, tex_end);
	}
	else {
		//glBindTexture(GL_TEXTURE_2D, gNormal);
		//glBindTexture(GL_TEXTURE_2D, FBO_gBuffer->getColourBuffer(entityManager->getNum()));
		glBindTexture(GL_TEXTURE_2D, FBO_postProcess->getColourBuffer(0));
	}
	glUniform1i(glGetUniformLocation(shaderManager->get_postProcessing_program(), "screenTexture"), 0);

	glDrawArrays(VAO_screenQuad->getDrawMode(), 0, VAO_screenQuad->getIndicesCount());
	unbindTextures(0, 1);
}

void Renderer::drawArrays(VertexArrayObject* VAO) {
	VAO->bind();
	glDrawArrays(VAO->getDrawMode(), 0, VAO->getIndicesCount());
}

void Renderer::drawArrays(Shader* shader, VertexArrayObject* VAO, RenderProperties* rp) {
	shader->uniform("model", *(rp->getModelMatrix()));

	shader->reset_texCount();
	auto* textures_2D = rp->getMaterial()->getTextures_2D();
	for (unsigned int i = 0; i < textures_2D->size(); i++) {
		shader->bindTex_2D((textures_2D->at(i).name).c_str(), textures_2D->at(i).tex);
	}
	auto* textures_Cubemap = rp->getMaterial()->getTextures_Cubemap();
	for (unsigned int i = 0; i < textures_Cubemap->size(); i++) {
		shader->bindTex_Cubemap((textures_Cubemap->at(i).name).c_str(), textures_Cubemap->at(i).tex);
	}

	VAO->bind();
	glDrawArrays(VAO->getDrawMode(), 0, VAO->getIndicesCount());
}

void Renderer::drawElements(VertexArrayObject* VAO) {
	//VAO->bind();
	glDrawElements(VAO->getDrawMode(), VAO->getIndicesCount(), GL_UNSIGNED_INT, 0);
}

void Renderer::setup_model(Shader* shader, VertexArrayObject* VAO, Material* mat) {
	shader->uniform("tiling", mat->getTiling());

	shader->reset_texCount();
	auto* textures_2D = mat->getTextures_2D();
	for (unsigned int i = 0; i < textures_2D->size(); i++) {
		shader->bindTex_2D((textures_2D->at(i).name).c_str(), textures_2D->at(i).tex);
	}
	auto* textures_Cubemap = mat->getTextures_Cubemap();
	for (unsigned int i = 0; i < textures_Cubemap->size(); i++) {
		shader->bindTex_Cubemap((textures_Cubemap->at(i).name).c_str(), textures_Cubemap->at(i).tex);
	}
	shader->uniform("hasSpecularMap", mat->getIfSpecular());

	VAO->bind();
}

void Renderer::drawElements(Shader* shader, VertexArrayObject* VAO, Material* mat) {
	shader->uniform("tiling", mat->getTiling());

	shader->reset_texCount();
	auto* textures_2D = mat->getTextures_2D();
	for (unsigned int i = 0; i < textures_2D->size(); i++) {
		shader->bindTex_2D((textures_2D->at(i).name).c_str(), textures_2D->at(i).tex);
	}
	auto* textures_Cubemap = mat->getTextures_Cubemap();
	for (unsigned int i = 0; i < textures_Cubemap->size(); i++) {
		shader->bindTex_Cubemap((textures_Cubemap->at(i).name).c_str(), textures_Cubemap->at(i).tex);
	}
	shader->uniform("hasSpecularMap", mat->getIfSpecular());

	VAO->bind();
	glDrawElements(VAO->getDrawMode(), VAO->getIndicesCount(), GL_UNSIGNED_INT, 0);
}

void Renderer::drawElements_w_primitiveRestart(Shader* shader, VertexArrayObject* VAO, RenderProperties* rp) {

	shader->uniform("model", *(rp->getModelMatrix()));
	shader->uniformsToShader(rp->getUniforms());

	Material* mat = rp->getMaterial();
	shader->uniform("tiling", mat->getTiling());

	shader->reset_texCount();
	auto* textures_2D = mat->getTextures_2D();
	for (unsigned int i = 0; i < textures_2D->size(); i++) {
		shader->bindTex_2D((textures_2D->at(i).name).c_str(), textures_2D->at(i).tex);
	}
	auto* textures_Cubemap = mat->getTextures_Cubemap();
	for (unsigned int i = 0; i < textures_Cubemap->size(); i++) {
		shader->bindTex_Cubemap((textures_Cubemap->at(i).name).c_str(), textures_Cubemap->at(i).tex);
	}
	shader->uniform("hasSpecularMap", mat->getIfSpecular());

	glEnable(GL_PRIMITIVE_RESTART);
	glPrimitiveRestartIndex(VAO->getRestartIndex());
	VAO->bind();
	glDrawElements(VAO->getDrawMode(), VAO->getIndicesCount(), GL_UNSIGNED_INT, 0);
	//drawElements(VAO);
	glDisable(GL_PRIMITIVE_RESTART);
}

void Renderer::bindTexture(const unsigned int& shader, const unsigned int& texture, const char* name) {
	glActiveTexture(GL_TEXTURE0 + num_nextAvailableTex);
	glBindTexture(GL_TEXTURE_2D, texture);
	glUniform1i(glGetUniformLocation(shader, name), num_nextAvailableTex);
	num_nextAvailableTex++;
}

void Renderer::bindCubeMapTexture(const unsigned int& shader, const unsigned int& texture, const char* name) {
	glActiveTexture(GL_TEXTURE0 + num_nextAvailableTex);
	glBindTexture(GL_TEXTURE_CUBE_MAP, texture);
	glUniform1i(glGetUniformLocation(shader, name), num_nextAvailableTex);
	num_nextAvailableTex++;
}

void Renderer::unbindTextures(const unsigned int& num) {
	glActiveTexture(GL_TEXTURE0 + num);
	glBindTexture(GL_TEXTURE_2D, 0);

}

void Renderer::unbindTextures(const unsigned int& first, const unsigned int& last) {
	for (unsigned int i = first; i < last + 1; i++) {
		glActiveTexture(GL_TEXTURE0 + i);
		glBindTexture(GL_TEXTURE_2D, 0);
	}
}

void Renderer::unbindAllTextures() {
	for (unsigned int i = 0; i < num_nextAvailableTex + 1; i++) {
		glActiveTexture(GL_TEXTURE0 + i);
		glBindTexture(GL_TEXTURE_2D, 0);
	}
}

void Renderer::enableDepthTest() {
	glEnable(GL_DEPTH_TEST);
}
void Renderer::disableDepthTest() {
	glDisable(GL_DEPTH_TEST);
}

void Renderer::enableCullFace() {
	glEnable(GL_CULL_FACE);
}
void Renderer::disableCullFace() {
	glDisable(GL_CULL_FACE);
}

void Renderer::enableBlend() {
	glEnable(GL_BLEND);
}
void Renderer::disableBlend() {
	glDisable(GL_BLEND);
}
void Renderer::setBlendFunction(const GLenum setting) {
	glBlendFunc(GL_SRC_ALPHA, setting);
}