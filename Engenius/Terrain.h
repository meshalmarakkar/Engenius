#ifndef TERRAIN
#define TERRAIN

#include <glm/gtc/type_ptr.hpp>
#include <GL/glew.h> 
#include <vector>
#include <glm/gtc/matrix_transform.hpp>
#include "Common.h"

class Terrain {
public:
	Terrain(glm::vec3 position, float size, unsigned int texture, float tiling, const float vertHeights[], unsigned int specularMap = 0);
	Terrain(glm::vec3 position, float size, unsigned int texture, unsigned int normalMap, float tiling, const float vertHeights[], unsigned int specularMap = 0);
	float getTerrainHeight(float const worldX, const float worldZ);
	glm::vec3 getCentre();
	bool get_ifMapped();
	void Draw(unsigned int shader);
	void DrawMapped(unsigned int shader);
	void renderGrass(unsigned int shader, float dt_secs);
	bool ifInTerrain(const float x, const float y);
	
	void setPointLightIDs(int id1, int id2, int id3);
	int getPointLightID(unsigned int i);

	void setSpotLightIDs(int id1, int id2, int id3);
	int getSpotLightID(unsigned int i);

private:
	glm::vec3 calculateNormal(int x, int z);
	void init(const float vertHeights[]);
	glm::mat4 model;
	unsigned int indicesCount;
	glm::vec3 position;
	glm::vec3 terrain_centre;
	float terrain_left;
	float terrain_right;
	float terrain_up;
	float terrain_down;
	unsigned int texture;
	unsigned int normalMap;
	unsigned int specularMap;
	bool hasSpecular;
	bool mapped;
	float tiling;
	const float TERRAIN_SIZE;
	unsigned int pointLightIDs[3]; //lights that effect object
	unsigned int spotLightIDs[3]; //lights that effect object

	int LAST_VERTEX;
	#define VERTEX_COUNT 10
	const int count = VERTEX_COUNT * VERTEX_COUNT;
	float heights[VERTEX_COUNT][VERTEX_COUNT];

	int numGrassTriangles;
	std::vector<glm::vec3> patchPositions;
	float timePassed;
	glm::mat4 grassModelMat;
	
	unsigned int VBOTexCoords;
	unsigned int VBONormals; 
	unsigned int VBOVertices; 
	unsigned int VBOIndices;
	unsigned int VBOTangents;
	unsigned int VBOBitangents;
	unsigned int VAOHeightmap; // One VAO for heightmap
	
	
	unsigned int VBOGrassData;
	unsigned int VAOGrass;
};

#endif
