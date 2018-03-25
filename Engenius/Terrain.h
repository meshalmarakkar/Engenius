#ifndef TERRAIN
#define TERRAIN

#include <glm/gtc/type_ptr.hpp>
#include <GL/glew.h> 
#include <vector>
#include <glm/gtc/matrix_transform.hpp>
#include "Common.h"

class Terrain {
public:
	Terrain(glm::vec3 position, float size, GLuint texture, const float vertHeights[], GLuint specularMap = 0);
	Terrain(glm::vec3 position, float size, GLuint texture, GLuint normalMap, const float vertHeights[], GLuint specularMap = 0);
	float getTerrainHeight(float const worldX, const float worldZ);
	glm::vec3 getCentre();
	bool get_ifMapped();
	void Draw(GLuint shader);
	void DrawMapped(GLuint shader);
	bool ifInTerrain(const float x, const float y);
	
	void setPointLightIDs(int id1, int id2);
	int getPointLightID(unsigned int i);

	void setSpotLightIDs(int id1, int id2);
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
	GLuint texture;
	GLuint normalMap;
	GLuint specularMap;
	bool hasSpecular;
	bool mapped;
	const float TERRAIN_SIZE;
	unsigned int pointLightIDs[2]; //lights that effect object
	unsigned int spotLightIDs[2]; //lights that effect object

	int LAST_VERTEX;
	#define VERTEX_COUNT 10
	const int count = VERTEX_COUNT * VERTEX_COUNT;
	float heights[VERTEX_COUNT][VERTEX_COUNT];
	
	GLuint VBOTexCoords;
	GLuint VBONormals; 
	GLuint VBOVertices; 
	GLuint VBOIndices;
	GLuint VBOTangents;
	GLuint VBOBitangents;
	GLuint VAOHeightmap; // One VAO for heightmap
};

#endif
