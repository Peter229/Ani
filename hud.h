#pragma once
#ifndef HUD_H
#define HUD_H

#include "resource_manager.h"
#include "shader.h"

struct Vertex2D {
	glm::vec2 pos;
	glm::vec2 tex;
};

class hud {
public:
	hud(glm::mat4 model);
	void render(Shader *shader);
	void cleanUp();
	void cleanUpMenu();
	void drawCrosshair();
	void drawString(std::string string, float posx, float posy);
	void drawTextures();
	void removeHudPart(int p);
	int checkCollision(double xpos, double ypos);

private:
	float verticesPlane[16] = {
		1.0f,  1.0f, 1.0f, 1.0f,
		1.0f, 0.0f, 1.0f, 0.0f,
		0.0f, 0.0f, 0.0f, 0.0f,
		0.0f,  1.0f, 0.0f, 1.0f
	};
	unsigned int indicesPlane[6] = {
		0, 2, 1,
		0, 3, 2
	};
	std::vector<Vertex2D> hudVerts;
	std::vector<std::vector<Vertex2D>> hudParts;

	std::vector<Vertex2D> menuVerts;

	unsigned int VAO, VBO, menuVAO, menuVBO;// , EBO;
	glm::mat4 model;
	void genVAO();
	void genMenuVAO();

	float sizeOfTextureImage = 24;
	float sizeOfTexture = 8;
	float scaleTexture = 1.0f;

	float sizeOfFontImage = 128.0f;
	float sizeOfFontLetterX = 8.0f;
	float sizeOfFontLetterY = 16.0f;
	float sizeOfUI = 8.0f;
	float scale = 0.06f;
	float crossHairSize = 0.45f;

	void compileHud();
};

#endif