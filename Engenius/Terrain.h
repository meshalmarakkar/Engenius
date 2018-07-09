#ifndef TERRAIN
#define TERRAIN

#include <glm/gtc/type_ptr.hpp>
#include <GL/glew.h> 
#include <vector>
#include <glm/gtc/matrix_transform.hpp>
#include "Common.h"
#include "VertexArrayObject.h"
#include "Material.h"

#define VERTEX_COUNT 10
#define LAST_VERTEX (VERTEX_COUNT - 1)
#define COUNT (VERTEX_COUNT * VERTEX_COUNT)

class Terrain {
public:	
	Terrain(const std::vector<glm::vec3> verts, const std::vector<glm::vec3> normals, const std::vector<glm::vec2> texCoords, std::vector<unsigned int> indices, glm::vec3 pos, const float size, GLuint tex_dif, GLuint tex_spec, const float heights[VERTEX_COUNT][VERTEX_COUNT], float tiling);
	Terrain(const std::vector<glm::vec3> verts, const std::vector<glm::vec3> normals, const std::vector<glm::vec2> texCoords, const std::vector<glm::vec3> tangents, const std::vector<glm::vec3> bitangents, std::vector<unsigned int> indices, glm::vec3 pos, const float size, GLuint tex_dif, GLuint tex_spec, GLuint tex_norm, const float heights[VERTEX_COUNT][VERTEX_COUNT], float tiling);
	
	float getTerrainHeight(float const worldX, const float worldZ);
	glm::vec3 getCentre();
	bool get_ifMapped();
	
	bool ifInTerrain(const float x, const float y);
	
	void setPointLightIDs(int id1, int id2, int id3);
	int getPointLightID(unsigned int i);

	void setSpotLightIDs(int id1, int id2, int id3);
	int getSpotLightID(unsigned int i);

	VertexArrayObject * getVAO();
	Material* getMaterial();
	float getTiling();

	VertexArrayObject * getVAO_Grass();
	glm::mat4 * getModelMatrix_Grass();

private:
	void init(bool ifMapped);
	void initGrass();

	float barryCentric(glm::vec3 p1, glm::vec3 p2, glm::vec3 p3, glm::vec2 pos);

private:
	VertexArrayObject * VAO;
	Material* material;

	glm::vec3 position;
	glm::vec3 terrain_centre;
	float terrain_left;
	float terrain_right;
	float terrain_up;
	float terrain_down;

	bool mapped;
	float tiling;
	const float TERRAIN_SIZE;
	unsigned int pointLightIDs[3]; //lights that effect object
	unsigned int spotLightIDs[3]; //lights that effect object

	float heights[VERTEX_COUNT][VERTEX_COUNT];

	VertexArrayObject * VAOGrass;
	glm::mat4 grassModelMat;
};

#endif
