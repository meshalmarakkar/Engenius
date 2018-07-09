#pragma once

#ifndef MATRIX4F
#define MATRIX4F

#include <assimp/Importer.hpp>

class Matrix4f
{
public:
	float m[4][4];

	Matrix4f(){	}
	Matrix4f(const aiMatrix4x4& AssimpMatrix)
	{
		m[0][0] = AssimpMatrix.a1; m[0][1] = AssimpMatrix.a2; m[0][2] = AssimpMatrix.a3; m[0][3] = AssimpMatrix.a4;
		m[1][0] = AssimpMatrix.b1; m[1][1] = AssimpMatrix.b2; m[1][2] = AssimpMatrix.b3; m[1][3] = AssimpMatrix.b4;
		m[2][0] = AssimpMatrix.c1; m[2][1] = AssimpMatrix.c2; m[2][2] = AssimpMatrix.c3; m[2][3] = AssimpMatrix.c4;
		m[3][0] = AssimpMatrix.d1; m[3][1] = AssimpMatrix.d2; m[3][2] = AssimpMatrix.d3; m[3][3] = AssimpMatrix.d4;
	}
	Matrix4f(const aiMatrix3x3& AssimpMatrix)
	{
		m[0][0] = AssimpMatrix.a1; m[0][1] = AssimpMatrix.a2; m[0][2] = AssimpMatrix.a3; m[0][3] = 0.0f;
		m[1][0] = AssimpMatrix.b1; m[1][1] = AssimpMatrix.b2; m[1][2] = AssimpMatrix.b3; m[1][3] = 0.0f;
		m[2][0] = AssimpMatrix.c1; m[2][1] = AssimpMatrix.c2; m[2][2] = AssimpMatrix.c3; m[2][3] = 0.0f;
		m[3][0] = 0.0f; m[3][1] = 0.0f; m[3][2] = 0.0f; m[3][3] = 1.0f;
	}

	inline void InitIdentity()
	{
		m[0][0] = 1.0f; m[0][1] = 0.0f; m[0][2] = 0.0f; m[0][3] = 0.0f;
		m[1][0] = 0.0f; m[1][1] = 1.0f; m[1][2] = 0.0f; m[1][3] = 0.0f;
		m[2][0] = 0.0f; m[2][1] = 0.0f; m[2][2] = 1.0f; m[2][3] = 0.0f;
		m[3][0] = 0.0f; m[3][1] = 0.0f; m[3][2] = 0.0f; m[3][3] = 1.0f;
	}

	inline Matrix4f operator*(const Matrix4f& Right) const
	{
		Matrix4f Ret;
		for (unsigned int i = 0; i < 4; i++) {
			for (unsigned int j = 0; j < 4; j++) {
				Ret.m[i][j] = m[i][0] * Right.m[0][j] +
					m[i][1] * Right.m[1][j] +
					m[i][2] * Right.m[2][j] +
					m[i][3] * Right.m[3][j];
			}
		}
		return Ret;
	}

	operator const float*() const {	return &(m[0][0]); }

	void InitScaleTransform(float ScaleX, float ScaleY, float ScaleZ) {
		m[0][0] = ScaleX; m[0][1] = 0.0f;   m[0][2] = 0.0f;   m[0][3] = 0.0f;
		m[1][0] = 0.0f;   m[1][1] = ScaleY; m[1][2] = 0.0f;   m[1][3] = 0.0f;
		m[2][0] = 0.0f;   m[2][1] = 0.0f;   m[2][2] = ScaleZ; m[2][3] = 0.0f;
		m[3][0] = 0.0f;   m[3][1] = 0.0f;   m[3][2] = 0.0f;   m[3][3] = 1.0f;
	}
	void InitTranslationTransform(float x, float y, float z) {
		m[0][0] = 1.0f; m[0][1] = 0.0f; m[0][2] = 0.0f; m[0][3] = x;
		m[1][0] = 0.0f; m[1][1] = 1.0f; m[1][2] = 0.0f; m[1][3] = y;
		m[2][0] = 0.0f; m[2][1] = 0.0f; m[2][2] = 1.0f; m[2][3] = z;
		m[3][0] = 0.0f; m[3][1] = 0.0f; m[3][2] = 0.0f; m[3][3] = 1.0f;
	}
};

#endif

#ifndef MODEL
#define MODEL

#include "Mesh.h"
#include "Renderer.h"

// Std. Includes
#include <string>
#include <iostream>
#include <vector>
using namespace std;
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
	Model(const string path, const bool justAnimation);
	Model(const std::string name, const string path);
	Model(const std::string name, const string path, const char* diffuse, const char* specular = NULL);
	Model(const std::string name, const string path, const char* diffuse, const char* specular, const char* normal);
	Model(const std::string name, const string path, const char* diffuse, const char* specular, const char* normal, const char* height, const float heightScale = 0.08f);

	void setParallaxed(const bool newValue);

	bool getIfMapped();

	std::vector<Mesh>* getMeshes() { return &meshes; }

	bool getToInstance();
	void setToInstance(const bool newValue);

	void setIfDisableCullFace(bool newVal);
	bool getIfDisableCullFace();

	void setName(std::string newVal);
	std::string getName();
	
	// Draws the model, and thus all its meshes
	void InstancedDraw(unsigned int shader, const std::vector<glm::mat4> modelMatrices, const std::vector<glm::vec2> pointIDs, const std::vector<glm::vec2> spotIDs);
	void Draw(unsigned int shader, const glm::mat4 modelMatrices, Renderer* renderer);
	void Draw(unsigned int shader, const glm::mat4 modelMatrices);

	void bindWall(unsigned int shader);
	void unbindWall();
	void drawWall(unsigned int shader, const glm::mat4 modelMatrix, Renderer* renderer);

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
	void loadTextures(vector<Texture> &textures, const char* str, string typeName);
	vector<Texture> loadMaterialTextures(aiMaterial* mat, aiTextureType type, string typeName);
	void loadModel(bool onlyAnimation = false);
	Mesh processMesh(aiMesh* mesh);

	bool mapped;
	bool parallaxed;
	bool hasSpecularMap;
	float heightScale;
	int numVertices;
	int numFaces;
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



