#include "collision.h"

float Collision::rayTriangleIntersect(const glm::vec3 &orig, const glm::vec3 &dir, const glm::vec3 &v0, const glm::vec3 &v1, const glm::vec3 &v2) {

	const float epsilon = 0.000001f;
	glm::vec3 vertex0 = v0;
	glm::vec3 vertex1 = v1;
	glm::vec3 vertex2 = v2;
	glm::vec3 edge1, edge2, h, s, q;

	float a, f, u, v;
	edge1 = vertex1 - vertex0;
	edge2 = vertex2 - vertex0;
	h = glm::cross(dir, edge2);
	a = glm::dot(edge1, h);
	if (a > -epsilon && a < epsilon) {
		return -1.0f;
	}
	f = 1.0f / a;
	s = orig - vertex0;
	u = f * glm::dot(s, h);
	if (u < 0.0f || u > 1.0f) {
		return -1.0f;
	}
	q = glm::cross(s, edge1);
	v = f * glm::dot(dir, q);
	if (v < 0.0f || u + v > 1.0f) {
		return -1.0f;
	}

	float t = f * glm::dot(edge2, q);
	if (t > epsilon) {
		return t;
	}
	else {
		return -1.0f;
	}
}

glm::vec3 Collision::CollisionSlide(CollisionPacket& cP, std::vector<Vertex3D>& vertPos) {

	cP.e_Velocity = cP.w_Velocity / cP.ellipsoidSpace;
	cP.e_Position = cP.w_Position / cP.ellipsoidSpace;
	cP.grounded = false;
	cP.collisionRecursionDepth = 0;
	glm::vec3 finalPosition = CollideWithWorld(cP, vertPos);

	cP.e_Velocity = glm::vec3(0.0f, -0.4f, 0.0f) / cP.ellipsoidSpace;
	cP.e_Position = finalPosition;
	cP.collisionRecursionDepth = 0;
	finalPosition = CollideWithWorld(cP, vertPos);

	if (cP.grounded) {
		//if (finalPosition.y - finalPosition2.y < 3 && finalPosition.y - finalPosition2.y > 0.1) {
			//finalPosition.y = finalPosition2.y; //* cP.ellipsoidSpace.y;
		//}
		//finalPosition = finalPosition * cP.ellipsoidSpace;
	}
	else {
		//finalPosition = finalPosition2 * cP.ellipsoidSpace;
	}

	finalPosition = finalPosition * cP.ellipsoidSpace;

	return finalPosition;
}

glm::vec3 Collision::CollideWithWorld(CollisionPacket& cP, std::vector<Vertex3D>& vertPos) {

	float unitScale = unitsPerMeter / 100.0f;
	float veryCloseDistance = 0.005f * unitScale;

	if (cP.collisionRecursionDepth > 5) {
		return cP.e_Position;
	}

	cP.e_normalizedVelocity = glm::normalize(cP.e_Velocity);

	cP.foundCollision = false;
	cP.nearestDistance = 0.0f;

	for (int triCounter = 0; triCounter < vertPos.size(); triCounter += 3) {

		glm::vec3 p0, p1, p2, tempVec;
		p0 = vertPos[triCounter].pos;
		p1 = vertPos[triCounter + 2].pos;
		p2 = vertPos[triCounter + 1].pos;

		p0 = p0 / cP.ellipsoidSpace;
		p1 = p1 / cP.ellipsoidSpace;
		p2 = p2 / cP.ellipsoidSpace;

		glm::vec3 triNormal;
		triNormal = glm::normalize(glm::cross((p1 - p0), (p2 - p0)));
		//triNormal *= glm::vec3(-1.0f);

		SphereCollidingWithTriangle(cP, p0, p1, p2, triNormal);
	}

	if (cP.foundCollision == false) {
		//player.onGround = false;
		return cP.e_Position + cP.e_Velocity;
	}

	//player.onGround = cP.grounded;
	//player.onGround = true;

	glm::vec3 destinationPoint = cP.e_Position + cP.e_Velocity;

	glm::vec3 newPosition = cP.e_Position;

	if (cP.nearestDistance >= veryCloseDistance) {

		glm::vec3 V = cP.e_Velocity;
		V = glm::normalize(V);
		V = V * (cP.nearestDistance - veryCloseDistance);
		newPosition = cP.e_Position + V;

		V = glm::normalize(V);
		cP.intersectionPoint -= veryCloseDistance * V;
	}

	glm::vec3 slidePlaneOrigin = cP.intersectionPoint;
	glm::vec3 slidePlaneNormal = newPosition - cP.intersectionPoint;
	slidePlaneNormal = glm::normalize(slidePlaneNormal);

	float x = slidePlaneOrigin.x;
	float y = slidePlaneOrigin.y;
	float z = slidePlaneOrigin.z;

	float A = slidePlaneNormal.x;
	float B = slidePlaneNormal.y;
	float C = slidePlaneNormal.z;
	float D = -((A*x) + (B*y) + (C*z));

	float planeConstant = D;

	float signedDistFromDestPointToSlidingPlane = glm::dot(destinationPoint, slidePlaneNormal) + planeConstant;

	glm::vec3 newDestinationPoint = destinationPoint - signedDistFromDestPointToSlidingPlane * slidePlaneNormal;

	glm::vec3 newVelocityVector = newDestinationPoint - cP.intersectionPoint;

	if (glm::length(newVelocityVector) < veryCloseDistance) {
		return newPosition;
	}

	cP.collisionRecursionDepth++;
	cP.e_Position = newPosition;
	cP.e_Velocity = newVelocityVector;
	return CollideWithWorld(cP, vertPos);
}

bool Collision::SphereCollidingWithTriangle(CollisionPacket& cP, glm::vec3 &p0, glm::vec3 &p1, glm::vec3 &p2, glm::vec3 &triNormal) {

	float facing = glm::dot(triNormal, cP.e_normalizedVelocity);
	if (facing <= 0) {

		glm::vec3 velocity = cP.e_Velocity;
		glm::vec3 position = cP.e_Position;

		float t0, t1;

		bool sphereInPlane = false;

		float x = p0.x;
		float y = p0.y;
		float z = p0.z;

		float A = triNormal.x;
		float B = triNormal.y;
		float C = triNormal.z;

		float D = -((A*x) + (B*y) + (C*z));

		float planeConstant = D;

		float signedDistFromPositionToTriPlane = glm::dot(position, triNormal) + planeConstant;

		float planeNormalDotVelocity = glm::dot(triNormal, velocity);

		if (planeNormalDotVelocity == 0.0f) {

			if (fabs(signedDistFromPositionToTriPlane) >= 1.0f) {

				return false;
			}
			else {

				sphereInPlane = true;
			}
		}
		else {

			t0 = (1.0f - signedDistFromPositionToTriPlane) / planeNormalDotVelocity;
			t1 = (-1.0f - signedDistFromPositionToTriPlane) / planeNormalDotVelocity;

			if (t0 > t1) {

				float temp = t0;
				t0 = t1;
				t1 = temp;
			}

			if (t0 > 1.0f || t1 < 0.0f) {

				return false;
			}

			if (t0 < 0.0f) {
				t0 = 0.0f;
			}
			if (t1 > 1.0f) {
				t1 = 1.0f;
			}
		}

		glm::vec3 collisionPoint;
		bool collidingWithTri = false;
		float t = 1.0f;

		if (!sphereInPlane) {

			glm::vec3 planeIntersectionPoint = (position + t0 * velocity - triNormal);

			if (checkPointInTriangle(planeIntersectionPoint, p0, p1, p2)) {

				collidingWithTri = true;
				t = t0;
				collisionPoint = planeIntersectionPoint;
			}
		}

		if (collidingWithTri == false) {

			float a, b, c;

			float velocityLengthSquared = glm::length(velocity);
			velocityLengthSquared *= velocityLengthSquared;

			a = velocityLengthSquared;

			float newT;

			b = 2.0f * glm::dot(velocity, position - p0);
			c = glm::length(p0 - position);
			c = (c*c) - 1.0f;
			if (getLowestRoot(a, b, c, t, &newT)) {
				t = newT;
				collidingWithTri = true;
				collisionPoint = p0;
			}

			b = 2.0f * glm::dot(velocity, position - p1);
			c = glm::length(p1 - position);
			c = (c*c) - 1.0f;
			if (getLowestRoot(a, b, c, t, &newT)) {
				t = newT;
				collidingWithTri = true;
				collisionPoint = p1;
			}

			b = 2.0f * glm::dot(velocity, position - p2);
			c = glm::length(p2 - position);
			c = (c*c) - 1.0f;
			if (getLowestRoot(a, b, c, t, &newT)) {
				t = newT;
				collidingWithTri = true;
				collisionPoint = p2;
			}

			glm::vec3 edge = p1 - p0;
			glm::vec3 spherePositionToVertex = p0 - position;
			float edgeLengthSquared = glm::length(edge);
			edgeLengthSquared *= edgeLengthSquared;
			float edgeDotVelocity = glm::dot(edge, velocity);
			float edgeDotSpherePositionToVertex = glm::dot(edge, spherePositionToVertex);
			float spherePositionToVertexLengthSquared = glm::length(spherePositionToVertex);
			spherePositionToVertexLengthSquared = spherePositionToVertexLengthSquared * spherePositionToVertexLengthSquared;

			a = edgeLengthSquared * -velocityLengthSquared + (edgeDotVelocity * edgeDotVelocity);
			b = edgeLengthSquared * (2.0f * glm::dot(velocity, spherePositionToVertex)) - (2.0f * edgeDotVelocity * edgeDotSpherePositionToVertex);
			c = edgeLengthSquared * (1.0f - spherePositionToVertexLengthSquared) + (edgeDotSpherePositionToVertex * edgeDotSpherePositionToVertex);

			if (getLowestRoot(a, b, c, t, &newT)) {

				float f = (edgeDotVelocity * newT - edgeDotSpherePositionToVertex) / edgeLengthSquared;
				if (f >= 0.0f && f <= 1.0f) {

					t = newT;
					collidingWithTri = true;
					collisionPoint = p0 + f * edge;
				}
			}

			edge = p2 - p1;
			spherePositionToVertex = p1 - position;
			edgeLengthSquared = glm::length(edge);
			edgeLengthSquared = edgeLengthSquared * edgeLengthSquared;
			edgeDotVelocity = glm::dot(edge, cP.e_Velocity);
			edgeDotSpherePositionToVertex = glm::dot(edge, spherePositionToVertex);
			spherePositionToVertexLengthSquared = glm::length(spherePositionToVertex);
			spherePositionToVertexLengthSquared = spherePositionToVertexLengthSquared * spherePositionToVertexLengthSquared;

			a = edgeLengthSquared * -velocityLengthSquared + (edgeDotVelocity * edgeDotVelocity);
			b = edgeLengthSquared * (2.0f * glm::dot(velocity, spherePositionToVertex)) - (2.0f * edgeDotVelocity * edgeDotSpherePositionToVertex);
			c = edgeLengthSquared * (1.0f - spherePositionToVertexLengthSquared) + (edgeDotSpherePositionToVertex * edgeDotSpherePositionToVertex);

			if (getLowestRoot(a, b, c, t, &newT)) {
				float f = (edgeDotVelocity * newT - edgeDotSpherePositionToVertex) / edgeLengthSquared;
				if (f >= 0.0f && f <= 1.0f) {
					t = newT;
					collidingWithTri = true;
					collisionPoint = p1 + f * edge;
				}
			}

			edge = p0 - p2;
			spherePositionToVertex = p2 - position;
			edgeLengthSquared = glm::length(edge);
			edgeLengthSquared = edgeLengthSquared * edgeLengthSquared;
			edgeDotVelocity = glm::dot(edge, cP.e_Velocity);
			edgeDotSpherePositionToVertex = glm::dot(edge, spherePositionToVertex);
			spherePositionToVertexLengthSquared = glm::length(spherePositionToVertex);
			spherePositionToVertexLengthSquared = spherePositionToVertexLengthSquared * spherePositionToVertexLengthSquared;

			a = edgeLengthSquared * -velocityLengthSquared + (edgeDotVelocity * edgeDotVelocity);
			b = edgeLengthSquared * (2.0f * glm::dot(velocity, spherePositionToVertex)) - (2.0f * edgeDotVelocity * edgeDotSpherePositionToVertex);
			c = edgeLengthSquared * (1.0f - spherePositionToVertexLengthSquared) + (edgeDotSpherePositionToVertex * edgeDotSpherePositionToVertex);

			if (getLowestRoot(a, b, c, t, &newT)) {
				float f = (edgeDotVelocity * newT - edgeDotSpherePositionToVertex) / edgeLengthSquared;
				if (f >= 0.0f && f <= 1.0f) {
					t = newT;
					collidingWithTri = true;
					collisionPoint = p2 + f * edge;
				}
			}
		}

		if (collidingWithTri == true) {

			float distToCollision = t * glm::length(velocity);

			if (cP.foundCollision == false || distToCollision < cP.nearestDistance) {

				//glm::vec3 dir = glm::cross(p1 - p0, p2 - p0);
				//glm::vec3 norm = glm::normalize(dir);

				float diff = 1 - abs(triNormal.y);
				if (diff < 0.02) {
					cP.grounded = true;
					//std::cout << diff << "\n";
				}
				else {
					cP.grounded = false;
				}

				cP.nearestDistance = distToCollision;
				cP.intersectionPoint = collisionPoint;

				cP.foundCollision = true;
				return true;
			}
		}
	}
	return false;
}

bool Collision::checkPointInTriangle(const glm::vec3& point, const glm::vec3& triV1, const glm::vec3& triV2, const glm::vec3& triV3) {

	glm::vec3 cp1 = glm::cross((triV3 - triV2), (point - triV2));
	glm::vec3 cp2 = glm::cross((triV3 - triV2), (triV1 - triV2));
	if (glm::dot(cp1, cp2) >= 0) {

		cp1 = glm::cross((triV3 - triV1), (point - triV1));
		cp2 = glm::cross((triV3 - triV1), (triV2 - triV1));
		if (glm::dot(cp1, cp2) >= 0) {

			cp1 = glm::cross((triV2 - triV1), (point - triV1));
			cp2 = glm::cross((triV2 - triV1), (triV3 - triV1));
			if (glm::dot(cp1, cp2) >= 0) {
				return true;
			}
		}
	}
	return false;
}

bool Collision::getLowestRoot(float a, float b, float c, float maxR, float* root) {

	float determinant = b*b - 4.0f*a*c;

	if (determinant < 0.0f) return false;

	float sqrtD = sqrt(determinant);
	float r1 = (-b - sqrtD) / (2 * a);
	float r2 = (-b + sqrtD) / (2 * a);

	if (r1 > r2) {
		float temp = r2;
		r2 = r1;
		r1 = temp;
	}

	if (r1 > 0.0f && r1 < maxR) {
		*root = r1;
		return true;
	}
	if (r2 > 0.0f && r2 < maxR) {
		*root = r2;
		return true;
	}

	return false;
}
