#include "MD5Model.h"

MD5Model::MD5Model()
	: m_iMD5Version(-1)
	, m_iNumJoints(0)
	, m_iNumMeshes(0)
	, m_bHasAnimation(false)
	, m_LocalToWorldMatrix(1)
	, m_WorldToLocalMatrix(1)
{}

MD5Model::~MD5Model() {

	MeshList::iterator iter = m_Meshes.begin();
	while (iter != m_Meshes.end()) {

		DestroyMesh(*iter);
		++iter;
	}
	m_Meshes.clear();
}

void DeleteVertexBuffer(GLuint& vboID) {

	if (vboID != 0) {

		glDeleteBuffers(1, &vboID);
		vboID = 0;
	}
}

void DeleteVertexArrayBuffer(GLuint& vaoID) {

	if (vaoID != 0) {

		glDeleteVertexArrays(1, &vaoID);
		vaoID = 0;
	}
}

inline void CreateVertexBuffer(GLuint& vboID) {

	DeleteVertexBuffer(vboID);
	glGenBuffers(1, &vboID);
}

void MD5Model::DestroyMesh(Mesh& mesh) {

	DeleteVertexBuffer(mesh.m_GLPositionBuffer);
	DeleteVertexBuffer(mesh.m_GLNormalBuffer);
	DeleteVertexBuffer(mesh.m_GLTexCoord);
	DeleteVertexBuffer(mesh.m_GLBoneIndex);
	DeleteVertexBuffer(mesh.m_GLBoneWeights);
	DeleteVertexBuffer(mesh.m_GLIndexBuffer);
	DeleteVertexArrayBuffer(mesh.m_GLVAO);
}

void MD5Model::SetWorldTransform(const glm::mat4x4 world) {

	m_LocalToWorldMatrix = world;
	m_WorldToLocalMatrix = glm::inverse(m_LocalToWorldMatrix);
}

glm::mat4x4 MD5Model::GetWorldTransform() const {

	return m_LocalToWorldMatrix;
}

glm::mat4x4 MD5Model::GetInverseWorldTransform() const {

	return m_WorldToLocalMatrix;
}

bool MD5Model::LoadModel(const std::string& filename) {

	if (!std::experimental::filesystem::exists(filename)) {
		std::cerr << "MD5Model::LoadModel: Failed to find file: " << filename << std::endl;
		return false;
	}

	std::experimental::filesystem::path filePath = filename;

	std::experimental::filesystem::path parent_path = filePath.parent_path();

	std::string param;
	std::string junk;

	std::ifstream file(filename);
	int fileLength = GetFileLength(file);
	assert(fileLength > 0);

	m_Joints.clear();
	m_Meshes.clear();

	file >> param;

	while (!file.eof()) {
		
		if (param == "MD5Version") {
			
			file >> m_iMD5Version;
			assert(m_iMD5Version == 10);
		}
		else if (param == "commandline") {

			IgnoreLine(file, fileLength);
		}
		else if (param == "numJoints") {

			file >> m_iNumJoints;
			m_Joints.reserve(m_iNumJoints);
			m_AnimatedBones.assign(m_iNumJoints, glm::mat4x4(1.0f));
		}
		else if (param == "numMeshes") {

			file >> m_iNumMeshes;
			m_Meshes.reserve(m_iNumMeshes);
		}
		else if (param == "joints") {

			Joint joint;
			file >> junk;
			for (int i = 0; i < m_iNumJoints; ++i) {
				
				file >> joint.m_Name >> joint.m_ParentID >> junk
					>> joint.m_Pos.x >> joint.m_Pos.y >> joint.m_Pos.z >> junk >> junk
					>> joint.m_Orient.x >> joint.m_Orient.y >> joint.m_Orient.z >> junk;

				RemoveQuotes(joint.m_Name);
				ComputeQuatW(joint.m_Orient);

				m_Joints.push_back(joint);

				IgnoreLine(file, fileLength);
			}
			file >> junk;

			BuildBindPose(m_Joints);
		}
		else if (param == "mesh") {

			Mesh mesh;
			int numVerts, numTris, numWeights;

			file >> junk;
			file >> param;
			while (param != "}") {

				if (param == "shader") {

					file >> mesh.m_Shader;
					RemoveQuotes(mesh.m_Shader);

					std::experimental::filesystem::path shaderPath(mesh.m_Shader);
					std::experimental::filesystem::path texturePath;
					if (shaderPath.has_parent_path()) {

						texturePath = shaderPath;
					}
					else {
						
						texturePath = parent_path / shaderPath;
					}

					if (!texturePath.has_extension()) {

						//texturePath.replace_extension(".tga");
					}

					mesh.m_TexID = 0;

					file.ignore(fileLength, '\n');
				}
				else if (param == "numverts") {

					file >> numVerts;
					IgnoreLine(file, fileLength);
					for (int i = 0; i < numVerts; ++i) {

						Vertex vert;

						file >> junk >> junk >> junk
							>> vert.m_Tex0.x >> vert.m_Tex0.y >> junk
							>> vert.m_StartWeight >> vert.m_WeightCount;

						IgnoreLine(file, fileLength);

						mesh.m_Verts.push_back(vert);
						mesh.m_Tex2DBuffer.push_back(vert.m_Tex0);
					}
				}
				else if (param == "numtris") {

					file >> numTris;
					IgnoreLine(file, fileLength);
					for (int i = 0; i < numTris; ++i) {

						Triangle tri;
						file >> junk >> junk >> tri.m_Indices[0] >> tri.m_Indices[1] >> tri.m_Indices[2];

						IgnoreLine(file, fileLength);

						mesh.m_Tris.push_back(tri);
						mesh.m_IndexBuffer.push_back((GLuint)tri.m_Indices[0]);
						mesh.m_IndexBuffer.push_back((GLuint)tri.m_Indices[1]);
						mesh.m_IndexBuffer.push_back((GLuint)tri.m_Indices[2]);
					}
				}
				else if (param == "numweights") {

					file >> numWeights;
					IgnoreLine(file, fileLength);
					for (int i = 0; i < numWeights; ++i) {

						Weight weight;
						file >> junk >> junk >> weight.m_JointID >> weight.m_Bias >> junk
							>> weight.m_Pos.x >> weight.m_Pos.y >> weight.m_Pos.z >> junk;

						//std::cout << weight.m_JointID << " " << weight.m_Bias << "\n";

						IgnoreLine(file, fileLength);
						mesh.m_Weights.push_back(weight);
					}
				}
				else {

					IgnoreLine(file, fileLength);
				}
				file >> param;
			}

			PrepareMesh(mesh);
			PrepareNormals(mesh);
			CreateVertexBuffers(mesh);

			m_Meshes.push_back(mesh);
		}

		file >> param;
	}

	assert(m_Joints.size() == m_iNumJoints);
	assert(m_Meshes.size() == m_iNumMeshes);

	//std::cout << m_Meshes.size() << " " << m_iNumMeshes << "\n";

	return true;
}

bool MD5Model::LoadAnim(const std::string& filename) {

	if (m_Animation.LoadAnimation(filename)) {

		m_bHasAnimation = CheckAnimation(m_Animation);
	}

	return m_bHasAnimation;
}

bool MD5Model::PrepareMesh(Mesh& mesh) {

	mesh.m_PositionBuffer.clear();
	mesh.m_Tex2DBuffer.clear();
	mesh.m_BoneIndex.clear();
	mesh.m_BoneWeights.clear();

	for (unsigned int i = 0; i < mesh.m_Verts.size(); ++i) {

		glm::vec3 finalPos(0);
		Vertex& vert = mesh.m_Verts[i];

		vert.m_Pos = glm::vec3(0);
		vert.m_Normal = glm::vec3(0);
		vert.m_BoneWeights = glm::vec4(0);
		vert.m_BoneIndices = glm::ivec4(0);

		for (int j = 0; j < vert.m_WeightCount; ++j) {

			assert(j < 4);

			Weight& weight = mesh.m_Weights[vert.m_StartWeight + j];
			Joint& joint = m_Joints[weight.m_JointID];

			glm::vec3 rotPos = joint.m_Orient * weight.m_Pos;

			vert.m_Pos += (joint.m_Pos + rotPos) * weight.m_Bias;
			vert.m_BoneIndices[j] = weight.m_JointID;
			vert.m_BoneWeights[j] = weight.m_Bias;
		}

		mesh.m_PositionBuffer.push_back(vert.m_Pos);
		mesh.m_Tex2DBuffer.push_back(vert.m_Tex0);
		mesh.m_BoneIndex.push_back(vert.m_BoneIndices);
		mesh.m_BoneWeights.push_back(vert.m_BoneWeights);
	}

	return true;
}

bool MD5Model::PrepareMesh(Mesh& mesh, const std::vector<glm::mat4x4>& skel) {

	for (unsigned int i = 0; i < mesh.m_Verts.size(); ++i) {

		const Vertex& vert = mesh.m_Verts[i];
		glm::vec3& pos = mesh.m_PositionBuffer[i];
		glm::vec3& normal = mesh.m_NormalBuffer[i];

		pos = glm::vec3(0);
		normal = glm::vec3(0);

		for (int j = 0; j < vert.m_WeightCount; ++j) {

			const Weight& weight = mesh.m_Weights[vert.m_StartWeight + j];
			const glm::mat4x4 boneMatrix = skel[weight.m_JointID];

			pos += glm::vec3((boneMatrix * glm::vec4(vert.m_Pos, 1.0f)) * weight.m_Bias);
			normal += glm::vec3((boneMatrix * glm::vec4(vert.m_Normal, 0.0f)) * weight.m_Bias);
		}
	}

	return true;
}

bool MD5Model::PrepareNormals(Mesh& mesh) {

	mesh.m_NormalBuffer.clear();

	for (unsigned int i = 0; i < mesh.m_Tris.size(); ++i) {

		glm::vec3 v0 = mesh.m_Verts[mesh.m_Tris[i].m_Indices[0]].m_Pos;
		glm::vec3 v1 = mesh.m_Verts[mesh.m_Tris[i].m_Indices[1]].m_Pos;
		glm::vec3 v2 = mesh.m_Verts[mesh.m_Tris[i].m_Indices[2]].m_Pos;

		glm::vec3 normal = glm::cross(v2 - v0, v1 - v0);

		mesh.m_Verts[mesh.m_Tris[i].m_Indices[0]].m_Normal += normal;
		mesh.m_Verts[mesh.m_Tris[i].m_Indices[1]].m_Normal += normal;
		mesh.m_Verts[mesh.m_Tris[i].m_Indices[2]].m_Normal += normal;
	}

	for (unsigned int i = 0; i < mesh.m_Verts.size(); ++i) {

		Vertex& vert = mesh.m_Verts[i];

		vert.m_Normal = glm::normalize(vert.m_Normal);
		mesh.m_NormalBuffer.push_back(vert.m_Normal);

		/*vert.m_Normal = glm::vec3(0);

		for (int j = 0; j < vert.m_WeightCount; ++j) {

			const Weight& weight = mesh.m_Weights[vert.m_StartWeight + j];
			const Joint& joint = m_Joints[weight.m_JointID];
			vert.m_Normal += (normal * joint.m_Orient) * weight.m_Bias;
		}*/
	}

	return true;
}

void MD5Model::Render(Shader* shader) {

	//glPushMatrix();
	//glMultMatrixf(glm::value_ptr(m_LocalToWorldMatrix));
	//std::cout << m_Meshes.size() << "\n";
	for (unsigned int i = 0; i < m_Meshes.size(); ++i) {
		if (i == 0) {
			glBindTexture(GL_TEXTURE_2D, Resource_Manager::getTexture("body"));
		}
		else {
			glBindTexture(GL_TEXTURE_2D, Resource_Manager::getTexture("head"));
		}
		RenderMesh(m_Meshes[i], shader);
	}

	//m_Animation.Render();

	//for (unsigned int i = 0; i < m_Meshes.size(); ++i) {

		//RenderNormals(m_Meshes[i]);
	//}

	//glPopMatrix();
}

void MD5Model::RenderMesh(const Mesh& mesh, Shader* shader) {

	glm::mat4 model = glm::mat4(1.0f);
	model = glm::rotate(model, glm::radians(270.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	//std::cout << m_AnimatedBones.size() << "\n";
	shader->setMat4Array("jointTransforms", m_AnimatedBones);

	//std::cout << mesh.m_BoneIndex[1].x << "\n";

	glBindVertexArray(mesh.m_GLVAO);
	shader->setMat4("model", model);
	glDrawElements(GL_TRIANGLES, mesh.m_IndexBuffer.size(), GL_UNSIGNED_INT, 0);
	//std::cout << mesh.m_IndexBuffer.size() << "\n";
}

void MD5Model::Update(float fDeltaTime) {

	if (m_bHasAnimation) {

		m_Animation.Update(fDeltaTime);
		
		const MatrixList& animatedSkeleton = m_Animation.GetSkeletonMatrixList();

		for (int i = 0; i < m_iNumJoints; ++i) {

			m_AnimatedBones[i] = animatedSkeleton[i] * m_InverseBindPose[i];
		}
	}
	else {

		m_AnimatedBones.assign(m_iNumJoints, glm::mat4x4(1.0));
	}

	for (unsigned int i = 0; i < m_Meshes.size(); ++i) {

		PrepareMesh(m_Meshes[i], m_AnimatedBones);
	}
}

bool MD5Model::CheckAnimation(const MD5Animation& animation) const {

	if (m_iNumJoints != animation.GetNumJoints()) {

		return false;
	}

	for (unsigned int i = 0; i < m_Joints.size(); ++i) {

		const Joint& meshJoint = m_Joints[i];
		const MD5Animation::JointInfo& animJoint = animation.GetJointInfo(i);

		if (meshJoint.m_Name != animJoint.m_Name ||
			meshJoint.m_ParentID != animJoint.m_ParentID) {
			
			return false;
		}
	}

	return true;
}

void MD5Model::BuildBindPose(const JointList& joints) {

	m_BindPose.clear();
	m_InverseBindPose.clear();

	JointList::const_iterator iter = joints.begin();
	while (iter != joints.end()) {

		const Joint& joint = (*iter);
		glm::mat4x4 boneTranslation = glm::translate(joint.m_Pos);
		glm::mat4x4 boneRotation = glm::toMat4(joint.m_Orient);

		glm::mat4x4 boneMatrix = boneTranslation * boneRotation;

		glm::mat4x4 inverseBoneMatrix = glm::inverse(boneMatrix);

		m_BindPose.push_back(boneMatrix);
		m_InverseBindPose.push_back(inverseBoneMatrix);

		++iter;
	}
}

bool MD5Model::CreateVertexBuffers(Mesh& mesh) {

	/*
	DestroyMesh(mesh);

	CreateVertexBuffer(mesh.m_GLPositionBuffer);
	CreateVertexBuffer(mesh.m_GLNormalBuffer);
	CreateVertexBuffer(mesh.m_GLTexCoord);
	CreateVertexBuffer(mesh.m_GLBoneWeights);
	CreateVertexBuffer(mesh.m_GLBoneIndex);
	CreateVertexBuffer(mesh.m_GLIndexBuffer);

	glBindBuffer(GL_ARRAY_BUFFER, mesh.m_GLPositionBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * mesh.m_PositionBuffer.size(), &mesh.m_PositionBuffer[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, mesh.m_GLNormalBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * mesh.m_NormalBuffer.size(), &mesh.m_NormalBuffer[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, mesh.m_GLTexCoord);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec2) * mesh.m_Tex2DBuffer.size(), &mesh.m_Tex2DBuffer[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, mesh.m_GLBoneWeights);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec4) * mesh.m_BoneWeights.size(), &mesh.m_BoneWeights[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, mesh.m_GLBoneIndex);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec4) * mesh.m_BoneIndex.size(), &mesh.m_BoneIndex[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh.m_GLIndexBuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint) * mesh.m_IndexBuffer.size(), &mesh.m_IndexBuffer[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	*/
	/*
	// position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 16 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	// normal attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 16 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	// texture coord attribute
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 16 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);
	// Weight Buffer
	glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, 16 * sizeof(float), (void*)(8 * sizeof(float)));
	glEnableVertexAttribArray(3);
	// Bone Index Buffer
	glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, 16 * sizeof(float), (void*)(12 * sizeof(float)));
	glEnableVertexAttribArray(4);*/
	
	DestroyMesh(mesh);
	
	glGenVertexArrays(1, &mesh.m_GLVAO);
	CreateVertexBuffer(mesh.m_GLPositionBuffer);
	CreateVertexBuffer(mesh.m_GLNormalBuffer);
	CreateVertexBuffer(mesh.m_GLTexCoord);
	CreateVertexBuffer(mesh.m_GLBoneWeights);
	CreateVertexBuffer(mesh.m_GLBoneIndex);
	CreateVertexBuffer(mesh.m_GLIndexBuffer);

	glBindVertexArray(mesh.m_GLVAO);

	glBindBuffer(GL_ARRAY_BUFFER, mesh.m_GLPositionBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * mesh.m_PositionBuffer.size(), &mesh.m_PositionBuffer[0], GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, mesh.m_GLNormalBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * mesh.m_NormalBuffer.size(), &mesh.m_NormalBuffer[0], GL_STATIC_DRAW);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(1);

	glBindBuffer(GL_ARRAY_BUFFER, mesh.m_GLTexCoord);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec2) * mesh.m_Tex2DBuffer.size(), &mesh.m_Tex2DBuffer[0], GL_STATIC_DRAW);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(2);

	glBindBuffer(GL_ARRAY_BUFFER, mesh.m_GLBoneWeights);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec4) * mesh.m_BoneWeights.size(), &mesh.m_BoneWeights[0], GL_STATIC_DRAW);
	glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(3);

	glBindBuffer(GL_ARRAY_BUFFER, mesh.m_GLBoneIndex);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::ivec4) * mesh.m_BoneIndex.size(), &mesh.m_BoneIndex[0], GL_STATIC_DRAW);
	glVertexAttribIPointer(4, 4, GL_INT, 0, 0);
	glEnableVertexAttribArray(4);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh.m_GLIndexBuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint) * mesh.m_IndexBuffer.size(), &mesh.m_IndexBuffer[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	return true;
}