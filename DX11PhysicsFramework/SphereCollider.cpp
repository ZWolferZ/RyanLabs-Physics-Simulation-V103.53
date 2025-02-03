#include "SphereCollider.h"
#include "AABBCollider.h"

bool SphereCollider::CollidesWith(SphereCollider& other)
{
	float combinedradii = this->radius + other.radius;

	Vector distance = this->GetPosition() - other.GetPosition();

	// Check for overlap
	if (combinedradii < distance.Magnitude())
	{
		return false;
	}

	return true;
}

bool SphereCollider::CollidesWith(AABBCollider& other)
{
	auto closestPoint = Vector(
		max(other.GetMinPoints().x, min(GetPosition().x, other.GetMaxPoints().x)),
		max(other.GetMinPoints().y, min(GetPosition().y, other.GetMaxPoints().y)),
		max(other.GetMinPoints().z, min(GetPosition().z, other.GetMaxPoints().z))
	);

	Vector distance = GetPosition() - closestPoint;
	float distance2 = distance.Magnitude() * distance.Magnitude();

	return distance2 < (GetRadius() * GetRadius());
}

Vector SphereCollider::GetCollisionNormal(const Collider& other)
{
	Vector normal = { 0, 0, 0 };

	if (auto otherSphere = dynamic_cast<const SphereCollider*>(&other))
	{
		// This is super based compared to the AABB one
		normal = this->GetPosition() - otherSphere->GetPosition();
	}
	else if (auto otherAABB = dynamic_cast<const AABBCollider*>(&other))
	{
		// Sphere to AABB
		auto closestPoint = Vector(
			max(otherAABB->GetMinPoints().x, min(this->GetPosition().x, otherAABB->GetMaxPoints().x)),
			max(otherAABB->GetMinPoints().y, min(this->GetPosition().y, otherAABB->GetMaxPoints().y)),
			max(otherAABB->GetMinPoints().z, min(this->GetPosition().z, otherAABB->GetMaxPoints().z))
		);

		normal = this->GetPosition() - closestPoint;
	}

	return normal.Normalise();
}