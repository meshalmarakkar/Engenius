#ifndef MODEL
#define MODEL

#include "Mesh.h"
#include "Matrix4f.h"

#include <string>
#include <iostream>
#include <vector>

#include <glm/gtc/type_ptr.hpp>
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

	void setIfDisableCullFace(const bool& newVal);
	bool getIfDisableCullFace();

	void setName(std::string newVal);
	std::string getName();

	void batch_modelMatrices(const glm::mat4& modelMatrix);
	void batch_pointIDs(const glm::ivec3& pointID);
	void batch_spotIDs(const glm::ivec3& spotID);
	void batch_tilings(const float& tiling);

	void cleanup();
	std::vector<Mesh>* getMeshes();
	std::vector<glm::mat4>* getModelMatrices();
	std::vector<glm::ivec3>* getPointIDs();
	std::vector<glm::ivec3>* getSpotIDs();
	std::vector<float>* getTilings();

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
	std::vector<Texture> loadMaterialTextures(aiMaterial* mat, aiTextureType type, const std::string& typeName);
	void loadModel(const bool& onlyAnimation = false);
	Mesh processMesh(aiMesh* mesh);

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

	std::vector<glm::mat4> modelMatrices;
	std::vector<glm::ivec3> pointIDs;
	std::vector<glm::ivec3> spotIDs;
	std::vector<float> tilings;
};

#endif



