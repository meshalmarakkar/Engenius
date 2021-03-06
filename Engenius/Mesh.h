#pragma once

#include <glm/glm.hpp>

#include "Common.h"
#include "TextureLoader.h"
#include "Material.h"

#pragma comment(lib, "assimp.lib")
#include <assimp/Importer.hpp>

#include "VertexArrayObject.h"

#include <string>
#include <vector>
using namespace std;

struct Vertex {
	// Position
	glm::vec3 Position;
	// Normal
	glm::vec3 Normal;
	// TexCoords
	glm::vec2 TexCoords;
	// Tangents
	glm::vec3 Tangents;
	// BiTangents
	glm::vec3 Bitangents;
	// Bone ID
	float weight[4];
	GLbyte id[4];

};
struct Texture {
	unsigned int id;
	string type;
	aiString path;
};
class Mesh {
public:
	/*  Mesh Data  */

	/*  Functions  */
	// Constructor
	
	Mesh(const vector<Vertex>& vertices, const vector<unsigned int>& indices, const vector<Texture>& textures);
	//~Mesh();

	VertexArrayObject* getVAO();

	void prepareInstancedDraw(const std::vector<glm::mat4>* modelMatrices, const std::vector<glm::ivec3>* pointIDs, const std::vector<glm::ivec3>* spotIDs, const std::vector<float>* tiling);

	void prepareInstancedDraw(const std::vector<glm::mat4>* modelMatrices);

	Material* getMaterial();

private:	
	vector<Vertex> vertices;
	vector<unsigned int> indices;
	vector<Texture> textures;

	Material* material;

	/*  Render data  */
	VertexArrayObject * VAO;
	unsigned int ID_vboModel, ID_vboPoint, ID_vboSpot, ID_vboTiling;
	/*  Functions    */
	// Initializes all the buffer objects/arrays
	void setupMesh();

	#define LOCATION_VERTEX			0
	#define LOCATION_NORMAL			1
	#define LOCATION_TEXCOORD		2
	#define LOCATION_TANGENT		3
	#define LOCATION_BITANGENT		4
	#define LOCATION_BONE_ID		5
	#define LOCATION_BONE_WEIGHT	6
	#define LOCATION_MODEL1			7
	#define LOCATION_MODEL2			8
	#define LOCATION_MODEL3			9
	#define LOCATION_MODEL4			10
	#define LOCATION_POINTID		11
	#define LOCATION_SPOTID			12
	#define LOCATION_TILING			13

	#define NUM_BONES_PER_VERTEX	4
};


