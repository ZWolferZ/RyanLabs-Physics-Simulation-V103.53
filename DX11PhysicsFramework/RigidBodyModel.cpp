#include "RigidBodyModel.h"
#include "AABBCollider.h"
#include "SphereCollider.h"

void RigidBodyModel::AddRelativeForce(const Vector& force, const Vector& point, float deltaTime)
{
	if (_mass != _transform->GetMass())
	{
		_mass = _transform->GetMass();
		_tensorSet = false;
	}

	if (_mass == 0)
	{
		return;
	}

	if (!_tensorSet) SetInertiaTensor();

	Vector torque = Vector::Cross(point, force);

	_angularVelocity = _transform->GetAngularVelocity() + CalculateAngularVelocity(torque, deltaTime);

	_transform->SetAngularVelocity(_angularVelocity);
}

void RigidBodyModel::SetInertiaTensor()
{
	if (_collider != nullptr && _collider->GetType() == "AABB_Collider")
	{
		auto box = dynamic_cast<const AABBCollider*>(_collider);
		float x = box->GetHalfExtents().x;
		float y = box->GetHalfExtents().y;
		float z = box->GetHalfExtents().z;
		float x2 = x * x;
		float y2 = y * y;
		float z2 = z * z;
		_inertiaTensor._11 = (1.0f / 12.0f) * _mass * (y2 + z2);
		_inertiaTensor._22 = (1.0f / 12.0f) * _mass * (x2 + z2);
		_inertiaTensor._33 = (1.0f / 12.0f) * _mass * (x2 + y2);

		_tensorSet = true;
	}
	else if (_collider != nullptr && _collider->GetType() == "SphereCollider")
	{
		auto sphere = dynamic_cast<const SphereCollider*>(_collider);
		float radius = sphere->GetRadius();
		float radius2 = radius * radius;
		float inertia = (2.0f / 5.0f) * _mass * radius2;
		_inertiaTensor._11 = inertia;
		_inertiaTensor._22 = inertia;
		_inertiaTensor._33 = inertia;

		_tensorSet = true;
	}
}

Vector RigidBodyModel::CalculateAngularVelocity(Vector torque, float deltaTime) const
{
	XMMATRIX inverseTensor = XMMatrixInverse(nullptr, XMLoadFloat3x3(&_inertiaTensor));

	XMVECTOR angularAcceleration = XMVector3Transform(XMVectorSet(torque.x, torque.y, torque.z, 0.0f), inverseTensor);

	auto angularAccelerationVector = Vector(
		XMVectorGetX(angularAcceleration),
		XMVectorGetY(angularAcceleration),
		XMVectorGetZ(angularAcceleration));

	Vector velocity = angularAccelerationVector * deltaTime;

	return velocity;
}