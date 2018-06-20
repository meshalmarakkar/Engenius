#include "TerrainManager.h"


TerrainManager::TerrainManager(LightingManager* lightingManager, Camera* camera, ShaderManager* shaderManager) : lightingManager(lightingManager), camera(camera), shaderManager(shaderManager){
	initTerrainToWorld();
	addLightIDs();
}

void TerrainManager::initTerrainToWorld() {
	//terrains.push_back(new Terrain(glm::vec3(20.0f, 0.0f, -20.0f), 100.0f, Common::loadTexture("diffuse.png", "../Engenius/Models/Environment/Terrain/whiteTile/"), Common::loadTexture("normal.png", "../Engenius/Models/Environment/Terrain/whiteTile/"), heightsPlaneZERO));
	//2.01921,-0.0575006,4.57893 - security door position
	terrains.push_back(new Terrain(glm::vec3(-4.0, 0.0f, 4.0f), 20.0f, Common::loadTexture("diffuse.png", "../Engenius/Models/Environment/Terrain/grassGround/"), Common::loadTexture("normal.png", "../Engenius/Models/Environment/Terrain/grassGround/"), 16.0f, heightsPlaneBumpy));
	//terrains.push_back(new Terrain(glm::vec3(-20.0f, 0.0f, -20.0f), 40.0f, Common::loadTexture("diffuse.png", "../Engenius/Models/Environment/Terrain/whiteTile/"), Common::loadTexture("normal.png", "../Engenius/Models/Environment/Terrain/whiteTile/"), 32.0f, heightsPlaneBumpy));

	terrains.resize(terrains.size());
	mappedTerrains.resize(terrains.size());

	alphaTest = 0.25f;
	windStrength = 4.0f;
	windDirection = glm::vec3(1.0, 0.0, 1.0);
	windDirection = glm::normalize(windDirection);
	grassTexture = Common::loadTexture("grassPack.png", "../Engenius/Textures/");// Common::loadTexture("grassPack.dds", "../Engenius/Textures/");
}


void TerrainManager::render(GLuint ifShadow, glm::vec3 playerPos, float dt_secs) {
	GLuint shader = shaderManager->get_terrain_program();

	glUseProgram(shader);
	
	glUniform1i(glGetUniformLocation(shader, "displayShadow"), ifShadow);

	farPlane_camEye_toShader(shader);
	camera->passViewProjToShader(shader);
	lightingManager->lightsToShader(shader);

	int numOfMapped = 0;

	for (unsigned int iter = 0; iter < terrains.size(); iter++) {
		if (terrains[iter]->get_ifMapped() == true) {
			if (std::hypotf(playerPos.x - terrains[iter]->getCentre().x, playerPos.z - terrains[iter]->getCentre().z) < 200) {
				mappedTerrains.at(numOfMapped) = terrains[iter];
				numOfMapped++;
			}
			else {
				lightsToShader(shader, terrains[iter]->getPointLightID(0), terrains[iter]->getPointLightID(1), terrains[iter]->getPointLightID(2), terrains[iter]->getSpotLightID(0), terrains[iter]->getSpotLightID(1), terrains[iter]->getSpotLightID(2));
				terrains[iter]->Draw(shader);
			}
		}
		else {
			lightsToShader(shader, terrains[iter]->getPointLightID(0), terrains[iter]->getPointLightID(1), terrains[iter]->getPointLightID(2), terrains[iter]->getSpotLightID(0), terrains[iter]->getSpotLightID(1), terrains[iter]->getSpotLightID(2));
			terrains[iter]->Draw(shader);
		}
	}

	if (numOfMapped > 0) {
		shader = shaderManager->get_terrain_mapped_program();

		glUseProgram(shader);
	
		glUniform1i(glGetUniformLocation(shader, "displayShadow"), ifShadow);

		farPlane_camEye_toShader(shader);
		camera->passViewProjToShader(shader);
		lightingManager->lightsToShader(shader);

		for (int i = 0; i < numOfMapped; i++) {
			lightsToShader(shader, mappedTerrains[i]->getPointLightID(0), mappedTerrains[i]->getPointLightID(1), mappedTerrains[i]->getPointLightID(2), mappedTerrains[i]->getSpotLightID(0), mappedTerrains[i]->getSpotLightID(1), mappedTerrains[i]->getSpotLightID(2));
			mappedTerrains[i]->DrawMapped(shader);
		}
	}

	////GRASS////
	shader = shaderManager->get_grass_program();
	glUseProgram(shader);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, grassTexture);
	glUniform1i(glGetUniformLocation(shader, "texture_diffuse1"), 0);
	glUniform1f(glGetUniformLocation(shader, "alphaTest"), alphaTest);
	glUniform1f(glGetUniformLocation(shader, "windStrength"), windStrength);
	glUniform3fv(glGetUniformLocation(shader, "windDirection"), 1, glm::value_ptr(windDirection));

	camera->passViewProjToShader(shader);

	for (unsigned int iter = 0; iter < terrains.size(); iter++) {
		terrains[iter]->renderGrass(shader, dt_secs);
	}
	Common::unbindTextures(0);
}

void TerrainManager::shadowDraw(GLuint shader) {
	for (unsigned int iter = 0; iter < terrains.size(); iter++) {
		terrains[iter]->Draw(shader);
	}
}

void TerrainManager::lightsToShader(GLuint shader, int point_id1, int point_id2, int point_id3, int spot_id1, int spot_id2, int spot_id3) {
	lightingManager->lightIDsToShader(shader, point_id1, point_id2, point_id3, spot_id1, spot_id2, spot_id3);
	lightingManager->shadowMapsToShader(shader, point_id1, point_id2, point_id3, spot_id1, spot_id2, spot_id3);
	//lightingManager->spotShadowMapsToShader(shader, spotLights_id1, spotLights_id2);
}

void TerrainManager::farPlane_camEye_toShader(GLuint shader) {
	glUniform1f(glGetUniformLocation(shader, "far_plane"), camera->getFarPlane());
	glUniform3fv(glGetUniformLocation(shader, "viewPos"), 1, glm::value_ptr(camera->getCameraEye()));
}

float TerrainManager::getTerrainHeight(const float x, const float z) {
	for (unsigned int i = 0; i < terrains.size(); i++) {
		if (terrains[i]->ifInTerrain(x, z))
			return terrains[i]->getTerrainHeight(x, z);
	}
	return 0.0f;
}

void TerrainManager::addLightIDs() {
	std::tuple<int, int, int> lightIDs;
	std::tuple<int, int, int> lightIDs2;

	for (unsigned int i = 0; i < terrains.size(); i++) {
		lightIDs = lightingManager->getClosestLights(terrains.at(i)->getCentre());
		terrains.at(i)->setPointLightIDs(std::get<0>(lightIDs), std::get<1>(lightIDs), std::get<2>(lightIDs));
		std::cout << "PointL: " << std::get<0>(lightIDs) << ", " << std::get<1>(lightIDs) << ", " << std::get<2>(lightIDs) << std::endl;
		lightIDs2 = lightingManager->getClosestSpotLights(terrains.at(i)->getCentre());
		terrains.at(i)->setSpotLightIDs(std::get<0>(lightIDs2), std::get<1>(lightIDs2), std::get<2>(lightIDs2));
		std::cout << "SpotL: " << std::get<0>(lightIDs2) << ", " << std::get<1>(lightIDs2) << ", " << std::get<2>(lightIDs2) << std::endl;
	}
}
