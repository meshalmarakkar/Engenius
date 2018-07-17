#ifndef HUDMANAGER
#define HUDMANAGER

#include <SDL_ttf.h>
#include <string>
#include <SOIL.h>
#include <glm/gtc/type_ptr.hpp>
#include <GL/glew.h>
#include <iostream>
#include <vector>
#include <unordered_map>
#include <map>
#include "HUDItem.h"
#include "WindowManager.h"

class HUDManager {
public:
	HUDManager(WindowManager* windowManager, const unsigned int& shader);
	void addHUD(const int& x, const int& y, const int& sizeX, const int& sizeY, const std::string& storageName, const std::string& text, TTF_Font* font, const bool& allowLowTransparency, const float& transparency = 0.0f);
	void addHUD(const int& x, const int& y, const int& sizeX, const int& sizeY, const std::string& storageName, const unsigned int& texture, const bool& allowLowTransparency, const float& transparency = 0.0f);
	void addMenuBarHUD(const int& x, const int& y, const int& sizeX, const int& sizeY, const std::string& storageName, const unsigned int& texture, const bool& allowLowTransparency, const float& transparency = 0.0f);
	void addMenuBarHUD(const int& x, const int& y, const int& sizeX, const int& sizeY, const std::string& storageName, const std::string& text, TTF_Font* font, const bool& allowLowTransparency, const float& transparency = 0.0f);
	void addAnimatedHUD(const int& x, const int& y, const int& sizeX, const int& sizeY, const unsigned int& texture, const bool& allowLowTransparency, const float& transparency = 0.0f);
	void render();

	void update(const float& dt_secs);
	bool checkIfClicked(const int& mouseX, const int& mouseY, const int& screenHeight, const std::string& buttonName);
	bool checkSubBarClicked(const int& mouseX, const int& mouseY, const int& screenHeight, const std::string& buttonName);
	void displaySelected(const std::string& buttonName);
	void swapComponent(const bool& next);
	std::string getCurrentComponent();

private:
	std::vector<glm::vec2> UVs;

	struct AnimatedHUD : public HUDItem {
		AnimatedHUD(const int& screenWidth, const int& screenHeight, const int& x, const int& y, const int& sizeX, const int& sizeY, const unsigned int& texture, const bool& allowLowTransparency, const float& transparency) : HUDItem(screenWidth, screenHeight, x, y, sizeX, sizeY, texture, allowLowTransparency, transparency) { ; }
		std::vector<glm::vec2> UVs;
		float duration;
	};

	struct MenubarHUD : public HUDItem {
		MenubarHUD(const int& screenWidth, const int& screenHeight, const int& x, const int& y, const int& sizeX, const int& sizeY, const unsigned int& texture, const bool& allowLowTransparency, const float& transparency) : HUDItem(screenWidth, screenHeight, x, y, sizeX, sizeY, texture, allowLowTransparency, transparency) { ; }
		std::unordered_map<std::string, HUDItem*> subOptions;
	};

	std::unordered_map<std::string, HUDItem*> HUDs;
	std::vector<AnimatedHUD*> AnimatedHUDs;
	std::unordered_map<std::string, MenubarHUD*> menuBarHUDs;
	void render(const std::vector<glm::vec2>& verts, const std::vector<glm::vec2>& UVs, const unsigned int& texture, const bool& allowLowTransparency, const float& transparency);
	void addSubOptions(std::unordered_map<std::string, HUDItem*> &subOptions, const int& x, const int& y, const int& sizeX, const int& sizeY, const std::string& storageName, const unsigned int& texture, const bool& allowLowTransparency, const float& transparency = 0.0f);
	void addSubOptions(std::unordered_map<std::string, HUDItem*> &subOptions, const int& x, const int& y, const int& sizeX, const int& sizeY, const std::string& storageName, const std::string& text, TTF_Font* font, const bool& allowLowTransparency, const float& transparency = 0.0f);

	void lightingSubOptions();

	WindowManager* windowManager;

	unsigned int shader;
	unsigned int vao;
	unsigned int vbo[2];
	TTF_Font * textFont;
	unsigned int crosshairTexture;
	unsigned int fireTexture;

	//EDITMODE
	const int menuBar_SizeX = 128;
	const int menuBar_SizeY = 32;
	int menuBar_PosY = 565;

	const int spacing = 2;

	const int subBar_SpaceFromEnd = 13;
	const int subBar_SizeX = 32;
	const int subBar_SizeY = 32;

	unsigned int tex_background;
	unsigned int tex_objects;
	unsigned int tex_bounding;
	unsigned int tex_lighting;
	unsigned int tex_audio;

	std::unordered_map<std::string, unsigned int> components;
	std::unordered_map<std::string, unsigned int>::iterator componentIter;

	unsigned int tex_eye;
	unsigned int tex_add;
	unsigned int tex_move;

	unsigned int tex_increase;
	unsigned int tex_decrease;
};

#endif
