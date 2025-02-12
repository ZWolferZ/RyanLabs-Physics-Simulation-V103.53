#pragma region Includes
// Include{s}
#include "RigidBodyModel.h"
#include "AABBCollider.h"
#include "SphereCollider.h"
#pragma endregion

#pragma region Rotational Force Method
void RigidBodyModel::AddRelativeForce(const Vector& force, const Vector& point, float deltaTime)
{
	// If the mass has changed then the inertia tensor needs to be recalculated
	if (_mass != _transform->GetMass())
	{
		_mass = _transform->GetMass();
		_tensorSet = false;
	}

	// Check if the mass is 0 and return if it is
	if (_mass == 0)
	{
		return;
	}

	// If the inertia tensor has not been set then set it
	if (!_tensorSet) SetInertiaTensor();

	// Calculate the torque
	Vector torque = Vector::Cross(point, force);

	// Calculate the angular velocity
	_angularVelocity = _transform->GetAngularVelocity() + CalculateAngularVelocity(torque, deltaTime);

	_transform->SetAngularVelocity(_angularVelocity);
}
#pragma endregion

#pragma region Helper Methods
void RigidBodyModel::SetInertiaTensor()
{
	// Calculate the inertia tensor based on the collider type

	// AABB Collider
	if (_collider != nullptr && _collider->GetType() == "AABB_Collider")
	{
		auto box = dynamic_cast<const AABBCollider*>(_collider); // Downcast the collider to an AABB Collider
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
	else if (_collider != nullptr && _collider->GetType() == "SphereCollider") // Sphere Collider
	{
		auto sphere = dynamic_cast<const SphereCollider*>(_collider); // Downcast the collider to a Sphere Collider
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
	// Calculate the inverse of the inertia tensor
	XMMATRIX inverseTensor = XMMatrixInverse(nullptr, XMLoadFloat3x3(&_inertiaTensor));

	// Calculate the angular acceleration
	XMVECTOR angularAcceleration = XMVector3Transform(XMVectorSet(torque.x, torque.y, torque.z, 0.0f), inverseTensor);

	// Convert the angular acceleration to one of my vectors
	Vector angularAccelerationVector = Vector(
		XMVectorGetX(angularAcceleration),
		XMVectorGetY(angularAcceleration),
		XMVectorGetZ(angularAcceleration));

	// Calculate the angular velocity
	Vector velocity = angularAccelerationVector * deltaTime;

	return velocity;
}
#pragma endregion