#include "Runtime/Core/Math/ScltMath.h"
#include "assimp/scene.h"
#include "assimp/Importer.hpp"
#include "assimp/postprocess.h"


void AssimpMatrix2Eigen(const aiMatrix4x4& from, Matrix4f& to) {
	to << from.a1, from.a2, from.a3, from.a4,
		from.b1, from.b2, from.b3, from.b4,
		from.c1, from.c2, from.c3, from.c4,
		from.d1, from.d2, from.d3, from.d4;
}