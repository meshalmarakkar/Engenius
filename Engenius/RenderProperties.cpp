#include "RenderProperties.h"

//#include <glm/gtc/matrix_transform.hpp>
//#define DEG_TO_RADIAN 0.017453293f

RenderProperties_Basic::RenderProperties_Basic(glm::mat4* modelMatrix) : modelMatrix(modelMatrix) {

}
glm::mat4* RenderProperties_Basic::getModelMatrix() {
	return modelMatrix;
}
void RenderProperties_Basic::setModelMatrix(glm::mat4* newModelMat) {
	modelMatrix = newModelMat;
}

RenderProperties_Uniforms::RenderProperties_Uniforms(glm::mat4* modelMatrix) : RenderProperties_Basic(modelMatrix) {

}
Uniforms* RenderProperties_Uniforms::getUniforms() {
	return &uniforms;
}


RenderProperties::RenderProperties(Material* material, glm::mat4* modelMatrix) : material(material), RenderProperties_Uniforms(modelMatrix) {

}
Material* RenderProperties::getMaterial() {
	return material;
}


//void RenderProperties::setModelMatrix(const glm::mat4& newMatrix) {
//	modelMatrix = newMatrix;
//}
//void RenderProperties::createModelMatrix(const glm::vec3& position) {
//	modelMatrix = glm::translate(modelMatrix, position);
//}
//void RenderProperties::createModelMatrix(const glm::vec3& position, const float& yaw) {
//	modelMatrix = glm::translate(modelMatrix, position);
//	modelMatrix = glm::rotate(modelMatrix, float(yaw * DEG_TO_RADIAN), glm::vec3(0.0f, 1.0f, 0.0f));
//}
//void RenderProperties::createModelMatrix(const glm::vec3& position, const glm::vec3& scale) {
//	modelMatrix = glm::translate(modelMatrix, position);
//	modelMatrix = glm::scale(modelMatrix, scale);
//}
//void RenderProperties::createModelMatrix(const glm::vec3& position, const float& yaw, const glm::vec3& scale) {
//	modelMatrix = glm::translate(modelMatrix, position);
//	modelMatrix = glm::rotate(modelMatrix, float(yaw * DEG_TO_RADIAN), glm::vec3(0.0f, 1.0f, 0.0f));
//	modelMatrix = glm::scale(modelMatrix, scale);
//}
//void RenderProperties::createModelMatrix(const glm::vec3& position, const float& pitch, const float& yaw, const glm::vec3& scale) {
//	modelMatrix = glm::translate(modelMatrix, position);
//	modelMatrix = glm::rotate(modelMatrix, float(yaw * DEG_TO_RADIAN), glm::vec3(0.0f, 1.0f, 0.0f));
//	modelMatrix = glm::rotate(modelMatrix, float(pitch * DEG_TO_RADIAN), glm::vec3(1.0f, 0.0f, 0.0f));
//	//modelMatrix = glm::rotate(modelMatrix, float(objectsInWorld.at(iter)->getRoll() * DEG_TO_RADIAN), glm::vec3(0.0f, 0.0f, 1.0f));
//	modelMatrix = glm::scale(modelMatrix, scale);
//}
//void RenderProperties::createModelMatrix(const glm::vec3& position, const glm::vec3& scale, const glm::vec3& rotation) {
//	modelMatrix = glm::translate(modelMatrix, position);
//	modelMatrix = glm::rotate(modelMatrix, float(rotation.y * DEG_TO_RADIAN), glm::vec3(0.0f, 1.0f, 0.0f));
//	modelMatrix = glm::rotate(modelMatrix, float(rotation.x * DEG_TO_RADIAN), glm::vec3(1.0f, 0.0f, 0.0f));
//	modelMatrix = glm::rotate(modelMatrix, float(rotation.z * DEG_TO_RADIAN), glm::vec3(0.0f, 0.0f, 1.0f));
//	modelMatrix = glm::scale(modelMatrix, scale);
//}