#include "PhysicsModel.h"

PhysicsModel::PhysicsModel(Transform* transform)
{
	_transform = transform;
}

void PhysicsModel::Update(float deltatime)
{
	Vector position = _transform->GetPosition();

	Vector trueAcceleration = _acceleration;

	_velocity += trueAcceleration;

	position += _velocity * deltatime;

	_transform->SetPosition(position);

}

Vector PhysicsModel::GetVelocity()
{
	return _velocity;
}

Vector PhysicsModel::SetVelocity(Vector newVelocity)
{
	return _velocity = newVelocity;
}

Vector PhysicsModel::GetAccerlation()
{
	return _acceleration;
}

Vector PhysicsModel::SetAccerlation(Vector newAccerlation)
{
	return _acceleration = newAccerlation;
}
