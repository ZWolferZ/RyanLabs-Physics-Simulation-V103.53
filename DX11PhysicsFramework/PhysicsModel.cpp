#include "PhysicsModel.h"

PhysicsModel::PhysicsModel(Transform* transform)
{
	_transform = transform;
}

void PhysicsModel::Update(float deltaTime)
{
	Vector position = _transform->GetPosition();

	Vector trueAcceleration = _acceleration;

	_velocity += trueAcceleration;

	position += _velocity * deltaTime;

	_transform->SetPosition(position);
}

Vector PhysicsModel::GetVelocity()
{
	return _velocity;
}

Vector PhysicsModel::SetVelocity(const Vector& newVelocity)
{
	return _velocity = newVelocity;
}

Vector PhysicsModel::GetAcceleration()
{
	return _acceleration;
}

Vector PhysicsModel::SetAcceleration(const Vector& newAcceleration)
{
	return _acceleration = newAcceleration;
}