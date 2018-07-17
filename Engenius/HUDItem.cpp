#include "HUDItem.h"

HUDItem::HUDItem(const int& screenWidth, const int& screenHeight, const int& x, const int& y, const int& sizeX, const int& sizeY, const unsigned int& texture, const bool& allowLowTransparency, const float& transparency) {
	init(screenWidth, screenHeight, x, y, sizeX, sizeY, texture, allowLowTransparency, transparency);
}

void HUDItem::init(const int& screenWidth, const int& screenHeight, const int& x, const int& y, const int& sizeX, const int& sizeY, const unsigned int& texture, const bool& allowLowTransparency, const float& transparency) {
	this->xy.x = static_cast<float>(x);
	this->xy.y = static_cast<float>(y);
	this->size.x = static_cast<float>(sizeX);
	this->size.y = static_cast<float>(sizeY);
	this->verts.resize(6);
	clicked = false;

	glm::vec2 vertex_up_left = glm::vec2(x, y + sizeY);
	glm::vec2 vertex_up_right = glm::vec2(x + sizeX, y + sizeY);
	glm::vec2 vertex_down_right = glm::vec2(x + sizeX, y);
	glm::vec2 vertex_down_left = glm::vec2(x, y);

	verts.at(0) = vertex_up_left;
	verts.at(1) = vertex_down_left;
	verts.at(2) = vertex_up_right;

	verts.at(3) = vertex_down_right;
	verts.at(4) = vertex_up_right;
	verts.at(5) = vertex_down_left;

	convert_ClipSpace_to_HomogeneousSpace(screenWidth, screenHeight);
	
	this->texture = texture;
	this->transparency = transparency;
	this->allowLowTransparency = allowLowTransparency;
}

void HUDItem::convert_ClipSpace_to_HomogeneousSpace(const int& screenWidth, const int& screenHeight) {
	for (unsigned int i = 0; i < verts.size(); i++) {
		// Output position of the vertex, in clip space
		// map [0..screenWidth][0..screenHeight] to [-1..1][-1..1]
		float centre_width = screenWidth * 0.5f;
		float centre_height = screenHeight*0.5f;
		glm::vec2 vertexPosition_homoneneousspace = verts[i] - glm::vec2(centre_width, centre_height); // [0..screenWidth][0..screenHeight] -> [-screenWidth/2..screenWidth/2][-screenHeight/2..screenHeight/2]
		vertexPosition_homoneneousspace /= glm::vec2(centre_width, centre_height);
		verts[i] = vertexPosition_homoneneousspace;
	}
}



void HUDItem::setTexture(unsigned int newTex) { texture = newTex; }
void HUDItem::setIfClicked(bool newVal) { clicked = newVal; }
void HUDItem::setTransparency(float newVal) { this->transparency = newVal; }
void HUDItem::setAllowLowTransparency(bool newVal) { allowLowTransparency = newVal; }


bool HUDItem::getAllowLowTransparency() { return allowLowTransparency; }
float HUDItem::getTransparency() { return transparency; }
glm::vec2 HUDItem::getXY() { return xy; }
glm::vec2 HUDItem::getSize() { return size; }
bool HUDItem::getIfClicked() { return clicked; }
const std::vector<glm::vec2> HUDItem::getVerts() { return verts; }
unsigned int HUDItem::getTexture() { return texture; }