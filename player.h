#pragma once

#include <glad/glad.h>
#include "shader.h"
#include "resource_manager.h"
#include "data_types.h"
#include "levelCreator.h"

class Player {

public:
	Player();
	~Player();

	void createPlayer();
	void render(Shader *shader);
	void move(int key);
	void updateYaw(float offset);
	void checkCollision(levelCreator* level);
	void cleanUp();
	glm::vec3 getPos();
	glm::vec3 pos;
private:
	unsigned int VAO, VBO;
	glm::vec3 minPos, maxPos;
	glm::vec3 vel, dir, right;
	float yaw, speed, friction;
	std::vector<Vertex3D> verts;

	void genVAO();
};