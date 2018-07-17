#include "Terrain.h"
#include <iostream>

Terrain::Terrain(const std::vector<glm::vec3> verts, const std::vector<glm::vec3> normals, const std::vector<glm::vec2> texCoords, std::vector<unsigned int> indices, glm::vec3 pos, const float size, Material* mat, const float heights[VERTEX_COUNT][VERTEX_COUNT], Material* mat_grass) : TERRAIN_SIZE(size), position(pos) {
	for (int z = 0; z < VERTEX_COUNT; z++) {
		for (int x = 0; x < VERTEX_COUNT; x++) {
			this->heights[x][z] = heights[x][z];
		}
	}

	int addition = VERTEX_COUNT * 2;
	for (int i = 1; i < LAST_VERTEX; i++) {
		std::vector<unsigned int>::iterator it = indices.begin();
		indices.insert(it + addition, COUNT);//insert adds before the chosen spot
		addition += VERTEX_COUNT * 2 + 1; //+1 cause insert means theres one more
	}

	const float HM_SIZE_X = VERTEX_COUNT;
	const float HM_SIZE_Y = VERTEX_COUNT;
	const int indicesCount = static_cast<unsigned int>(HM_SIZE_X*(HM_SIZE_Y - 1) * 2 + HM_SIZE_Y - 2);

	
	VAO = new VertexArrayObject(indicesCount, GL_TRIANGLE_STRIP);
	typedef Buffer_Options b;
	//verts
	VAO->genBuffer_andAddData(b::gl_array_buffer, sizeof(glm::vec3)*verts.size(), &verts[0], b::gl_static_draw);
	VAO->enableVertexAttribArray(0, 3, b::gl_float, b::gl_false, 0, 0);
	//normals
	VAO->genBuffer_andAddData(b::gl_array_buffer, sizeof(glm::vec3)*normals.size(), &normals[0], b::gl_static_draw);
	VAO->enableVertexAttribArray(1, 3, b::gl_float, b::gl_false, 0, 0);
	//texCoords
	VAO->genBuffer_andAddData(b::gl_array_buffer, sizeof(glm::vec2)*texCoords.size(), &texCoords[0], b::gl_static_draw);
	VAO->enableVertexAttribArray(2, 2, b::gl_float, b::gl_false, 0, 0);
	
	//indices
	VAO->genBuffer_andAddData(b::gl_element_array_buffer, sizeof(unsigned int) * indices.size(), &indices[0], b::gl_static_draw);
	//	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, VBOIndices);
	//	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * indices.size(), &indices[0], GL_STATIC_DRAW);

	VAO->setRestartIndex(COUNT);

	Common::createModelMatrix(model, this->position, 0.0f, glm::vec3(1.0f, 1.0f, 1.0f));
	renderProperties = new RenderProperties(mat, &model);
	//material = new Material(tex_dif, tex_spec, 0, model, tiling);

	float HALF_TERRAIN_SIZE = TERRAIN_SIZE * 0.5f;
	terrain_centre = glm::vec3(position.x + HALF_TERRAIN_SIZE, position.y, position.z - HALF_TERRAIN_SIZE);

	terrain_left = position.x;
	terrain_right = position.x + TERRAIN_SIZE;
	terrain_up = position.z;
	terrain_down = position.z + TERRAIN_SIZE;

	mapped = false;

	if (!(mat_grass == nullptr))
		initGrass(mat_grass);
//	glBindVertexArray(0);
}

Terrain::Terrain(const std::vector<glm::vec3> verts, const std::vector<glm::vec3> normals, const std::vector<glm::vec2> texCoords, const std::vector<glm::vec3> tangents, const std::vector<glm::vec3> bitangents, std::vector<unsigned int> indices, glm::vec3 pos, const float size, Material* mat, const float heights[VERTEX_COUNT][VERTEX_COUNT], Material* mat_grass) : TERRAIN_SIZE(size), position(pos) {
	
	for (int z = 0; z < VERTEX_COUNT; z++) {
		for (int x = 0; x < VERTEX_COUNT; x++) {
			this->heights[x][z] = heights[x][z];
		}
	}

	VAO = new VertexArrayObject(GL_TRIANGLE_STRIP);
	typedef Buffer_Options b;
	//verts
	VAO->genBuffer_andAddData(b::gl_array_buffer, sizeof(glm::vec3)*verts.size(), &verts[0], b::gl_static_draw);
	VAO->enableVertexAttribArray(0, 3, b::gl_float, b::gl_false, 0, 0);
	//normals
	VAO->genBuffer_andAddData(b::gl_array_buffer, sizeof(glm::vec3)*normals.size(), &normals[0], b::gl_static_draw);
	VAO->enableVertexAttribArray(1, 3, b::gl_float, b::gl_false, 0, 0);
	//texCoords
	VAO->genBuffer_andAddData(b::gl_array_buffer, sizeof(glm::vec2)*texCoords.size(), &texCoords[0], b::gl_static_draw);
	VAO->enableVertexAttribArray(2, 2, b::gl_float, b::gl_false, 0, 0);
	//tangents
	VAO->genBuffer_andAddData(b::gl_array_buffer, sizeof(glm::vec3)*tangents.size(), &tangents[0], b::gl_static_draw);
	VAO->enableVertexAttribArray(3, 3, b::gl_float, b::gl_false, 0, 0);
	//bitangents
	VAO->genBuffer_andAddData(b::gl_array_buffer, sizeof(glm::vec3)*bitangents.size(), &bitangents[0], b::gl_static_draw);
	VAO->enableVertexAttribArray(4, 3, b::gl_float, b::gl_false, 0, 0);

	//indices
	int addition = VERTEX_COUNT * 2;
	for (int i = 1; i < VERTEX_COUNT - 1; i++) {
		std::vector<unsigned int>::iterator it = indices.begin();
		indices.insert(it + addition, COUNT);//insert adds before the chosen spot
		addition += VERTEX_COUNT * 2 + 1; //+1 cause insert means theres one more
	}
	VAO->genBuffer_andAddData(b::gl_element_array_buffer, sizeof(unsigned int) * indices.size(), &indices[0], b::gl_static_draw);
//	VAO->enableVertexAttribArray_I(5, 1, b::gl_byte, 0, 0);
	//	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, VBOIndices);
	//	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * indices.size(), &indices[0], GL_STATIC_DRAW);

	const float HM_SIZE_X = VERTEX_COUNT;
	const float HM_SIZE_Y = VERTEX_COUNT;
	VAO->setIndicesCount(static_cast<unsigned int>(HM_SIZE_X*(HM_SIZE_Y - 1) * 2 + HM_SIZE_Y - 2));

	VAO->setRestartIndex(COUNT);

	Common::createModelMatrix(model, this->position, 0.0f, glm::vec3(1.0f, 1.0f, 1.0f));
	renderProperties = new RenderProperties(mat, &model);

	float HALF_TERRAIN_SIZE = TERRAIN_SIZE * 0.5f;
	terrain_centre = glm::vec3(position.x + HALF_TERRAIN_SIZE, position.y, position.z - HALF_TERRAIN_SIZE);

	terrain_left = position.x;
	terrain_right = position.x + TERRAIN_SIZE;
	terrain_up = position.z;
	terrain_down = position.z + TERRAIN_SIZE;

	mapped = true;

	if (!(mat_grass == nullptr))
		initGrass(mat_grass);
//	glBindVertexArray(0);
}

void Terrain::initGrass(Material* mat_grass) {
	float grassPatchOffsetMin = 0.25f;//1.5f
	float grassPatchOffsetMax = 0.75f;//2.5f
									  //float grassPatchHeight = 5.0f;

	glm::vec3 currentPatchPos(terrain_left + grassPatchOffsetMin, 0.0f, terrain_down - grassPatchOffsetMin);
	int numGrassTriangles = 0;
	std::vector<glm::vec3> patchPositions;

	while (currentPatchPos.x < terrain_right) {
		currentPatchPos.z = terrain_down - grassPatchOffsetMin;

		while (currentPatchPos.z > terrain_up)
		{
			currentPatchPos.y = getTerrainHeight(currentPatchPos.x, currentPatchPos.z) - 0.15f;// 0.3f;
			patchPositions.push_back(currentPatchPos);

			numGrassTriangles += 1;

			currentPatchPos.z -= grassPatchOffsetMin + (grassPatchOffsetMax - grassPatchOffsetMin) * float(rand() % 1000) * 0.001f;
		}

		currentPatchPos.x += grassPatchOffsetMin + (grassPatchOffsetMax - grassPatchOffsetMin) * float(rand() % 1000) * 0.001f;
	}

	//has to be at origin. position respective of this. think unity objects and position respective of group
	Common::createModelMatrix(grassModelMat, glm::vec3(0.0f), glm::vec3(1.0f, 0.25f, 1.0f), glm::vec3(0.0f));

	renderProperties_grass = new RenderProperties(mat_grass, &grassModelMat);

	VAOGrass = new VertexArrayObject(numGrassTriangles, Buffer_Options::gl_points);
	typedef Buffer_Options b;
	VAOGrass->genBuffer_andAddData(b::gl_array_buffer, sizeof(glm::vec3)*patchPositions.size(), &patchPositions[0], b::gl_static_draw);
	VAOGrass->enableVertexAttribArray(0, 3, b::gl_float, b::gl_false, 0, 0);
}

float Terrain::barryCentric(const glm::vec3& p1, const glm::vec3& p2, const glm::vec3& p3, const glm::vec2& pos) {
	float det = (p2.z - p3.z) * (p1.x - p3.x) + (p3.x - p2.x) * (p1.z - p3.z);
	float l1 = ((p2.z - p3.z) * (pos.x - p3.x) + (p3.x - p2.x) * (pos.y - p3.z)) / det;
	float l2 = ((p3.z - p1.z) * (pos.x - p3.x) + (p1.x - p3.x) * (pos.y - p3.z)) / det;
	float l3 = 1.0f - l1 - l2;
	return l1 * p1.y + l2 * p2.y + l3 * p3.y;
}

bool Terrain::ifInTerrain(const float& x, const float& z) {
	if (x >= terrain_left
		&& x <= terrain_right) {
		if (z >= terrain_up
			&& z <= terrain_down) {
			return true;
		}				
	}
	return false;
}

float Terrain::getTerrainHeight(const float& worldX, const float& worldZ) {
	float terrainX = worldX - this->position.x;
	float terrainZ = worldZ - this->position.z;
	float gridSquareSize = TERRAIN_SIZE / ((float)LAST_VERTEX);
	int gridX = (int)std::floor(terrainX / gridSquareSize);
	int gridZ = (int)std::floor(terrainZ / gridSquareSize);
	if (gridX >= LAST_VERTEX || gridZ >= LAST_VERTEX || gridX < 0 || gridZ < 0) {//outside grid
		return 0.0f;
	}

	float xCoord = std::fmod(terrainX, gridSquareSize) / gridSquareSize;
	float zCoord = std::fmod(terrainZ, gridSquareSize) / gridSquareSize;

	float answer;
	if (xCoord <= (1 - zCoord)) {
		answer = barryCentric(glm::vec3(0, heights[gridX][gridZ], 0), glm::vec3(1, heights[gridX + 1][gridZ], 0), glm::vec3(0, heights[gridX][gridZ + 1], 1), glm::vec2(xCoord, zCoord));
	}
	else {
		answer = barryCentric(glm::vec3(1, heights[gridX + 1][gridZ], 0), glm::vec3(1, heights[gridX + 1][gridZ + 1], 1), glm::vec3(0, heights[gridX][gridZ + 1], 1), glm::vec2(xCoord, zCoord));
	}

	return answer;
}

glm::vec3 Terrain::getCentre() {
	return terrain_centre;
}

bool Terrain::get_ifMapped() {
	return mapped;
}

void Terrain::setPointLightIDs(const int& id1, const int& id2, const int& id3) {
	pointLightIDs[0] = id1;
	pointLightIDs[1] = id2;
	pointLightIDs[2] = id3;
}

int Terrain::getPointLightID(const unsigned int& i) {
	return pointLightIDs[i];
}

void Terrain::setSpotLightIDs(const int& id1, const int& id2, const int& id3) {
	spotLightIDs[0] = id1;
	spotLightIDs[1] = id2;
	spotLightIDs[2] = id3;
}

int Terrain::getSpotLightID(const unsigned int& i) {
	return spotLightIDs[i];
}

int* Terrain::getPointLightID_Pointer(const unsigned int& i) {
	return &pointLightIDs[i];
}

int* Terrain::getSpotLightID_Pointer(const unsigned int& i) {
	return &spotLightIDs[i];
}

VertexArrayObject* Terrain::getVAO() {
	return VAO;
}

RenderProperties* Terrain::getRenderProperties() {
	return renderProperties;
}

VertexArrayObject * Terrain::getVAO_Grass() {
	return VAOGrass;
}

RenderProperties* Terrain::getRenderProperties_grass() {
	return renderProperties_grass;
}