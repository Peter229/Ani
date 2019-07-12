#include "data_types.h"

float rayPlaneIntersection(glm::vec3 dir, glm::vec3 orig, glm::vec3 center, glm::vec3 normal) {
	float denom = glm::dot(normal, dir);
	if (abs(denom) > 0.0001f) {
		float t = glm::dot((center - orig), normal) / denom;
		if (t >= 0) return t;
	}
	return -9999.9f;
}