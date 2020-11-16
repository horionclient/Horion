#pragma once

#include "HMath.h"

namespace Voxel {

	typedef struct {
		float x;
		float y;
		float z;
	} Point3;

	typedef struct {
		Point3 v1; /* Vertex1 */
		Point3 v2; /* Vertex2 */
		Point3 v3; /* Vertex3 */
	} Triangle3;

	void getBoundingBox(AABB& aabbOut, const Triangle3& tri);
	bool intersects(const vec3_t& voxel, const Triangle3& triangle);
}