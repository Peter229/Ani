#include "windowGL.h"

#define STB_IMAGE_IMPLEMENTATION

#include "game.h"


int main() {

	//Start Window
	if (WindowGL::start() == -1) {
		return -1;
	}

	Game* game = new Game();
	game->start(SCR_WIDTH, SCR_HEIGHT);

	//Create Variables To Measure Game Speed + How Fast The Game Should Run
	float deltaTime = 0.0f;
	float lastFrame = 0.0f;

	float deltaTimeT = 0;
	float deltaTime2 = 0;
	double lastTimeA = glfwGetTime();
	double lastTimeB = glfwGetTime();
	int nbFrames = 0;

	while (!glfwWindowShouldClose(WindowGL::getWindow())) { //Game Is Running Until The Window Is Closed

		double currentTimeA = glfwGetTime();
		nbFrames++;
		if (currentTimeA - lastTimeA >= 1.0) {
			printf("%f ms/frame\n%d Frames Per Second\n", 1000.0 / double(nbFrames), nbFrames);
			nbFrames = 0;
			lastTimeA += 1.0;
		}

		//Delta Time To Get How Fast The Game Should Run
		float currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		//Get Inputs
		//glfwPollEvents();
		WindowGL::update();

		game->update(WindowGL::getKeys(), WindowGL::getMousePos(), deltaTime);

		if (currentTimeA - lastTimeB >= 0.0166) { //Call physics 60 times a second
			//bob->Update(0.0166f);
			lastTimeB += 0.0166;

			game->tick();
		}

		game->render();

		//bob->Render(&aniShader);
		//Go To Next Frame
		glfwSwapBuffers(WindowGL::getWindow());
	}

	//Game Has Ended, Destroy Everything
	game->cleanUp();

	WindowGL::end();
	return 0;
}