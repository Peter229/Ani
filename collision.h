#pragma once

#include "data_types.h"

#include <vector>

#include <iostream>

const float unitsPerMeter = 100.0f;

struct CollisionPacket {

	glm::vec3 ellipsoidSpace;
	glm::vec3 w_Position;
	glm::vec3 w_Velocity;

	glm::vec3 e_Position;
	glm::vec3 e_Velocity;
	glm::vec3 e_normalizedVelocity;

	bool foundCollision;
	float nearestDistance;
	glm::vec3 intersectionPoint;
	bool grounded;
	int collisionRecursionDepth;
};

class Collision {

public:
	static glm::vec3 CollisionSlide(CollisionPacket& cP, std::vector<Vertex3D>& vertPos);

	static float rayTriangleIntersect(const glm::vec3 &orig, const glm::vec3 &dir, const glm::vec3 &v0, const glm::vec3 &v1, const glm::vec3 &v2);

private:
	static glm::vec3 CollideWithWorld(CollisionPacket& cP, std::vector<Vertex3D>& vertPos);

	static bool SphereCollidingWithTriangle(CollisionPacket& cP, glm::vec3 &p0, glm::vec3 &p1, glm::vec3 &p2, glm::vec3 &triNormal);

	static bool checkPointInTriangle(const glm::vec3& point, const glm::vec3& triV1, const glm::vec3& triV2, const glm::vec3& triV3);

	static bool getLowestRoot(float a, float b, float c, float maxR, float* root);
};