#include "Model.h"
#include "TextureLoader.h"
#include "Material.h"

//just for animation loading
Model::Model(const std::string& path, const bool& justAnimation) : path(path) {
	this->loadModel(justAnimation);
}

//auto texture loading
Model::Model(const std::string& name, const std::string& path) : name(name), path(path) {
	customTextures.diffuse = NULL;
	customTextures.specular = NULL;
	customTextures.normal = NULL;
	customTextures.height = NULL;
	this->loadModel();
}
Model::Model(const std::string& name, const std::string& path, const char* diffuse, const char* specular) : name(name), path(path) {
	customTextures.diffuse = diffuse;
	customTextures.specular = specular;
	customTextures.normal = NULL;
	customTextures.height = NULL;
	this->loadModel();
}
Model::Model(const std::string& name, const std::string& path, const char* diffuse, const char* specular, const char* normal) : name(name), path(path) {
	customTextures.diffuse = diffuse;
	customTextures.specular = specular;
	customTextures.normal = normal;
	customTextures.height = NULL;
	this->loadModel();
}

void Model::setIfDisableCullFace(const bool& newVal) {
	disableCullFace = newVal;
}
bool Model::getIfDisableCullFace() {
	return disableCullFace;
}

void Model::setName(std::string newVal) {
	name = newVal;
}
std::string Model::getName() {
	return name;
}

void Model::batch_modelMatrices(const glm::mat4& modelMatrix) {
	modelMatrices.push_back(modelMatrix);
}
void Model::batch_pointIDs(const glm::ivec3& pointID) {
	pointIDs.push_back(pointID);
}
void Model::batch_spotIDs(const glm::ivec3& spotID) {
	spotIDs.push_back(spotID);
}
void Model::batch_tilings(const float& tiling) {
	tilings.push_back(tiling);
}
void Model::cleanup() {
	modelMatrices.clear();
	pointIDs.clear();
	spotIDs.clear();
	tilings.clear();
}

std::vector<Mesh>* Model::getMeshes() {
	return &meshes;
}

std::vector<glm::mat4>* Model::getModelMatrices() {
	return &modelMatrices;
}
std::vector<glm::ivec3>* Model::getPointIDs() {
	return &pointIDs;
}
std::vector<glm::ivec3>* Model::getSpotIDs() {
	return &spotIDs;
}
std::vector<float>* Model::getTilings() {
	return &tilings;
}

void Model::loadModel(const bool& onlyAnimation)
{
	// Read file via ASSIMP
	scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_FlipUVs | aiProcess_CalcTangentSpace);

	// Check for errors
	if (!scene || scene->mFlags == AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) // if is Not Zero
	{
		if (!onlyAnimation) {
			cout << "ERROR::ASSIMP:: " << importer.GetErrorString() << endl;
		}
		return;
	}
	
	m_NumBones = 0;

	if (!onlyAnimation) {

		// Process all of the meshes
		for (unsigned int i = 0; i < scene->mNumMeshes; i++)
		{
			aiMesh* mesh = scene->mMeshes[i];
			this->meshes.push_back(this->processMesh(mesh));
		//	cout << "vertices: " << mesh->mNumVertices << endl;
		}
	}
}

Mesh Model::processMesh(aiMesh* mesh)
{
	// Data to fill
	vector<Vertex> vertices;
	vector<unsigned int>indices;

	// Walk through each of the mesh's vertices
	for (unsigned int i = 0; i < mesh->mNumVertices; i++)
	{
		Vertex vertex;
		glm::vec3 vector; // We declare a placeholder vector since assimp uses its own vector class that doesn't directly convert to glm's vec3 class so we transfer the data to this placeholder glm::vec3 first.
						  // Positions
		if (mesh->HasPositions()) {
			vector.x = mesh->mVertices[i].x;
			vector.y = mesh->mVertices[i].y;
			vector.z = mesh->mVertices[i].z;
			vertex.Position = vector;
		}
		// Normals
		if (mesh->HasNormals()) {
			vector.x = mesh->mNormals[i].x;
			vector.y = mesh->mNormals[i].y;
			vector.z = mesh->mNormals[i].z;
			vertex.Normal = vector;
		}
		// Texture Coordinates
		if (mesh->HasTextureCoords(0)) // Does the mesh contain texture coordinates? can use this too - mesh->mTextureCoords[0]
		{		
			glm::vec2 vec;
			// A vertex can contain up to 8 different texture coordinates. We thus make the assumption that we won't 
			// use models where a vertex can have multiple texture coordinates so we always take the first set (0).
			vec.x = mesh->mTextureCoords[0][i].x;
			vec.y = mesh->mTextureCoords[0][i].y;
			vertex.TexCoords = vec;
		}
		else {
			vertex.TexCoords = glm::vec2(0.0f, 0.0f);
		}

		if (mesh->HasTangentsAndBitangents()) {
			vector.x = mesh->mTangents[i].x;
			vector.y = mesh->mTangents[i].y;
			vector.z = mesh->mTangents[i].z;
			vertex.Tangents = vector;

			vector.x = mesh->mBitangents[i].x;
			vector.y = mesh->mBitangents[i].y;
			vector.z = mesh->mBitangents[i].z;
			vertex.Bitangents = vector;
		}

		vertices.push_back(vertex);
	}

	// Now walk through each of the mesh's faces (a face is a mesh its triangle) and retrieve the corresponding vertex indices.
	for (unsigned int i = 0; i < mesh->mNumFaces; i++)
	{
		aiFace face = mesh->mFaces[i];
		// Retrieve all indices of the face and store them in the indices vector
		for (unsigned int j = 0; j < face.mNumIndices; j++)
			indices.push_back(face.mIndices[j]);
	}

	if (scene->HasAnimations()) {
		int boneArraysSize = mesh->mNumVertices*NUM_BONES_PER_VERTEX;
		std::vector<GLbyte> boneIDs;
		boneIDs.resize(boneArraysSize);
		std::vector<float> boneWeights;
		boneWeights.resize(boneArraysSize);

		unsigned int rightHandBoneNo;
		for (unsigned int i = 0; i < mesh->mNumBones; i++) {
			GLbyte BoneIndex = 0;
			string BoneName(mesh->mBones[i]->mName.data);
			if (BoneName == "mixamorig_RightHand")
				rightHandBoneNo = i;

			if (m_BoneMapping.find(BoneName) == m_BoneMapping.end()) {
				// Allocate an index for a new bone
				BoneIndex = m_NumBones;
				m_NumBones++;
				BoneInfo bi;
				m_BoneInfo.push_back(bi);
				m_BoneInfo[BoneIndex].BoneOffset = mesh->mBones[i]->mOffsetMatrix;
				m_BoneMapping[BoneName] = BoneIndex;
			}
			else {
				BoneIndex = m_BoneMapping[BoneName];
			}
			
			for (unsigned int j = 0; j < mesh->mBones[i]->mNumWeights; j++)
			{
				aiVertexWeight weight = mesh->mBones[i]->mWeights[j];
				// where to start reading vertex weights
				unsigned int vertexStart = weight.mVertexId*NUM_BONES_PER_VERTEX;
				// fill teh arrays

		
				for (unsigned int k = 0; k < NUM_BONES_PER_VERTEX; k++)
				{
					if (boneWeights.at(vertexStart + k) == 0.0f) //if 0 not filled with weight
					{
						boneWeights.at(vertexStart + k) = weight.mWeight;
						boneIDs.at(vertexStart + k) = BoneIndex; // i ID of current bone
						vertices.at(weight.mVertexId).id[k] = BoneIndex; // vertices = data?
						vertices.at(weight.mVertexId).weight[k] = weight.mWeight;
						break; //?
					}
				}
			}
		}
	}

	vector<Texture> textures;
	if (customTextures.diffuse != NULL) {
		vector<Texture> diffuseMaps;
		loadTextures(diffuseMaps, customTextures.diffuse, TextureNames::diffuse);
		textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());
	}
	if (customTextures.specular != NULL) {
		vector<Texture> specularMaps;
		loadTextures(specularMaps, customTextures.specular, TextureNames::specular);
		textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
	}
	if (customTextures.normal != NULL) {
		vector<Texture> normalMaps;
		loadTextures(normalMaps, customTextures.normal, TextureNames::normal);
		textures.insert(textures.end(), normalMaps.begin(), normalMaps.end());
	}

	if (mesh->mMaterialIndex >= 0)
	{
		aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];

		std::vector<Texture> diffuseMaps = loadMaterialTextures(material, aiTextureType_DIFFUSE, TextureNames::diffuse);
		textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());

		std::vector<Texture> specularMaps = loadMaterialTextures(material, aiTextureType_SPECULAR, TextureNames::specular);
		textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());

		std::vector<Texture> normalMaps = loadMaterialTextures(material, aiTextureType_HEIGHT, TextureNames::normal);
		textures.insert(textures.end(), normalMaps.begin(), normalMaps.end());
	}

	textures.resize(textures.size()); //so theres no extra space

	// Return a mesh object created from the extracted mesh data
	return Mesh(vertices, indices, textures);
}

// checks all material textures of a given type and loads the textures if they're not loaded yet.
void Model::loadTextures(vector<Texture> &textures, const char* str, const std::string& typeName) {

	// check if texture was loaded before and if so, continue to next iteration: skip loading a new texture
	bool skip = false;
	for (unsigned int j = 0; j < textures_loaded.size(); j++)
	{
		if (std::strcmp(textures_loaded[j].path.C_Str(), str) == 0)
		{
			textures.push_back(textures_loaded[j]);
			skip = true; // a texture with the same filepath has already been loaded, continue to next one. 
			break;
		}
	}

	if (!skip)
	{   // if texture hasn't been loaded already, load it
		Texture texture;
		texture.id = TextureLoader::loadTexture(str, this->path);
		texture.type = typeName;
		texture.path = str;
		textures.push_back(texture);
		textures_loaded.push_back(texture);  // store it as texture loaded for entire model, to ensure we won't unnecesery load duplicate textures.
	}
}
vector<Texture> Model::loadMaterialTextures(aiMaterial *mat, aiTextureType type, const std::string& typeName)
{
	vector<Texture> textures;
	for (unsigned int i = 0; i < mat->GetTextureCount(type); i++)
	{
		aiString str;
		mat->GetTexture(type, i, &str);
		loadTextures(textures, str.C_Str(), typeName);
	}
	return textures;
}