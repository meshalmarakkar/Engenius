#ifndef HUDITEM
#define HUDITEM

#include <GL/glew.h>
#include <glm/gtc/type_ptr.hpp>

#include <vector>

class HUDItem {
public:
	HUDItem(const int& screenWidth, const int& screenHeight, const int& x, const int& y, const int& sizeX, const int& sizeY, const unsigned int& texture, const bool& allowLowTransparency, const float& transparency = 0.0f);
	void convert_ClipSpace_to_HomogeneousSpace(const int& screenWidth, const int& screenHeight);

	void setTexture(unsigned int newTex);
	void setIfClicked(bool newVal);
	void setTransparency(float newVal);
	void setAllowLowTransparency(bool newVal);

	bool getAllowLowTransparency();
	float getTransparency();
	bool getIfClicked();
	const std::vector<glm::vec2> getVerts();
	unsigned int getTexture();
	glm::vec2 getXY();
	glm::vec2 getSize();

protected:
	glm::vec2 xy;
	glm::vec2 size;
	bool clicked;
	unsigned int texture;
	std::vector<glm::vec2> verts;
	float transparency;
	bool allowLowTransparency;

private:
	void init(const int& screenWidth, const int& screenHeight, const int& x, const int& y, const int& sizeX, const int& sizeY, const unsigned int& texture, const bool& allowLowTransparency, const float& transparency);
};

#endif