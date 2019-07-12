#pragma once
#ifndef WORLD_H
#define WORLD_H

#include "resource_manager.h"
#include "glm/glm.hpp"
#include "PerlinNoise.h"
#include "shader.h"
#include "data_types.h"

class World {

public:
	void createWorld(int seed);
	void render(Shader *shader);
	void cleanUp();
	float heightAtPoint(glm::vec3);

private:
	unsigned int VAO, VBO, worldSize = 64;
	std::vector<Vertex3D> verts;
	void genVAO();
	glm::vec3 calcAvgNormal(int seed, int seedTwo, glm::vec3 pos);
};

#endif