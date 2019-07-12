#pragma once

#include <glad/glad.h>
#include "glm\glm.hpp"
#include <glm/gtc/matrix_transform.hpp>

#include "shader.h"

class ShadowMap {

public:
	ShadowMap();
	~ShadowMap();

	void createShadow();
	void cleanUp();
	void update(Shader *shader);
	void render();

	unsigned int* getDepthMap();

private:
	const unsigned int SHADOW_WIDTH = 1024, SHADOW_HEIGHT = 1024;

	unsigned int depthMapFBO, depthMap;

	void createFBO();
};