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

void Mesh::prepareInstancedDraw(const std::vector<glm::mat4>* modelMatrices, const std::vector<glm::ivec3>* pointIDs, const std::vector<glm::ivec3>* spotIDs, const std::vector<float>* tiling) {

	VAO->bind();

	VAO->addData(ID_vboModel, Buffer_Options::gl_array_buffer, modelMatrices->size() * sizeof(glm::mat4), &modelMatrices->at(0), Buffer_Options::gl_static_draw);

	GLsizei sizeOf_Vec4 = sizeof(glm::vec4);
	GLsizei sizeOf_4_Vec4 = sizeOf_Vec4 * 4;

	VAO->enableVertexAttribArray(LOCATION_MODEL1, 4, Buffer_Options::gl_float, Buffer_Options::gl_false, sizeOf_4_Vec4, 0);
	VAO->enableVertexAttribArray(LOCATION_MODEL2, 4, Buffer_Options::gl_float, Buffer_Options::gl_false, sizeOf_4_Vec4, sizeOf_Vec4);
	VAO->enableVertexAttribArray(LOCATION_MODEL3, 4, Buffer_Options::gl_float, Buffer_Options::gl_false, sizeOf_4_Vec4, (sizeOf_Vec4 * 2));
	VAO->enableVertexAttribArray(LOCATION_MODEL4, 4, Buffer_Options::gl_float, Buffer_Options::gl_false, sizeOf_4_Vec4, (sizeOf_Vec4 * 3));

	VAO->setDivisor_multi(0, 6, 0);
	VAO->setDivisor_multi(LOCATION_MODEL1, LOCATION_MODEL4, 1);

	VAO->addData(ID_vboPoint, Buffer_Options::gl_array_buffer, pointIDs->size() * sizeof(glm::ivec3), &pointIDs->at(0), Buffer_Options::gl_static_draw);
	VAO->addData(ID_vboSpot, Buffer_Options::gl_array_buffer, spotIDs->size() * sizeof(glm::ivec3), &spotIDs->at(0), Buffer_Options::gl_static_draw);
	VAO->addData(ID_vboTiling, Buffer_Options::gl_array_buffer, tiling->size() * sizeof(float), &tiling->at(0), Buffer_Options::gl_static_draw);

	VAO->enableVertexAttribArray_I(LOCATION_POINTID, 3, Buffer_Options::gl_int, 0, 0);
	VAO->enableVertexAttribArray_I(LOCATION_SPOTID, 3, Buffer_Options::gl_int, 0, 0);
	VAO->enableVertexAttribArray(LOCATION_TILING, 1, Buffer_Options::gl_float, Buffer_Options::gl_false, 0, 0);

	VAO->setDivisor_multi(LOCATION_POINTID, LOCATION_TILING, 1);

}

void Mesh::prepareInstancedDraw(const std::vector<glm::mat4>* modelMatrices) {

	VAO->bind();

	VAO->addData(ID_vboModel, Buffer_Options::gl_array_buffer, modelMatrices->size() * sizeof(glm::mat4), &modelMatrices->at(0), Buffer_Options::gl_static_draw);

	GLsizei sizeOf_Vec4 = sizeof(glm::vec4);
	GLsizei sizeOf_4_Vec4 = sizeOf_Vec4 * 4;

	VAO->enableVertexAttribArray(LOCATION_MODEL1, 4, Buffer_Options::gl_float, Buffer_Options::gl_false, sizeOf_4_Vec4, 0);
	VAO->enableVertexAttribArray(LOCATION_MODEL2, 4, Buffer_Options::gl_float, Buffer_Options::gl_false, sizeOf_4_Vec4, sizeOf_Vec4);
	VAO->enableVertexAttribArray(LOCATION_MODEL3, 4, Buffer_Options::gl_float, Buffer_Options::gl_false, sizeOf_4_Vec4, (sizeOf_Vec4 * 2));
	VAO->enableVertexAttribArray(LOCATION_MODEL4, 4, Buffer_Options::gl_float, Buffer_Options::gl_false, sizeOf_4_Vec4, (sizeOf_Vec4 * 3));

	VAO->setDivisor_multi(0, 6, 0);
	VAO->setDivisor_multi(LOCATION_MODEL1, LOCATION_MODEL4, 1);
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