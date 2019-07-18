#define STB_IMAGE_IMPLEMENTATION

#include "game.h"


int main() {

	//Start Window
	Game* game = new Game();
	game->start();

	//Run the game
	game->loop();

	//Game Has Ended, Destroy Everything
	game->cleanUp();

	return 0;
}