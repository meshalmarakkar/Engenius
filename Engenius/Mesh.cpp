#include "Mesh.h"

Mesh::Mesh(const vector<Vertex> vertices, const vector<GLuint> indices, const vector<Texture> textures) : vertices(vertices), indices(indices), textures(textures)
{
	this->setupMesh();
}

void Mesh::bindWall(GLuint shader) {
	// bind appropriate textures
	unsigned int diffuseNr = 1;
	unsigned int specularNr = 1;
	unsigned int normalNr = 1;
	unsigned int heightNr = 1;

	for (GLuint i = 0; i < this->textures.size(); i++)
	{
		glActiveTexture(GL_TEXTURE0 + i);

		std::string name = this->textures[i].type;
		std::string number;
		if (name == "texture_diffuse")
			number = std::to_string(diffuseNr++);
		else if (name == "texture_specular")
			number = std::to_string(specularNr++);
		else if (name == "texture_normal")
			number = std::to_string(normalNr++);
		else if (name == "texture_height")
			number = std::to_string(heightNr++);

		glUniform1i(glGetUniformLocation(shader, (name + number).c_str()), i);
		glBindTexture(GL_TEXTURE_2D, this->textures[i].id);
	}

	//// Draw mesh
	glBindVertexArray(this->VAO);
	Common::enableVertexAttribArray(0, 6);

	glUniform1i(glGetUniformLocation(shader, "instanced"), false);

}

void Mesh::unbindWall() {
	Common::unbindTextures(0, textures.size() - 1);
	Common::disableVertexAttribArray(0, 6);
	glBindVertexArray(0);
}

void Mesh::drawWall(GLuint shader, const glm::mat4 modelMatrix) {
	glUniformMatrix4fv(glGetUniformLocation(shader, "uniform_model"), 1, GL_FALSE, glm::value_ptr(modelMatrix));
	glDrawElements(GL_TRIANGLES, this->indices.size(), GL_UNSIGNED_INT, 0);
}

void Mesh::Draw(GLuint shader, const glm::mat4 modelMatrices)
{
	// bind appropriate textures
	unsigned int diffuseNr = 1;
	unsigned int specularNr = 1;
	unsigned int normalNr = 1;
	unsigned int heightNr = 1;

	for (GLuint i = 0; i < this->textures.size(); i++)
	{
		glActiveTexture(GL_TEXTURE0 + i);

		std::string name = this->textures[i].type;
		std::string number;
		if (name == "texture_diffuse")
			number = std::to_string(diffuseNr++);
		else if (name == "texture_specular")
			number = std::to_string(specularNr++);
		else if (name == "texture_normal")
			number = std::to_string(normalNr++);
		else if (name == "texture_height")
			number = std::to_string(heightNr++);

		glUniform1i(glGetUniformLocation(shader, (name + number).c_str()), i);
		glBindTexture(GL_TEXTURE_2D, this->textures[i].id);
	}

	//// Draw mesh
	glBindVertexArray(this->VAO);
	Common::enableVertexAttribArray(0, 6);

	glUniform1i(glGetUniformLocation(shader, "instanced"), false);

	glUniformMatrix4fv(glGetUniformLocation(shader, "uniform_model"), 1, GL_FALSE, glm::value_ptr(modelMatrices));
	glDrawElements(GL_TRIANGLES, this->indices.size(), GL_UNSIGNED_INT, 0);
	
	Common::unbindTextures(0, textures.size() - 1);
	Common::disableVertexAttribArray(0, 6);
	glBindVertexArray(0);
}

void Mesh::InstancedDraw(GLuint shader, const std::vector<glm::mat4> modelMatrices, const std::vector<glm::vec2> pointIDs, const std::vector<glm::vec2> spotIDs)
{	
	// bind appropriate textures
	unsigned int diffuseNr = 1;
	unsigned int specularNr = 1;
	unsigned int normalNr = 1;
	unsigned int heightNr = 1;

	for (GLuint i = 0; i < this->textures.size(); i++)
	{
		glActiveTexture(GL_TEXTURE0 + i);

		std::string name = this->textures[i].type;
		std::string number;
		if (name == "texture_diffuse")
			number = std::to_string(diffuseNr++);
		else if (name == "texture_specular")
			number = std::to_string(specularNr++);
		else if (name == "texture_normal")
			number = std::to_string(normalNr++);
		else if (name == "texture_height")
			number = std::to_string(heightNr++);

		glUniform1i(glGetUniformLocation(shader, (name + number).c_str()), i);
		glBindTexture(GL_TEXTURE_2D, this->textures[i].id);
	}

	//// Draw mesh
	glBindVertexArray(this->VAO);
	Common::enableVertexAttribArray(0, 6);

	glUniform1i(glGetUniformLocation(shader, "instanced"), true);

	glBindBuffer(GL_ARRAY_BUFFER, this->vboModel);
	glBufferData(GL_ARRAY_BUFFER, modelMatrices.size() * sizeof(glm::mat4), &modelMatrices.at(0), GL_STATIC_DRAW);

	Common::enableVertexAttribArray(7, 10);
	GLsizei sizeOf_Vec4 = sizeof(glm::vec4);
	GLsizei sizeOf_4_Vec4 = sizeOf_Vec4 * 4;
	glVertexAttribPointer(7, 4, GL_FLOAT, GL_FALSE, sizeOf_4_Vec4, (GLvoid*)0);
	glVertexAttribPointer(8, 4, GL_FLOAT, GL_FALSE, sizeOf_4_Vec4, (GLvoid*)sizeOf_Vec4);
	glVertexAttribPointer(9, 4, GL_FLOAT, GL_FALSE, sizeOf_4_Vec4, (GLvoid*)(sizeOf_Vec4 * 2));
	glVertexAttribPointer(10, 4, GL_FLOAT, GL_FALSE, sizeOf_4_Vec4, (GLvoid*)(sizeOf_Vec4 * 3));

	glVertexAttribDivisor(0, 0);
	glVertexAttribDivisor(1, 0);
	glVertexAttribDivisor(2, 0);
	glVertexAttribDivisor(3, 0);
	glVertexAttribDivisor(4, 0);
	glVertexAttribDivisor(5, 0);
	glVertexAttribDivisor(6, 0);
	glVertexAttribDivisor(7, 1);
	glVertexAttribDivisor(8, 1);
	glVertexAttribDivisor(9, 1);
	glVertexAttribDivisor(10, 1);

	Common::enableVertexAttribArray(11, 12);
	glBindBuffer(GL_ARRAY_BUFFER, this->vboPointIDs);
	glBufferData(GL_ARRAY_BUFFER, 2 * sizeof(int), &pointIDs, GL_STATIC_DRAW);
	glVertexAttribPointer(11, 2, GL_FLOAT, GL_FALSE, 0, (GLvoid*)0);
	glBindBuffer(GL_ARRAY_BUFFER, this->vboSpotIDs);
	glBufferData(GL_ARRAY_BUFFER, 2 * sizeof(int), &spotIDs, GL_STATIC_DRAW);
	glVertexAttribPointer(12, 2, GL_FLOAT, GL_FALSE, 0, (GLvoid*)0);

	glVertexAttribDivisor(11, 1);
	glVertexAttribDivisor(12, 1);

	unsigned int num = modelMatrices.size();
	glDrawElementsInstanced(GL_TRIANGLES, this->indices.size(), GL_UNSIGNED_INT, 0, num);

	Common::disableVertexAttribArray(7, 10);
	Common::disableVertexAttribArray(11);
   
	Common::unbindTextures(0, textures.size()-1);
	Common::disableVertexAttribArray(0, 6);
	glBindVertexArray(0);
}

void Mesh::setupMesh()
{
	// Create buffers/arrays
	glGenVertexArrays(1, &this->VAO);
	glGenBuffers(1, &this->VBO);
	glGenBuffers(1, &this->EBO);
	glGenBuffers(1, &this->vboModel);

	// Load data into vertex buffers
	glBindVertexArray(this->VAO);

	// A great thing about structs is that their memory layout is sequential for all its items.
	// The effect is that we can simply pass a pointer to the struct and it translates perfectly to a glm::vec3/2 array which
	// again translates to 3/2 floats which translates to a byte array.
	glBindBuffer(GL_ARRAY_BUFFER, this->VBO);
	glBufferData(GL_ARRAY_BUFFER, this->vertices.size() * sizeof(Vertex), &this->vertices.at(0), GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, this->indices.size() * sizeof(GLuint), &this->indices[0], GL_STATIC_DRAW);

	// Set the vertex attribute pointers
	// Positions
	glEnableVertexAttribArray(LOCATION_VERTEX);
	glVertexAttribPointer(LOCATION_VERTEX, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)0);
	// Normals
	glEnableVertexAttribArray(LOCATION_NORMAL);
	glVertexAttribPointer(LOCATION_NORMAL, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, Normal));
	// Texture Coords
	glEnableVertexAttribArray(LOCATION_TEXCOORD);
	glVertexAttribPointer(LOCATION_TEXCOORD, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, TexCoords));
	// Tangents
	glEnableVertexAttribArray(LOCATION_TANGENT);
	glVertexAttribPointer(LOCATION_TANGENT, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, Tangents));
	// Tangents
	glEnableVertexAttribArray(LOCATION_BITANGENT);
	glVertexAttribPointer(LOCATION_BITANGENT, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, Bitangents));
	// Bone IDs
	glEnableVertexAttribArray(LOCATION_BONE_ID);
	glVertexAttribIPointer(LOCATION_BONE_ID, 4, GL_BYTE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, id));
	// Weights
	glEnableVertexAttribArray(LOCATION_BONE_WEIGHT);
	glVertexAttribPointer(LOCATION_BONE_WEIGHT, 4, GL_FLOAT, GL_TRUE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, weight));
	
	glBindVertexArray(0);
}