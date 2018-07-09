#pragma once

#include <string>

namespace TextureLoader {

	int loadTexture(const std::string type, const std::string path);
	int loadTextureNonTransparent(const std::string type, const std::string path);
	unsigned int loadCubeMap(const char *fname[6]);
};