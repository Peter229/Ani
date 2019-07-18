#pragma once

#include "windowGL.h"
#include "includesGL.h"

#include "shader.h"
#include "camera.h"
#include "hud.h"
#include "MD5Model.h"
#include "levelCreator.h"
#include "player.h"
#include "shadowMap.h"

class Game {

public:
	Game();
	~Game();

	void start();

	void loop();

	void update(GLboolean* Keys, double* mousePos, float deltaTime);
	void logic();

	void tick();

	void render();

	void cleanUp();
private:
	hud* huddy;
	MD5Model* bob;
	levelCreator* level;
	Player* player;
	ShadowMap* shadowMap;
	Camera* camera;
	Shader* terrainShader;
	Shader* hudShader;
	Shader* aniShader;
	Shader* depthShader;
	Shader* shadowShader;
	bool firstMouse;
	double lastX, lastY;
	int windowWidth, windowHeight, dir, move;
	bool m1, shift, shaderVersion, r, up, down, vup, vdown, left, right, del, walk, walkToggle, ctrl, space, v, cameraPos, tab, mouseEnable;
	glm::vec3 startMouse, endMouse;

	float deltaTime;
	float lastFrame;

	float deltaTimeT;
	float deltaTime2;
	double lastTimeA;
	double lastTimeB;
	int nbFrames;

};