#include "GameManager.h"
#include "WindowManager.h"
using namespace std;

#if _DEBUG
//sets subsystem to windows so console doesn't show up
#pragma comment(linker, "/subsystem:\"console\" /entry:\"WinMainCRTStartup\"")
//#pragma comment(linker, "/SUBSYSTEM:windows /ENTRY:mainCRTStartup")
#endif


float gameTime(unsigned int &lastTime) {
	//clock_t currentTime = clock();

	//clock_t delta = currentTime - lastTime;
	//float dt_secs = delta / CLOCKS_PER_SEC;

	//if (dt_secs != 0.017f) dt_secs = 0.017f; // is this fair???
	//lastTime = currentTime;
	float dt_secs = 0.017f;
	return dt_secs;
}

// Program entry point - SDL manages the actual WinMain entry point for us
int main(int argc, char *argv[]) {
	WindowManager * windowManager = new WindowManager();
	GameManager * game = new GameManager();

	// Required on Windows *only* init GLEW to access OpenGL beyond 1.1
	glewExperimental = GL_TRUE;
	GLenum err = glewInit();

	if (GLEW_OK != err) { // glewInit failed, something is seriously wrong
		std::cout << "glewInit failed, aborting." << endl;
		exit(1);
	}
	cout << glGetString(GL_VERSION) << endl;

	game->init(windowManager);

	unsigned int lastTime = clock();
	//SDL_Event sdlEvent;
	bool running = true; // set running to true
	while (running) {	// the event loop
		float dt_secs = gameTime(lastTime);
		running = game->ControlCheck(dt_secs, windowManager);
		game->update(dt_secs);
		game->draw(windowManager);
	}

	windowManager->destroy();
	return 0;
}