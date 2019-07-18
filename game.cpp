#include "game.h"

Game::Game() {

}
 
Game::~Game() {

}

void Game::start() {

	if (WindowGL::start() == -1) {
		std::cout << "Failed to create window\n";
	}

	deltaTime = 0.0f;
	lastFrame = 0.0f;

	deltaTimeT = 0;
	deltaTime2 = 0;
	lastTimeA = glfwGetTime();
	lastTimeB = glfwGetTime();
	nbFrames = 0;

	this->windowWidth = SCR_WIDTH;
	this->windowHeight = SCR_HEIGHT;

	m1 = shaderVersion = shift = r = del = up = down = vup = vdown = left = right = del = walk = walkToggle = ctrl = space = v = cameraPos = tab = mouseEnable = false;

	endMouse = startMouse = glm::vec3(0.0f, 0.0f, 0.0f);

	move = 0;

	double lastX = windowWidth / 2.0f, lastY = windowHeight / 2.0f;
	bool firstMouse = true;

	//Load Shaders
	terrainShader = new Shader("Shaders/Vertex/vertexShader.txt", "Shaders/Fragment/fragmentShader.txt");
	hudShader = new Shader("Shaders/Vertex/hudVertexShader.txt", "Shaders/Fragment/hudFragmentShader.txt");
	aniShader = new Shader("Shaders/Vertex/vertexAniShader.txt", "Shaders/Fragment/fragmentAniShader.txt");
	depthShader = new Shader("Shaders/Vertex/vertexDepth.txt", "Shaders/Fragment/fragmentDepth.txt");
	shadowShader = new Shader("Shaders/Vertex/vertexShadow.txt", "Shaders/Fragment/fragmentShadow.txt");

	//Load Textures
	Resource_Manager::loadTexture("Textures/grass.png", "tex", 1);
	Resource_Manager::loadTexture("Textures/textures.png", "textures", 1);
	Resource_Manager::loadTexture("Textures/font.png", "font", 1);
	Resource_Manager::loadTexture("Textures/crosshair.png", "crosshair", 1);
	Resource_Manager::loadTexture("Textures/DCrosshair.png", "DCrosshair", 1);
	Resource_Manager::loadTexture("bob_head.png", "head", 0);
	Resource_Manager::loadTexture("bob_body.png", "body", 0);

	huddy = new hud(glm::mat4(1.0f), windowWidth, windowHeight);

	huddy->drawString("Top 5 Bruh Moments", -38.0f, -10.5f);
	huddy->drawCrosshair();
	huddy->drawTextures();

	bob = new MD5Model();
	bob->LoadModel("boblampclean.md5mesh");
	bob->LoadAnim("boblampclean.md5anim");
	//bob->LoadModel("bob_lamp_update_export.md5mesh");
	//bob->LoadAnim("bob_lamp_update_export.md5anim");

	level = new levelCreator();
	level->createBrush(glm::vec3(0.0f), glm::vec3(40.0f, 1.0f, 40.0f));
	level->createBrush(glm::vec3(-1.0f, 1.0f, 0.0f), glm::vec3(1.0f, 10.0f, 40.0f));

	player = new Player();
	player->createPlayer();

	shadowMap = new ShadowMap();
	shadowMap->createShadow();

	glm::vec3 lightPos(-2.0f, 10.0f, -1.0);

	shadowShader->use();
	shadowShader->setInt("diffuseTexture", 0);
	shadowShader->setInt("shadowMap", 1);

	camera = new Camera(glm::vec3(0.0f, 5.0f, 0.0f));
}

void Game::loop() {

	while (!glfwWindowShouldClose(WindowGL::getWindow())) {

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
		WindowGL::update();

		update(WindowGL::getKeys(), WindowGL::getMousePos(), deltaTime);

		if (currentTimeA - lastTimeB >= 0.0166) { //Call physics 60 times a second

			lastTimeB += 0.0166;

			tick();
		}

		render();

		//Go To Next Frame
		glfwSwapBuffers(WindowGL::getWindow());
	}
}

void Game::update(GLboolean* Keys, double* mousePos, float deltaTime) {

	//Mouse Movement
	double xpos = mousePos[0];
	double ypos = mousePos[1];

	if (!mouseEnable) {
		if (firstMouse) {

			lastX = xpos;
			lastY = ypos;
			firstMouse = false;
		}

		float xoffset = xpos - lastX;
		float yoffset = lastY - ypos;

		lastX = xpos;
		lastY = ypos;

		camera->ProcessMouseMovement(xoffset, yoffset);

		//Mouse Buttons
		if (Keys[GLFW_MOUSE_BUTTON_LEFT] == GLFW_PRESS && m1 == false) {
			if (shift) {
				float t = rayPlaneIntersection(camera->Front, camera->Position, glm::vec3(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
				startMouse = camera->Position + (camera->Front * t);
				startMouse.x = floor(startMouse.x);
				startMouse.y = floor(startMouse.y);
				startMouse.z = floor(startMouse.z);
				//level->createBrush(bPos);
			}
			else if (ctrl) {
				level->findTri(camera->Position, camera->Front);
			}
			else if (space) {
				level->findVertex(camera->Position, camera->Front, move);
			}
			else {
				level->findBrushes(camera->Front, camera->Position);
			}
			m1 = true;
		}
		if (m1 == true) {
			if (Keys[GLFW_MOUSE_BUTTON_LEFT] == GLFW_RELEASE) {
				m1 = false;
				if (shift) {
					float t = rayPlaneIntersection(camera->Front, camera->Position, glm::vec3(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
					endMouse = camera->Position + (camera->Front * t);
					endMouse.x = floor(endMouse.x);
					endMouse.y = floor(endMouse.y);
					endMouse.z = floor(endMouse.z);
					level->createBrush(startMouse, glm::vec3(endMouse.x - startMouse.x, 1.0f, endMouse.z - startMouse.z));
				}
			}
		}
	}
	else {
		firstMouse = true;
		if (Keys[GLFW_MOUSE_BUTTON_LEFT] == GLFW_PRESS) {
			huddy->checkCollision(xpos, ypos);
		}
		//std::cout << xpos << " " << ypos << "\n";
	}

	//KeyBoard Inputs
	if (walk) {
		if (Keys[GLFW_KEY_W] == GLFW_PRESS) {
			player->move(0);
		}
		if (Keys[GLFW_KEY_S] == GLFW_PRESS) {
			player->move(1);
		}
		if (Keys[GLFW_KEY_A] == GLFW_PRESS) {
			player->move(2);
		}
		if (Keys[GLFW_KEY_D] == GLFW_PRESS) {
			player->move(3);
		}
		if (Keys[GLFW_KEY_E] == GLFW_PRESS) {
			player->move(4);
		}
		if (Keys[GLFW_KEY_Q] == GLFW_PRESS) {
			player->move(5);
		}
		if (Keys[GLFW_KEY_SPACE] == GLFW_PRESS) {
			player->move(6);
		}
	}
	else {
		if (Keys[GLFW_KEY_W] == GLFW_PRESS) {
			camera->ProcessKeyboard(FORWARD, deltaTime);
		}
		if (Keys[GLFW_KEY_S] == GLFW_PRESS) {
			camera->ProcessKeyboard(BACKWARD, deltaTime);
		}
		if (Keys[GLFW_KEY_A] == GLFW_PRESS) {
			camera->ProcessKeyboard(LEFT, deltaTime);
		}
		if (Keys[GLFW_KEY_D] == GLFW_PRESS) {
			camera->ProcessKeyboard(RIGHT, deltaTime);
		}
	}

	if (Keys[GLFW_KEY_R] == GLFW_PRESS && r == false) {
		r = true;
		shaderVersion = !shaderVersion;
	}
	if (Keys[GLFW_KEY_R] == GLFW_RELEASE && r == true) {
		r = false;
	}

	if (Keys[GLFW_KEY_UP] == GLFW_PRESS && up == false) {
		up = true;
		dir = 0;
	}
	if (Keys[GLFW_KEY_UP] == GLFW_RELEASE && up == true) {
		up = false;
	}

	if (Keys[GLFW_KEY_DOWN] == GLFW_PRESS && down == false) {
		down = true;
		dir = 1;
	}
	if (Keys[GLFW_KEY_DOWN] == GLFW_RELEASE && down == true) {
		down = false;
	}

	if (Keys[GLFW_KEY_LEFT] == GLFW_PRESS && left == false) {
		left = true;
		dir = 2;
	}
	if (Keys[GLFW_KEY_LEFT] == GLFW_RELEASE && left == true) {
		left = false;
	}

	if (Keys[GLFW_KEY_RIGHT] == GLFW_PRESS && right == false) {
		right = true;
		dir = 3;
	}
	if (Keys[GLFW_KEY_RIGHT] == GLFW_RELEASE && right == true) {
		right = false;
	}

	if (Keys[GLFW_KEY_PAGE_UP] == GLFW_PRESS && vup == false) {
		vup = true;
		dir = 4;
	}
	if (Keys[GLFW_KEY_PAGE_UP] == GLFW_RELEASE && vup == true) {
		vup = false;
	}

	if (Keys[GLFW_KEY_PAGE_DOWN] == GLFW_PRESS && vdown == false) {
		vdown = true;
		dir = 5;
	}
	if (Keys[GLFW_KEY_PAGE_DOWN] == GLFW_RELEASE && vdown == true) {
		vdown = false;
	}

	if (Keys[GLFW_KEY_DELETE] == GLFW_PRESS && del == false) {
		del = true;
		dir = 60;
	}
	if (Keys[GLFW_KEY_DELETE] == GLFW_RELEASE && del == true) {
		del = false;
	}

	if (Keys[GLFW_KEY_LEFT_SHIFT] == GLFW_PRESS && shift == false) {
		shift = true;
	}
	if (Keys[GLFW_KEY_LEFT_SHIFT] == GLFW_RELEASE && shift == true) {
		shift = false;
	}

	if (Keys[GLFW_KEY_LEFT_CONTROL] == GLFW_PRESS && ctrl == false) {
		ctrl = true;
	}
	if (Keys[GLFW_KEY_LEFT_CONTROL] == GLFW_RELEASE && ctrl == true) {
		ctrl = false;
	}

	if (Keys[GLFW_KEY_P] == GLFW_PRESS && walkToggle == false) {
		walk = !walk;
		walkToggle = true;
	}
	if (Keys[GLFW_KEY_P] == GLFW_RELEASE && walkToggle == true) {
		walkToggle = false;
	}

	if (Keys[GLFW_KEY_SPACE] == GLFW_PRESS && space == false) {
		space = true;
	}
	if (Keys[GLFW_KEY_SPACE] == GLFW_RELEASE && space == true) {
		space = false;
	}

	if (Keys[GLFW_KEY_1] == GLFW_PRESS) {
		move = 0;
	}

	if (Keys[GLFW_KEY_2] == GLFW_PRESS) {
		move = 1;
	}

	if (Keys[GLFW_KEY_3] == GLFW_PRESS) {
		move = 2;
	}

	if (Keys[GLFW_KEY_4] == GLFW_PRESS) {
		move = 3;
	}

	if (Keys[GLFW_KEY_5] == GLFW_PRESS) {
		move = 4;
	}

	if (Keys[GLFW_KEY_6] == GLFW_PRESS) {
		move = 5;
	}

	if (Keys[GLFW_KEY_V] == GLFW_PRESS && v == false) {
		cameraPos = !cameraPos;
		v = true;
	}
	if (Keys[GLFW_KEY_V] == GLFW_RELEASE && v == true) {
		v = false;
	}

	if (Keys[GLFW_KEY_TAB] == GLFW_PRESS && tab == false) {
		mouseEnable = !mouseEnable;
		tab = true;
	}
	if (Keys[GLFW_KEY_TAB] == GLFW_RELEASE && tab == true) {
		tab = false;
	}

	logic();
}

void Game::logic() {

	if (dir != -1) {
		if (shift) {
			dir += 6;
		}
		level->editBrush(dir);
		dir = -1;
	}
}

void Game::tick() {

	player->updateYaw(camera->Yaw);

	//for (int i = 0; i < keyQueue.size(); i++) {
		//player->move();
	//}

	//keyQueue.clear();

	player->checkCollision(level);
}

void Game::render() {

	if (cameraPos) {
		camera->Position = player->getPos();
	}

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(0.2f, 0.2f, 0.9f, 1.0f);

	glm::mat4 projection = glm::perspective(glm::radians(camera->Zoom), (float)windowWidth / (float)windowHeight, 0.1f, 400.0f);
	glm::mat4 view = camera->GetViewMatrix();
	glm::mat4 model = glm::mat4(1.0f);

	glm::vec3 lightPos = glm::vec3(-5.0f, 20.0f, -5.0f);

	//Depth Shader
	float near_plane = 1.0f, far_plane = 50.5f;
	glm::mat4 lightProjection = glm::ortho(-40.0f, 40.0f, -40.0f, 40.0f, near_plane, far_plane);
	glm::mat4 lightView = glm::lookAt(lightPos, glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	glm::mat4 lightSpaceMatrix = lightProjection * lightView;

	//Set Shader
	if (shaderVersion) {
		glCullFace(GL_NONE);
		depthShader->use();
		depthShader->setMat4("lightSpaceMatrix", lightSpaceMatrix);
		//depthShader.setMat4("model", model);
		shadowMap->update(depthShader);
		level->render(depthShader);
		player->render(depthShader);

		glCullFace(GL_FRONT);

		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glViewport(0, 0, windowWidth, windowHeight);
	}

	//Render With Basic Shader
	terrainShader->use();
	//Set Projection Matrix
	terrainShader->setMat4("projection", projection);
	//Set View Matrix
	terrainShader->setMat4("view", view);

	player->render(terrainShader);
	level->renderBrushSelection(terrainShader);

	if (shaderVersion) {
		shadowShader->use();
		shadowShader->setMat4("projection", projection);
		shadowShader->setMat4("view", view);
		shadowShader->setMat4("lightSpaceMatrix", lightSpaceMatrix);
		shadowShader->setVec3("viewPos", camera->Position);
		shadowShader->setVec3("lightPos", lightPos);
		level->render(shadowShader, shadowMap->getDepthMap());
	}
	else {
		level->render(terrainShader, 0);
	}

	glActiveTexture(GL_TEXTURE0);

	//Render Hud
	hudShader->use();
	huddy->render(hudShader);

}

void Game::cleanUp() {

	delete bob;

	Resource_Manager::clearData();

	huddy->cleanUp();
	huddy->cleanUpMenu();
	level->cleanUp();
	player->cleanUp();
	shadowMap->cleanUp();

	WindowGL::end();
}