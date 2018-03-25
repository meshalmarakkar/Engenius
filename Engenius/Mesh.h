#pragma once

#include <GL/glew.h> // Contains all the necessery OpenGL includes
#include <glm/glm.hpp>

#include "Common.h"

#pragma comment(lib, "assimp.lib")
#include <assimp/Importer.hpp>

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
	GLuint id;
	string type;
	aiString path;
};
class Mesh {
public:
	/*  Mesh Data  */
	vector<Vertex> vertices;
	vector<GLuint> indices;
	vector<Texture> textures;
	/*  Functions  */
	// Constructor
	
	Mesh(const vector<Vertex> vertices, const vector<GLuint> indices, const vector<Texture> textures);
	//~Mesh();
	// Render the mesh
	void InstancedDraw(GLuint shader, const std::vector<glm::mat4> modelMatrices, const std::vector<glm::vec2> pointIDs, const std::vector<glm::vec2> spotIDs);
	void Draw(GLuint shader, const glm::mat4 modelMatrices);

	void bindWall(GLuint shader);
	void unbindWall();
	void drawWall(GLuint shader, const glm::mat4 modelMatrix);

private:
	/*  Render data  */
	GLuint VAO, VBO, EBO, vboModel, vboPointIDs, vboSpotIDs;
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

	#define NUM_BONES_PER_VERTEX	4
};


