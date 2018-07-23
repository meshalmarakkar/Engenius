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
	//int SCREENWIDTH = 800; //4:3 ratio
	//int SCREENHEIGHT = 600;
	//int SCREENWIDTH = 1600; //16:9 ratio
	//int SCREENHEIGHT = 900;
	int SCREENWIDTH = 1024; //16:9 ratio
	int SCREENHEIGHT = 576;
//	int SCREENWIDTH = 768; //16:9 ratio
//	int SCREENHEIGHT = 432;
	int fullScreenWidth;
	int fullScreenHeight;
	bool fullScreen;
};

#endif