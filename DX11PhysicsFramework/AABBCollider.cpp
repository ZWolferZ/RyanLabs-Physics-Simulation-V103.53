#include "AABBCollider.h"
#include "SphereCollider.h"

bool AABBCollider::CollidesWith(SphereCollider& other)
{
	auto closestPoint = Vector(
		max(GetMinPoints().x, min(other.GetPosition().x, GetMaxPoints().x)),
		max(GetMinPoints().y, min(other.GetPosition().y, GetMaxPoints().y)),
		max(GetMinPoints().z, min(other.GetPosition().z, GetMaxPoints().z))
	);

	Vector distance = other.GetPosition() - closestPoint;
	float distance2 = distance.Magnitude() * distance.Magnitude();

	// Check for overlap
	return distance2 < (other.GetRadius() * other.GetRadius());
}

bool AABBCollider::CollidesWith(AABBCollider& other)
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

Vector AABBCollider::GetCollisionNormal(const Collider& other)
{
	Vector normal = { 0.0f, 0.0f, 0.0f };

	// SHADOW WIZARD MONEY GANG, WE LOVE CASTING SPELLS HERE (This is a joke about casting, I give up)
	if (auto otherAABB = dynamic_cast<const AABBCollider*>(&other))
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
			Vector(-1.0f, 0.0f, 0.0f), // Left face
			Vector(1.0f, 0.0f, 0.0f), // Right face
			Vector(0.0f, -1.0f, 0.0f), // Bottom face
			Vector(0.0f, 1.0f, 0.0f), // Top face
			Vector(0.0f, 0.0f, -1.0f), // Back face
			Vector(0.0f, 0.0f, 1.0f) // Front face
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
	else if (auto otherSphere = dynamic_cast<const SphereCollider*>(&other))
	{
		// AABB to Sphere
		auto closestPoint = Vector(
			max(_minPoints.x, min(otherSphere->GetPosition().x, _minPoints.x + _halfExtents.x * 2.0f)),
			max(_minPoints.y, min(otherSphere->GetPosition().y, _minPoints.y + _halfExtents.y * 2.0f)),
			max(_minPoints.z, min(otherSphere->GetPosition().z, _minPoints.z + _halfExtents.z * 2.0f))
		);

		normal = otherSphere->GetPosition() - closestPoint;
	}

	return normal.Normalise();
}

void AABBCollider::Update()
{
	// Update the collider values based on where the hell the object is
	_minPoints = _transform->GetPosition() + _minExtent;
	_maxPoints = _minPoints + Vector(dx, dy, dz);

	dx = _maxPoints.x - _minPoints.x;
	dy = _maxPoints.y - _minPoints.y;
	dz = _maxPoints.z - _minPoints.z;
	_halfExtents = Vector(dx / 2.0f, dy / 2.0f, dz / 2.0f);
}