#ifndef WINDOWMANAGER
#define WINDOWMANAGER

#include "SDL.h" 
#include <iostream>

class WindowManager {
public:
	WindowManager();
	void destroy();
	SDL_Window * getWindow();
	void toggleFullScreen();
	int getScreenWidth();
	int getScreenHeight();

private:
	void setupRC();

	SDL_Window * window; // window handle
	SDL_GLContext glContext; // OpenGL context handle
	const int SCREENWIDTH = 800;
	const int SCREENHEIGHT = 600;
	int fullScreenWidth;
	int fullScreenHeight;
	bool fullScreen;

	//#define SCREENWIDTH 800.0f
	//#define SCREENHEIGHT 600.0f
	//#define SCREENWIDTH 1024.0f
	//#define SCREENHEIGHT 768.0f
};

#endif