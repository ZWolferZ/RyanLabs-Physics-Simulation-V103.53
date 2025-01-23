#include "RigidBodyModel.h"
#include "AABB_Collider.h"
#include "SphereCollider.h"

void RigidBodyModel::AddRelativeForce(const Vector& force, const Vector& point)
{
	Vector torque = Vector::Cross(point - _transform->GetPosition(), force);
	AddForce(torque);
}

void RigidBodyModel::SetInertiaTensor()
{
	if (auto box = dynamic_cast<const AABB_Collider*>(_collider))
	{

	}
	else if (auto otherSphere = dynamic_cast<const SphereCollider*>(_collider))
	{

	}
}
