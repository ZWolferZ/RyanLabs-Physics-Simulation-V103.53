#pragma region Includes
// Include{s}
#include "SphereCollider.h"
#include "AABBCollider.h"
#pragma endregion

#pragma region Collision Methods
bool SphereCollider::CollidesWith(SphereCollider& other)
{
	// Calculate the combined radii
	float combinedradii = radius + other.radius;

	// Calculate the distance between the two spheres
	Vector distance = GetPosition() - other.GetPosition();

	// Check for overlap
	if (combinedradii < distance.Magnitude())
	{
		return false;
	}

	return true;
}

bool SphereCollider::CollidesWith(AABBCollider& other)
{
	// Calculate the closest point on the AABB to the sphere
	Vector closestPoint = Vector(
		max(other.GetMinPoints().x, min(GetPosition().x, other.GetMaxPoints().x)),
		max(other.GetMinPoints().y, min(GetPosition().y, other.GetMaxPoints().y)),
		max(other.GetMinPoints().z, min(GetPosition().z, other.GetMaxPoints().z))
	);

	// Calculate the distance between the sphere and the closest point
	Vector distance = GetPosition() - closestPoint;

	float distance2 = distance.Magnitude() * distance.Magnitude();

	// Check for overlap
	return distance2 < (GetRadius() * GetRadius());
}
#pragma endregion

#pragma region Getter
Vector SphereCollider::GetCollisionNormal(const Collider& other)
{
	Vector normal = { 0, 0, 0 };

	if (auto otherSphere = dynamic_cast<const SphereCollider*>(&other)) // Downcast to sphere collider
	{
		// This is super based compared to the AABB one
		normal = this->GetPosition() - otherSphere->GetPosition();
	}
	else if (auto otherAABB = dynamic_cast<const AABBCollider*>(&other)) // Downcast to AABB collider
	{
		// Get the closest point on the AABB to the sphere
		Vector closestPoint = Vector(
			max(otherAABB->GetMinPoints().x, min(this->GetPosition().x, otherAABB->GetMaxPoints().x)),
			max(otherAABB->GetMinPoints().y, min(this->GetPosition().y, otherAABB->GetMaxPoints().y)),
			max(otherAABB->GetMinPoints().z, min(this->GetPosition().z, otherAABB->GetMaxPoints().z))
		);

		normal = this->GetPosition() - closestPoint;
	}

	return normal.Normalise();
}
#pragma endregion