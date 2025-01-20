#include "AABB_Collider.h"
#include "SphereCollider.h"

bool AABB_Collider::CollidesWith(SphereCollider& other)
{
	// TODO:
	// Implement AABB to Sphere collision detection
	return false;
}

bool AABB_Collider::CollidesWith(AABB_Collider& other)
{
	// Pretty sure you can just use positions for this but what do I know.
	Vector centerA = GetMinPoints() + GetHalfExtents();
	Vector centerB = other.GetMinPoints() + other.GetHalfExtents();
	Vector halfExtentsA = GetHalfExtents();
	Vector halfExtentsB = other.GetHalfExtents();

	// Check for overlap
	return (abs(centerA.x - centerB.x) <= (halfExtentsA.x + halfExtentsB.x)) &&
		(abs(centerA.y - centerB.y) <= (halfExtentsA.y + halfExtentsB.y)) &&
		(abs(centerA.z - centerB.z) <= (halfExtentsA.z + halfExtentsB.z));
}

Vector AABB_Collider::GetCollisionNormal(const Collider& other)
{
	Vector normal = { 0, 0, 0 };

	// SHADOW WIZARD MONEY GANG, WE LOVE CASTING SPELLS HERE (This is a joke about casting, I give up)
	if (auto otherAABB = dynamic_cast<const AABB_Collider*>(&other))
	{
		Vector centerA = _minPoints + _halfExtents;
		Vector centerB = otherAABB->GetMinPoints() + otherAABB->GetHalfExtents();
		Vector halfExtentsA = GetHalfExtents();
		Vector halfExtentsB = otherAABB->GetHalfExtents();

		//AHHHHHHHHHHHHHHHHHHH WHAT I AM EVEN LOOKING AT
		float faceDistances[6] = {
			(centerA.x + halfExtentsA.x) - (centerB.x - halfExtentsB.x), // Left face
			(centerB.x + halfExtentsB.x) - (centerA.x - halfExtentsA.x), // Right face
			(centerA.y + halfExtentsA.y) - (centerB.y - halfExtentsB.y), // Bottom face
			(centerB.y + halfExtentsB.y) - (centerA.y - halfExtentsA.y), // Top face
			(centerA.z + halfExtentsA.z) - (centerB.z - halfExtentsB.z), // Back face
			(centerB.z + halfExtentsB.z) - (centerA.z - halfExtentsA.z) // Front face
		};

		// Cube Normals
		Vector normals[6] = {
			Vector(-1, 0, 0), // Left face
			Vector(1, 0, 0), // Right face
			Vector(0, -1, 0), // Bottom face
			Vector(0, 1, 0), // Top face
			Vector(0, 0, -1), // Back face
			Vector(0, 0, 1) // Front face
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
	else
	{
		auto otherSphere = dynamic_cast<const SphereCollider*>(&other);
		// TODO: Implement Sphere to AABB collision normal
	}

	return normal.Normalise();
}

void AABB_Collider::Update()
{
	// Update the collider values based on where the hell the object is
	_minPoints = _transform->GetPosition() + _minExtent;
	_maxPoints = _minPoints + Vector(dx, dy, dz);

	dx = _maxPoints.x - _minPoints.x;
	dy = _maxPoints.y - _minPoints.y;
	dz = _maxPoints.z - _minPoints.z;
	_halfExtents = Vector(dx / 2, dy / 2, dz / 2);
}