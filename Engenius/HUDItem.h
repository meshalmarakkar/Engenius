#ifndef HUDITEM
#define HUDITEM

#include <GL/glew.h>
#include <glm/gtc/type_ptr.hpp>

#include <vector>

class HUDItem {
public:
	HUDItem(int screenWidth, int screenHeight, int x, int y, int sizeX, int sizeY, GLuint texture, bool allowLowTransparency, float transparency = 0.0f);
	void convert_ClipSpace_to_HomogeneousSpace(int screenWidth, int screenHeight);

	void setTexture(GLuint newTex);
	void setIfClicked(bool newVal);
	void setTransparency(float newVal);
	void setAllowLowTransparency(bool newVal);

	bool getAllowLowTransparency();
	float getTransparency();
	bool getIfClicked();
	const std::vector<glm::vec2> getVerts();
	GLuint getTexture();
	glm::vec2 getXY();
	glm::vec2 getSize();

protected:
	glm::vec2 xy;
	glm::vec2 size;
	bool clicked;
	GLuint texture;
	std::vector<glm::vec2> verts;
	float transparency;
	bool allowLowTransparency;

private:
	void init(int screenWidth, int screenHeight, int x, int y, int sizeX, int sizeY, GLuint texture, bool allowLowTransparency, float transparency);
};

#endif