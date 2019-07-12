#include "MD5Extras.h"

void RemoveQuotes(std::string& str) {

	size_t n;
	while ((n = str.find('\"')) != std::string::npos) str.erase(n, 1);
}

int GetFileLength(std::istream& file) {

	int pos = file.tellg();
	file.seekg(0, std::ios::end);
	int length = file.tellg();
	file.seekg(pos);

	return length;
}

void IgnoreLine(std::istream& file, int length) {

	file.ignore(length, '\n');
}

void ComputeQuatW(glm::quat& quat) {

	float t = 1.0f - (quat.x * quat.x) - (quat.y * quat.y) - (quat.z * quat.z);
	if (t < 0.0f) {
		quat.w = 0.0f;
	}
	else {
		quat.w = -sqrtf(t);
	}
}