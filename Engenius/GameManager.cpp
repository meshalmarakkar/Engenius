#include "GameManager.h"


GameManager::GameManager() {
	dt_secs = 0.0f;
}

GameManager::~GameManager() {
	delete particleGenerator;
	delete lightingManager;
	delete camera;
	delete entityManager;
	delete hudManager;
	delete inputManager;
	delete mousePicker;
	delete audioManager;
	delete editModeManager;
	delete shaderManager;
	delete windowManager;
	delete colManager;
	delete terrainManager;
}

bool GameManager::ControlCheck(const float dt_secs) {
	this->dt_secs = dt_secs;
	return inputManager->KeyboardControls(windowManager->getWindow(), dt_secs);
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
	colManager = new CollisionManager();

	terrainManager = new TerrainManager(lightingManager, camera, shaderManager);

	tex_pause = TextureLoader::loadTexture("Menu.png", "../Engenius/Textures/");
	tex_end = TextureLoader::loadTexture("Exit.png", "../Engenius/Textures/");
	audioManager->startGame(camera->getCameraEye());
	entityManager = new EntityManager(camera, particleGenerator, lightingManager, shaderManager, colManager, terrainManager, audioManager);

	editModeManager = new EditModeManager(entityManager, colManager, particleGenerator, hudManager, lightingManager, audioManager, shaderManager);

	mousePicker = new MousePicker(windowManager, entityManager, terrainManager);

	inputManager = new InputManager(windowManager, camera, entityManager, mousePicker, lightingManager, hudManager, audioManager, editModeManager, colManager, terrainManager);

	glEnable(GL_DEPTH_TEST);
	//glDepthMask(GL_TRUE); //write to depth buffer
	//glDepthFunc(GL_LESS);

	glEnable(GL_CULL_FACE);

	glEnable(GL_BLEND); //blend and deferred shading....nope....
	glBlendFunc(GL_SRC_ALPHA, GL_ONE);

	renderer = new Renderer(windowManager, shaderManager);
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

	renderer->unbindTextures(0, lightingManager->getNumOfLights());

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
		renderer->setupFrame_Deferred();
		renderScene_GBuffer();

		//Deferred Shading (DEFERRED SHADING P.2)
		renderer->setupFrame_PostProcess();

		//unsigned int shader = shaderManager->get_deferredShading_program();
		unsigned int shader = shaderManager->get_deferredShading_mapped_program();
		glUseProgram(shader);
		entityManager->farPlane_camEye_toShader(shader);

		renderer->toShader_Buffers(shader);

		lightingManager->lightsToShader(shader);
		if (lightingManager->getIfShadow() == true)
			glUniform1i(glGetUniformLocation(shader, "displayShadow"), true);
		else
			glUniform1i(glGetUniformLocation(shader, "displayShadow"), false);

		//RENDER TO SCREEN WITH POST-PROCESSING
		glDisable(GL_DEPTH_TEST);
		renderer->draw_screenQuad();
		//Common::unbindTextures(0, 7);
	}
	else { 
		renderer->setupFrame_PostProcess();

		renderScene();
		glDisable(GL_DEPTH_TEST);
	}

	//glBindVertexArray(quadVAO);
	if (lightingManager->getIfBloom()) {
		renderer->SS_Bloom();
	}		

////	lightingManager->displayDepthMap(shaderManager->get_depthMapRender_program());
//
//	//DEFERRED RENDERING BLENDING PROBLEM!!!!!
//	if (!entityManager->getIfDef()) {
//		/*glEnable(GL_BLEND);
//		glBlendFunc(GL_SRC_ALPHA, GL_ONE);*/
//	}

	renderer->draw_finalDisplay(lightingManager->getExposure(), lightingManager->getIfBloom(), lightingManager->getPostProcessNum(), entityManager->getIfPause(), tex_pause, entityManager->getIfEndGame(), tex_end);

	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	//At end to see objects with transparency
	if (editModeManager->get_ifEditMode())
		hudManager->render();
	glEnable(GL_DEPTH_TEST);
	
	SDL_GL_SwapWindow(windowManager->getWindow()); // swap buffers
}