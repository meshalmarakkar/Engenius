#ifndef TERRAIN
#define TERRAIN

#include <glm/gtc/type_ptr.hpp>
#include <vector>
#include "Common.h"
#include "VertexArrayObject.h"
#include "Material.h"
#include "RenderProperties.h"

#define VERTEX_COUNT 10
#define LAST_VERTEX (VERTEX_COUNT - 1)
#define COUNT (VERTEX_COUNT * VERTEX_COUNT)

class Terrain {
public:	
	Terrain(const std::vector<glm::vec3> verts, const std::vector<glm::vec3> normals, const std::vector<glm::vec2> texCoords, std::vector<unsigned int> indices, glm::vec3 pos, const float size, Material* mat, const float heights[VERTEX_COUNT][VERTEX_COUNT], Material* mat_grass);
	Terrain(const std::vector<glm::vec3> verts, const std::vector<glm::vec3> normals, const std::vector<glm::vec2> texCoords, const std::vector<glm::vec3> tangents, const std::vector<glm::vec3> bitangents, std::vector<unsigned int> indices, glm::vec3 pos, const float size, Material* mat, const float heights[VERTEX_COUNT][VERTEX_COUNT], Material* mat_grass);
	
	float getTerrainHeight(const float& worldX, const float& worldZ);
	glm::vec3 getCentre();
	bool get_ifMapped();
	
	bool ifInTerrain(const float& x, const float& y);
	
	void setPointLightIDs(const int& id1, const int& id2, const int& id3);
	int getPointLightID(const unsigned int& i);

	void setSpotLightIDs(const int& id1, const int& id2, const int& id3);
	int getSpotLightID(const unsigned int& i);

	VertexArrayObject * getVAO();
	RenderProperties* getRenderProperties();

	VertexArrayObject * getVAO_Grass();
	RenderProperties* getRenderProperties_grass();

	int* getPointLightID_Pointer(const unsigned int& i);
	int* getSpotLightID_Pointer(const unsigned int& i);

private:
	void initGrass(Material* mat_grass);

	float barryCentric(const glm::vec3& p1, const glm::vec3& p2, const glm::vec3& p3, const glm::vec2& pos);
private:
	VertexArrayObject * VAO;
	RenderProperties* renderProperties;

	glm::vec3 position;
	glm::vec3 terrain_centre;
	float terrain_left;
	float terrain_right;
	float terrain_up;
	float terrain_down;

	glm::mat4 model;
	glm::mat4 grassModelMat;
	bool mapped;

	const float TERRAIN_SIZE;
	int pointLightIDs[3]; //lights that effect object
	int spotLightIDs[3]; //lights that effect object

	float heights[VERTEX_COUNT][VERTEX_COUNT];

	VertexArrayObject * VAOGrass;
	RenderProperties* renderProperties_grass;
};

#endif
