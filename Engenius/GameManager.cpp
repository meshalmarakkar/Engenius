#include "GameManager.h"

GameManager::GameManager() {
	dt_secs = 0.0f;
}

bool GameManager::ControlCheck(const float dt_secs) {
	this->dt_secs = dt_secs;
	return inputManager->KeyboardControls(windowManager->getWindow(), dt_secs);
}

void GameManager::initOtherFBOs() {
	const GLsizei SCREEN_WIDTH = (GLsizei)windowManager->getScreenWidth();
	const GLsizei SCREEN_HEIGHT = (GLsizei)windowManager->getScreenHeight();

	const unsigned int width = windowManager->getScreenWidth();
	const unsigned int height = windowManager->getScreenHeight();
	FBO_postProcess = new Framebuffer(width, height, true);
	FBO_postProcess->createTexturesAndAttach(2, width, height, Tex_Options::gl_linear, Tex_Options::gl_linear, Tex_Options::gl_clamp_to_edge, Tex_Options::gl_clamp_to_edge);

	FBO_pingpong[0] = new Framebuffer(width, height, false);
	FBO_pingpong[0]->createTexturesAndAttach(1, width, height, Tex_Options::gl_linear, Tex_Options::gl_linear, Tex_Options::gl_clamp_to_edge, Tex_Options::gl_clamp_to_edge);

	FBO_pingpong[1] = new Framebuffer(width, height, false);
	FBO_pingpong[1]->createTexturesAndAttach(1, width, height, Tex_Options::gl_linear, Tex_Options::gl_linear, Tex_Options::gl_clamp_to_edge, Tex_Options::gl_clamp_to_edge);
}

void GameManager::initDeferredRendering() {
	const GLsizei SCREEN_WIDTH = (GLsizei)windowManager->getScreenWidth();
	const GLsizei SCREEN_HEIGHT = (GLsizei)windowManager->getScreenHeight();

	const unsigned int width = (GLsizei)windowManager->getScreenWidth();
	const unsigned int height = (GLsizei)windowManager->getScreenHeight();

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

void GameManager::init(WindowManager * windowManager) {
	this->windowManager = windowManager;
	SDL_GL_SwapWindow(windowManager->getWindow()); // swap buffers once

	shaderManager = new ShaderManager();

	camera = new Camera(windowManager->getScreenWidth(), windowManager->getScreenHeight());
	hudManager = new HUDManager(windowManager, shaderManager->get_hud_program());
	audioManager = new AudioManager();
	

	particleGenerator = new ParticleManager(shaderManager->get_particle_program());
	lightingManager = new LightingManager(camera->getCameraEye(), camera->getCameraAt());
	//TODO: check what has to be stored and what has to be deleted from gameManager; like colManager for example
	CollisionManager * colManager = new CollisionManager();

	TerrainManager * terrainManager = new TerrainManager(lightingManager, camera, shaderManager);
	tex_pause = Common::loadTexture("Menu.png", "../Engenius/Textures/");
	tex_end = Common::loadTexture("Exit.png", "../Engenius/Textures/");
	audioManager->startGame(camera->getCameraEye());
	entityManager = new EntityManager(camera, particleGenerator, lightingManager, shaderManager, colManager, terrainManager, audioManager);

	editModeManager = new EditModeManager(entityManager, colManager, particleGenerator, hudManager, lightingManager, audioManager, shaderManager);

	//skybox = new Skybox(shaderManager->get_skybox_program(), entityManager->getENVModel("cube"));

	mousePicker = new MousePicker(windowManager, entityManager, terrainManager);

	inputManager = new InputManager(windowManager, camera, entityManager, mousePicker, lightingManager, hudManager, audioManager, editModeManager, colManager, terrainManager);

	glEnable(GL_DEPTH_TEST);
	//glDepthMask(GL_TRUE); //write to depth buffer
	//glDepthFunc(GL_LESS);

	glEnable(GL_CULL_FACE);

	glEnable(GL_BLEND); //blend and deferred shading....nope....
	glBlendFunc(GL_SRC_ALPHA, GL_ONE);

	// -------------------------

	glGenVertexArrays(1, &quadVAO);
	glGenBuffers(1, &quadVBO);
	glBindVertexArray(quadVAO);
	glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));

	initOtherFBOs();
	initDeferredRendering();
}

void GameManager::renderScene() {
	//skybox->renderSkybox(camera->getProjection(), camera->getView());

	entityManager->draw(dt_secs);

	unsigned int shader;
	if (editModeManager->get_ifEditMode()) {
		shader = shaderManager->get_simple_program();
		glUseProgram(shader);
		camera->passViewProjToShader(shader);
		editModeManager->draw(shader);
	}

	Common::unbindTextures(0, lightingManager->getNumOfLights());

	particleGenerator->renderParticles(camera->getView(), camera->getProjection()); //render last to blend with all the objecs!!
}

void GameManager::renderScene_GBuffer() {
	entityManager->draw_GBuffer();
}

void GameManager::update(float _dt_secs) {
	int viewMode = editModeManager->getViewMode();
	
	camera->Update(entityManager->getPlayer()->getPos(), viewMode);

	const glm::vec3 cameraEye = camera->getCameraEye();

	if (editModeManager->get_ifEditMode()) {
		glm::vec3 currentTerrainPointOfMouse = mousePicker->TerrainPointOfMouse(cameraEye, camera->getView(), camera->getProjection());
		editModeManager->update(currentTerrainPointOfMouse, dt_secs);
	}
	entityManager->Update(dt_secs);

	//handPos = playerPosition + handPos;
	//lightSources.spotLight.position = cameraEye;//handPos;
	//lightSources.spotLight.direction = cameraAt;

	particleGenerator->Update(dt_secs, cameraEye);
	if (editModeManager->get_ifEditMode())
		hudManager->update(dt_secs);
}

bool done = false;
void GameManager::draw() {
	if (lightingManager->getIfShadow() == true && done ==false) {
		for (unsigned int i = 0; i < lightingManager->getNumOfPointLights(); i++) {
			unsigned int shader = shaderManager->get_depthShader_program();
			lightingManager->setUpShadowRender_Pointlights(shader, i); // render using light's point of view
			entityManager->shadow_draw(shader);
			done = true;
		}
	}

	if (entityManager->getIfDef()) {
		//G-Buffer Render (DEFERRED SHADING P.1)
		FBO_gBuffer->bind();
		FBO_gBuffer->edit_viewport(windowManager->getScreenWidth(), windowManager->getScreenHeight());
		FBO_gBuffer->clearBuffers();
		FBO_gBuffer->clearScreen();

		renderScene_GBuffer();

		//Deferred Shading (DEFERRED SHADING P.2)
		FBO_postProcess->bind();
		FBO_postProcess->edit_viewport(windowManager->getScreenWidth(), windowManager->getScreenHeight());
		FBO_postProcess->clearBuffers();
		FBO_postProcess->clearScreen();

		//unsigned int shader = shaderManager->get_deferredShading_program();
		unsigned int shader = shaderManager->get_deferredShading_mapped_program();
		glUseProgram(shader);
		entityManager->farPlane_camEye_toShader(shader);

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

		lightingManager->lightsToShader(shader);
		if (lightingManager->getIfShadow() == true)
			glUniform1i(glGetUniformLocation(shader, "displayShadow"), true);
		else
			glUniform1i(glGetUniformLocation(shader, "displayShadow"), false);

		//RENDER TO SCREEN WITH POST-PROCESSING
		glDisable(GL_DEPTH_TEST); //SOMEWHERE LOWER???
		glBindVertexArray(quadVAO);
		glDrawArrays(GL_TRIANGLES, 0, 6);
		Common::unbindTextures(0, 7);
	}
	else {
		//RENDER OBJECTS
		FBO_postProcess->bind();
		FBO_postProcess->edit_viewport(windowManager->getScreenWidth(), windowManager->getScreenHeight());
		FBO_postProcess->clearBuffers();
		FBO_postProcess->clearScreen();

		renderScene();
		////////////////////////////////
		glDisable(GL_DEPTH_TEST); //SOMEWHERE LOWER???
		glBindVertexArray(quadVAO);
	}

	//glBindVertexArray(quadVAO);
	if (lightingManager->getIfBloom()) {
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

		unsigned int amount = 10;
		bool horizontal = true, first_iteration = true;
		glUseProgram(shaderManager->get_gaussianBlur_program());
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

			glDrawArrays(GL_TRIANGLES, 0, 6);
			horizontal = !horizontal;
			if (first_iteration)
				first_iteration = false;
		}

				
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, FBO_pingpong[!horizontal]->getColourBuffer(0));// pingpongColorbuffers[!horizontal]);
		glUniform1i(glGetUniformLocation(shaderManager->get_postProcessing_program(), "bloomBlur"), 1);
	}		
//	glDrawArrays(GL_TRIANGLES, 0, 6);

	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

//	lightingManager->displayDepthMap(shaderManager->get_depthMapRender_program());

	//DEFERRED RENDERING BLENDING PROBLEM!!!!!
	if (!entityManager->getIfDef()) {
		/*glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE);*/
	}

	glUseProgram(shaderManager->get_postProcessing_program());
	glUniform1f(glGetUniformLocation(shaderManager->get_postProcessing_program(), ("exposure")), lightingManager->getExposure());
	glUniform1i(glGetUniformLocation(shaderManager->get_postProcessing_program(), ("bloom")), lightingManager->getIfBloom());
	glUniform1i(glGetUniformLocation(shaderManager->get_postProcessing_program(), ("effect")), lightingManager->getPostProcessNum());


	glActiveTexture(GL_TEXTURE0);
	if (entityManager->getIfPause())
		glBindTexture(GL_TEXTURE_2D, tex_pause);
	else if (entityManager->getIfEndGame()) {
		glBindTexture(GL_TEXTURE_2D, tex_end);
	}
	else {
		//glBindTexture(GL_TEXTURE_2D, gNormal);
		//glBindTexture(GL_TEXTURE_2D, FBO_gBuffer->getColourBuffer(entityManager->getNum()));
		glBindTexture(GL_TEXTURE_2D, FBO_postProcess->getColourBuffer(0));
	}
	glUniform1i(glGetUniformLocation(shaderManager->get_postProcessing_program(), "screenTexture"), 0);

	glDrawArrays(GL_TRIANGLES, 0, 6);

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

	Common::unbindTextures(0, 1);
//	glBindVertexArray(0);

	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	//At end to see objects with transparency
	if (editModeManager->get_ifEditMode())
		hudManager->render();
	glEnable(GL_DEPTH_TEST);
	
	SDL_GL_SwapWindow(windowManager->getWindow()); // swap buffers
}