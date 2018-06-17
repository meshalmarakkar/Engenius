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

	glGenFramebuffers(1, &postProcessFBO);
	glBindFramebuffer(GL_FRAMEBUFFER, postProcessFBO);

	// create a floating point colour buffer
	glGenTextures(2, colourBuffer);
	for (unsigned int i = 0; i < 2; i++)
	{
		glBindTexture(GL_TEXTURE_2D, colourBuffer[i]);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, SCREEN_WIDTH, SCREEN_HEIGHT, 0, GL_RGB, GL_FLOAT, NULL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i, GL_TEXTURE_2D, colourBuffer[i], 0);
	}

	// tell OpenGL which color attachments of this framebuffer we'll use as defualt is just the first one
	unsigned int attachments[2] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1 };
	glDrawBuffers(2, attachments);

	// create a depth buffer renderbuffer object
	glGenRenderbuffers(1, &rboDepth);
	glBindRenderbuffer(GL_RENDERBUFFER, rboDepth);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, SCREEN_WIDTH, SCREEN_HEIGHT);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, rboDepth);

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		std::cout << "Framebuffer not complete!" << std::endl;
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	glGenFramebuffers(2, pingpongFBO);
	glGenTextures(2, pingpongColorbuffers);
	for (unsigned int i = 0; i < 2; i++)
	{
		glBindFramebuffer(GL_FRAMEBUFFER, pingpongFBO[i]);
		glBindTexture(GL_TEXTURE_2D, pingpongColorbuffers[i]);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, SCREEN_WIDTH, SCREEN_HEIGHT, 0, GL_RGB, GL_FLOAT, NULL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, pingpongColorbuffers[i], 0);
		// also check if framebuffers are complete (no need for depth buffer)
		if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
			std::cout << "Framebuffer not complete!" << std::endl;
	}
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void GameManager::initDeferredRendering() {
	const GLsizei SCREEN_WIDTH = (GLsizei)windowManager->getScreenWidth();
	const GLsizei SCREEN_HEIGHT = (GLsizei)windowManager->getScreenHeight();
	//configure g-buffer framebuffer
	glGenFramebuffers(1, &gBuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, gBuffer);

	//position colour buffer
	glGenTextures(1, &gPosition);
	glBindTexture(GL_TEXTURE_2D, gPosition);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, SCREEN_WIDTH, SCREEN_HEIGHT, 0, GL_RGB, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, gPosition, 0);

	// normal color buffer
	glGenTextures(1, &gNormal);
	glBindTexture(GL_TEXTURE_2D, gNormal);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, SCREEN_WIDTH, SCREEN_HEIGHT, 0, GL_RGB, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, gNormal, 0);

	// color + specular color buffer
	glGenTextures(1, &gAlbedoSpecular);
	glBindTexture(GL_TEXTURE_2D, gAlbedoSpecular);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, SCREEN_WIDTH, SCREEN_HEIGHT, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT2, GL_TEXTURE_2D, gAlbedoSpecular, 0);

	// PointLight ID buffer
	glGenTextures(1, &gPointLightID);
	glBindTexture(GL_TEXTURE_2D, gPointLightID);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16, SCREEN_WIDTH, SCREEN_HEIGHT, 0, GL_RGB, GL_INT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT3, GL_TEXTURE_2D, gPointLightID, 0);

	// SpotLight ID buffer
	glGenTextures(1, &gSpotLightID);
	glBindTexture(GL_TEXTURE_2D, gSpotLightID);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16, SCREEN_WIDTH, SCREEN_HEIGHT, 0, GL_RGB, GL_INT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT4, GL_TEXTURE_2D, gSpotLightID, 0);

	// TBN_T buffer
	glGenTextures(1, &gTBN_T);
	glBindTexture(GL_TEXTURE_2D, gTBN_T);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, SCREEN_WIDTH, SCREEN_HEIGHT, 0, GL_RGB, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT5, GL_TEXTURE_2D, gTBN_T, 0);

	// TBN_B buffer
	glGenTextures(1, &gTBN_B);
	glBindTexture(GL_TEXTURE_2D, gTBN_B);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, SCREEN_WIDTH, SCREEN_HEIGHT, 0, GL_RGB, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT6, GL_TEXTURE_2D, gTBN_B, 0);

	// TBN_N buffer
	glGenTextures(1, &gTBN_N);
	glBindTexture(GL_TEXTURE_2D, gTBN_N);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, SCREEN_WIDTH, SCREEN_HEIGHT, 0, GL_RGB, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT7, GL_TEXTURE_2D, gTBN_N, 0);

	// tell OpenGL which color attachments we'll use (of this framebuffer) for rendering m
	unsigned int attachments[8] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2, GL_COLOR_ATTACHMENT3, 
									GL_COLOR_ATTACHMENT4, GL_COLOR_ATTACHMENT5, GL_COLOR_ATTACHMENT6, GL_COLOR_ATTACHMENT7 };
	glDrawBuffers(8, attachments);
	// create and attach depth buffer (renderbuffer)

	glGenRenderbuffers(1, &rboDepthDefer);
	glBindRenderbuffer(GL_RENDERBUFFER, rboDepthDefer);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, SCREEN_WIDTH, SCREEN_HEIGHT);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, rboDepthDefer);

	// finally check if framebuffer is complete
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		std::cout << "Framebuffer not complete!" << std::endl;
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

	GLuint shader;
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
			GLuint shader = shaderManager->get_depthShader_program();
			lightingManager->setUpShadowRender_Pointlights(shader, i); // render using light's point of view
			entityManager->shadow_draw(shader);
			done = true;
		}
	}

	if (entityManager->getIfDef()) {
		//G-Buffer Render (DEFERRED SHADING P.1)
		glBindFramebuffer(GL_FRAMEBUFFER, gBuffer);
		glViewport(0, 0, (GLsizei)windowManager->getScreenWidth(), (GLsizei)windowManager->getScreenHeight());
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

		renderScene_GBuffer();

		//Deferred Shading (DEFERRED SHADING P.2)
		glBindFramebuffer(GL_FRAMEBUFFER, postProcessFBO);
		glViewport(0, 0, (GLsizei)windowManager->getScreenWidth(), (GLsizei)windowManager->getScreenHeight());
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

		//GLuint shader = shaderManager->get_deferredShading_program();
		GLuint shader = shaderManager->get_deferredShading_mapped_program();
		glUseProgram(shader);
		entityManager->farPlane_camEye_toShader(shader);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, gPosition);
		glUniform1i(glGetUniformLocation(shader, "gPosition"), 0);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, gNormal);
		glUniform1i(glGetUniformLocation(shader, "gNormal"), 1);
		glActiveTexture(GL_TEXTURE2);
		glBindTexture(GL_TEXTURE_2D, gAlbedoSpecular);
		glUniform1i(glGetUniformLocation(shader, "gAlbedoSpecular"), 2);
		glActiveTexture(GL_TEXTURE3);
		glBindTexture(GL_TEXTURE_2D, gPointLightID);
		glUniform1i(glGetUniformLocation(shader, "gPointLightIDs"), 3);
		glActiveTexture(GL_TEXTURE4);
		glBindTexture(GL_TEXTURE_2D, gSpotLightID);
		glUniform1i(glGetUniformLocation(shader, "gSpotLightIDs"), 4);
		glActiveTexture(GL_TEXTURE5);
		glBindTexture(GL_TEXTURE_2D, gTBN_T);
		glUniform1i(glGetUniformLocation(shader, "gTBN_T"), 5);
		glActiveTexture(GL_TEXTURE6);
		glBindTexture(GL_TEXTURE_2D, gTBN_B);
		glUniform1i(glGetUniformLocation(shader, "gTBN_B"), 6);
		glActiveTexture(GL_TEXTURE7);
		glBindTexture(GL_TEXTURE_2D, gTBN_N);
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
		glBindFramebuffer(GL_FRAMEBUFFER, postProcessFBO);
		glViewport(0, 0, (GLsizei)windowManager->getScreenWidth(), (GLsizei)windowManager->getScreenHeight());
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

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
			glBindFramebuffer(GL_FRAMEBUFFER, pingpongFBO[horizontal]);/*
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			glClearColor(0.0f, 0.0f, 0.0f, 1.0f);*/
			glUniform1i(glGetUniformLocation(shaderManager->get_gaussianBlur_program(), ("horizontal")), horizontal);
					
			// bind texture of other framebuffer (or scene if first iteration)
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, first_iteration ? colourBuffer[1] : pingpongColorbuffers[!horizontal]);
			glUniform1i(glGetUniformLocation(shaderManager->get_gaussianBlur_program(), "textureToBlur"), 0);

			glDrawArrays(GL_TRIANGLES, 0, 6);
			horizontal = !horizontal;
			if (first_iteration)
				first_iteration = false;
		}

				
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, pingpongColorbuffers[!horizontal]);
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
		glBindTexture(GL_TEXTURE_2D, colourBuffer[0]);
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
	glBindVertexArray(0);

	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	//At end to see objects with transparency
	if (editModeManager->get_ifEditMode())
		hudManager->render();
	glEnable(GL_DEPTH_TEST);
	
	SDL_GL_SwapWindow(windowManager->getWindow()); // swap buffers
}