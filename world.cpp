#include "world.h"

void World::createWorld(int seed) {

	for (int x = 0; x < worldSize; x++) {
		for (int z = 0; z < worldSize; z++) {

			int seed = 123;

			PerlinNoise sn(seed);
			float bl = 12 * sn.noise(0.011 * x, 0.041 * z, 0.040 * 5);
			float br = 12 * sn.noise(0.011 * (x + 1), 0.041 * z, 0.040 * 5);
			float tr = 12 * sn.noise(0.011 * (x + 1), 0.041 * (z + 1), 0.040 * 5);
			float tl = 12 * sn.noise(0.011 * x, 0.041 * (z + 1), 0.040 * 5);

			int seedTwo = seed * 523423 + 2331 + 42131;

			PerlinNoise bn(seedTwo);
			float bl2 = bn.noise(0.61 * x, 0.61 * z, 0.10 * 5);
			float br2 = bn.noise(0.61 * (x + 1), 0.61 * z, 0.10 * 5);
			float tr2 = bn.noise(0.61 * (x + 1), 0.61 * (z + 1), 0.10 * 5);
			float tl2 = bn.noise(0.61 * x, 0.61 * (z + 1), 0.10 * 5);

			//bl += bl2;
			//br += br2;
			//tr += tr2;
			//tl += tl2;

			glm::vec3 vbl = glm::vec3(x, bl, z);
			glm::vec3 vbr = glm::vec3(x + 1.0f, br, z);
			glm::vec3 vtr = glm::vec3(x + 1.0f, tr, z + 1.0f);
			glm::vec3 vtl = glm::vec3(x, tl, z + 1.0f);

			glm::vec3 nbl = calcAvgNormal(seed, seedTwo, vbl);
			glm::vec3 nbr = calcAvgNormal(seed, seedTwo, vbr);
			glm::vec3 ntr = calcAvgNormal(seed, seedTwo, vtr);
			glm::vec3 ntl = calcAvgNormal(seed, seedTwo, vtl);

			/*glm::vec3 fNorm = glm::normalize(glm::cross(vtr - vbl, vbr - vbl));
			glm::vec3 sNorm = glm::normalize(glm::cross(vtl - vbl, vtr - vbl));
			glm::vec3 aNorm = glm::normalize(glm::vec3((fNorm.x + sNorm.x) / 2, (fNorm.y + sNorm.y) / 2, (fNorm.z + sNorm.z) / 2));
			*/

			float texX = (float)x / worldSize;
			float texZ = (float)z / worldSize;
			float texX1 = (float)(x + 1) / worldSize;
			float texZ1 = (float)(z + 1) / worldSize;

			Vertex3D bottomLeft = { vbl, nbl, glm::vec2(texX, texZ) };
			Vertex3D bottomRight = { vbr, nbr, glm::vec2(texX1, texZ) };
			Vertex3D topRight = { vtr, ntr, glm::vec2(texX1, texZ1) };
			Vertex3D topLeft = { vtl, ntl, glm::vec2(texX, texZ1) };

			verts.push_back(bottomLeft);
			verts.push_back(topRight);
			verts.push_back(bottomRight);

			verts.push_back(bottomLeft);
			verts.push_back(topLeft);
			verts.push_back(topRight);
		}
	}

	cleanUp();
	genVAO();
}

float World::heightAtPoint(glm::vec3 pos) {

	//int seed = 123;

	//PerlinNoise sn(seed);
	//float y = 12 * sn.noise(0.011 * pos.x, 0.041 * pos.z, 0.040 * 5);

	if (pos.x < worldSize && pos.z < worldSize && pos.x > 0 && pos.z > 0) {

		int ix = floor(pos.x);
		int iz = floor(pos.z);
		float disX = pos.x - ix;
		float disZ = pos.z - iz;

		glm::vec3 p1 = glm::vec3(0.0f), p2 = glm::vec3(1.0f), p3 = glm::vec3(2.0f);

		if (disX > disZ) {
			p1 = verts[(ix * worldSize + iz) * 6].pos;
			p2 = verts[((ix * worldSize + iz) * 6) + 1].pos;
			p3 = verts[((ix * worldSize + iz) * 6) + 2].pos;

		}
		else {
			p1 = verts[((ix * worldSize + iz) * 6) + 3].pos;
			p2 = verts[((ix * worldSize + iz) * 6) + 4].pos;
			p3 = verts[((ix * worldSize + iz) * 6) + 5].pos;
		}
		
		float det = (p2.z - p3.z) * (p1.x - p3.x) + (p3.x - p2.x) * (p1.z - p3.z);

		float l1 = ((p2.z - p3.z) * (pos.x - p3.x) + (p3.x - p2.x) * (pos.z - p3.z)) / det;

		float l2 = ((p3.z - p1.z) * (pos.x - p3.x) + (p1.x - p3.x) * (pos.z - p3.z)) / det;

		float l3 = 1.0f - l1 - l2;

		return l1 * p1.y + l2 * p2.y + l3 * p3.y;
	}
	return 0.0f;
}

void World::render(Shader *shader) {

	glBindTexture(GL_TEXTURE_2D, Resource_Manager::getTexture("stone"));
	glBindVertexArray(VAO);
	glm::mat4 model = glm::mat4(1.0f);
	shader->setMat4("model", model);
	glDrawArrays(GL_TRIANGLES, 0, verts.size());
}

void World::cleanUp() {

	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
}

void World::genVAO() {

	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex3D) * verts.size(), &verts[0], GL_STATIC_DRAW);

	// position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	// normal attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	// texture coord attribute
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);
}

glm::vec3 World::calcAvgNormal(int seed, int seedTwo, glm::vec3 pos) {

	PerlinNoise pn(seed);
	PerlinNoise bn(seedTwo);

	float y1 = 12 * pn.noise(0.011 * (pos.x + 1), 0.041 * pos.z, 0.040 * 5);
	//y1 += bn.noise(0.61 * (pos.x + 1), 0.61 * pos.z, 0.10 * 5);
	float y2 = 12 * pn.noise(0.011 * pos.x, 0.041 * (pos.z + 1), 0.040 * 5);
	//y2 += bn.noise(0.61 * pos.x, 0.61 * (pos.z + 1), 0.10 * 5);
	float y3 = 12 * pn.noise(0.011 * (pos.x - 1), 0.041 * pos.z, 0.040 * 5);
	//y3 += bn.noise(0.61 * (pos.x - 1), 0.61 * pos.z, 0.10 * 5);
	float y4 = 12 * pn.noise(0.011 * pos.x, 0.041 * (pos.z - 1), 0.040 * 5);
	//y4 += bn.noise(0.61 * pos.x, 0.61 * (pos.z - 1), 0.10 * 5);


	std::vector<glm::vec3> lPos = { pos, glm::vec3(pos.x + 1, y1, pos.z), glm::vec3(pos.x, y2, pos.z + 1), glm::vec3(pos.x - 1, y3, pos.z),
									glm::vec3(pos.x, y4, pos.z - 1) };

	glm::vec3 finalNormal = glm::vec3(0.0f);
	finalNormal += glm::normalize(glm::cross(lPos[2] - lPos[0], lPos[1] - lPos[0]));
	finalNormal += glm::normalize(glm::cross(lPos[1] - lPos[0], lPos[4] - lPos[0]));

	finalNormal += glm::normalize(glm::cross(lPos[3] - lPos[0], lPos[2] - lPos[0]));
	finalNormal += glm::normalize(glm::cross(lPos[4] - lPos[0], lPos[3] - lPos[0]));

	finalNormal.x /= 4;
	finalNormal.y /= 4;
	finalNormal.z /= 4;

	finalNormal = glm::normalize(finalNormal);

	return finalNormal;
}