#pragma once

#include <glad/glad.h>
#include "shader.h"
#include "resource_manager.h"
#include "data_types.h"
#include "collision.h"
#include <math.h>

class Brush {

public:
	Brush();
	Brush(glm::vec3 pos);
	Brush(glm::vec3 pos, glm::vec3 add);
	~Brush();
	
	void createBrush(glm::vec3 pos, glm::vec3 add);
	std::vector<Vertex3D>* getVerts();
	bool rayAABB(glm::vec3 orig, glm::vec3 dir);
	float rayAABBf(glm::vec3 orig, glm::vec3 dir);
	void moveBrush(int dir);
	void rotateBrush(int axis, int dir);
	void render(Shader *shader);
	void cleanUp();
	void genVAO();
	void triAABB(glm::vec3 boxCenter, glm::vec3 boxHalfSize, glm::vec3 vel, glm::vec3 *translation);
	bool findTri(const glm::vec3 &orig, const glm::vec3 &dir);
	bool findVertex(const glm::vec3 &orig, const glm::vec3 &dir, int move);

private:
	unsigned int VAO, VBO;
	std::vector<Vertex3D> verts;
	glm::vec3 minPos, maxPos;
	glm::vec3 pos, add;

	bool triBoxOverlap(glm::vec3 boxCenter, glm::vec3 boxHalfSize, int tri);
	bool planeBoxOverlap(glm::vec3 normal, glm::vec3 v0, glm::vec3 boxHalfSize);

	bool triangleAABB(glm::vec3 c, glm::vec3 e, int tri, glm::vec3 *translation);

	bool PlaneBoxCollision(float normal[], float vertex[], glm::vec3 maxBox);
	bool TriangleBoxCollision(glm::vec3 boxCenter, glm::vec3 boxHalfSize, int triangleVertices);

	bool TriangleBoxCollision(glm::vec3 c, glm::vec3 e, glm::vec3 vel, int tri, glm::vec3 *translation);
};