#include "Mesh.h"
#include <GL/glew.h> // Contains all the necessery OpenGL includes

Mesh::Mesh(const vector<Vertex>& vertices, const vector<unsigned int>& indices, const vector<Texture>& textures) : vertices(vertices), indices(indices), textures(textures)
{
	this->setupMesh();
	material = new Material();
	for (unsigned int i = 0; i < textures.size(); i++)
		material->addTexture_2D(TextureType(textures[i].type, textures[i].id));
}

VertexArrayObject* Mesh::getVAO() {
	return VAO;
}

Material* Mesh::getMaterial() {
	return material;
}

void Mesh::prepareInstancedDraw(const std::vector<glm::mat4>& modelMatrices, const std::vector<glm::vec3>& pointIDs, const std::vector<glm::vec3>& spotIDs, const std::vector<float>& tiling) {

	VAO->bind();

	VAO->addData(ID_vboModel, Buffer_Options::gl_array_buffer, modelMatrices.size() * sizeof(glm::mat4), &modelMatrices.at(0), Buffer_Options::gl_static_draw);

	GLsizei sizeOf_Vec4 = sizeof(glm::vec4);
	GLsizei sizeOf_4_Vec4 = sizeOf_Vec4 * 4;

	VAO->enableVertexAttribArray(LOCATION_MODEL1, 4, Buffer_Options::gl_float, Buffer_Options::gl_false, sizeOf_4_Vec4, 0);
	VAO->enableVertexAttribArray(LOCATION_MODEL2, 4, Buffer_Options::gl_float, Buffer_Options::gl_false, sizeOf_4_Vec4, sizeOf_Vec4);
	VAO->enableVertexAttribArray(LOCATION_MODEL3, 4, Buffer_Options::gl_float, Buffer_Options::gl_false, sizeOf_4_Vec4, (sizeOf_Vec4 * 2));
	VAO->enableVertexAttribArray(LOCATION_MODEL4, 4, Buffer_Options::gl_float, Buffer_Options::gl_false, sizeOf_4_Vec4, (sizeOf_Vec4 * 3));

	VAO->setDivisor_multi(0, 6, 0);
	VAO->setDivisor_multi(LOCATION_MODEL1, LOCATION_MODEL4, 1);

	VAO->addData(ID_vboPoint, Buffer_Options::gl_array_buffer, pointIDs.size() * sizeof(glm::vec3), &pointIDs.at(0), Buffer_Options::gl_static_draw);
	VAO->addData(ID_vboSpot, Buffer_Options::gl_array_buffer, spotIDs.size() * sizeof(glm::vec3), &spotIDs.at(0), Buffer_Options::gl_static_draw);
	VAO->addData(ID_vboTiling, Buffer_Options::gl_array_buffer, tiling.size() * sizeof(float), &tiling.at(0), Buffer_Options::gl_static_draw);

	VAO->enableVertexAttribArray(LOCATION_POINTID, 3, Buffer_Options::gl_float, Buffer_Options::gl_false, 0, 0);
	VAO->enableVertexAttribArray(LOCATION_SPOTID, 3, Buffer_Options::gl_float, Buffer_Options::gl_false, 0, 0);
	VAO->enableVertexAttribArray(LOCATION_TILING, 1, Buffer_Options::gl_float, Buffer_Options::gl_false, 0, 0);

	VAO->setDivisor_multi(LOCATION_POINTID, LOCATION_TILING, 1);

}

void Mesh::InstancedDraw(const unsigned int& shader, const std::vector<glm::mat4>& modelMatrices, const std::vector<glm::vec2>& pointIDs, const std::vector<glm::vec2>& spotIDs)
{	
	// bind appropriate textures
	unsigned int diffuseNr = 1;
	unsigned int specularNr = 1;
	unsigned int normalNr = 1;
	unsigned int heightNr = 1;

	for (unsigned int i = 0; i < this->textures.size(); i++)
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
	VAO->bind();
	//glBindVertexArray(this->VAO);
	//Common::enableVertexAttribArray(0, 6);

	glUniform1i(glGetUniformLocation(shader, "instanced"), true);

	VAO->addData(ID_vboModel, Buffer_Options::gl_array_buffer, modelMatrices.size() * sizeof(glm::mat4), &modelMatrices.at(0), Buffer_Options::gl_static_draw);
	/*glBindBuffer(GL_ARRAY_BUFFER, this->vboModel);
	glBufferData(GL_ARRAY_BUFFER, modelMatrices.size() * sizeof(glm::mat4), &modelMatrices.at(0), GL_STATIC_DRAW);*/

	//Common::enableVertexAttribArray(7, 10);
	GLsizei sizeOf_Vec4 = sizeof(glm::vec4);
	GLsizei sizeOf_4_Vec4 = sizeOf_Vec4 * 4;

	VAO->enableVertexAttribArray(LOCATION_MODEL1, 4, Buffer_Options::gl_float, Buffer_Options::gl_false, sizeOf_4_Vec4, 0);
	VAO->enableVertexAttribArray(LOCATION_MODEL2, 4, Buffer_Options::gl_float, Buffer_Options::gl_false, sizeOf_4_Vec4, sizeOf_Vec4);
	VAO->enableVertexAttribArray(LOCATION_MODEL3, 4, Buffer_Options::gl_float, Buffer_Options::gl_false, sizeOf_4_Vec4, (sizeOf_Vec4 * 2));
	VAO->enableVertexAttribArray(LOCATION_MODEL4, 4, Buffer_Options::gl_float, Buffer_Options::gl_false, sizeOf_4_Vec4, (sizeOf_Vec4 * 3));

	/*glVertexAttribPointer(7, 4, GL_FLOAT, GL_FALSE, sizeOf_4_Vec4, (GLvoid*)0);
	glVertexAttribPointer(8, 4, GL_FLOAT, GL_FALSE, sizeOf_4_Vec4, (GLvoid*)sizeOf_Vec4);
	glVertexAttribPointer(9, 4, GL_FLOAT, GL_FALSE, sizeOf_4_Vec4, (GLvoid*)(sizeOf_Vec4 * 2));
	glVertexAttribPointer(10, 4, GL_FLOAT, GL_FALSE, sizeOf_4_Vec4, (GLvoid*)(sizeOf_Vec4 * 3));*/

	VAO->setDivisor_multi(0, 6, 0);
	VAO->setDivisor_multi(7, 10, 1);

	/*glVertexAttribDivisor(0, 0);
	glVertexAttribDivisor(1, 0);
	glVertexAttribDivisor(2, 0);
	glVertexAttribDivisor(3, 0);
	glVertexAttribDivisor(4, 0);
	glVertexAttribDivisor(5, 0);
	glVertexAttribDivisor(6, 0);
	glVertexAttribDivisor(7, 1);
	glVertexAttribDivisor(8, 1);
	glVertexAttribDivisor(9, 1);
	glVertexAttribDivisor(10, 1);*/

	VAO->addData(ID_vboPoint, GL_ARRAY_BUFFER, 3 * sizeof(int), &pointIDs, Buffer_Options::gl_static_draw);
	VAO->addData(ID_vboSpot, GL_ARRAY_BUFFER, 3 * sizeof(int), &spotIDs, Buffer_Options::gl_static_draw);
	
	VAO->enableVertexAttribArray_I(LOCATION_POINTID, 3, Buffer_Options::gl_int, 0, 0);
	VAO->enableVertexAttribArray_I(LOCATION_SPOTID, 3, Buffer_Options::gl_int, 0, 0);

	/*Common::enableVertexAttribArray(11, 12);
	glBindBuffer(GL_ARRAY_BUFFER, this->vboPointIDs);
	glBufferData(GL_ARRAY_BUFFER, 2 * sizeof(int), &pointIDs, GL_STATIC_DRAW);
	glVertexAttribPointer(11, 2, GL_FLOAT, GL_FALSE, 0, (GLvoid*)0);
	glBindBuffer(GL_ARRAY_BUFFER, this->vboSpotIDs);
	glBufferData(GL_ARRAY_BUFFER, 2 * sizeof(int), &spotIDs, GL_STATIC_DRAW);
	glVertexAttribPointer(12, 2, GL_FLOAT, GL_FALSE, 0, (GLvoid*)0);*/

	VAO->setDivisor_multi(11, 12, 1);

	/*glVertexAttribDivisor(11, 1);
	glVertexAttribDivisor(12, 1);*/

	unsigned int num = modelMatrices.size();
	glDrawElementsInstanced(GL_TRIANGLES, VAO->getIndicesCount(), GL_UNSIGNED_INT, 0, num);

	//Common::disableVertexAttribArray(7, 10);
	//Common::disableVertexAttribArray(11);
  
	//TODO : this
//	textureManager->unbindTextures(0, textures.size()-1);
	//Common::disableVertexAttribArray(0, 6);
//	glBindVertexArray(0);
}

void Mesh::setupMesh()
{
	VAO = new VertexArrayObject(indices.size());
	VAO->genBuffer_andAddData(Buffer_Options::gl_array_buffer, this->vertices.size() * sizeof(Vertex), &this->vertices.at(0), Buffer_Options::gl_static_draw);
	VAO->genBuffer_andAddData(Buffer_Options::gl_element_array_buffer, this->indices.size() * sizeof(unsigned int), &this->indices[0], Buffer_Options::gl_static_draw);
	ID_vboModel = VAO->genBuffer();
	ID_vboPoint = VAO->genBuffer();
	ID_vboSpot = VAO->genBuffer();
	ID_vboTiling = VAO->genBuffer();

	VAO->enableVertexAttribArray(LOCATION_VERTEX, 3, Buffer_Options::gl_float, Buffer_Options::gl_false, sizeof(Vertex), 0);
	VAO->enableVertexAttribArray(LOCATION_NORMAL, 3, Buffer_Options::gl_float, Buffer_Options::gl_false, sizeof(Vertex), offsetof(Vertex, Normal));
	VAO->enableVertexAttribArray(LOCATION_TEXCOORD, 2, Buffer_Options::gl_float, Buffer_Options::gl_false, sizeof(Vertex), offsetof(Vertex, TexCoords));
	VAO->enableVertexAttribArray(LOCATION_TANGENT, 3, Buffer_Options::gl_float, Buffer_Options::gl_false, sizeof(Vertex), offsetof(Vertex, Tangents));
	VAO->enableVertexAttribArray(LOCATION_BITANGENT, 3, Buffer_Options::gl_float, Buffer_Options::gl_false, sizeof(Vertex), offsetof(Vertex, Bitangents));
	VAO->enableVertexAttribArray_I(LOCATION_BONE_ID, 4, Buffer_Options::gl_byte, sizeof(Vertex), offsetof(Vertex, id));
	VAO->enableVertexAttribArray(LOCATION_BONE_WEIGHT, 4, Buffer_Options::gl_float, Buffer_Options::gl_true, sizeof(Vertex), offsetof(Vertex, weight));
}