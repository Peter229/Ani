#pragma once

#include "brush.h"


class levelCreator {

public:
	levelCreator();
	~levelCreator();
	void extendOct(glm::vec3 dir, glm::vec3 orig);
	void createBrush(glm::vec3 pos);
	void createBrush(glm::vec3 pos, glm::vec3 add);
	void optimize();
	void render(Shader *shader);
	void render(Shader *shader, unsigned int* depthMap);
	void renderBrushSelection(Shader *shader);
	void cleanUp();
	void findBrushes(glm::vec3 dir, glm::vec3 orig);
	void editBrush(int dir);
	void rotateBrush(int axis, int dir);
	glm::vec3 playerCollision(glm::vec3 boxCenter, glm::vec3 boxHalfSize, glm::vec3 vel);
	void findTri(const glm::vec3 &orig, const glm::vec3 &dir);
	void findVertex(const glm::vec3 &orig, const glm::vec3 &dir, int move);

private:
	unsigned int VAO, VBO;
	int selectedBrushIndex;
	//Brush* selectedBrush;
	std::vector<Vertex3D> verts;
	std::vector<Brush*> brushes;
	void genVAO();

};