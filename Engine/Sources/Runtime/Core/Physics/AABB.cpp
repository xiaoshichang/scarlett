#include "AABB.h"

using namespace scarlett;


void scarlett::AABB::Initialize(Vector3f & minPoint, Vector3f & maxPoint)
{
	this->minPoint = minPoint;
	this->maxPoint = maxPoint;
}

void scarlett::AABB::Initialize(aiMesh * mesh)
{
	float maxx = 0.0f;
	float minx = 0.0f;
	float maxy = 0.0f;
	float miny = 0.0f;
	float maxz = 0.0f;
	float minz = 0.0f;

	auto count = mesh->mNumVertices;
	auto data = mesh->mVertices;

	for (int idx = 0; idx < count; ++idx)
	{
		float x = data[idx].x;
		float y = data[idx].y;
		float z = data[idx].z;

		if (x < minx)
		{
			minx = x;
			minPoint.x = x;
		}
		else if (x > maxx)
		{
			maxx = x;
			maxPoint.x = x;
		}

		if (y < miny)
		{
			miny = y;
			minPoint.y = y;
		}
		else if (y > maxy)
		{
			maxy = y;
			maxPoint.y = y;
		}

		if (z < minz)
		{
			minz = z;
			minPoint.z = z;
		}
		else if (z > maxz)
		{
			maxz = z;
			maxPoint.z = z;
		}
	}
}

float  scarlett::AABB::btFsels(float x, float a, float b)
{
	if (x > 0)
	{
		return a;
	}
	else
	{
		return b;
	}
}

// http://allenchou.net/2013/12/game-physics-collision-detection-csos-support-functions/
// support function
Vector3f scarlett::AABB::LocalGetSupportVertex(const Vector3f & dir)
{
	return Vector3f(
		btFsels(dir.x, maxPoint.x, -maxPoint.x),
		btFsels(dir.y, maxPoint.y, -maxPoint.y),
		btFsels(dir.z, maxPoint.z, -maxPoint.z)
	);
}
