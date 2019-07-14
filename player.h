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
	void setPos(glm::vec3 pos);
	void setGrounded(bool grounded);

private:
	unsigned int VAO, VBO;
	glm::vec3 minPos, maxPos;
	glm::vec3 vel, dir, right, pos;
	float yaw, speed, friction;
	std::vector<Vertex3D> verts;
	bool grounded;
	void genVAO();
};