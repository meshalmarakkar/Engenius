#ifndef MODEL
#define MODEL

#include "Mesh.h"
#include "Renderer.h"
#include "Matrix4f.h"

// Std. Includes
#include <string>
#include <iostream>
#include <vector>

// GL Includes
#include <GL/glew.h> // Contains all the necessery OpenGL includes
#include <glm/glm.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <unordered_map>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>


class Model
{
public:
	// Constructor, expects a filepath to a 3D model.
	Model(const std::string& path, const bool& justAnimation);
	Model(const std::string& name, const string& path);
	Model(const std::string& name, const string& path, const char* diffuse, const char* specular = NULL);
	Model(const std::string& name, const string& path, const char* diffuse, const char* specular, const char* normal);

	bool getIfMapped();

	std::vector<Mesh>* getMeshes() { return &meshes; }

	bool getToInstance();
	void setToInstance(const bool& newValue);

	void setIfDisableCullFace(const bool& newVal);
	bool getIfDisableCullFace();

	void setName(std::string newVal);
	std::string getName();
	
	// Draws the model, and thus all its meshes
	void InstancedDraw(const unsigned int& shader, const std::vector<glm::mat4>& modelMatrices, const std::vector<glm::vec2>& pointIDs, const std::vector<glm::vec2>& spotIDs);
	void Draw(const unsigned int& shader, const glm::mat4& modelMatrices, Renderer* renderer);
	void Draw(const unsigned int& shader, const glm::mat4& modelMatrices);

	void bindWall(const unsigned int& shader);
	void unbindWall();
	void drawWall(const unsigned int& shader, const glm::mat4& modelMatrix, Renderer* renderer);

protected:
	const aiScene* scene;
	std::string path;

	#define NUM_BONES_PER_VERTEX 4
	std::unordered_map<string, unsigned int> m_BoneMapping; // maps a bone name to its index
	unsigned int m_NumBones;
	struct BoneInfo
	{
		Matrix4f BoneOffset;
		Matrix4f FinalTransformation;

		BoneInfo()
		{
			//initialise everything to zero
			for (int x = 0; x < NUM_BONES_PER_VERTEX; x++) {
				for (int y = 0; y < NUM_BONES_PER_VERTEX; y++) {
					BoneOffset.m[x][y] = 0.0f;
					FinalTransformation.m[x][y] = 0.0f;
				}
			}
		}
	};
	std::vector<BoneInfo> m_BoneInfo;

private:
	void loadTextures(vector<Texture> &textures, const char* str, const std::string& typeName);
	vector<Texture> loadMaterialTextures(aiMaterial* mat, aiTextureType type, const std::string& typeName);
	void loadModel(const bool& onlyAnimation = false);
	Mesh processMesh(aiMesh* mesh);

	bool mapped;
	bool hasSpecularMap;
	bool toInstance;
	bool disableCullFace;
	std::string name;

	Assimp::Importer importer;	
	std::vector<Mesh> meshes;

	vector<Texture> textures_loaded;

	struct CustomTextures {
		const char* diffuse;
		const char* specular;
		const char* normal;
		const char* height;
	};
	CustomTextures customTextures;
};

#endif



