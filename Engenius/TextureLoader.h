#pragma once

#include <string>
#include <SDL_ttf.h>

namespace TextureLoader {

	unsigned int textToTexture(const char * str, TTF_Font* font);
	int loadTexture_SS(const std::string& path);
	int loadTexture(const std::string& type, const std::string& path);
	int loadTextureNonTransparent(const std::string& type, const std::string& path);
	unsigned int loadCubeMap(const char *fname[6]);
};