#include "SphereCollider.h"

#include "AABB_Collider.h"

bool SphereCollider::CollidesWith(SphereCollider& other)
{
	float combinedradii = this->radius + other.radius;

	Vector distance = this->GetPosition() - other.GetPosition();

	if (combinedradii < distance.Magnitude())
	{
		return false;
	}

	return true;
}

bool SphereCollider::CollidesWith(AABB_Collider& other)
{
	// TODO: Implement Sphere to AABB collision detection
	return false;
}

Vector SphereCollider::GetCollisionNormal(const Collider& other)
{
	Vector normal = { 0, 0, 0 };

	// SHADOW WIZARD MONEY GANG, WE LOVE CASTING SPELLS HERE (This is a joke about casting, I give up)
	if (auto otherSphere = dynamic_cast<const SphereCollider*>(&other))
	{
		normal = this->GetPosition() - otherSphere->GetPosition();
	}
	else
	{
		// TODO: Implement Sphere to AABB collision normal
		auto otherAABB = dynamic_cast<const AABB_Collider*>(&other);
	}

	return normal.Normalise();
}