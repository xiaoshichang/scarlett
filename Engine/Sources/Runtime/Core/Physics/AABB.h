#pragma once
#include "Runtime/Core/Math/ScltMath.h"
#include "assimp/scene.h"
#include "assimp/Importer.hpp"

namespace scarlett
{
	class AABB
	{
	public:
		void Initialize(Vector3f& minPoint, Vector3f& maxPoint);
		void Initialize(aiMesh * mesh);


		float btFsels(float x, float a, float b);
		Vector3f LocalGetSupportVertex(const Vector3f& dir);

	public:
		Vector3f minPoint;
		Vector3f maxPoint;


	};
}