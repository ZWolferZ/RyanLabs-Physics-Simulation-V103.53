#include "RigidBodyModel.h"

void RigidBodyModel::AddRelativeForce(const Vector& force, const Vector& point)
{
	Vector torque = Vector::Cross(point - _transform->GetPosition(), force);
	AddForce(torque);
}