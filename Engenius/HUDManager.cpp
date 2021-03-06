#include "HUDManager.h"
#include "TextureLoader.h"
#include "ErrorCheck.h"

#include <iostream>

// initialize hud manager, open font
HUDManager::HUDManager(WindowManager* windowManager, Shader* shader) : windowManager(windowManager), shaderProgram(shader) {

	glm::vec2 uv_up_left = glm::vec2(0.0f, 0.0f);
	glm::vec2 uv_up_right = glm::vec2(1.0f, 0.0f);
	glm::vec2 uv_down_right = glm::vec2(1.0f, 1.0f);
	glm::vec2 uv_down_left = glm::vec2(0.0f, 1.0f);

	UVs.push_back(uv_up_left);
	UVs.push_back(uv_down_left);
	UVs.push_back(uv_up_right);

	UVs.push_back(uv_down_right);
	UVs.push_back(uv_up_right);
	UVs.push_back(uv_down_left);


	//// set up TrueType / SDL_ttf
	if (TTF_Init() == -1)
		std::cout << "TTF failed to initialise." << std::endl;

	textFont = TTF_OpenFont("../Engenius/Fonts/OpenSans-Regular.ttf", 24);
	if (textFont == NULL)
		std::cout << "Failed to open font - OpenSans-Regular.ttf" << std::endl;

	#define tl TextureLoader 
	crosshairTexture = tl::loadTexture_SS("../Engenius/Textures/crosshair.png");
	fireTexture = tl::loadTexture_SS("../Engenius/Textures/firetorch2.png");

	tex_background = tl::loadTexture_SS("../Engenius/Buttons/button_background.png");
	tex_objects = tl::loadTexture_SS("../Engenius/Buttons/button_objects.png");
	tex_bounding = tl::loadTexture_SS("../Engenius/Buttons/button_bounding.png");
	tex_lighting = tl::loadTexture_SS("../Engenius/Buttons/button_lighting.png");
	tex_audio = tl::loadTexture_SS("../Engenius/Buttons/button_audio.png");

	components.insert(std::make_pair("ambient", tl::loadTexture_SS("../Engenius/Buttons/button_ambient.png")));
	components.insert(std::make_pair("diffuse", tl::loadTexture_SS("../Engenius/Buttons/button_diffuse.png")));
	components.insert(std::make_pair("specular", tl::loadTexture_SS("../Engenius/Buttons/button_specular.png")));
	components.insert(std::make_pair("constant", tl::loadTexture_SS("../Engenius/Buttons/button_constant.png")));
	components.insert(std::make_pair("linear", tl::loadTexture_SS("../Engenius/Buttons/button_linear.png")));
	components.insert(std::make_pair("quadratic", tl::loadTexture_SS("../Engenius/Buttons/button_quadratic.png")));
	components.insert(std::make_pair("range", tl::loadTexture_SS("../Engenius/Buttons/button_range.png")));

	componentIter = components.begin();

	tex_eye = tl::loadTexture_SS("../Engenius/Buttons/icon_eye.png");
	tex_add = tl::loadTexture_SS("../Engenius/Buttons/icon_add.png");
	tex_move = tl::loadTexture_SS("../Engenius/Buttons/icon_move.png");

	tex_increase = tl::loadTexture_SS("../Engenius/Buttons/icon_up.png");
	tex_decrease = tl::loadTexture_SS("../Engenius/Buttons/icon_down.png");

	int spacingPlusSizeX = spacing + menuBar_SizeX;
	menuBar_PosY = windowManager->getScreenHeight() - 35;
	addMenuBarHUD(spacing, menuBar_PosY, menuBar_SizeX, menuBar_SizeY, "menu_objects", tex_objects, true, 0.4f);
	addMenuBarHUD(spacing + spacingPlusSizeX, menuBar_PosY, menuBar_SizeX, menuBar_SizeY, "menu_bounding", tex_bounding, true, 0.4f);
	addMenuBarHUD(spacing + spacingPlusSizeX + spacingPlusSizeX, menuBar_PosY, menuBar_SizeX, menuBar_SizeY, "menu_lighting", tex_lighting, true, 0.4f);
	lightingSubOptions();
	addMenuBarHUD(spacing + spacingPlusSizeX + spacingPlusSizeX + spacingPlusSizeX, menuBar_PosY, menuBar_SizeX, menuBar_SizeY, "menu_audio", tex_audio, true, 0.4f);

	//addMenuBarHUD(spacing + spacingPlusSizeX + spacingPlusSizeX + spacingPlusSizeX + spacingPlusSizeX, menuBar_PosY, menuBar_SizeX, menuBar_SizeY, "menu_test", "test", textFont, true, 0.4f);


	addAnimatedHUD(spacing, windowManager->getScreenHeight() / 2, menuBar_SizeX, menuBar_SizeX, fireTexture, false);

	VAO = new VertexArrayObject(6, Buffer_Options::gl_triangles);
	//generate two buffers
	VBO_verts = VAO->genBuffer_andAddData(Buffer_Options::gl_array_buffer, 6 * sizeof(glm::vec2), NULL, Buffer_Options::gl_stream_draw);
	VAO->enableVertexAttribArray(VBO_verts, 2, Buffer_Options::gl_float, Buffer_Options::gl_false, 0, 0);

	VBO_uvs = VAO->genBuffer_andAddData(Buffer_Options::gl_array_buffer, UVs.size() * sizeof(glm::vec2), &UVs[0], Buffer_Options::gl_static_draw);
	VAO->enableVertexAttribArray(VBO_uvs, 2, Buffer_Options::gl_float, Buffer_Options::gl_false, 0, 0);
}

void HUDManager::lightingSubOptions() {
	//remember the y pos of sub option is automatically subracted by size of menu bar
	int menuBarY = menuBar_PosY + menuBar_SizeY + spacing;
	std::unordered_map<std::string, HUDItem*> &light_sub = menuBarHUDs.at("menu_lighting")->subOptions;

	int xyzPos_x = (windowManager->getScreenWidth() - (menuBar_SizeX + subBar_SizeX + spacing));

	addSubOptions(light_sub, windowManager->getScreenWidth() - spacing - subBar_SizeX, menuBarY, subBar_SizeX, subBar_SizeY, "sub_increase", tex_increase, false);
	addSubOptions(light_sub, windowManager->getScreenWidth() - spacing - subBar_SizeX, menuBarY - subBar_SizeY, subBar_SizeX, subBar_SizeY, "sub_decrease", tex_decrease, false);
	addSubOptions(light_sub, xyzPos_x, menuBarY - (menuBar_SizeY / 2) - spacing, menuBar_SizeX, menuBar_SizeY, "sub_component", (*componentIter).second, true);

	addSubOptions(light_sub, xyzPos_x - spacing - spacing, menuBarY,
		(menuBar_SizeX + subBar_SizeX + spacing + spacing),
		(menuBar_SizeY + menuBar_SizeY + spacing), "sub_background_components", tex_background, true, 0.2f);


	//	addSubOptions(light_sub, xyzPos_x, menuBarY - subBar_SizeY - spacing, 32, 32, "sub_x", "x: ", textFont, false);
	//addSubOptions(light_sub, xyzPos_x, light_sub.at("sub_x")->getXY().y - subBar_SizeY - spacing, 32, 32, "sub_y", "y: ", textFont, false);
	//addSubOptions(light_sub, xyzPos_x, light_sub.at("sub_y")->getXY().y - subBar_SizeY - spacing, 32, 32, "sub_z", "z: ", textFont, false);

	//xyzPos_x = (windowManager->getScreenWidth() - (menuBar_SizeX ^ 2)) + subBar_SizeX + spacing;
	//addSubOptions(light_sub, xyzPos_x, menuBarY - subBar_SizeY - spacing, 64, 32, "sub_xInfo", "xi", textFont, false);
	//addSubOptions(light_sub, xyzPos_x, light_sub.at("sub_xInfo")->getXY().y - subBar_SizeY - spacing, 64, 32, "sub_yInfo", "yi", textFont, false);
	//addSubOptions(light_sub, xyzPos_x, light_sub.at("sub_yInfo")->getXY().y - subBar_SizeY - spacing, 64, 32, "sub_zInfo", "zi", textFont, false);


}

void HUDManager::addHUD(const int& x, const int& y, const int& sizeX, const int& sizeY, const std::string& storageName, const unsigned int& texture, const bool& allowLowTransparency, const float& transparency){
	HUDItem * h = new HUDItem(windowManager->getScreenWidth(), windowManager->getScreenHeight(), x, y, sizeX, sizeY, texture, allowLowTransparency);
	HUDs.insert(std::pair<std::string, HUDItem*>(storageName, h));
}
void HUDManager::addHUD(const int& x, const int& y, const int& sizeX, const int& sizeY, const std::string& storageName, const std::string& text, TTF_Font* font, const bool& allowLowTransparency, const float& transparency) {
	const char *cstr = text.c_str();
	unsigned int texture = TextureLoader::textToTexture(cstr, textFont);
	addHUD(x, y, sizeX, sizeY, storageName, allowLowTransparency, texture);
}

void HUDManager::addSubOptions(std::unordered_map<std::string, HUDItem*> &subOptions, const int& x, const int& y, const int& sizeX, const int& sizeY, const std::string& storageName, const unsigned int& texture, const bool& allowLowTransparency, const float& transparency) {
	int newx = x;
	int newy = y - sizeY - spacing;
	HUDItem * h = new HUDItem(windowManager->getScreenWidth(), windowManager->getScreenHeight(), newx, newy, sizeX, sizeY, texture, allowLowTransparency, transparency);
	subOptions.insert(std::pair<std::string, HUDItem*>(storageName, h));
}
void HUDManager::addSubOptions(std::unordered_map<std::string, HUDItem*> &subOptions, const int& x, const int& y, const int& sizeX, const int& sizeY, const std::string& storageName, const std::string& text, TTF_Font* font, const bool& allowLowTransparency, const float& transparency) {
	const char *cstr = text.c_str();
	addSubOptions(subOptions, x, y, sizeX, sizeY, storageName, allowLowTransparency, TextureLoader::textToTexture(cstr, font));
}

void HUDManager::addMenuBarHUD(const int& x, const int& y, const int& sizeX, const int& sizeY, const std::string& storageName, const unsigned int& texture, const bool& allowLowTransparency, const float& transparency) {
	MenubarHUD * menuh = new MenubarHUD(windowManager->getScreenWidth(), windowManager->getScreenHeight(), x, y, sizeX, sizeY, texture, allowLowTransparency, transparency);
	int temp_x;
	temp_x = x + subBar_SpaceFromEnd;
	addSubOptions(menuh->subOptions, temp_x, y, subBar_SizeX, subBar_SizeY, "sub_add", tex_add, false);
	temp_x = temp_x + subBar_SizeX + spacing;
	addSubOptions(menuh->subOptions, temp_x, y, subBar_SizeX, subBar_SizeY, "sub_move", tex_move, false);
	temp_x = temp_x + subBar_SizeX + spacing;
	addSubOptions(menuh->subOptions, temp_x, y, subBar_SizeX, subBar_SizeY, "sub_visualize", tex_eye, false);

	addSubOptions(menuh->subOptions, x, y, menuBar_SizeX, menuBar_SizeY, "sub_background", tex_background, true, 0.2f);
	menuBarHUDs.insert(std::pair<std::string, MenubarHUD*>(storageName, menuh));
}

void HUDManager::addMenuBarHUD(const int& x, const int& y, const int& sizeX, const int& sizeY, const std::string& storageName, const std::string& text, TTF_Font* font, const bool& allowLowTransparency, const float& transparency) {
	const char *cstr = text.c_str();
	unsigned int texture = TextureLoader::textToTexture(cstr, font);
	addMenuBarHUD(x, y, sizeX, sizeY, storageName, allowLowTransparency, texture);
}

void HUDManager::addAnimatedHUD(const int& x, const int& y, const int& sizeX, const int& sizeY, const unsigned int& texture, const bool& allowLowTransparency, const float& transparency) {
	AnimatedHUD * h = new AnimatedHUD(windowManager->getScreenWidth(), windowManager->getScreenHeight(), x, y, sizeX, sizeY, texture, allowLowTransparency, transparency);
	h->UVs.resize(6);
	h->duration = 0.0f;
	AnimatedHUDs.push_back(h);
}

void HUDManager::update(const float& dt_secs) {
	for (unsigned int i = 0; i < AnimatedHUDs.size(); i++) {
		AnimatedHUDs.at(i)->duration += dt_secs + 0.70f;

		int parser = static_cast<int>(AnimatedHUDs.at(i)->duration); //where in atlas 

		const int FINAL_TEX_POS = 7; // (7,7) is bottom right and last of "gif"
		const int TEX_IN_LINE = 8; // textures in each row or column 
		const float TEXTURES_IN_A_LINE = static_cast<const float>(TEX_IN_LINE);

		int textureX = (parser % TEX_IN_LINE);
		int textureY = (parser / TEX_IN_LINE);

		if (textureX >= FINAL_TEX_POS && textureY >= FINAL_TEX_POS) {
			AnimatedHUDs.at(i)->duration = 0.0;
		}

		float uv_x = textureX / TEXTURES_IN_A_LINE;
		float uv_y = textureY / TEXTURES_IN_A_LINE;

		//How it works - size of texture (1,1) so x+1 is end then divide to get to x of needed texture in atlas
		glm::vec2 uv_up_left = glm::vec2(uv_x, uv_y);
		glm::vec2 uv_up_right = glm::vec2(uv_x + 1.0f / TEXTURES_IN_A_LINE, uv_y);
		glm::vec2 uv_down_right = glm::vec2(uv_x + 1.0f / TEXTURES_IN_A_LINE, (uv_y + 1.0f / TEXTURES_IN_A_LINE));
		glm::vec2 uv_down_left = glm::vec2(uv_x, (uv_y + 1.0f / TEXTURES_IN_A_LINE));

		AnimatedHUDs.at(i)->UVs.at(0) = uv_up_left;
		AnimatedHUDs.at(i)->UVs.at(1) = uv_down_left;
		AnimatedHUDs.at(i)->UVs.at(2) = uv_up_right;

		AnimatedHUDs.at(i)->UVs.at(3) = uv_down_right;
		AnimatedHUDs.at(i)->UVs.at(4) = uv_up_right;
		AnimatedHUDs.at(i)->UVs.at(5) = uv_down_left;
	}
}

void HUDManager::render(const std::vector<glm::vec2>& verts, const unsigned int& texture, const bool& allowLowTransparency, const float& transparency, Renderer* renderer) {
	VAO->updateData(VBO_verts, Buffer_Options::gl_array_buffer, verts.size() * sizeof(glm::vec2), &verts.at(0));
	shaderProgram->uniform("transparency", transparency);
	shaderProgram->uniform("allowLowTransparency", allowLowTransparency);
	shaderProgram->reset_bindTex_2D("textureMap", texture);
	renderer->drawArrays_WO_bind(VAO);
}

void HUDManager::render_animated(const std::vector<glm::vec2>& verts, const std::vector<glm::vec2>& UVs, const unsigned int& texture, const bool& allowLowTransparency, const float& transparency, Renderer* renderer) {
	VAO->updateData(VBO_uvs, Buffer_Options::gl_array_buffer, UVs.size() * sizeof(glm::vec2), &UVs.at(0));
	render(verts, texture, allowLowTransparency, transparency, renderer);
}

void HUDManager::render(Renderer* renderer) {
	shaderProgram->bind();
	VAO->bind();

	renderer->enableBlend();
	renderer->setBlendFunction(BlendOptions::gl_one_minus_src_alpha);

	for (std::unordered_map<std::string, HUDItem*>::iterator it = HUDs.begin(); it != HUDs.end(); ++it) {
		render(it->second->getVerts(), it->second->getTexture(), it->second->getAllowLowTransparency(), it->second->getTransparency(), renderer);
	}

	for (unsigned int i = 0; i < AnimatedHUDs.size(); i++) {
		render_animated(AnimatedHUDs.at(i)->getVerts(), AnimatedHUDs.at(i)->UVs, AnimatedHUDs.at(i)->getTexture(), AnimatedHUDs.at(i)->getAllowLowTransparency(), AnimatedHUDs.at(i)->getTransparency(), renderer);
	}

	//reset UVs in VAO to non animated version
	VAO->updateData(VBO_uvs, Buffer_Options::gl_array_buffer, UVs.size() * sizeof(glm::vec2), &UVs.at(0));

	for (std::unordered_map<std::string, MenubarHUD*>::iterator it = menuBarHUDs.begin(); it != menuBarHUDs.end(); ++it) {
		render(it->second->getVerts(), it->second->getTexture(), it->second->getAllowLowTransparency(), it->second->getTransparency(), renderer);
		if (it->second->getIfClicked() == true) {
			for (std::unordered_map<std::string, HUDItem*>::iterator iter = it->second->subOptions.begin(); iter != it->second->subOptions.end(); ++iter) {
				render(iter->second->getVerts(), iter->second->getTexture(), iter->second->getAllowLowTransparency(), iter->second->getTransparency(), renderer);
			}
		}
	}

	renderer->disableBlend();
}

bool HUDManager::checkIfClicked(const int& mouseX, const int& mouseY, const int& screenHeight, const std::string& buttonName) {
	glm::vec2 pos = menuBarHUDs.at(buttonName)->getXY();
	glm::vec2 size = menuBarHUDs.at(buttonName)->getSize();
	if (mouseX > pos.x && mouseX < pos.x + size.x) {
		if (mouseY < screenHeight - pos.y && mouseY > screenHeight - pos.y - size.y) {
			for (std::unordered_map<std::string, MenubarHUD*>::iterator it = menuBarHUDs.begin(); it != menuBarHUDs.end(); ++it) {
				it->second->setIfClicked(false);
			}
			menuBarHUDs.at(buttonName)->setIfClicked(true);
			return true;
		}
	}
	return false;
}

bool HUDManager::checkSubBarClicked(const int& mouseX, const int& mouseY, const int& screenHeight, const std::string& buttonName) {
	bool foundClicked = false;
	MenubarHUD* activeOption;
	for (std::unordered_map<std::string, MenubarHUD*>::iterator it = menuBarHUDs.begin(); it != menuBarHUDs.end(); ++it) {
		if (it->second->getIfClicked() == true) {
			activeOption = it->second;
			foundClicked = true;
		}
	}

	if (foundClicked) {
		glm::vec2 pos = activeOption->subOptions.at(buttonName)->getXY();
		glm::vec2 size = activeOption->subOptions.at(buttonName)->getSize();
		if (mouseX > pos.x && mouseX < pos.x + size.x) {
			if (mouseY < screenHeight - pos.y && mouseY > screenHeight - pos.y - size.y) {
				for (std::unordered_map<std::string, HUDItem*>::iterator it = activeOption->subOptions.begin(); it != activeOption->subOptions.end(); ++it) {
					it->second->setIfClicked(false);
				}
				activeOption->subOptions.at(buttonName)->setIfClicked(true);
				return true;
			}
		}
	}
	return false;
}

void HUDManager::displaySelected(const std::string& buttonName) {
	for (std::unordered_map<std::string, MenubarHUD*>::iterator it = menuBarHUDs.begin(); it != menuBarHUDs.end(); ++it) {
		it->second->setAllowLowTransparency(true);
		it->second->setTransparency(0.4f);
		it->second->setIfClicked(false);
	}
	menuBarHUDs.at(buttonName)->setAllowLowTransparency(false);
	menuBarHUDs.at(buttonName)->setTransparency(1.0f);
	menuBarHUDs.at(buttonName)->setIfClicked(true);
}

void HUDManager::swapComponent(const bool& next) {
	if (next) {
		componentIter++;
		if (componentIter == components.end())
			componentIter = components.begin();
	}
	else {
		if (componentIter == components.begin())
			componentIter = components.end();
		componentIter--; // minus anyway as end is after last object
	}

	menuBarHUDs.at("menu_lighting")->subOptions.at("sub_component")->setTexture((*componentIter).second);
}

std::string HUDManager::getCurrentComponent() {
	return (*componentIter).first;
}