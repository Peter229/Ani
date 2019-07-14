#include "brush.h"

Brush::Brush() {
	this->pos = glm::vec3(0.0f);
	this->add = glm::vec3(1.0f);
	createBrush(glm::vec3(0.0f), glm::vec3(1.0f));
}

Brush::Brush(glm::vec3 pos) {
	this->pos = pos;
	this->add = glm::vec3(1.0f);
	createBrush(pos, glm::vec3(1.0f));
}

Brush::Brush(glm::vec3 pos, glm::vec3 add) {
	this->pos = pos;
	this->add = add;
	createBrush(pos, add);
}

Brush::~Brush() {
	
	cleanUp();
	verts.clear();
}

void Brush::createBrush(glm::vec3 pos, glm::vec3 add) {

	verts.clear();

	glm::vec3 verta = pos;
	glm::vec3 vertb = glm::vec3(pos.x, pos.y, pos.z + add.z);
	glm::vec3 vertc = glm::vec3(pos.x + add.x, pos.y, pos.z);
	glm::vec3 vertd = glm::vec3(pos.x + add.x, pos.y, pos.z + add.z);
	glm::vec3 verte = glm::vec3(pos.x, pos.y + add.y, pos.z);
	glm::vec3 vertf = glm::vec3(pos.x, pos.y + add.y, pos.z + add.z);
	glm::vec3 vertg = glm::vec3(pos.x + add.x, pos.y + add.y, pos.z);
	glm::vec3 verth = glm::vec3(pos.x + add.x, pos.y + add.y, pos.z + add.z);

	glm::vec3 norma = glm::vec3(1.0f, 0.0f, 0.0f);
	glm::vec3 normb = glm::vec3(0.0f, 1.0f, 0.0f);
	glm::vec3 normc = glm::vec3(0.0f, 0.0f, 1.0f);
	glm::vec3 normd = glm::vec3(-1.0f, 0.0f, 0.0f);
	glm::vec3 norme = glm::vec3(0.0f, -1.0f, 0.0f);
	glm::vec3 normf = glm::vec3(0.0f, 0.0f, -1.0f);


	glm::vec2 texa = glm::vec2(0.0f, 0.0f);
	glm::vec2 texb = glm::vec2(add.x, 0.0f);
	glm::vec2 texc = glm::vec2(0.0f, add.z);
	glm::vec2 texd = glm::vec2(add.x, add.z);
	glm::vec2 texe = glm::vec2(0.0f, add.y);
	glm::vec2 texf = glm::vec2(add.x, add.y);
	glm::vec2 texg = glm::vec2(add.z, add.y);
	glm::vec2 texh = glm::vec2(add.z, 0.0f);

	//Edge Bottom
	Vertex3D vert1 = { verta, norme, texa };
	Vertex3D vert2 = { vertb, norme, texc };
	Vertex3D vert3 = { vertd, norme, texd };
	Vertex3D vert4 = { verta, norme, texa };
	Vertex3D vert5 = { vertd, norme, texd };
	Vertex3D vert6 = { vertc, norme, texb };
	verts.push_back(vert1);
	verts.push_back(vert2);
	verts.push_back(vert3);
	verts.push_back(vert4);
	verts.push_back(vert5);
	verts.push_back(vert6);

	//Edge Top
	vert1 = { verte, normb, texb };
	vert2 = { verth, normb, texc };
	vert3 = { vertf, normb, texd };
	vert4 = { verte, normb, texb };
	vert5 = { vertg, normb, texa };
	vert6 = { verth, normb, texc };
	verts.push_back(vert1);
	verts.push_back(vert2);
	verts.push_back(vert3);
	verts.push_back(vert4);
	verts.push_back(vert5);
	verts.push_back(vert6);

	//Edge Right
	vert1 = { verta, normd, texa };
	vert2 = { verte, normd, texe };
	vert3 = { vertb, normd, texh };
	vert4 = { vertb, normd, texh };
	vert5 = { verte, normd, texe };
	vert6 = { vertf, normd, texg };
	verts.push_back(vert1);
	verts.push_back(vert2);
	verts.push_back(vert3);
	verts.push_back(vert4);
	verts.push_back(vert5);
	verts.push_back(vert6);

	//Edge Left
	vert1 = { vertc, norma, texh };
	vert2 = { vertd, norma, texa };
	vert3 = { vertg, norma, texg };
	vert4 = { vertd, norma, texa };
	vert5 = { verth, norma, texe };
	vert6 = { vertg, norma, texg };
	verts.push_back(vert1);
	verts.push_back(vert2);
	verts.push_back(vert3);
	verts.push_back(vert4);
	verts.push_back(vert5);
	verts.push_back(vert6);

	//Edge Front
	vert1 = { vertb, normc, texa };
	vert2 = { vertf, normc, texe };
	vert3 = { verth, normc, texf };
	vert4 = { vertb, normc, texa };
	vert5 = { verth, normc, texf };
	vert6 = { vertd, normc, texb };
	verts.push_back(vert1);
	verts.push_back(vert2);
	verts.push_back(vert3);
	verts.push_back(vert4);
	verts.push_back(vert5);
	verts.push_back(vert6);

	//Edge Back
	vert1 = { verta, normf, texb };
	vert2 = { vertg, normf, texe };
	vert3 = { verte, normf, texf };
	vert4 = { verta, normf, texb };
	vert5 = { vertc, normf, texa };
	vert6 = { vertg, normf, texe };
	verts.push_back(vert1);
	verts.push_back(vert2);
	verts.push_back(vert3);
	verts.push_back(vert4);
	verts.push_back(vert5);
	verts.push_back(vert6);

	minPos = pos;
	maxPos = pos + add;
}

std::vector<Vertex3D>* Brush::getVerts() {

	return &verts;
}

bool Brush::rayAABB(glm::vec3 orig, glm::vec3 dir) {

	glm::vec3 bounds[2];
	bounds[0] = minPos;
	bounds[1] = maxPos;

	glm::vec3 invdir = 1.0f / dir;
	int sign[3];
	sign[0] = (invdir.x < 0);
	sign[1] = (invdir.y < 0);
	sign[2] = (invdir.z < 0);

	float tmin, tmax, tymin, tymax, tzmin, tzmax;

	tmin = (bounds[sign[0]].x - orig.x) * invdir.x;
	tmax = (bounds[1 - sign[0]].x - orig.x) * invdir.x;
	tymin = (bounds[sign[1]].y - orig.y) * invdir.y;
	tymax = (bounds[1 - sign[1]].y - orig.y) * invdir.y;

	if ((tmin > tymax) || (tymin > tmax)) {

		return false;
	}
	if (tymin > tmin) {
		tmin = tymin;
	}
	if (tymax < tmax) {
		tmax = tymax;
	}

	tzmin = (bounds[sign[2]].z - orig.z) * invdir.z;
	tzmax = (bounds[1 - sign[2]].z - orig.z) * invdir.z;

	if ((tmin > tzmax) || (tzmin > tmax)) {
		return false;
	}
	if (tzmin > tmin) {
		tmin = tzmin;
	}
	if (tzmax < tmax) {
		tmax = tzmax;
	}

	if (tmin < 0.0f && tmax < 0.0f) {
		return false;
	}

	return true;
}

float Brush::rayAABBf(glm::vec3 orig, glm::vec3 dir) {

	glm::vec3 bounds[2];
	bounds[0] = minPos;
	bounds[1] = maxPos;

	glm::vec3 invdir = 1.0f / dir;
	int sign[3];
	sign[0] = (invdir.x < 0);
	sign[1] = (invdir.y < 0);
	sign[2] = (invdir.z < 0);

	float tmin, tmax, tymin, tymax, tzmin, tzmax;

	tmin = (bounds[sign[0]].x - orig.x) * invdir.x;
	tmax = (bounds[1 - sign[0]].x - orig.x) * invdir.x;
	tymin = (bounds[sign[1]].y - orig.y) * invdir.y;
	tymax = (bounds[1 - sign[1]].y - orig.y) * invdir.y;

	if ((tmin > tymax) || (tymin > tmax)) {

		return -9.0f;
	}
	if (tymin > tmin) {
		tmin = tymin;
	}
	if (tymax < tmax) {
		tmax = tymax;
	}

	tzmin = (bounds[sign[2]].z - orig.z) * invdir.z;
	tzmax = (bounds[1 - sign[2]].z - orig.z) * invdir.z;

	if ((tmin > tzmax) || (tzmin > tmax)) {
		return -9.0f;
	}
	if (tzmin > tmin) {
		tmin = tzmin;
	}
	if (tzmax < tmax) {
		tmax = tzmax;
	}

	if (tmin < 0.0f && tmax < 0.0f) {
		return -9.0f;
	}

	if (tmin < tmax) {
		return tmin;
	}

	return tmax;
}

void Brush::moveBrush(int dir) {

	glm::vec3 movePos = glm::vec3(0.0f);

	if (dir == 0) { //Up
		movePos = glm::vec3(1.0f, 0.0f, 0.0f);
	}
	else if (dir == 1) { //Down
		movePos = glm::vec3(-1.0f, 0.0f, 0.0f);
	}
	else if (dir == 2) { //Left
		movePos = glm::vec3(0.0f, 0.0f, -1.0f);
	}
	else if (dir == 3) { //Right
		movePos = glm::vec3(0.0f, 0.0f, 1.0f);
	}
	else if (dir == 4) { //VUp
		movePos = glm::vec3(0.0f, 1.0f, 0.0f);
	}
	else if (dir == 5){ //VDown
		movePos = glm::vec3(0.0f, -1.0f, 0.0f);
	}

	for (int i = 0; i < verts.size(); i++) {
		verts[i].pos += movePos;
	}

	pos += movePos;

	minPos = pos;
	maxPos = pos + add;

	cleanUp();
	genVAO();
}

void Brush::rotateBrush(int axis, int dir) {

	if (axis == 0) {

	}
}

void Brush::cleanUp() {

	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
}

void Brush::render(Shader* shader) {
	
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glLineWidth(4);
	glBindTexture(GL_TEXTURE_2D, Resource_Manager::getTexture("crosshair"));
	glBindVertexArray(VAO);
	glm::mat4 model = glm::mat4(1.0f);
	shader->setMat4("model", model);
	glDrawArrays(GL_TRIANGLES, 0, verts.size());
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}

void Brush::genVAO() {

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

bool Brush::findTri(const glm::vec3 &orig, const glm::vec3 &dir) {

	float distance = 1024.0f;
	int tri = -1;

	for (int i = 0; i < verts.size(); i += 3) {
		float t = Collision::rayTriangleIntersect(orig, dir, verts[i + 0].pos, verts[i + 1].pos, verts[i + 2].pos);
		if (t < distance && t != -1.0f) {
			distance = t;
			tri = i;
		}
	}

	if (distance == 1024.0f) {
		return false;
	}

	Vertex3D p1 = verts[tri + 0];
	Vertex3D p2 = verts[tri + 1]; 
	Vertex3D p3 = verts[tri + 2];

	glm::vec3 point = orig + (dir * distance);

	/*glm::vec3 f1 = p1.pos - point;
	glm::vec3 f2 = p2.pos - point;
	glm::vec3 f3 = p3.pos - point;

	float a = glm::length(glm::cross(p1.pos - p2.pos, p1.pos - p3.pos));
	float a1 = glm::length(glm::cross(f2, f3)) / a;
	float a2 = glm::length(glm::cross(f3, f1)) / a;
	float a3 = glm::length(glm::cross(f1, f2)) / a;
	*/

	glm::vec2 uv = calcUVs(p1, p2, p3, point);

	Vertex3D newVert = { point, p1.normal, uv };

	verts.erase(verts.begin() + tri, verts.begin() + tri + 3);
	
	verts.insert(verts.begin() + tri, p1);
	verts.insert(verts.begin() + tri, newVert);
	verts.insert(verts.begin() + tri, p2);

	verts.insert(verts.begin() + tri + 3, p1);
	verts.insert(verts.begin() + tri + 3, p3);
	verts.insert(verts.begin() + tri + 3, newVert);

	verts.insert(verts.begin() + tri + 6, p2);
	verts.insert(verts.begin() + tri + 6, newVert);
	verts.insert(verts.begin() + tri + 6, p3);
	
	cleanUp();
	genVAO();

	return true;
}

bool Brush::findVertex(const glm::vec3 &orig, const glm::vec3 &dir, int move) {

	float distance = 1024.0f;
	int tri = -1;

	for (int i = 0; i < verts.size(); i += 3) {
		float t = Collision::rayTriangleIntersect(orig, dir, verts[i + 0].pos, verts[i + 1].pos, verts[i + 2].pos);
		if (t < distance && t != -1.0f) {
			distance = t;
			tri = i;
		}
	}

	if (distance == 1024.0f) {
		return false;
	}

	glm::vec3 point = orig + (dir * distance);

	float closeDis = 0.4f;

	bool found = false;

	Vertex3D delVerts[3];

	for (int i = 0; i < verts.size(); i++) {
		if (fabs(point.x) - fabs(verts[i].pos.x) < closeDis && fabs(point.x) - fabs(verts[i].pos.x) > -closeDis) {
			if (fabs(point.y) - fabs(verts[i].pos.y) < closeDis && fabs(point.y) - fabs(verts[i].pos.y) > -closeDis) {
				if (fabs(point.z) - fabs(verts[i].pos.z) < closeDis && fabs(point.z) - fabs(verts[i].pos.z) > -closeDis) {
					found = true;
					if (move == 0) {
						verts[i].pos.y += 1.0f;
					}else if (move == 1) {
						verts[i].pos.y -= 1.0f;
					}
					else if (move == 2) {
						verts[i].pos.x += 1.0f;
					}
					else if (move == 3) {
						verts[i].pos.x -= 1.0f;
					}
					else if (move == 4) {
						verts[i].pos.z += 1.0f;
					}
					else if (move == 5) {
						verts[i].pos.z -= 1.0f;
					}
					else if (move == 66) {
						if (verts[tri].pos == verts[i].pos) {
							for (int h = 0; h < 3; h++) {
								if (delVerts[1].pos == verts[tri + 1].pos || delVerts[1].pos == verts[tri + 2].pos) {

								}
							}
							delVerts[1] = verts[tri + 1];
							delVerts[2] = verts[tri + 2];
						}else if(verts[tri + 1].pos == verts[i].pos) {
							delVerts[0] = verts[tri + 0];
							delVerts[2] = verts[tri + 2];
						}
						else if (verts[tri + 2].pos == verts[i].pos) {
							delVerts[0] = verts[tri + 0];
							delVerts[1] = verts[tri + 1];
						}
					}
				}
			}
		}
	}

	if (found) {
		calculateAvgNormals();
		cleanUp();
		genVAO();
		return true;
	}
	else {
		return false;
	}
}

void Brush::reCalculateNormals() {

	for (int i = 0; i < verts.size(); i += 3) {
		glm::vec3 norm = glm::normalize(glm::cross(verts[i + 2].pos - verts[i].pos, verts[i + 1].pos - verts[i].pos));
		verts[i].normal = norm;
		verts[i + 1].normal = norm;
		verts[i + 2].normal = norm;
	}
}

void Brush::calculateAvgNormals() {

	reCalculateNormals();

	std::vector<Vertex3D> tempVerts;
	std::vector<int> vertID;
	int numVerts;

	bool found = false;

	for (int i = 0; i < verts.size(); i++) {
		found = false;
		for (int j = 0; j < tempVerts.size(); j++) {
			if (verts[i].pos == tempVerts[j].pos) {
				found = true;
			}
		}
		if (!found) {
			tempVerts.push_back(verts[i]);
		}
	}

	for (int j = 0; j < tempVerts.size(); j++) {
		numVerts = 0;
		vertID.clear();
		for (int i = 0; i < verts.size(); i++) {
			if (tempVerts[j].pos == verts[i].pos) {
				if (glm::dot(tempVerts[j].normal, verts[i].normal) > 0.5) {
					vertID.push_back(i);
					numVerts++;
				}
			}
		}

		glm::vec3 avgNormal = glm::vec3(0.0f, 0.0f, 0.0f);

		for (int y = 0; y < vertID.size(); y++) {

			avgNormal += verts[vertID[y]].normal;
		}

		avgNormal = glm::normalize(avgNormal / ((float)numVerts));

		for (int z = 0; z < vertID.size(); z++) {

			verts[vertID[z]].normal = avgNormal;
		}
	}
}

glm::vec2 Brush::calcUVs(Vertex3D p0, Vertex3D p1, Vertex3D p2, glm::vec3 point) {

	glm::vec3 f1 = p0.pos - point;
	glm::vec3 f2 = p1.pos - point;
	glm::vec3 f3 = p2.pos - point;

	float a = glm::length(glm::cross(p0.pos - p1.pos, p0.pos - p2.pos));
	float a1 = glm::length(glm::cross(f2, f3)) / a;
	float a2 = glm::length(glm::cross(f3, f1)) / a;
	float a3 = glm::length(glm::cross(f1, f2)) / a;

	glm::vec2 uv = p0.tex * a1 + p1.tex * a2 + p2.tex * a3;

	return uv;
}

void Brush::triAABB(glm::vec3 boxCenter, glm::vec3 boxHalfSize, glm::vec3 vel, glm::vec3 *translation) {

	glm::vec3 transVec;

	for (int i = 0; i < verts.size(); i += 3) {
		//std::cout << verts[i].pos.x << " " << verts[i].pos.y << " " << verts[i].pos.z << "\n";
		//std::cout << verts[i+1].pos.x << " " << verts[i + 1].pos.y << " " << verts[i + 1].pos.z << "\n";
		//std::cout << verts[i + 2].pos.x << " " << verts[i + 2].pos.y << " " << verts[i + 2].pos.z << "\n";
		//if (triBoxOverlap(boxCenter, boxHalfSize, i)) {
			//std::cout << "COLL\n";
		//}
		//if (triangleAABB(boxCenter, boxHalfSize, i, translation)) {
			//std::cout << "COLL\n";
		//}
		//if (TriangleBoxCollision(boxCenter, boxHalfSize, i)) {
			//std::cout << "COLL\n";
		//}
		if (TriangleBoxCollision(boxCenter, boxHalfSize, vel, i, translation)) {
			//translation->x = transVec.x;
			//translation->y = transVec.y;
			//translation->z = transVec.z;
			//std::cout << "COLL\n";
			//boxCenter += *translation;
		}
	}
}

bool Brush::triBoxOverlap(glm::vec3 boxCenter, glm::vec3 boxHalfSize, int tri) {

	glm::vec3 v0, v1, v2;

	float min, max, p0, p1, p2, rad, fex, fey, fez;

	glm::vec3 normal, e0, e1, e2;

	boxCenter.x = boxCenter.x;
	boxCenter.y = boxCenter.y;
	boxCenter.z = boxCenter.z;

	v0 = verts[tri + 0].pos - boxCenter;
	v1 = verts[tri + 1].pos - boxCenter;
	v2 = verts[tri + 2].pos - boxCenter;

	e0 = v1 - v0; //Tri Edges
	e1 = v2 - v1;
	e2 = v0 - v2;

	fex = fabsf(e0.x);
	fey = fabsf(e0.y);
	fez = fabsf(e0.z);

	//Axis Test 1
	p0 = e0.z * v0.y - e0.y * v0.z;
	p2 = e0.z * v2.y - e0.y * v2.z;
	if (p0 < p2) {
		min = p0;
		max = p2;
	}
	else {
		min = p2;
		max = p0;
	}
	rad = fez * boxHalfSize.y + fey * boxHalfSize.z;

	if (min > rad || max < -rad) {
		return false;
	}
	
	//Axis Test 2
	p0 = -e0.z * v0.x + e0.x * v0.z;
	p2 = -e0.z * v2.x + e0.x * v2.z;
	if (p0 < p2) {
		min = p0;
		max = p2;
	}
	else {
		min = p2;
		max = p0;
	}
	rad = fez * boxHalfSize.x + fex * boxHalfSize.z;

	if (min > rad || max < -rad) {
		return false;
	}

	//Axis Test 3
	p1 = e0.y * v1.x - e0.x * v1.y;
	p2 = e0.y * v2.x - e0.x * v2.y;
	if (p2 < p1) {
		min = p2;
		max = p1;
	}
	else {
		min = p1;
		max = p2;
	}
	rad = fey * boxHalfSize.x + fex * boxHalfSize.y;

	if (min > rad || max < -rad) {
		return false;
	}

	fex = fabsf(e1.x);
	fey = fabsf(e1.y);
	fez = fabsf(e1.z);

	//Axis Test 1
	p0 = e1.z * v0.y - e1.y * v0.z;
	p2 = e1.z * v2.y - e1.y * v2.z;
	if (p0 < p2) {
		min = p0;
		max = p2;
	}
	else {
		min = p2;
		max = p0;
	}
	rad = fez * boxHalfSize.y + fey * boxHalfSize.z;

	if (min > rad || max < -rad) {
		return false;
	}

	//Axis Test 2
	p0 = -e1.z * v0.x + e1.x * v0.z;
	p2 = -e1.z * v2.x + e1.x * v2.z;
	if (p0 < p2) {
		min = p0;
		max = p2;
	}
	else {
		min = p2;
		max = p0;
	}
	rad = fez * boxHalfSize.x + fex * boxHalfSize.z;

	if (min > rad || max < -rad) {
		return false;
	}

	//Axis Test 4
	p0 = e1.y * v0.x - e1.x * v0.y;
	p1 = e1.y * v1.x - e1.x * v1.y;
	if (p0 < p1) {
		min = p0;
		max = p1;
	}
	else {
		min = p1;
		max = p0;
	}
	rad = fey * boxHalfSize.x + fex * boxHalfSize.y;

	if (min > rad || max < -rad) {
		return false;
	}

	fex = fabsf(e2.x);
	fey = fabsf(e2.y);
	fez = fabsf(e2.z);

	//Axis Test 5
	p0 = e2.z * v0.y - e2.y * v0.z;
	p1 = e2.z * v1.y - e2.y * v1.z;
	if (p0 < p1) {
		min = p0;
		max = p1;
	}
	else {
		min = p1;
		max = p0;
	}
	rad = fez * boxHalfSize.y + fey * boxHalfSize.z;

	if (min > rad || max < -rad) {
		return false;
	}

	//Axis Test 6
	p0 = -e2.z * v0.x + e2.x * v0.z;
	p1 = -e2.z * v1.x + e2.x * v1.z;
	if (p0 < p1) {
		min = p0;
		max = p1;
	}
	else {
		min = p1;
		max = p0;
	}
	rad = fez * boxHalfSize.x + fex * boxHalfSize.z;

	if (min > rad || max < -rad) {
		return false;
	}

	//Axis Test 3
	p1 = e2.y * v1.x - e2.x * v1.y;
	p2 = e2.y * v2.x - e2.x * v2.y;
	if (p2 < p1) {
		min = p2;
		max = p1;
	}
	else {
		min = p1;
		max = p2;
	}
	rad = fey * boxHalfSize.x + fex * boxHalfSize.y;

	if (min > rad || max < -rad) {
		return false;
	}

	min = max = v0.x;
	if (v1.x < min) { min = v1.x; }
	if (v1.x > max) { max = v1.x; }
	if (v2.x < min) { min = v2.x; }
	if (v2.x > min) { max = v2.x; }
	if (min > boxHalfSize.x || max < -boxHalfSize.x) { return false; }

	min = max = v0.y;
	if (v1.y < min) { min = v1.y; }
	if (v1.y > max) { max = v1.y; }
	if (v2.y < min) { min = v2.y; }
	if (v2.y > min) { max = v2.y; }
	if (min > boxHalfSize.y || max < -boxHalfSize.y) { return false; }

	min = max = v0.z;
	if (v1.z < min) { min = v1.z; }
	if (v1.z > max) { max = v1.z; }
	if (v2.z < min) { min = v2.z; }
	if (v2.z > min) { max = v2.z; }
	if (min > boxHalfSize.z || max < -boxHalfSize.z) { return false; }

	normal = glm::cross(e0, e1);

	if (!planeBoxOverlap(normal, v0, boxHalfSize)) { return false; }

	return true;
}

bool Brush::planeBoxOverlap(glm::vec3 normal, glm::vec3 v0, glm::vec3 boxHalfSize) {

	int q;

	float vmin[3], vmax[3], vert[3], normalA[3], maxBox[3];
	float v, n, b;

	vert[0] = v0.x;
	vert[1] = v0.y;
	vert[2] = v0.z;

	normalA[0] = normal.x;
	normalA[1] = normal.y;
	normalA[2] = normal.z;

	maxBox[0] = boxHalfSize.x;
	maxBox[1] = boxHalfSize.y;
	maxBox[2] = boxHalfSize.z;

	for (q = 0; q <= 2; q++) {

		v = vert[q];

		if (normalA[q] > 0.0f) {

			vmin[q] = -maxBox[q] - v;
			vmax[q] = maxBox[q] - v;
		}
		else {

			vmin[q] = maxBox[q] - v;
			vmax[q] = -maxBox[q] - v;
		}
	}

	glm::vec3 gmin, gmax;
	gmin.x = vmin[0];
	gmin.y = vmin[1];
	gmin.z = vmin[2];

	gmax.x = vmax[0];
	gmax.y = vmax[1];
	gmax.z = vmax[2];

	if (glm::dot(normal, gmin) > 0.0f) { return false; }
	if (glm::dot(normal, gmin) >= 0.0f) { return true; }

	return false;
}

bool Brush::triangleAABB(glm::vec3 c, glm::vec3 e, int tri, glm::vec3 *translation){

	float distance = 99999.9f;
	glm::vec3 axis;

	glm::vec3 v0 = verts[tri + 0].pos - c;
	glm::vec3 v1 = verts[tri + 1].pos - c;
	glm::vec3 v2 = verts[tri + 2].pos - c;

	glm::vec3 f0 = v1 - v0;
	glm::vec3 f1 = v2 - v1;
	glm::vec3 f2 = v0 - v2;

	float a00l = sqrt(f0.z * f0.z + f0.y + f0.y);
	float a00y = -f0.z / a00l;
	float a00z = f0.y / a00l;
	float a00r = e.y * abs(a00y) + e.z * abs(a00z);
	float a00p0 = v0.y * a00y + v0.z * a00z;
	float a00p1 = v1.y * a00y + v1.z * a00z;
	float a00p2 = v2.y * a00y + v2.z * a00z;
	float a00min = a00p0;
	if (a00min > a00p1) {
		a00min = a00p1;
	}
	if (a00min > a00p2) {
		a00min = a00p2;
	}
	float a00max = a00p0;
	if (a00min < a00p1) {
		a00min = a00p1;
	}
	if (a00min < a00p2) {
		a00min = a00p2;
	}
	if (a00min > a00r) return false;
	if (a00max < -a00r) {
		return false;
	}
	else {
		if (a00min < -a00r) {
			distance = -(a00max + a00r);
		}
		else {
			distance = a00r - a00min;
		}
		axis.x = 0.0f;
		axis.y = a00y;
		axis.z = a00z;
	}

	float a01l = sqrt(f1.z * f1.z + f1.y + f1.y);
	float a01y = -f1.z / a01l;
	float a01z = f1.y / a01l;
	float a01r = e.y * abs(a01y) + e.z * abs(a01z);
	float a01p0 = v0.y * a01y + v0.z * a01z;
	float a01p1 = v1.y * a01y + v1.z * a01z;
	float a01p2 = v2.y * a01y + v2.z * a01z;
	float a01min = a01p0;
	if (a01min > a01p1) {
		a01min = a01p1;
	}
	if (a01min > a01p2) {
		a01min = a01p2;
	}
	float a01max = a01p0;
	if (a01min < a01p1) {
		a01min = a01p1;
	}
	if (a01min < a01p2) {
		a01min = a01p2;
	}
	if (a01min > a01r) return false;
	if (a01max < -a01r) {
		return false;
	}
	else {
		float newDistance;
		if (a01min < -a01r) {
			newDistance = -(a01max + a01r);
		}
		else {
			newDistance = a01r - a01min;
		}
		if (abs(newDistance) < abs(distance)) {
			distance = newDistance;
			axis.x = 0.0f;
			axis.y = a01y;
			axis.z = a01z;
		}
	}

	float a02l = sqrt(f2.z * f2.z + f2.y + f2.y);
	float a02y = -f2.z / a02l;
	float a02z = f2.y / a02l;
	float a02r = e.y * abs(a02y) + e.z * abs(a02z);
	float a02p0 = v0.y * a02y + v0.z * a02z;
	float a02p1 = v1.y * a02y + v1.z * a02z;
	float a02p2 = v2.y * a02y + v2.z * a02z;
	float a02min = a02p0;
	if (a02min > a02p1) {
		a02min = a02p1;
	}
	if (a02min > a02p2) {
		a02min = a02p2;
	}
	float a02max = a02p0;
	if (a02min < a02p1) {
		a02min = a02p1;
	}
	if (a02min < a02p2) {
		a02min = a02p2;
	}
	if (a02min > a02r) return false;
	if (a02max < -a02r) {
		return false;
	}
	else {
		float newDistance;
		if (a01min < -a02r) {
			newDistance = -(a02max + a02r);
		}
		else {
			newDistance = a02r - a02min;
		}
		if (abs(newDistance) < abs(distance)) {
			distance = newDistance;
			axis.x = 0.0f;
			axis.y = a02y;
			axis.z = a02z;
		}
	}

	float a10l = sqrt(f0.z * f0.z + f0.x + f0.x);
	float a10x = f0.z / a10l;
	float a10z = -f0.x / a10l;
	float a10r = e.x * abs(a10x) + e.z * abs(a10z);
	float a10p0 = v0.x * a10x + v0.z * a10z;
	float a10p1 = v1.x * a10x + v1.z * a10z;
	float a10p2 = v2.x * a10x + v2.z * a10z;
	float a10min = a10p0;
	if (a10min > a10p1) {
		a10min = a10p1;
	}
	if (a10min > a10p2) {
		a10min = a10p2;
	}
	float a10max = a10p0;
	if (a10min < a10p1) {
		a10min = a10p1;
	}
	if (a10min < a10p2) {
		a10min = a10p2;
	}
	if (a10min > a10r) return false;
	if (a10max < -a10r) {
		return false;
	}
	else {
		float newDistance;
		if (a10min < -a10r) {
			newDistance = -(a10max + a10r);
		}
		else {
			newDistance = a10r - a10min;
		}
		if (abs(newDistance) < abs(distance)) {
			distance = newDistance;
			axis.x = a10x;
			axis.y = 0.0f;
			axis.z = a10z;
		}
	}

	float a11l = sqrt(f1.z * f1.z + f1.x + f1.x);
	float a11x = f1.z / a11l;
	float a11z = -f1.x / a11l;
	float a11r = e.x * abs(a11x) + e.z * abs(a11z);
	float a11p0 = v0.x * a11x + v0.z * a11z;
	float a11p1 = v1.x * a11x + v1.z * a11z;
	float a11p2 = v2.x * a11x + v2.z * a11z;
	float a11min = a11p0;
	if (a11min > a11p1) {
		a11min = a11p1;
	}
	if (a11min > a11p2) {
		a11min = a11p2;
	}
	float a11max = a11p0;
	if (a11min < a11p1) {
		a11min = a11p1;
	}
	if (a11min < a11p2) {
		a11min = a11p2;
	}
	if (a11min > a11r) return false;
	if (a11max < -a11r) {
		return false;
	}
	else {
		float newDistance;
		if (a11min < -a11r) {
			newDistance = -(a11max + a11r);
		}
		else {
			newDistance = a11r - a11min;
		}
		if (abs(newDistance) < abs(distance)) {
			distance = newDistance;
			axis.x = a11x;
			axis.y = 0.0f;
			axis.z = a11z;
		}
	}

	float a12l = sqrt(f2.z * f2.z + f2.x + f2.x);
	float a12x = f2.z / a12l;
	float a12z = -f2.x / a12l;
	float a12r = e.x * abs(a12x) + e.z * abs(a12z);
	float a12p0 = v0.x * a12x + v0.z * a12z;
	float a12p1 = v1.x * a12x + v1.z * a12z;
	float a12p2 = v2.x * a12x + v2.z * a12z;
	float a12min = a12p0;
	if (a12min > a12p1) {
		a12min = a12p1;
	}
	if (a12min > a12p2) {
		a12min = a12p2;
	}
	float a12max = a12p0;
	if (a12min < a12p1) {
		a12min = a12p1;
	}
	if (a12min < a12p2) {
		a12min = a12p2;
	}
	if (a12min > a12r) return false;
	if (a12max < -a12r) {
		return false;
	}
	else {
		float newDistance;
		if (a12min < -a12r) {
			newDistance = -(a12max + a12r);
		}
		else {
			newDistance = a12r - a12min;
		}
		if (abs(newDistance) < abs(distance)) {
			distance = newDistance;
			axis.x = a12x;
			axis.y = 0.0f;
			axis.z = a12z;
		}
	}

	float a20l = sqrt(f0.y * f0.y + f0.x + f0.x);
	float a20x = -f0.y / a20l;
	float a20y = f0.x / a20l;
	float a20r = e.x * abs(a20x) + e.y * abs(a20y);
	float a20p0 = v0.x * a20x + v0.y * a20y;
	float a20p1 = v1.x * a20x + v1.y * a20y;
	float a20p2 = v2.x * a20x + v2.y * a20y;
	float a20min = a20p0;
	if (a20min > a20p1) {
		a20min = a20p1;
	}
	if (a20min > a20p2) {
		a20min = a20p2;
	}
	float a20max = a20p0;
	if (a20min < a20p1) {
		a20min = a20p1;
	}
	if (a20min < a20p2) {
		a20min = a20p2;
	}
	if (a20min > a20r) return false;
	if (a20max < -a20r) {
		return false;
	}
	else {
		float newDistance;
		if (a20min < -a20r) {
			newDistance = -(a20max + a20r);
		}
		else {
			newDistance = a20r - a20min;
		}
		if (abs(newDistance) < abs(distance)) {
			distance = newDistance;
			axis.x = a20x;
			axis.y = a20y;
			axis.z = 0.0f;
		}
	}

	float a21l = sqrt(f1.y * f1.y + f1.x + f1.x);
	float a21x = -f1.y / a21l;
	float a21y = f1.x / a21l;
	float a21r = e.x * abs(a21x) + e.y * abs(a21y);
	float a21p0 = v0.x * a21x + v0.y * a21y;
	float a21p1 = v1.x * a21x + v1.y * a21y;
	float a21p2 = v2.x * a21x + v2.y * a21y;
	float a21min = a21p0;
	if (a21min > a21p1) {
		a21min = a21p1;
	}
	if (a21min > a21p2) {
		a21min = a21p2;
	}
	float a21max = a21p0;
	if (a21min < a21p1) {
		a21min = a21p1;
	}
	if (a21min < a21p2) {
		a21min = a21p2;
	}
	if (a21min > a21r) return false;
	if (a21max < -a21r) {
		return false;
	}
	else {
		float newDistance;
		if (a21min < -a21r) {
			newDistance = -(a21max + a21r);
		}
		else {
			newDistance = a21r - a21min;
		}
		if (abs(newDistance) < abs(distance)) {
			distance = newDistance;
			axis.x = a21x;
			axis.y = a21y;
			axis.z = 0.0f;
		}
	}

	float a22l = sqrt(f2.y * f2.y + f2.x + f2.x);
	float a22x = -f2.y / a22l;
	float a22y = f2.x / a22l;
	float a22r = e.x * abs(a22x) + e.y * abs(a22y);
	float a22p0 = v0.x * a22x + v0.y * a22y;
	float a22p1 = v1.x * a22x + v1.y * a22y;
	float a22p2 = v2.x * a22x + v2.y * a22y;
	float a22min = a22p0;
	if (a22min > a22p1) {
		a22min = a22p1;
	}
	if (a22min > a22p2) {
		a22min = a22p2;
	}
	float a22max = a21p0;
	if (a22min < a22p1) {
		a22min = a22p1;
	}
	if (a22min < a22p2) {
		a22min = a22p2;
	}
	if (a22min > a22r) return false;
	if (a22max < -a22r) {
		return false;
	}
	else {
		float newDistance;
		if (a22min < -a22r) {
			newDistance = -(a22max + a22r);
		}
		else {
			newDistance = a22r - a22min;
		}
		if (abs(newDistance) < abs(distance)) {
			distance = newDistance;
			axis.x = a22x;
			axis.y = a22y;
			axis.z = 0.0f;
		}
	}

	float b0max = v0.x;
	if (b0max < v1.x) {
		b0max = v1.x;
	}
	if (b0max < v2.x) {
		b0max = v2.x;
	}
	if (b0max < -e.x) {
		return false;
	}
	else {
		float newDistance = -(e.x + b0max);
		if (abs(newDistance) < abs(distance)) {

			distance = newDistance;
			axis.x = 1.0f;
			axis.y = 0.0f;
			axis.z = 0.0f;
		}
	}

	float b0min = v0.x;
	if (b0min > v1.x) {
		b0min = v1.x;
	}
	if (b0min > v2.x) {
		b0min = v2.x;
	}
	if (b0min > e.x) {
		return false;
	}
	else {
		float newDistance = b0min - e.x;
		if (abs(newDistance) < abs(distance)) {

			distance = newDistance;
			axis.x = -1.0f;
			axis.y = 0.0f;
			axis.z = 0.0f;
		}
	}

	float b1max = v0.y;
	if (b1max < v1.y) {
		b1max = v1.y;
	}
	if (b1max < v2.y) {
		b1max = v2.y;
	}
	if (b1max < -e.y) {
		return false;
	}
	else {
		float newDistance = -(e.y + b1max);
		if (abs(newDistance) < abs(distance)) {

			distance = newDistance;
			axis.x = 0.0f;
			axis.y = 1.0f;
			axis.z = 0.0f;
		}
	}

	float b1min = v0.y;
	if (b1min > v1.y) {
		b1min = v1.y;
	}
	if (b1min > v2.y) {
		b1min = v2.y;
	}
	if (b1min > e.y) {
		return false;
	}
	else {
		float newDistance = b1min - e.y;
		if (abs(newDistance) < abs(distance)) {

			distance = newDistance;
			axis.x = 0.0f;
			axis.y = -1.0f;
			axis.z = 0.0f;
		}
	}

	float b2max = v0.z;
	if (b2max < v1.z) {
		b2max = v1.z;
	}
	if (b2max < v2.z) {
		b2max = v2.z;
	}
	if (b2max < -e.z) {
		return false;
	}
	else {
		float newDistance = -(e.z + b2max);
		if (abs(newDistance) < abs(distance)) {

			distance = newDistance;
			axis.x = 0.0f;
			axis.y = 0.0f;
			axis.z = 1.0f;
		}
	}

	float b2min = v0.z;
	if (b2min > v1.z) {
		b2min = v1.z;
	}
	if (b2min > v2.z) {
		b2min = v2.z;
	}
	if (b2min > e.z) {
		return false;
	}
	else {
		float newDistance = b2min - e.z;
		if (abs(newDistance) < abs(distance)) {

			distance = newDistance;
			axis.x = 0.0f;
			axis.y = 0.0f;
			axis.z = -1.0f;
		}
	}

	glm::vec3 pn = glm::vec3(-f0.z * f1.y + f0.y * f1.z, f0.z * f1.x - f0.x * f1.z, -f0.y * f1.x + f0.x * f1.y);
	float pnl = sqrt(pn.x * pn.x + pn.y * pn.y + pn.z * pn.z);
	pn.x /= pnl;
	pn.y /= pnl;
	pn.z /= pnl;
	float pd = v0.x * pn.x + v0.y * pn.y + v0.z * pn.z;
	float pr = e.x * abs(pn.x) + e.y * abs(pn.y) + e.z * abs(pn.z);
	if (abs(pd) > pr) {
		return false;
	}
	else {
		float newDistance = -pr - pd;
		if (abs(newDistance) < abs(distance)) {

			distance = newDistance;
			axis = pn;
		}
	}
	distance *= -1;
	translation->x = axis.x * distance;
	translation->y = axis.y * distance;
	translation->z = axis.z * distance;

	std::cout << translation->x << " " << translation->y << " " << translation->z << "\n";

	return true;
}

bool Brush::PlaneBoxCollision(float normal[], float vertex[], glm::vec3 maxBox)
{
	int q;
	float vmin[3], vmax[3], v;
	for (q = 0; q <= 2; q++)
	{
		v = vertex[q];
		if (normal[q] > 0)
		{
			vmin[q] = -maxBox[q] - v;
			vmax[q] = maxBox[q] - v;
		}
		else
		{
			vmin[q] = maxBox[q] - v;
			vmax[q] = -maxBox[q] - v;
		}
	}
	if ((normal[0] * vmin[0]) + (normal[1] * vmin[1]) + (normal[2] * vmin[2]) > 0) return 0;
	if ((normal[0] * vmax[0]) + (normal[1] * vmax[1]) + (normal[2] * vmax[2]) >= 0) return 1;
	return 0;
}

bool Brush::TriangleBoxCollision(glm::vec3 boxCenter, glm::vec3 boxHalfSize, int triangleVertices)
{
	float v0[3], v1[3], v2[3];
	float min, max, p0, p1, p2, rad, fex, fey, fez;
	float normal[3], e0[3], e1[3], e2[3];
	v0[0] = verts[triangleVertices + 0].pos.x - boxCenter[0];
	v0[1] = verts[triangleVertices + 0].pos.y - boxCenter[1];
	v0[2] = verts[triangleVertices + 0].pos.z - boxCenter[2];
	v1[0] = verts[triangleVertices + 1].pos.x - boxCenter[0];
	v1[1] = verts[triangleVertices + 1].pos.y - boxCenter[1];
	v1[2] = verts[triangleVertices + 1].pos.z - boxCenter[2];
	v2[0] = verts[triangleVertices + 2].pos.x - boxCenter[0];
	v2[1] = verts[triangleVertices + 2].pos.y - boxCenter[1];
	v2[2] = verts[triangleVertices + 2].pos.z - boxCenter[2];
	e0[0] = v1[0] - v0[0];
	e0[1] = v1[1] - v0[1];
	e0[2] = v1[2] - v0[2];
	e1[0] = v2[0] - v1[0];
	e1[1] = v2[1] - v1[1];
	e1[2] = v2[2] - v1[2];
	e2[0] = v0[0] - v2[0];
	e2[1] = v0[1] - v2[1];
	e2[2] = v0[2] - v2[2];
	fex = abs(e0[0]);
	fey = abs(e0[1]);
	fez = abs(e0[2]);
	p0 = e0[2] * v0[1] - e0[1] * v0[2];
	p2 = e0[2] * v2[1] - e0[1] * v2[2];
	if (p0 < p2)
	{
		min = p0;
		max = p2;
	}
	else
	{
		min = p2;
		max = p0;
	}
	rad = fez * boxHalfSize[1] + fey * boxHalfSize[2];
	if ((min > rad) || (max < -rad)) return false;
	p0 = -e0[2] * v0[0] + e0[0] * v0[2];
	p2 = -e0[2] * v2[0] + e0[0] * v2[2];
	if (p0 < p2)
	{
		min = p0;
		max = p2;
	}
	else
	{
		min = p2;
		max = p0;
	}
	rad = fez * boxHalfSize[0] + fex * boxHalfSize[2];
	if ((min > rad) || (max < -rad)) return false;
	p1 = e0[1] * v1[0] - e0[0] * v1[1];
	p2 = e0[1] * v2[0] - e0[0] * v2[1];
	if (p2 < p1)
	{
		min = p2;
		max = p1;
	}
	else
	{
		min = p1;
		max = p2;
	}
	rad = fey * boxHalfSize[0] + fex * boxHalfSize[1];
	if ((min > rad) || (max < -rad)) return false;
	fex = abs(e1[0]);
	fey = abs(e1[1]);
	fez = abs(e1[2]);
	p0 = e1[2] * v0[1] - e1[1] * v0[2];
	p2 = e1[2] * v2[1] - e1[1] * v2[2];
	if (p0 < p2)
	{
		min = p0;
		max = p2;
	}
	else
	{
		min = p2;
		max = p0;
	}
	rad = fez * boxHalfSize[1] + fey * boxHalfSize[2];
	if ((min > rad) || (max < -rad)) return 0;
	p0 = -e1[2] * v0[0] + e1[0] * v0[2];
	p2 = -e1[2] * v2[0] + e1[0] * v2[2];
	if (p0 < p2)
	{
		min = p0;
		max = p2;
	}
	else
	{
		min = p2;
		max = p0;
	}
	rad = fez * boxHalfSize[0] + fex * boxHalfSize[2];
	if ((min > rad) || (max < -rad)) return 0;
	p0 = e1[1] * v0[0] - e1[0] * v0[1];
	p1 = e1[1] * v1[0] - e1[0] * v1[1];
	if (p0 < p1)
	{
		min = p0;
		max = p1;
	}
	else
	{
		min = p1;
		max = p0;
	}
	rad = fey * boxHalfSize[0] + fex * boxHalfSize[1];
	if ((min > rad) || (max < -rad)) return 0;
	fex = abs(e2[0]);
	fey = abs(e2[1]);
	fez = abs(e2[2]);
	p0 = e2[2] * v0[1] - e2[1] * v0[2];
	p1 = e2[2] * v1[1] - e2[1] * v1[2];
	if (p0 < p1)
	{
		min = p0;
		max = p1;
	}
	else
	{
		min = p1;
		max = p0;
	}
	rad = fez * boxHalfSize[1] + fey * boxHalfSize[2];
	if ((min > rad) || (max < -rad)) return 0;
	p0 = -e2[2] * v0[0] + e2[0] * v0[2];
	p1 = -e2[2] * v1[0] + e2[0] * v1[2];
	if (p0 < p1)
	{
		min = p0;
		max = p1;
	}
	else
	{
		min = p1;
		max = p0;
	}
	rad = fez * boxHalfSize[0] + fex * boxHalfSize[2];
	if ((min > rad) || (max < -rad)) return 0;
	p1 = e2[1] * v1[0] - e2[0] * v1[1];
	p2 = e2[1] * v2[0] - e2[0] * v2[1];
	if (p2 < p1)
	{
		min = p2;
		max = p1;
	}
	else
	{
		min = p1;
		max = p2;
	}
	rad = fey * boxHalfSize[0] + fex * boxHalfSize[1];
	if ((min > rad) || (max < -rad)) return false;
	min = fmin(v0[0], v1[0]);
	min = fmin(min, v2[0]);
	max = fmax(v0[0], v1[0]);
	max = fmax(max, v2[0]);
	if ((min > boxHalfSize[0]) || (max < -boxHalfSize[0])) return false;
	min = fmin(v0[1], v1[1]);
	min = fmin(min, v2[1]);
	max = fmax(v0[1], v1[1]);
	max = fmax(max, v2[1]);
	if ((min > boxHalfSize[1]) || (max < -boxHalfSize[1])) return false;
	min = fmin(v0[2], v1[2]);
	min = fmin(min, v2[2]);
	max = fmax(v0[2], v1[2]);
	max = fmax(max, v2[2]);
	if ((min > boxHalfSize[2]) || (max < -boxHalfSize[2])) return false;
	normal[0] = e0[1] * e1[2] - e0[2] * e1[1];
	normal[1] = e0[2] * e1[0] - e0[0] * e1[2];
	normal[2] = e0[0] * e1[1] - e0[1] * e1[0];
	if (!PlaneBoxCollision(normal, v0, boxHalfSize)) return 0;
	return 1;
}

bool Brush::TriangleBoxCollision(glm::vec3 c, glm::vec3 e, glm::vec3 vel, int tri, glm::vec3 *translation)
{

	if (glm::dot(verts[tri + 0].normal, vel) > 0.0f) {
		return false;
	}

	float distance = 99999;
	float axis[3];
	float v0[3] =
	{
			verts[tri + 0].pos.x - c[0],
			verts[tri + 0].pos.y - c[1],
			verts[tri + 0].pos.z - c[2]
	};
	float v1[3] =
	{
			verts[tri + 1].pos.x - c[0],
			verts[tri + 1].pos.y - c[1],
			verts[tri + 1].pos.z - c[2]
	};
	float v2[3] =
	{
			verts[tri + 2].pos.x - c[0],
			verts[tri + 2].pos.y - c[1],
			verts[tri + 2].pos.z - c[2]
	};
	float f0[3] =
	{
			v1[0] - v0[0],
			v1[1] - v0[1],
			v1[2] - v0[2]
	};
	float f1[3] =
	{
			v2[0] - v1[0],
			v2[1] - v1[1],
			v2[2] - v1[2]
	};
	float f2[3] =
	{
			v0[0] - v2[0],
			v0[1] - v2[1],
			v0[2] - v2[2]
	};
	float a00l = sqrt(f0[2] * f0[2] + f0[1] * f0[1]);
	if (a00l != 0) {
		float a00y = -f0[2] / a00l;
		float a00z = f0[1] / a00l;
		float a00r = e[1] * abs(a00y) + e[2] * abs(a00z);
		float a00p0 = v0[1] * a00y + v0[2] * a00z;
		float a00p1 = v1[1] * a00y + v1[2] * a00z;
		float a00p2 = v2[1] * a00y + v2[2] * a00z;
		float a00min = fmin(a00p0, a00p1);
		a00min = fmin(a00min, a00p2);
		float a00max = fmax(a00p0, a00p1);
		a00max = fmax(a00max, a00p2);
		if (a00min > a00r) return false;
		if (a00max < -a00r) return false;
		else
		{
			if (a00min < -a00r)
			{
				distance = -(a00max + a00r);
			}
			else
			{
				distance = a00r - a00min;
			}
			axis[0] = 0;
			axis[1] = a00y;
			axis[2] = a00z;
		}
	}
	float a01l = sqrt(f1[2] * f1[2] + f1[1] * f1[1]);
	float a01y = -f1[2] / a01l;
	float a01z = f1[1] / a01l;
	float a01r = e[1] * abs(a01y) + e[2] * abs(a01z);
	float a01p0 = v0[1] * a01y + v0[2] * a01z;
	float a01p1 = v1[1] * a01y + v1[2] * a01z;
	float a01p2 = v2[1] * a01y + v2[2] * a01z;
	float a01min = fmin(a01p0, a01p1);
	a01min = fmin(a01min, a01p2);
	float a01max = fmax(a01p0, a01p1);
	a01max = fmax(a01max, a01p2);
	if (a01min > a01r) return false;
	if (a01max < -a01r) return false;
	else
	{
		float newDistance;
		if (a01min < -a01r)
		{
			newDistance = -(a01max + a01r);
		}
		else
		{
			newDistance = a01r - a01min;
		}
		if (abs(newDistance) < abs(distance))
		{
			distance = newDistance;
			axis[0] = 0;
			axis[1] = a01y;
			axis[2] = a01z;
		}
	}
	float a02l = sqrt(f2[2] * f2[2] + f2[1] * f2[1]);
	float a02y = -f2[2] / a02l;
	float a02z = f2[1] / a02l;
	float a02r = e[1] * abs(a02y) + e[2] * abs(a02z);
	float a02p0 = v0[1] * a02y + v0[2] * a02z;
	float a02p1 = v1[1] * a02y + v1[2] * a02z;
	float a02p2 = v2[1] * a02y + v2[2] * a02z;
	float a02min = fmin(a02p0, a02p1);
	a02min = fmin(a02min, a02p2);
	float a02max = fmax(a02p0, a02p1);
	a02max = fmax(a02max, a02p2);
	if (a02min > a02r) return false;
	if (a02max < -a02r) return false;
	else
	{
		float newDistance;
		if (a02min < -a02r)
		{
			newDistance = -(a02max + a02r);
		}
		else
		{
			newDistance = a02r - a02min;
		}
		if (abs(newDistance) < abs(distance))
		{
			distance = newDistance;
			axis[0] = 0;
			axis[1] = a02y;
			axis[2] = a02z;
		}
	}
	float a10l = sqrt(f0[2] * f0[2] + f0[0] * f0[0]);
	float a10x = f0[2] / a10l;
	float a10z = -f0[0] / a10l;
	float a10r = e[0] * abs(a10x) + e[2] * abs(a10z);
	float a10p0 = v0[0] * a10x + v0[2] * a10z;
	float a10p1 = v1[0] * a10x + v1[2] * a10z;
	float a10p2 = v2[0] * a10x + v2[2] * a10z;
	float a10min = fmin(a10p0, a10p1);
	a10min = fmin(a10min, a10p2);
	float a10max = fmax(a10p0, a10p1);
	a10max = fmax(a10max, a10p2);
	if (a10min > a10r) return false;
	if (a10max < -a10r) return false;
	else
	{
		float newDistance;
		if (a10min < -a10r)
		{
			newDistance = -(a10max + a10r);
		}
		else
		{
			newDistance = a10r - a10min;
		}
		if (abs(newDistance) < abs(distance))
		{
			distance = newDistance;
			axis[0] = a10x;
			axis[1] = 0;
			axis[2] = a10z;
		}
	}
	float a11l = sqrt(f1[2] * f1[2] + f1[0] * f1[0]);
	float a11x = f1[2] / a11l;
	float a11z = -f1[0] / a11l;
	float a11r = e[0] * abs(a11x) + e[2] * abs(a11z);
	float a11p0 = v0[0] * a11x + v0[2] * a11z;
	float a11p1 = v1[0] * a11x + v1[2] * a11z;
	float a11p2 = v2[0] * a11x + v2[2] * a11z;
	float a11min = fmin(a11p0, a11p1);
	a11min = fmin(a11min, a11p2);
	float a11max = fmax(a11p0, a11p1);
	a11max = fmax(a11max, a11p2);
	if (a11min > a11r) return false;
	if (a11max < -a11r) return false;
	else
	{
		float newDistance;
		if (a11min < -a11r)
		{
			newDistance = -(a11max + a11r);
		}
		else
		{
			newDistance = a11r - a11min;
		}
		if (abs(newDistance) < abs(distance))
		{
			distance = newDistance;
			axis[0] = a11x;
			axis[1] = 0;
			axis[2] = a11z;
		}
	}
	float a12l = sqrt(f2[2] * f2[2] + f2[0] * f2[0]);
	float a12x = f2[2] / a12l;
	float a12z = -f2[0] / a12l;
	float a12r = e[0] * abs(a12x) + e[2] * abs(a12z);
	float a12p0 = v0[0] * a12x + v0[2] * a12z;
	float a12p1 = v1[0] * a12x + v1[2] * a12z;
	float a12p2 = v2[0] * a12x + v2[2] * a12z;
	float a12min = fmin(a12p0, a12p1);
	a12min = fmin(a12min, a12p2);
	float a12max = fmax(a12p0, a12p1);
	a12max = fmax(a12max, a12p2);
	if (a12min > a12r) return false;
	if (a12max < -a12r) return false;
	else
	{
		float newDistance;
		if (a12min < -a12r)
		{
			newDistance = -(a12max + a12r);
		}
		else
		{
			newDistance = a12r - a12min;
		}
		if (abs(newDistance) < abs(distance))
		{
			distance = newDistance;
			axis[0] = a12x;
			axis[1] = 0;
			axis[2] = a12z;
		}
	}
	float a20l = sqrt(f0[1] * f0[1] + f0[0] * f0[0]);
	float a20x = -f0[1] / a20l;
	float a20y = f0[0] / a20l;
	float a20r = e[0] * abs(a20x) + e[1] * abs(a20y);
	float a20p0 = v0[0] * a20x + v0[1] * a20y;
	float a20p1 = v1[0] * a20x + v1[1] * a20y;
	float a20p2 = v2[0] * a20x + v2[1] * a20y;
	float a20min = fmin(a20p0, a20p1);
	a20min = fmin(a20min, a20p2);
	float a20max = fmax(a20p0, a20p1);
	a20max = fmax(a20max, a20p2);
	if (a20min > a20r) return false;
	if (a20max < -a20r) return false;
	else
	{
		float newDistance;
		if (a20min < -a20r)
		{
			newDistance = -(a20max + a20r);
		}
		else
		{
			newDistance = a20r - a20min;
		}
		if (abs(newDistance) < abs(distance))
		{
			distance = newDistance;
			axis[0] = a20x;
			axis[1] = a20y;
			axis[2] = 0;
		}
	}
	float a21l = sqrt(f1[1] * f1[1] + f1[0] * f1[0]);
	float a21x = -f1[1] / a21l;
	float a21y = f1[0] / a21l;
	float a21r = e[0] * abs(a21x) + e[1] * abs(a21y);
	float a21p0 = v0[0] * a21x + v0[1] * a21y;
	float a21p1 = v1[0] * a21x + v1[1] * a21y;
	float a21p2 = v2[0] * a21x + v2[1] * a21y;
	float a21min = fmin(a21p0, a21p1);
	a21min = fmin(a21min, a21p2);
	float a21max = fmax(a21p0, a21p1);
	a21max = fmax(a21max, a21p2);
	if (a21min > a21r) return false;
	if (a21max < -a21r) return false;
	else
	{
		float newDistance;
		if (a21min < -a21r)
		{
			newDistance = -(a21max + a21r);
		}
		else
		{
			newDistance = a21r - a21min;
		}
		if (abs(newDistance) < abs(distance))
		{
			distance = newDistance;
			axis[0] = a21x;
			axis[1] = a21y;
			axis[2] = 0;
		}
	}
	float a22l = sqrt(f2[1] * f2[1] + f2[0] * f2[0]);
	float a22x = -f2[1] / a22l;
	float a22y = f2[0] / a22l;
	float a22r = e[0] * abs(a22x) + e[1] * abs(a22y);
	float a22p0 = v0[0] * a22x + v0[1] * a22y;
	float a22p1 = v1[0] * a22x + v1[1] * a22y;
	float a22p2 = v2[0] * a22x + v2[1] * a22y;
	float a22min = fmin(a22p0, a22p1);
	a22min = fmin(a22min, a22p2);
	float a22max = fmax(a22p0, a22p1);
	a22max = fmax(a22max, a22p2);
	if (a22min > a22r) return false;
	if (a22max < -a22r) return false;
	else
	{
		float newDistance;
		if (a22min < -a22r)
		{
			newDistance = -(a22max + a22r);
		}
		else
		{
			newDistance = a22r - a22min;
		}
		if (abs(newDistance) < abs(distance))
		{
			distance = newDistance;
			axis[0] = a22x;
			axis[1] = a22y;
			axis[2] = 0;
		}
	}
	float b0max = fmax(v0[0], v1[0]);
	b0max = fmax(b0max, v2[0]);
	if (b0max < -e[0]) return false;
	else
	{
		float newDistance = -(e[0] + b0max);
		if (abs(newDistance) < abs(distance))
		{
			distance = newDistance;
			axis[0] = 1;
			axis[1] = 0;
			axis[2] = 0;
		}
	}
	float b0min = fmin(v0[0], v1[0]);
	b0min = fmin(b0min, v2[0]);
	if (b0min > e[0]) return false;
	else
	{
		float newDistance = b0min - e[0];
		if (abs(newDistance) < abs(distance))
		{
			distance = newDistance;
			axis[0] = -1;
			axis[1] = 0;
			axis[2] = 0;
		}
	}
	float b1max = fmax(v0[1], v1[1]);
	b1max = fmax(b1max, v2[1]);
	if (b1max < -e[1]) return false;
	else
	{
		float newDistance = -(e[1] + b1max);
		if (abs(newDistance) < abs(distance))
		{
			distance = newDistance;
			axis[0] = 0;
			axis[1] = 1;
			axis[2] = 0;
		}
	}
	float b1min = fmin(v0[1], v1[1]);
	b1min = fmin(b1min, v2[1]);
	if (b1min > e[1]) return false;
	else
	{
		float newDistance = b1min - e[1];
		if (abs(newDistance) < abs(distance))
		{
			distance = newDistance;
			axis[0] = 0;
			axis[1] = -1;
			axis[2] = 0;
		}
	}
	float b2max = fmax(v0[2], v1[2]);
	b2max = fmax(b2max, v2[2]);
	if (b2max < -e[2]) return false;
	else
	{
		float newDistance = -(e[2] + b2max);
		if (abs(newDistance) < abs(distance))
		{
			distance = newDistance;
			axis[0] = 0;
			axis[1] = 0;
			axis[2] = 1;
		}
	}
	float b2min = fmin(v0[2], v1[2]);
	b2min = fmin(b2min, v2[2]);
	if (b2min > e[2]) return false;
	else
	{
		float newDistance = b2min - e[2];
		if (abs(newDistance) < abs(distance))
		{
			distance = newDistance;
			axis[0] = 0;
			axis[1] = 0;
			axis[2] = -1;
		}
	}
	float pn[3] =
	{
			-f0[2] * f1[1] + f0[1] * f1[2],
			f0[2] * f1[0] - f0[0] * f1[2],
			-f0[1] * f1[0] + f0[0] * f1[1]
	};
	float pnl = sqrt(pn[0] * pn[0] + pn[1] * pn[1] + pn[2] * pn[2]);
	pn[0] /= pnl;
	pn[1] /= pnl;
	pn[2] /= pnl;
	pn[0] *= -1;
	pn[1] *= -1;
	pn[2] *= -1;
	float pd = v0[0] * pn[0] + v0[1] * pn[1] + v0[2] * pn[2];
	float pr = e[0] * abs(pn[0]) + e[1] * abs(pn[1]) + e[2] * abs(pn[2]);
	if (abs(pd) > pr) return false;
	else
	{
		float newDistance = -pr - pd;
		if (abs(newDistance) < abs(distance))
		{
			distance = newDistance;
			axis[0] = pn[0];
			axis[1] = pn[1];
			axis[2] = pn[2];
		}
	}
	distance *= -1;
	translation->x += axis[0] * distance;
	translation->y += axis[1] * distance;
	translation->z += axis[2] * distance;

	return true;
}