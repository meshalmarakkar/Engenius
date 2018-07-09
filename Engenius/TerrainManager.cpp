#include "TerrainManager.h"
#include "TextureLoader.h"

TerrainManager::TerrainManager(LightingManager* lightingManager, Camera* camera, ShaderManager* shaderManager) : lightingManager(lightingManager), camera(camera), shaderManager(shaderManager){
	initTerrainToWorld();
	addLightIDs();
	timePassed = 0.0f;
}

// Returns true iif v1 can be considered equal to v2
bool TerrainManager::is_near(float v1, float v2) {
	return fabs(v1 - v2) < 0.01f;
}

bool TerrainManager::getSimilarVertexIndex(
	glm::vec3 &in_vertex, glm::vec2 &in_uv, glm::vec3 &in_normal,
	std::vector<glm::vec3> &out_vertices, std::vector<glm::vec2> &out_uvs, std::vector<glm::vec3> &out_normals,
	unsigned int & result
) {
	// Lame linear search
	for (unsigned int i = 0; i<out_vertices.size(); i++) {
		if (
			is_near(in_vertex.x, out_vertices[i].x) &&
			is_near(in_vertex.y, out_vertices[i].y) &&
			is_near(in_vertex.z, out_vertices[i].z) &&
			is_near(in_uv.x, out_uvs[i].x) &&
			is_near(in_uv.y, out_uvs[i].y) &&
			is_near(in_normal.x, out_normals[i].x) &&
			is_near(in_normal.y, out_normals[i].y) &&
			is_near(in_normal.z, out_normals[i].z)
			) {
			result = i;
			return true;
		}
	}
	// No other vertex could be used instead.
	// Looks like we'll have to add it to the VBO.
	return false;
}

void TerrainManager::indexVBO_TBN(
	std::vector<glm::vec3> &in_vertices, std::vector<glm::vec2> &in_uvs, std::vector<glm::vec3> &in_normals,
	std::vector<glm::vec3> &in_tangents, std::vector<glm::vec3> &in_bitangents,
	std::vector<glm::vec3> &out_vertices, std::vector<glm::vec2> &out_uvs, std::vector<glm::vec3> &out_normals,
	std::vector<glm::vec3> &out_tangents, std::vector<glm::vec3> &out_bitangents
) {
	// For each input vertex
	for (unsigned int i = 0; i<in_vertices.size(); i++) {

		// Try to find a similar vertex in out_XXXX
		unsigned int index;
		bool found = getSimilarVertexIndex(in_vertices[i], in_uvs[i], in_normals[i], out_vertices, out_uvs, out_normals, index);

		if (found) { // A similar vertex is already in the VBO, use it instead !
					 // Average the tangents and the bitangents
			out_tangents[index] += in_tangents[i];
			out_bitangents[index] += in_bitangents[i];
			//out_normals[index] += in_normals[i];
		}
		else { // If not, it needs to be added in the output data.
			out_vertices.push_back(in_vertices[i]);
			out_uvs.push_back(in_uvs[i]);
			out_normals.push_back(in_normals[i]);
			out_tangents.push_back(in_tangents[i]);
			out_bitangents.push_back(in_bitangents[i]);
		}
	}
}

void TerrainManager::indexVBO(
	std::vector<glm::vec3> &in_vertices, std::vector<glm::vec2> &in_uvs, std::vector<glm::vec3> &in_normals,
	std::vector<glm::vec3> &out_vertices, std::vector<glm::vec2> &out_uvs, std::vector<glm::vec3> &out_normals
) {
	// For each input vertex
	for (unsigned int i = 0; i<in_vertices.size(); i++) {

		// Try to find a similar vertex in out_XXXX
		unsigned int index;
		bool found = getSimilarVertexIndex(in_vertices[i], in_uvs[i], in_normals[i], out_vertices, out_uvs, out_normals, index);

		if (found) { // A similar vertex is already in the VBO, use it instead !
					 // Average the tangents and the bitangents
					 //out_normals[index] += in_normals[i];
		}
		else { // If not, it needs to be added in the output data.
			out_vertices.push_back(in_vertices[i]);
			out_uvs.push_back(in_uvs[i]);
			out_normals.push_back(in_normals[i]);
		}
	}
}

glm::vec3 TerrainManager::calculateNormal(int x, int z, float heights[VERTEX_COUNT][VERTEX_COUNT]) {
	//	float heightL = heights[x - 1][z]; //left
	//	float heightR = heights[x + 1][z]; //right
	//	float heightD = heights[x][z - 1]; //down
	//	float heightU = heights[x][z + 1]; //up

	//	glm::vec3 normal(heightL - heightR, 2.0f, heightD - heightU);
	////	ALTERNATIVE
	////	glm::vec3 horizontal(2.0f, heightR - heightL, 0.0f);
	////	glm::vec3 vertical(0.0f, heightU - heightD, 2.0f);
	////	glm::vec3 normal = glm::normalize(glm::cross(vertical, horizontal));
	//	normal = glm::normalize(normal);
	//
	//	return normal;

	//TAKES INTO ACCOUNT THE EDGES DON'T HAVE EITHER -1 or +1 FOR HEIGHTS
	glm::vec3 normal(0.0f, 0.0f, 0.0f);

	glm::vec3 out;
	if (z > 0) {
		out = glm::vec3(0.0f, heights[z - 1][x] - heights[z][x], -1.0f);
	}
	glm::vec3 in;
	if (z < VERTEX_COUNT - 1) {
		in = glm::vec3(0.0f, heights[z + 1][x] - heights[z][x], 1.0f);
	}
	glm::vec3 left;
	if (x > 0) {
		left = glm::vec3(-1.0f, heights[z][x - 1] - heights[z][x], 0.0f);
	}
	glm::vec3 right;
	if (x < VERTEX_COUNT - 1) {
		right = glm::vec3(1.0f, heights[z][x + 1] - heights[z][x], 0.0f);
	}

	if (x > 0 && z > 0) {
		normal += glm::normalize(glm::cross(out, left));
	}
	if (x > 0 && z < VERTEX_COUNT - 1) {
		normal += glm::normalize(glm::cross(left, in));
	}
	if (x < VERTEX_COUNT - 1 && z < VERTEX_COUNT - 1) {
		normal += glm::normalize(glm::cross(in, right));
	}
	if (x < VERTEX_COUNT - 1 && z > 0) {
		normal += glm::normalize(glm::cross(right, out));
	}

	return normal;
}

void TerrainManager::computeTangentBasis(
	std::vector<glm::vec3> &vertices, std::vector<glm::vec2> &textureCoords, std::vector<unsigned int> &indices, std::vector<glm::vec3> &normals,
	std::vector<glm::vec3> &tangents, std::vector<glm::vec3> &bitangents
) {

	for (unsigned int i = 0; i<indices.size() - 1; i += 3) {

		// Shortcuts for vertices
		glm::vec3 & v0 = vertices[indices[i]];
		glm::vec3 & v1 = vertices[indices[i + 1]];
		glm::vec3 & v2 = vertices[indices[i + 2]];

		// Shortcuts for UVs
		glm::vec2 & uv0 = textureCoords[indices[i]];
		glm::vec2 & uv1 = textureCoords[indices[i + 1]];
		glm::vec2 & uv2 = textureCoords[indices[i + 2]];

		// Edges of the triangle : postion delta
		glm::vec3 deltaPos1 = v1 - v0;
		glm::vec3 deltaPos2 = v2 - v0;
		// UV delta
		glm::vec2 deltaUv1 = uv1 - uv0;
		glm::vec2 deltaUv2 = uv2 - uv0;

		float r = 1.0f / (deltaUv1.x * deltaUv2.y - deltaUv1.y * deltaUv2.x);

		glm::vec3 tangent = glm::normalize((deltaPos1 * deltaUv2.y - deltaPos2 * deltaUv1.y) * r);
		glm::vec3 bitangent = glm::normalize((deltaPos2 * deltaUv1.x - deltaPos1 * deltaUv2.x) * r);

		tangents.push_back(tangent);
		tangents.push_back(tangent);
		tangents.push_back(tangent);

		bitangents.push_back(bitangent);
		bitangents.push_back(bitangent);
		bitangents.push_back(bitangent);
	}

	//// See "Going Further"
	for (unsigned int i = 0; i<vertices.size(); i += 1)
	{
		glm::vec3 & n = normals[i];
		glm::vec3 & t = tangents[i];
		glm::vec3 & b = bitangents[i];

		// Gram-Schmidt orthogonalize
		t = glm::normalize(t - n * glm::dot(n, t));

		// Calculate handedness
		if (glm::dot(glm::cross(n, t), b) < 0.0f) {
			t = t * -1.0f;
		}
	}
}

void TerrainManager::initTerrainToWorld() {
	//2.01921,-0.0575006,4.57893 - security door position
	
	createTerrain(glm::vec3(-4.0, 0.0f, 4.0f), 20.0f, "diffuse.png", "../Engenius/Models/Environment/Terrain/grassGround/", "", "", heightsPlaneBumpy);

	//createTerrain(glm::vec3(-4.0, 0.0f, 4.0f), 20.0f, "diffuse.png", "../Engenius/Models/Environment/Terrain/grassGround/", "", "", "normal.png", "../Engenius/Models/Environment/Terrain/grassGround/", heightsPlaneBumpy);
	//createTerrain(glm::vec3(-4.0, 0.0f, 4.0f), 20.0f, "diffuse.bmp", "../Engenius/Models/Environment/Terrain/brick/", "", "", "normal.bmp", "../Engenius/Models/Environment/Terrain/brick/", heightsPlaneBumpy);
	
	terrains.resize(terrains.size());
	mappedTerrains.resize(terrains.size());

	alphaTest = 0.25f; //0.9f;
	windStrength = 4.0f;
	windDirection = glm::vec3(1.0, 0.0, 1.0);
	windDirection = glm::normalize(windDirection);
	grassTexture = TextureLoader::loadTexture("grassPack.png", "../Engenius/Textures/");// Common::loadTexture("grassPack.dds", "../Engenius/Textures/");
}

BaseAttributes TerrainManager::calculateBaseAttributes(float size, const float vertHeights[]) {
	float HALF_TERRAIN_SIZE = size * 0.5f;

	BaseAttributes attrib;
	attrib.vertices.resize(COUNT);
	attrib.normals.resize(COUNT);
	attrib.textureCoords.resize(COUNT);

	float lakeH[COUNT];
	for (int j = 0; j < COUNT; j++) {
		lakeH[j] = vertHeights[j];
	}

	int vertexPointer = 0;
	for (int z = 0; z < VERTEX_COUNT; z++) {
		for (int x = 0; x < VERTEX_COUNT; x++) {
			attrib.vertices[vertexPointer].x = (float)x / ((float)LAST_VERTEX) * size;

			attrib.vertices[vertexPointer].y = lakeH[vertexPointer];
			attrib.heights[x][z] = lakeH[vertexPointer];
			attrib.vertices[vertexPointer].z = (float)z / ((float)LAST_VERTEX) * size;
			attrib.textureCoords[vertexPointer].x = (float)x / ((float)LAST_VERTEX);
			attrib.textureCoords[vertexPointer].y = (float)z / ((float)LAST_VERTEX);
			vertexPointer++;
		}
	}

	int vp = 0;
	for (int z = 0; z < VERTEX_COUNT; z++) {
		for (int x = 0; x < VERTEX_COUNT; x++) {
			attrib.normals[vp] = calculateNormal(x, z, attrib.heights);
			vp++;
		}
	}

	for (int z = 0; z < LAST_VERTEX; z++) { //cause 0-8 and bottom calc goes to 9
		for (int x = 0; x < LAST_VERTEX; x += 2) {
			unsigned int topLeft = (z*VERTEX_COUNT) + x;
			unsigned int topRight = topLeft + 1;
			unsigned int bottomLeft = ((z + 1)*VERTEX_COUNT) + x;
			unsigned int bottomRight = bottomLeft + 1;
			attrib.indices.push_back(topLeft);
			attrib.indices.push_back(bottomLeft);
			attrib.indices.push_back(topRight);
			attrib.indices.push_back(bottomRight);
		}
	}
	
	return attrib;
}

void TerrainManager::createTerrain(glm::vec3 pos, float size, std::string textype_diff, std::string texpath_diff, std::string textype_spec, std::string texpath_spec, const float vertHeights[], float tiling) {
	BaseAttributes attrib = calculateBaseAttributes(size, vertHeights);
	std::vector<glm::vec3> indexed_vertices;
	std::vector<glm::vec3> indexed_normals;
	std::vector<glm::vec2> indexed_textureCoords;
	indexVBO(attrib.vertices, attrib.textureCoords, attrib.normals, indexed_vertices, indexed_textureCoords, indexed_normals);

	terrains.push_back(new Terrain(indexed_vertices, indexed_normals, indexed_textureCoords, attrib.indices, pos, size, TextureLoader::loadTexture(textype_diff, texpath_diff), TextureLoader::loadTexture(textype_spec, texpath_spec), attrib.heights, tiling));
}

void TerrainManager::createTerrain(glm::vec3 pos, float size, std::string textype_diff, std::string texpath_diff, std::string textype_spec, std::string texpath_spec, std::string textype_norm, std::string texpath_norm, const float vertHeights[], float tiling) {

	BaseAttributes attrib = calculateBaseAttributes(size, vertHeights);

	std::vector<glm::vec3> indexed_vertices;
	std::vector<glm::vec3> indexed_normals;
	std::vector<glm::vec2> indexed_textureCoords;

	std::vector<glm::vec3> tangents;
	std::vector<glm::vec3> bitangents;
	computeTangentBasis(attrib.vertices, attrib.textureCoords, attrib.indices, attrib.normals, tangents, bitangents);
	std::vector<glm::vec3> indexed_tangents;
	std::vector<glm::vec3> indexed_bitangents;
	indexVBO_TBN(attrib.vertices, attrib.textureCoords, attrib.normals, tangents, bitangents, indexed_vertices, indexed_textureCoords, indexed_normals, indexed_tangents, indexed_bitangents);

	terrains.push_back(new Terrain(indexed_vertices, indexed_normals, indexed_textureCoords, indexed_tangents, indexed_bitangents, attrib.indices, pos, size, TextureLoader::loadTexture(textype_diff, texpath_diff), TextureLoader::loadTexture(textype_spec, texpath_spec), TextureLoader::loadTexture(textype_norm, texpath_norm), attrib.heights, tiling));
}

void TerrainManager::render(Renderer* renderer, unsigned int ifShadow, glm::vec3 playerPos, float dt_secs) {
	unsigned int shader = shaderManager->get_terrain_program();

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
				renderer->drawElements_w_primitiveRestart(shader, terrains[iter]->getVAO(), terrains[iter]->getMaterial());
			}
		}
		else {
			lightsToShader(shader, terrains[iter]->getPointLightID(0), terrains[iter]->getPointLightID(1), terrains[iter]->getPointLightID(2), terrains[iter]->getSpotLightID(0), terrains[iter]->getSpotLightID(1), terrains[iter]->getSpotLightID(2));
			renderer->drawElements_w_primitiveRestart(shader, terrains[iter]->getVAO(), terrains[iter]->getMaterial());
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
			renderer->drawElements_w_primitiveRestart(shader, mappedTerrains[i]->getVAO(), mappedTerrains[i]->getMaterial());
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

	glUniform1i(glGetUniformLocation(shader, "displayShadow"), ifShadow);

	farPlane_camEye_toShader(shader);
	camera->passViewProjToShader(shader);
	lightingManager->lightsToShader(shader);

	timePassed += dt_secs;
	glDisable(GL_CULL_FACE);
	for (unsigned int iter = 0; iter < terrains.size(); iter++) {
		lightsToShader(shader, terrains[iter]->getPointLightID(0), terrains[iter]->getPointLightID(1), terrains[iter]->getPointLightID(2), terrains[iter]->getSpotLightID(0), terrains[iter]->getSpotLightID(1), terrains[iter]->getSpotLightID(2));
		glUniformMatrix4fv(glGetUniformLocation(shader, "model"), 1, GL_FALSE, glm::value_ptr(*(terrains[iter]->getModelMatrix_Grass())));
		glUniform1f(glGetUniformLocation(shader, "timePassed"), timePassed);
		renderer->drawArrays(terrains[iter]->getVAO_Grass());
		//terrains[iter]->renderGrass(shader, dt_secs);
	}
	glEnable(GL_CULL_FACE);
	glBindTexture(GL_TEXTURE_2D, 0);
	//Common::unbindTextures(0);
}

void TerrainManager::shadowDraw(unsigned int shader, Renderer* renderer) {
	for (unsigned int iter = 0; iter < terrains.size(); iter++) {
		renderer->drawElements_w_primitiveRestart(shader, terrains[iter]->getVAO(), terrains[iter]->getMaterial());
	}
}

void TerrainManager::lightsToShader(unsigned int shader, int point_id1, int point_id2, int point_id3, int spot_id1, int spot_id2, int spot_id3) {
	lightingManager->lightIDsToShader(shader, point_id1, point_id2, point_id3, spot_id1, spot_id2, spot_id3);
	lightingManager->shadowMapsToShader(shader, point_id1, point_id2, point_id3, spot_id1, spot_id2, spot_id3);
	//lightingManager->spotShadowMapsToShader(shader, spotLights_id1, spotLights_id2);
}

void TerrainManager::farPlane_camEye_toShader(unsigned int shader) {
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