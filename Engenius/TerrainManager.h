#ifndef TERRAINMANAGER
#define TERRAINMANAGER

#include "Terrain.h"
#include "LightingManager.h"
#include "ShaderManager.h"
#include "Material.h"
#include "Renderer.h"

struct BaseAttributes{
	std::vector<glm::vec3> vertices;
	std::vector<glm::vec3> normals;
	std::vector<glm::vec2> textureCoords;
	std::vector<unsigned int> indices;
	float heights[VERTEX_COUNT][VERTEX_COUNT];
};

class TerrainManager {
public:
	TerrainManager(LightingManager* lightingManager, ShaderManager* shaderManager);
	float getTerrainHeight(const float& x, const float& z);
	void render(Renderer* renderer, const glm::vec3& playerPos, const float& dt_secs);
	void shadowDraw(const unsigned int& shader, Renderer* render);

private:
	BaseAttributes calculateBaseAttributes(const float& size, const float vertHeights[]);

	void createTerrain(const glm::vec3& pos, const float& size, Material* mat, const float vertHeights[], Material* mat_grass = nullptr);

	glm::vec3 calculateNormal(const int& x, const int& z, const float heights[VERTEX_COUNT][VERTEX_COUNT]);

	bool is_near(const float& v1, const float& v2);
	bool getSimilarVertexIndex(glm::vec3 & in_vertex, glm::vec2 & in_uv, glm::vec3 & in_normal, std::vector<glm::vec3>& out_vertices, std::vector<glm::vec2>& out_uvs, std::vector<glm::vec3>& out_normals, unsigned int & result);
	void indexVBO_TBN(std::vector<glm::vec3>& in_vertices, std::vector<glm::vec2>& in_uvs, std::vector<glm::vec3>& in_normals, std::vector<glm::vec3>& in_tangents, std::vector<glm::vec3>& in_bitangents, std::vector<glm::vec3>& out_vertices, std::vector<glm::vec2>& out_uvs, std::vector<glm::vec3>& out_normals, std::vector<glm::vec3>& out_tangents, std::vector<glm::vec3>& out_bitangents);
	void indexVBO(std::vector<glm::vec3>& in_vertices, std::vector<glm::vec2>& in_uvs, std::vector<glm::vec3>& in_normals, std::vector<glm::vec3>& out_vertices, std::vector<glm::vec2>& out_uvs, std::vector<glm::vec3>& out_normals);
	void computeTangentBasis(std::vector<glm::vec3>& vertices, std::vector<glm::vec2>& textureCoords, std::vector<unsigned int>& indices, std::vector<glm::vec3>& normals, std::vector<glm::vec3>& tangents, std::vector<glm::vec3>& bitangents);

	void initTerrainToWorld();
	void addLightIDs();

private:
	LightingManager * lightingManager;
	ShaderManager* shaderManager;

	////GRASS////
	float alphaTest;
	float windStrength;
	glm::vec3 windDirection;
	float timePassed;

	Material* mat_grass;
	Material* mat_ground;

	std::vector<Terrain*> terrains;
	std::vector<Terrain*> mappedTerrains;

	const float heightsPlane[VERTEX_COUNT * VERTEX_COUNT] = {
		0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 
		0.0f, -1.0f, -1.0f, -1.0f, -1.0f, -1.0f, -1.0f, -1.0f, -1.0f, 0.0f,
		0.0f, -1.0f, -1.0f, -1.0f, -1.0f, -1.0f, -1.0f, -1.0f, -1.0f, 0.0f, 
		0.0f, -1.0f, -1.0f, -1.0f, -1.0f, -1.0f, -1.0f, -1.0f, -1.0f, 0.0f,
		0.0f, -1.0f, -1.0f, -1.0f, -1.0f, -1.0f, -1.0f, -1.0f, -1.0f, 0.0f, 
		0.0f, -1.0f, -1.0f, -1.0f, -1.0f, -1.0f, -1.0f, -1.0f, -1.0f, 0.0f,
		0.0f, -1.0f, -1.0f, -1.0f, -1.0f, -1.0f, -1.0f, -1.0f, -1.0f, 0.0f, 
		0.0f, -1.0f, -1.0f, -1.0f, -1.0f, -1.0f, -1.0f, -1.0f, -1.0f, 0.0f,
		0.0f, -1.0f, -1.0f, -1.0f, -1.0f, -1.0f, -1.0f, -1.0f, -1.0f, 0.0f, 
		0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f,
	};

	const float heightsPlaneBumpy[VERTEX_COUNT * VERTEX_COUNT] = {
		0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f,
		0.0f, 0.5f, 0.5f, -0.5f, -0.5f, -0.5f, -0.5f, -0.5f, -0.5f, 0.0f,
		0.0f, 0.5f, 0.5f, -0.5f, -0.5f, -0.5f, 0.5f, 0.5f, -0.5f, 0.0f,
		0.0f, 0.5f, -0.5f, -0.5f, -0.5f, -0.5f, 0.5f, 0.5f, -0.5f, 0.0f,
		0.0f, -0.5f, -0.5f, 0.5f, -0.5f, -0.5f, 0.5f, -0.5f, 0.5f, 0.0f,
		0.0f, -0.5f, 0.5f, 0.5f, -0.5f, -0.5f, -0.5f, -0.5f, 0.5f, 0.0f,
		0.0f, -0.5f, 0.5f, -0.5f, -0.5f, -0.5f, -0.5f, -0.5f, -0.5f, 0.0f,
		0.0f, -0.5f, -0.5f, 0.5f, -0.5f, -0.5f, 0.5f, 0.5f, -0.5f, 0.0f,
		0.0f, -0.5f, -0.5f, -0.5f, -0.5f, -0.5f, 0.5f, 0.5f, -0.5f, 0.0f,
		0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f,
	};

	const float heightsPlaneZERO[VERTEX_COUNT * VERTEX_COUNT] = {
		0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f,
	};

};

#endif