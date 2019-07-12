#pragma once

#include "MD5Animation.h"

void DeleteVertexBuffer(GLuint& vboID);

void DeleteVertexArrayBuffer(GLuint& vaoID);

inline void CreateVertexBuffer(GLuint& vboID);

class MD5Model {

public:
	explicit MD5Model();
	virtual ~MD5Model();

	bool LoadModel(const std::string& filename);
	bool LoadAnim(const std::string& filename);

	void SetWorldTransform(const glm::mat4x4 world);
	glm::mat4x4 GetWorldTransform() const;
	glm::mat4x4 GetInverseWorldTransform() const;

	void Update(float fDeltaTime);
	void Render(Shader *shader);

protected:
	typedef std::vector<glm::vec3> PositionBuffer;
	typedef std::vector<glm::vec3> NormalBuffer;
	typedef std::vector<glm::vec2> Tex2DBuffer;
	typedef std::vector<glm::vec4> WeightBuffer;
	typedef std::vector<glm::ivec4> BoneIndexBuffer;

	typedef std::vector<GLuint> IndexBuffer;

	struct Vertex {
		glm::vec3 m_Pos;
		glm::vec3 m_Normal;
		glm::vec2 m_Tex0;
		glm::vec4 m_BoneWeights;
		glm::ivec4 m_BoneIndices;

		int m_StartWeight;
		int m_WeightCount;
	};
	typedef std::vector<Vertex> VertexList;

	struct Triangle {
		int m_Indices[3];
	};
	typedef std::vector<Triangle> TriangleList;

	struct Weight {
		int m_JointID;
		float m_Bias;
		glm::vec3 m_Pos;
	};
	typedef std::vector<Weight> WeightList;

	struct Joint {
		std::string m_Name;
		int m_ParentID;
		glm::vec3 m_Pos;
		glm::quat m_Orient;
	};
	typedef std::vector<Joint> JointList;

	struct Mesh {

		Mesh()
			: m_GLVAO(0)
			, m_GLPositionBuffer(0)
			, m_GLNormalBuffer(0)
			, m_GLBoneWeights(0)
			, m_GLBoneIndex(0)
			, m_GLTexCoord(0)
			, m_GLIndexBuffer(0)
		{}

		std::string m_Shader;

		VertexList m_Verts;
		TriangleList m_Tris;
		WeightList m_Weights;

		GLuint m_TexID;

		PositionBuffer m_PositionBuffer;
		NormalBuffer m_NormalBuffer;
		WeightBuffer m_BoneWeights;
		BoneIndexBuffer m_BoneIndex;

		Tex2DBuffer m_Tex2DBuffer;
		IndexBuffer m_IndexBuffer;

		GLuint m_GLVAO;
		GLuint m_GLPositionBuffer;
		GLuint m_GLNormalBuffer;
		GLuint m_GLBoneWeights;
		GLuint m_GLBoneIndex;
		GLuint m_GLTexCoord;
		GLuint m_GLIndexBuffer;
	};
	typedef std::vector<Mesh> MeshList;

	void BuildBindPose(const JointList& joints);

	bool PrepareMesh(Mesh& mesh);
	bool MD5Model::PrepareMesh(Mesh& mesh, const std::vector<glm::mat4x4>& skel);
	bool PrepareNormals(Mesh& mesh);

	void DestroyMesh(Mesh& mesh);

	bool CreateVertexBuffers(Mesh& mesh);

	//void RenderGPU(const Mesh& mesh);

	void RenderMesh(const Mesh& mesh, Shader* shader);
	//void RenderNormals(const Mesh& mesh);

	//void RenderSkeleton(const JointList& joints);

	bool CheckAnimation(const MD5Animation& animation) const;

private:
	typedef std::vector<glm::mat4x4> MatrixList;

	int m_iMD5Version;
	int m_iNumJoints;
	int m_iNumMeshes;

	bool m_bHasAnimation;

	JointList m_Joints;
	MeshList m_Meshes;

	MD5Animation m_Animation;

	MatrixList m_BindPose;
	MatrixList m_InverseBindPose;

	MatrixList m_AnimatedBones;

	glm::mat4x4 m_LocalToWorldMatrix;
	glm::mat4x4 m_WorldToLocalMatrix;
};