#include "SphereCollider.h"

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

Vector SphereCollider::GetCollisionNormal(const Collider& other)
{
	Vector collisionNormal = this->GetPosition() - other.GetPosition();

	return collisionNormal.Normalise();
}