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
	//int SCREENWIDTH = 800;
	//int SCREENHEIGHT = 600;
	//int SCREENWIDTH = 1600;
	//int SCREENHEIGHT = 900;
	int SCREENWIDTH = 1024;
	int SCREENHEIGHT = 576;
	int fullScreenWidth;
	int fullScreenHeight;
	bool fullScreen;

	//#define SCREENWIDTH 800.0f
	//#define SCREENHEIGHT 600.0f
	//#define SCREENWIDTH 1024.0f
	//#define SCREENHEIGHT 768.0f
};

#endif