#include "PhysicsModel.h"

PhysicsModel::PhysicsModel(Transform* transform)
{
	_transform = transform;
}

void PhysicsModel::Update(float deltaTime)
{
	Vector position = _transform->GetPosition();

	_velocity += _acceleration;

	position += _velocity * deltaTime;

	_transform->SetPosition(position);
}

Vector PhysicsModel::GetVelocity() const
{
	return _velocity;
}

Vector PhysicsModel::SetVelocity(const Vector& newVelocity)
{
	return _velocity = newVelocity;
}

Vector PhysicsModel::GetAcceleration() const
{
	return _acceleration;
}

Vector PhysicsModel::SetAcceleration(const Vector& newAcceleration)
{
	return _acceleration = newAcceleration;
}