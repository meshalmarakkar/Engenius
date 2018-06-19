#include "Terrain.h"
#include <iostream>

Terrain::Terrain(glm::vec3 position, float size, GLuint texture, float tiling, const float vertHeights[], GLuint specularMap) : position(position), TERRAIN_SIZE(size), texture(texture), tiling(tiling), mapped(false), hasSpecular(false) {
	if (specularMap == 0)
		hasSpecular = false;
	init(vertHeights);
}

Terrain::Terrain(glm::vec3 position, float size, GLuint texture, GLuint normalMap, float tiling,  const float vertHeights[], GLuint specularMap) : position(position), TERRAIN_SIZE(size), texture(texture), tiling(tiling), normalMap(normalMap), specularMap(specularMap), mapped(true){
	if (specularMap == 0)
		hasSpecular = false;
	init(vertHeights);
}

// Returns true iif v1 can be considered equal to v2
bool is_near(float v1, float v2) {
	return fabs(v1 - v2) < 0.01f;
}

bool getSimilarVertexIndex(
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

void indexVBO_TBN(
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

void indexVBO(
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

glm::vec3 Terrain::calculateNormal(int x, int z) {
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

void computeTangentBasis(
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

void Terrain::init(const float vertHeights[]) {
	LAST_VERTEX = VERTEX_COUNT - 1;

	float HALF_TERRAIN_SIZE = TERRAIN_SIZE * 0.5f;
	terrain_centre = glm::vec3(position.x + HALF_TERRAIN_SIZE, position.y, position.z - HALF_TERRAIN_SIZE);

	terrain_left = position.x;
	terrain_right = position.x + TERRAIN_SIZE;
	terrain_up = position.z;
	terrain_down = position.z + TERRAIN_SIZE;

	glGenVertexArrays(1, &VAOHeightmap); // Create one VAO
	glGenBuffers(1, &VBOVertices);
	glGenBuffers(1, &VBONormals);
	glGenBuffers(1, &VBOTexCoords);
	glGenBuffers(1, &VBOIndices);

	std::vector<glm::vec3> vertices;
	std::vector<glm::vec3> normals;
	std::vector<glm::vec2> textureCoords;
	vertices.resize(count);
	normals.resize(count);
	textureCoords.resize(count);

	float lakeH[VERTEX_COUNT * VERTEX_COUNT];
	for (int j = 0; j < VERTEX_COUNT * VERTEX_COUNT; j++) {
		lakeH[j] = vertHeights[j];
	}

	int vertexPointer = 0;
	for (int z = 0; z < VERTEX_COUNT; z++) {
		for (int x = 0; x < VERTEX_COUNT; x++) {
			vertices[vertexPointer].x = (float)x / ((float)LAST_VERTEX) * TERRAIN_SIZE;

			vertices[vertexPointer].y = lakeH[vertexPointer];
			heights[x][z] = lakeH[vertexPointer];
			vertices[vertexPointer].z = (float)z / ((float)LAST_VERTEX) * TERRAIN_SIZE;
			textureCoords[vertexPointer].x = (float)x / ((float)LAST_VERTEX);
			textureCoords[vertexPointer].y = (float)z / ((float)LAST_VERTEX);
			vertexPointer++;
		}
	}

	int vp = 0;
	for (int z = 0; z < VERTEX_COUNT; z++) {
		for (int x = 0; x < VERTEX_COUNT; x++) {
			normals[vp] = calculateNormal(x, z);
			vp++;
		}
	}

	std::vector<unsigned int> indices;

	for (int z = 0; z < LAST_VERTEX; z++) { //cause 0-8 and bottom calc goes to 9
		for (int x = 0; x < LAST_VERTEX; x += 2) {
			unsigned int topLeft = (z*VERTEX_COUNT) + x;
			unsigned int topRight = topLeft + 1;
			unsigned int bottomLeft = ((z + 1)*VERTEX_COUNT) + x;
			unsigned int bottomRight = bottomLeft + 1;
			indices.push_back(topLeft);
			indices.push_back(bottomLeft);
			indices.push_back(topRight);
			indices.push_back(bottomRight);
		}
	}

	Common::createModelMatrix(model, this->position, 0.0f, glm::vec3(1.0f, 1.0f, 1.0f));

	std::vector<glm::vec3> indexed_vertices;
	std::vector<glm::vec3> indexed_normals;
	std::vector<glm::vec2> indexed_textureCoords;

	if (mapped) {
		glGenBuffers(1, &VBOTangents);
		glGenBuffers(1, &VBOBitangents);
		std::vector<glm::vec3> tangents;
		std::vector<glm::vec3> bitangents;
		computeTangentBasis(vertices, textureCoords, indices, normals, tangents, bitangents);
		std::vector<glm::vec3> indexed_tangents;
		std::vector<glm::vec3> indexed_bitangents;
		indexVBO_TBN(vertices, textureCoords, normals, tangents, bitangents, indexed_vertices, indexed_textureCoords, indexed_normals, indexed_tangents, indexed_bitangents);

		glBindVertexArray(VAOHeightmap);

		glBindBuffer(GL_ARRAY_BUFFER, VBOTangents);
		glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * indexed_tangents.size(), &indexed_tangents[0], GL_STATIC_DRAW);
		glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 0, 0);

		glBindBuffer(GL_ARRAY_BUFFER, VBOBitangents);
		glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * indexed_bitangents.size(), &indexed_bitangents[0], GL_STATIC_DRAW);
		glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, 0, 0);
	}
	else {
		indexVBO(vertices, textureCoords, normals, indexed_vertices, indexed_textureCoords, indexed_normals);
		glBindVertexArray(VAOHeightmap);
	}

	glBindBuffer(GL_ARRAY_BUFFER, VBOVertices);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * indexed_vertices.size(), &indexed_vertices[0], GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

	glBindBuffer(GL_ARRAY_BUFFER, VBONormals);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * indexed_normals.size(), &indexed_normals[0], GL_STATIC_DRAW);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);

	glBindBuffer(GL_ARRAY_BUFFER, VBOTexCoords);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec2) * indexed_textureCoords.size(), &indexed_textureCoords[0], GL_STATIC_DRAW);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, 0);

	int addition = VERTEX_COUNT * 2;
	for (int i = 1; i < LAST_VERTEX; i++) {
		std::vector<unsigned int>::iterator it = indices.begin();
		indices.insert(it + addition, count);//insert adds before the chosen spot
		addition += VERTEX_COUNT * 2 + 1; //+1 cause insert means theres one more
	}

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, VBOIndices);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * indices.size(), &indices[0], GL_STATIC_DRAW);

	const float HM_SIZE_X = VERTEX_COUNT;
	const float HM_SIZE_Y = VERTEX_COUNT;
	indicesCount = static_cast<unsigned int>(HM_SIZE_X*(HM_SIZE_Y - 1) * 2 + HM_SIZE_Y - 2);
	
	////GRASS////
	glGenBuffers(1, &VBOGrassData);
	float grassPatchOffsetMin = 1.5f;
	float grassPatchOffsetMax = 2.5f;
	float grassPatchHeight = 5.0f;

	//float HALF_TERRAIN_SIZE = TERRAIN_SIZE * 0.5f;
	//terrain_centre = glm::vec3(position.x + HALF_TERRAIN_SIZE, position.y, position.z + HALF_TERRAIN_SIZE);

	//terrain_left = position.x;
	//terrain_right = position.x + TERRAIN_SIZE;
	//terrain_up = position.z;
	//terrain_down = position.z + TERRAIN_SIZE;

	//	terrain centre : 6, 0, 14
	//	terrain_left - 4
	//	terrain_right16
	//	terrain_up4
	//	terrain_down24
	//	num of triangles : 85

	glm::vec3 currentPatchPos(terrain_left + grassPatchOffsetMin, 0.0f, terrain_down - grassPatchOffsetMin);
	numGrassTriangles = 0;

	while (currentPatchPos.x < terrain_right) {
		currentPatchPos.z = terrain_down - grassPatchOffsetMin;

		while (currentPatchPos.z > terrain_up)
		{
			currentPatchPos.y = getTerrainHeight(currentPatchPos.x, currentPatchPos.z);// -0.2f;
			patchPositions.push_back(currentPatchPos);

			numGrassTriangles += 1;

			currentPatchPos.z -= grassPatchOffsetMin + (grassPatchOffsetMax - grassPatchOffsetMin) * float(rand() % 1000) * 0.001f;
		}

		currentPatchPos.x += grassPatchOffsetMin + (grassPatchOffsetMax - grassPatchOffsetMin) * float(rand() % 1000) * 0.001f;
	}

	timePassed = 0.0f;
	std::cout << "num of triangles: " << numGrassTriangles << std::endl;

	for (int p = 0; p < patchPositions.size(); p++) {
		std::cout << p << " -> x:" << (patchPositions[p]).x << ", y:" << patchPositions[p].y << ", z:" << (patchPositions[p]).z << "\n";
	}

	std::cout << "terrain centre: " << terrain_centre.x << ", " << terrain_centre.y << ", " << terrain_centre.z <<
		"\nterrain_left" << terrain_left << "\nterrain_right" << terrain_right <<
		"\nterrain_up" << terrain_up << "\nterrain_down" << terrain_down << std::endl;

	glGenVertexArrays(1, &VAOGrass);
	glBindVertexArray(VAOGrass);
	glBindBuffer(GL_ARRAY_BUFFER, VBOGrassData);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * patchPositions.size(), &patchPositions.at(0), GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);


	glEnableVertexAttribArray(0);
	glBindVertexArray(0);
}

void Terrain::Draw(GLuint shader) {
	glBindVertexArray(VAOHeightmap);
	Common::enableVertexAttribArray(0, 2);

	glUniformMatrix4fv(glGetUniformLocation(shader, "model"), 1, GL_FALSE, glm::value_ptr(model));
	glUniform1f(glGetUniformLocation(shader, "tiling"), tiling);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture);
	glUniform1i(glGetUniformLocation(shader, "texture_diffuse1"), 0);

	if (hasSpecular) {
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, specularMap);
		glUniform1i(glGetUniformLocation(shader, "texture_specular1"), 1);
	}

	glEnable(GL_PRIMITIVE_RESTART);
	glPrimitiveRestartIndex(count);
	glDrawElements(GL_TRIANGLE_STRIP, indicesCount, GL_UNSIGNED_INT, 0);
	glDisable(GL_PRIMITIVE_RESTART);

	Common::unbindTextures(0, 1);
	Common::disableVertexAttribArray(0,2);
	glBindVertexArray(0);
}

void Terrain::DrawMapped(GLuint shader) {
	glBindVertexArray(VAOHeightmap);
	Common::enableVertexAttribArray(0, 4);

	glUniformMatrix4fv(glGetUniformLocation(shader, "model"), 1, GL_FALSE, glm::value_ptr(model));
	glUniform1f(glGetUniformLocation(shader, "tiling"), tiling);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture);
	glUniform1i(glGetUniformLocation(shader, "texture_diffuse1"), 0);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, normalMap);
	glUniform1i(glGetUniformLocation(shader, "texture_normal1"), 1);

	if (hasSpecular) {
		glActiveTexture(GL_TEXTURE2);
		glBindTexture(GL_TEXTURE_2D, specularMap);
		glUniform1i(glGetUniformLocation(shader, "texture_specular1"), 2);
	}

	glEnable(GL_PRIMITIVE_RESTART);
	glPrimitiveRestartIndex(count);
	glDrawElements(GL_TRIANGLE_STRIP, indicesCount, GL_UNSIGNED_INT, 0);
	glDisable(GL_PRIMITIVE_RESTART);

	Common::unbindTextures(0, 2);
	Common::disableVertexAttribArray(0, 4);
	glBindVertexArray(0);
}

void Terrain::renderGrass(GLuint shader, float dt_secs) {
	//GRASS
	glBindVertexArray(VAOGrass);
	Common::enableVertexAttribArray(0);
	glm::mat4 grassModelMat;
	Common::createModelMatrix(grassModelMat, this->position, glm::vec3(1.0f), glm::vec3(0.0f));
	//Common::createModelMatrix(grassModelMat, glm::vec3(terrain_left, 0.0f, terrain_up), glm::vec3(1.0f), glm::vec3(0.0f));
	//Common::createModelMatrix(grassModelMat, terrain_centre, glm::vec3(1.0f), glm::vec3(0.0f));
	glUniformMatrix4fv(glGetUniformLocation(shader, "model"), 1, GL_FALSE, glm::value_ptr(grassModelMat));
	
	glUniform1f(glGetUniformLocation(shader, "timePassed"), timePassed);
	timePassed += dt_secs; 

	glDisable(GL_CULL_FACE);

	glDrawArrays(GL_POINTS, 0, numGrassTriangles);
	Common::disableVertexAttribArray(0, 0);
	glBindVertexArray(0);
	glEnable(GL_CULL_FACE);
}

float barryCentric(glm::vec3 p1, glm::vec3 p2, glm::vec3 p3, glm::vec2 pos) {
	float det = (p2.z - p3.z) * (p1.x - p3.x) + (p3.x - p2.x) * (p1.z - p3.z);
	float l1 = ((p2.z - p3.z) * (pos.x - p3.x) + (p3.x - p2.x) * (pos.y - p3.z)) / det;
	float l2 = ((p3.z - p1.z) * (pos.x - p3.x) + (p1.x - p3.x) * (pos.y - p3.z)) / det;
	float l3 = 1.0f - l1 - l2;
	return l1 * p1.y + l2 * p2.y + l3 * p3.y;
}

bool Terrain::ifInTerrain(const float x, const float z) {
	if (x >= terrain_left
		&& x <= terrain_right) {
		if (z >= terrain_up
			&& z <= terrain_down) {
			return true;
		}				
	}
	return false;
}

float Terrain::getTerrainHeight(float const worldX, const float worldZ) {
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

void Terrain::setPointLightIDs(int id1, int id2, int id3) {
	pointLightIDs[0] = id1;
	pointLightIDs[1] = id2;
	pointLightIDs[2] = id3;
}

int Terrain::getPointLightID(unsigned int i) {
	return pointLightIDs[i];
}

void Terrain::setSpotLightIDs(int id1, int id2, int id3) {
	spotLightIDs[0] = id1;
	spotLightIDs[1] = id2;
	spotLightIDs[2] = id3;
}

int Terrain::getSpotLightID(unsigned int i) {
	return spotLightIDs[i];
}