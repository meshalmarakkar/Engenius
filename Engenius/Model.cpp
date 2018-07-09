#include "Model.h"
#include "TextureLoader.h"

//just for animation loading
Model::Model(const std::string path, const bool justAnimation) : path(path) {
	this->loadModel(justAnimation);
}

//auto texture loading
Model::Model(const std::string name, const std::string path) : name(name), path(path) {
	customTextures.diffuse = NULL;
	customTextures.specular = NULL;
	customTextures.normal = NULL;
	customTextures.height = NULL;
	this->loadModel();
}

Model::Model(const std::string name, const string path, const char* diffuse, const char* specular) : name(name), path(path) {
	customTextures.diffuse = diffuse;
	customTextures.specular = specular;
	customTextures.normal = NULL;
	customTextures.height = NULL;
	this->loadModel();
}
Model::Model(const std::string name, const string path, const char* diffuse, const char* specular, const char* normal) : name(name), path(path) {
	customTextures.diffuse = diffuse;
	customTextures.specular = specular;
	customTextures.normal = normal;
	customTextures.height = NULL;
	this->loadModel();
}
Model::Model(const std::string name, const string path, const char* diffuse, const char* specular, const char* normal, const char* height, const float heightScale) : name(name), path(path) {
	customTextures.diffuse = diffuse;
	customTextures.specular = specular;
	customTextures.normal = normal;
	customTextures.height = height;
	this->parallaxed = true;
	this->heightScale = heightScale;
	this->loadModel();
}

void Model::setIfDisableCullFace(bool newVal) {
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

bool Model::getIfMapped() {
	return mapped;
}

bool Model::getToInstance() {
	return toInstance;
}

void Model::setParallaxed(const bool newVal) {
	parallaxed = newVal;
 }

void Model::setToInstance(const bool newValue) {
	toInstance = newValue;
}

void Model::bindWall(unsigned int shader) {
	glUniform1f(glGetUniformLocation(shader, "hasSpecularMap"), hasSpecularMap);
	glUniform1i(glGetUniformLocation(shader, "instanced"), false);
	this->meshes.at(0).sendTex(shader);
	//this->meshes.at(0).bindWall(shader);
}
void Model::unbindWall() {
	this->meshes.at(0).unbindWall();
}
void Model::drawWall(unsigned int shader, const glm::mat4 modelMatrix, Renderer* renderer) {
	//glUniform1f(glGetUniformLocation(shader, "hasSpecularMap"), hasSpecularMap);
	//glUniform1i(glGetUniformLocation(shader, "instanced"), false);
	glUniformMatrix4fv(glGetUniformLocation(shader, "uniform_model"), 1, GL_FALSE, glm::value_ptr(modelMatrix));

	renderer->drawElements(this->meshes.at(0).getVAO());
	//this->meshes.at(0).drawWall(shader, modelMatrix);
}

void Model::InstancedDraw(unsigned int shader, const std::vector<glm::mat4> modelMatrices, const std::vector<glm::vec2> pointIDs, const std::vector<glm::vec2> spotIDs) {
	glUniform1f(glGetUniformLocation(shader, "hasSpecularMap"), hasSpecularMap);
	glUniform1i(glGetUniformLocation(shader, "instanced"), true);
	for (unsigned int i = 0; i < this->meshes.size(); i++)
		this->meshes.at(i).InstancedDraw(shader, modelMatrices, pointIDs, spotIDs);
}
void Model::Draw(unsigned int shader, const glm::mat4 modelMatrix, Renderer* renderer) {
	glUniform1f(glGetUniformLocation(shader, "hasSpecularMap"), hasSpecularMap);
	glUniform1i(glGetUniformLocation(shader, "instanced"), false);
	glUniformMatrix4fv(glGetUniformLocation(shader, "uniform_model"), 1, GL_FALSE, glm::value_ptr(modelMatrix));
	for (unsigned int i = 0; i < this->meshes.size(); i++) {
		Mesh* m = &(this->meshes.at(i));
		m->sendTex(shader);
		renderer->drawElements(m->getVAO());
	}
}
void Model::Draw(unsigned int shader, const glm::mat4 modelMatrix) {
	glUniform1f(glGetUniformLocation(shader, "hasSpecularMap"), hasSpecularMap);
	glUniform1i(glGetUniformLocation(shader, "instanced"), false);
	//glUniformMatrix4fv(glGetUniformLocation(shader, "uniform_model"), 1, GL_FALSE, glm::value_ptr(modelMatrix));
	for (unsigned int i = 0; i < this->meshes.size(); i++) {
		this->meshes.at(i).Draw(shader, modelMatrix);
	}
}
void Model::loadModel(bool onlyAnimation)
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
	// Retrieve the directory path of the filepath
	m_NumBones = 0;

	if (!onlyAnimation) {
		setParallaxed(false);
		toInstance = false;

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
		loadTextures(diffuseMaps, customTextures.diffuse, "texture_diffuse");
		textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());
	}
	if (customTextures.specular != NULL) {
		vector<Texture> specularMaps;
		loadTextures(specularMaps, customTextures.specular, "texture_specular");
		textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
		hasSpecularMap = true;
	}
	else {
		hasSpecularMap = false;
	}
	if (customTextures.normal != NULL) {
		vector<Texture> normalMaps;
		loadTextures(normalMaps, customTextures.normal, "texture_normal");
		textures.insert(textures.end(), normalMaps.begin(), normalMaps.end());
		mapped = true;
	}
	else {
		mapped = false;
	}
	if (customTextures.height != NULL) {
		vector<Texture> heightMaps;
		loadTextures(heightMaps, customTextures.height, "texture_height");
		textures.insert(textures.end(), heightMaps.begin(), heightMaps.end());
	}

	if (mesh->mMaterialIndex >= 0)
	{
		aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];

		std::vector<Texture> diffuseMaps = loadMaterialTextures(material, aiTextureType_DIFFUSE, "texture_diffuse");
		textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());

		std::vector<Texture> specularMaps = loadMaterialTextures(material, aiTextureType_SPECULAR, "texture_specular");
		textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());

		std::vector<Texture> normalMaps = loadMaterialTextures(material, aiTextureType_HEIGHT, "texture_normal");
		textures.insert(textures.end(), normalMaps.begin(), normalMaps.end());

		std::vector<Texture> heightMaps = loadMaterialTextures(material, aiTextureType_AMBIENT, "texture_height");
		textures.insert(textures.end(), heightMaps.begin(), heightMaps.end());
	}

	textures.resize(textures.size()); //so theres no extra space

	// Return a mesh object created from the extracted mesh data
	return Mesh(vertices, indices, textures);
}

// checks all material textures of a given type and loads the textures if they're not loaded yet.
void Model::loadTextures(vector<Texture> &textures, const char* str, string typeName) {

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
		if (typeName == "texture_specular")
			hasSpecularMap = true;
		if (typeName == "texture_normal")
			mapped = true;
		texture.path = str;
		textures.push_back(texture);
		textures_loaded.push_back(texture);  // store it as texture loaded for entire model, to ensure we won't unnecesery load duplicate textures.
	}
}
vector<Texture> Model::loadMaterialTextures(aiMaterial *mat, aiTextureType type, string typeName)
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