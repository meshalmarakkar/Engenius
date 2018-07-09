#ifndef TERRAINMANAGER
#define TERRAINMANAGER

#include "Terrain.h"
#include "LightingManager.h"
#include "Camera.h"
#include "ShaderManager.h"
#include "TextureLoader.h"
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
	TerrainManager(LightingManager* lightingManager, Camera* camera, ShaderManager* shaderManager);
	float getTerrainHeight(const float x, const float z);
	void render(Renderer* renderer, unsigned int ifShadow, glm::vec3 playerPos, float dt_secs);
	void shadowDraw(unsigned int shader, Renderer* render);

private:
	BaseAttributes calculateBaseAttributes(float size, const float vertHeights[]);

	void createTerrain(glm::vec3 pos, float size, std::string textype_diff, std::string texpath_diff, std::string textype_spec, std::string texpath_spec, std::string textype_norm, std::string texpath_norm, const float vertHeights[], float tiling = 16.0f);
	void createTerrain(glm::vec3 pos, float size, std::string textype_diff, std::string texpath_diff, std::string textype_spec, std::string texpath_spec, const float vertHeights[], float tiling = 16.0f);

	glm::vec3 calculateNormal(int x, int z, float heights[VERTEX_COUNT][VERTEX_COUNT]);

	bool is_near(float v1, float v2);
	bool getSimilarVertexIndex(glm::vec3 & in_vertex, glm::vec2 & in_uv, glm::vec3 & in_normal, std::vector<glm::vec3>& out_vertices, std::vector<glm::vec2>& out_uvs, std::vector<glm::vec3>& out_normals, unsigned int & result);
	void indexVBO_TBN(std::vector<glm::vec3>& in_vertices, std::vector<glm::vec2>& in_uvs, std::vector<glm::vec3>& in_normals, std::vector<glm::vec3>& in_tangents, std::vector<glm::vec3>& in_bitangents, std::vector<glm::vec3>& out_vertices, std::vector<glm::vec2>& out_uvs, std::vector<glm::vec3>& out_normals, std::vector<glm::vec3>& out_tangents, std::vector<glm::vec3>& out_bitangents);
	void indexVBO(std::vector<glm::vec3>& in_vertices, std::vector<glm::vec2>& in_uvs, std::vector<glm::vec3>& in_normals, std::vector<glm::vec3>& out_vertices, std::vector<glm::vec2>& out_uvs, std::vector<glm::vec3>& out_normals);
	void computeTangentBasis(std::vector<glm::vec3>& vertices, std::vector<glm::vec2>& textureCoords, std::vector<unsigned int>& indices, std::vector<glm::vec3>& normals, std::vector<glm::vec3>& tangents, std::vector<glm::vec3>& bitangents);

	void initTerrainToWorld();
	void lightsToShader(unsigned int shader, int point_id1, int point_id2, int point_id3, int spot_id1, int spot_id2, int spot_id3);
	void farPlane_camEye_toShader(unsigned int shader);
	void addLightIDs();

private:
	LightingManager * lightingManager;
	Camera* camera;
	ShaderManager* shaderManager;

	////GRASS////
	float alphaTest;
	float windStrength;
	glm::vec3 windDirection;
	unsigned int grassTexture;
	float timePassed;

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