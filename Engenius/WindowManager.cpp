#include "WindowManager.h"

WindowManager::WindowManager() {
	setupRC(); // Create window and render context 
	fullScreen = false;
}

void WindowManager::destroy() {
	SDL_GL_DeleteContext(glContext);
	SDL_DestroyWindow(window);
	SDL_Quit();
}

SDL_Window * WindowManager::getWindow() {
	return window;
}

void exitFatalError(char *message)
{
	std::cout << message << " " << std::endl;
	std::cout << SDL_GetError();
	SDL_Quit();
	exit(1);
}

void WindowManager::setupRC() {
	if (SDL_Init(SDL_INIT_VIDEO) < 0) // Initialize video
		exitFatalError("Unable to initialize SDL");

	// Request an OpenGL 3.0 context.
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);  // double buffering on
	SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, 8); // 8 bit alpha buffering
	SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS, 1);
	SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, 4); // Turn on x4 multisampling anti-aliasing (MSAA)

	window = SDL_CreateWindow("Honours Project Game", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
		SCREENWIDTH, SCREENHEIGHT, SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN);
	if (!window) // Check window was created OK
		exitFatalError("Unable to create window");

	/*void SDL_GL_GetDrawableSize(SDL_Window* window,
	int*        w,
	int*        h) use with glviewport*/

	//https://wiki.libsdl.org/SDL_CreateWindow and more cool stuff

	//flags - SDL_WINDOW_FULLSCREEN for "real" fullscreen with a videomode change; 
	//			SDL_WINDOW_FULLSCREEN_DESKTOP for "fake" fullscreen that takes the size of the desktop; 
	//			and 0 for windowed mode.
	//	SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN);

	SDL_DisplayMode dm;
	if (SDL_GetCurrentDisplayMode(0, &dm) != 0)
	{
		SDL_Log("SDL_GetDesktopDisplayMode failed: %s", SDL_GetError());
	}
	fullScreenWidth = dm.w;
	fullScreenHeight = dm.h;

	glContext = SDL_GL_CreateContext(window); // Create opengl context and attach to window
	SDL_GL_SetSwapInterval(1); // set swap buffers to sync with monitor's vertical refresh rate
}

void WindowManager::toggleFullScreen() {
	if (fullScreen == true) {
		SDL_SetWindowFullscreen(window, 0);
		SDL_SetWindowSize(window, SCREENWIDTH, SCREENHEIGHT);
		SDL_SetWindowPosition(window, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED);
		fullScreen = false;
	}
	else {
		SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN);
		fullScreen = true;
	}
}

int WindowManager::getScreenWidth() {
	if (fullScreen) {
		//std::cout << fullScreen << ": " << fullScreenWidth << std::endl;
		return fullScreenWidth;
	}
	else {
		//std::cout << fullScreen << ": " << SCREENWIDTH << std::endl;
		return SCREENWIDTH;
	}
}
int WindowManager::getScreenHeight() {
	if (fullScreen) {
		//std::cout << fullScreen << ": " << fullScreenHeight << std::endl;
		return fullScreenHeight;
	}
	else {
		//std::cout << fullScreen << ": " << SCREENHEIGHT << std::endl;
		return SCREENHEIGHT;
	}
}