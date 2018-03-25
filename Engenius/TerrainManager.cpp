#include "TerrainManager.h"


TerrainManager::TerrainManager(LightingManager* lightingManager, Camera* camera, ShaderManager* shaderManager) : lightingManager(lightingManager), camera(camera), shaderManager(shaderManager){
	initTerrainToWorld();
	addLightIDs();
}

void TerrainManager::initTerrainToWorld() {
//	terrains.push_back(new Terrain(glm::vec3(-8.0f, 0.0f, -14.0f), 28.0f, Common::loadTexture("diffuse.jpg", "../Engenius/Models/Environment/Terrain/Asphalt/"), Common::loadTexture("normal.jpg", "../Engenius/Models/Environment/Terrain/Asphalt/"), heightsPlane, Common::loadTexture("specular.jpg", "../Engenius/Models/Environment/Terrain/Asphalt/")));
//	terrains.push_back(new Terrain(glm::vec3(20.0f, 0.0f, -20.0f), 100.0f, Common::loadTexture("diffuse.png", "../Engenius/Models/Environment/Terrain/BlackWhiteTile/"), Common::loadTexture("normal.png", "../Engenius/Models/Environment/Terrain/BlackWhiteTile/"), heightsPlaneZERO));

	terrains.resize(terrains.size());
	mappedTerrains.resize(terrains.size());
}


void TerrainManager::render(GLuint ifShadow, glm::vec3 playerPos) {
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
				lightsToShader(shader, terrains[iter]->getPointLightID(0), terrains[iter]->getPointLightID(1), terrains[iter]->getSpotLightID(0), terrains[iter]->getSpotLightID(1));
				terrains[iter]->Draw(shader);
			}
		}
		else {
			lightsToShader(shader, terrains[iter]->getPointLightID(0), terrains[iter]->getPointLightID(1), terrains[iter]->getSpotLightID(0), terrains[iter]->getSpotLightID(1));
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
			lightsToShader(shader, mappedTerrains[i]->getPointLightID(0), mappedTerrains[i]->getPointLightID(1), mappedTerrains[i]->getSpotLightID(0), mappedTerrains[i]->getSpotLightID(1));
			mappedTerrains[i]->DrawMapped(shader);
		}
	}
}

void TerrainManager::shadowDraw(GLuint shader) {
	for (unsigned int iter = 0; iter < terrains.size(); iter++) {
		terrains[iter]->Draw(shader);
	}
}

void TerrainManager::lightsToShader(GLuint shader, unsigned int pointLights_id1, unsigned int pointLights_id2, unsigned int spotLights_id1, unsigned int spotLights_id2) {
	lightingManager->lightIDsToShader(shader, pointLights_id1, pointLights_id2, spotLights_id1, spotLights_id2);
	lightingManager->shadowMapsToShader(shader, pointLights_id1, pointLights_id2, spotLights_id1, spotLights_id2);
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
	std::pair<int, int> lightIDs;
	std::pair<int, int> lightIDs2;

	for (unsigned int i = 0; i < terrains.size(); i++) {
		lightIDs = lightingManager->getClosestLights(terrains.at(i)->getCentre());
		terrains.at(i)->setPointLightIDs(lightIDs.first, lightIDs.second);
		std::cout << "TPointL: " << lightIDs.first << ", " << lightIDs.second << std::endl;
		lightIDs2 = lightingManager->getClosestSpotLights(terrains.at(i)->getCentre());
		terrains.at(i)->setSpotLightIDs(lightIDs2.first, lightIDs2.second);
		std::cout << "SpotL: " << lightIDs2.first << ", " << lightIDs2.second << std::endl;
	}
}
