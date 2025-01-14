#include "AABB_Collider.h"

bool AABB_Collider::CollidesWith(SphereCollider& other)
{
	return false;
}

bool AABB_Collider::CollidesWith(AABB_Collider& other)
{
	// Check if the two AABBs are colliding
	return (_minPoints.x <= other._maxPoints.x && _maxPoints.x >= other._minPoints.x) &&
		(_minPoints.y <= other._maxPoints.y && _maxPoints.y >= other._minPoints.y) &&
		(_minPoints.z <= other._maxPoints.z && _maxPoints.z >= other._minPoints.z);
}

Vector AABB_Collider::GetCollisionNormal(const Collider& other)
{
	Vector normal = { 0,0,0 };

	// WE CASTING SPELLS HERE (This is a joke about casting, I give up)
	if (const AABB_Collider* otherAABB = dynamic_cast<const AABB_Collider*>(&other))
	{
		float faceDistances[6] = {
			_maxPoints.x - otherAABB->GetMinPoints().x, // Left face
			otherAABB->GetMaxPoints().x - _minPoints.x, // Right face
			_maxPoints.y - otherAABB->GetMinPoints().y, // Bottom face
			otherAABB->GetMaxPoints().y - _minPoints.y, // Top face
			_maxPoints.z - otherAABB->GetMinPoints().z, // Back face
			otherAABB->GetMaxPoints().z - _minPoints.z  // Front face
		};

		// Cube Normals
		Vector normals[6] = {
			Vector(-1, 0, 0), // Left face
			Vector(1, 0, 0),  // Right face
			Vector(0, -1, 0), // Bottom face
			Vector(0, 1, 0),  // Top face
			Vector(0, 0, -1), // Back face
			Vector(0, 0, 1)   // Front face
		};

		float closestFace = faceDistances[0];
		int index = 0;

		// Iterate through the distances to find the closest collision normal
		for (int i = 1; i < 6; ++i)
		{
			if (faceDistances[i] < closestFace)
			{
				closestFace = faceDistances[i];
				index = i;
			}
		}

		normal = normals[index];
	}

	return normal;
}

void AABB_Collider::Update()
{
	// Update the collider values based on where the hell the object is
	_minPoints = _transform->GetPosition() + _minExtent;
	_maxPoints = _minPoints + Vector(dx, dy, dz);
}