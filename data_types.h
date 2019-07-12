#pragma once
#ifndef DATA_TYPES_H
#define DATA_TYPES_H

#include "glm/glm.hpp"

struct Vertex3D {
	glm::vec3 pos;
	glm::vec3 normal;
	glm::vec2 tex;
};

float rayPlaneIntersection(glm::vec3 dir, glm::vec3 orig, glm::vec3 center, glm::vec3 normal);

#endif