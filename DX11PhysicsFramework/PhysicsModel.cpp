#include "PhysicsModel.h"
#include "Collider.h"

PhysicsModel::PhysicsModel(Transform* transform)
{
	_transform = transform;
}

PhysicsModel::PhysicsModel(Transform* transform, float mass)
{
	_transform = transform;
	_mass = mass;
}

void PhysicsModel::Update(float deltaTime)
{
	// Calls the AABB collider update function
	// Sphere collider does need an update function but the functionality is there I guess
	if (IsCollideable())
	{
		_collider->Update();
	}

	if (_simulateGravity)
	{
		_netForce += GravityForce();
	}

	if (_simulateDrag)
	{
		_netForce += Dragforce();
	}

	if (_simulateFriction)
	{
		_netForce += FrictionForce();
	}

	Vector position = _transform->GetPosition();

	if (!_constantAcceleration)
	{
		_acceleration += _netForce / _mass;
	}

	_velocity += _acceleration * deltaTime;

	Debug::Debug_WriteNumber("Gravity", _velocity.y);

	position += _velocity * deltaTime;

	_transform->SetPosition(position);

	if (!_constantAcceleration)
	{
		// Reset net force and acceleration
		_netForce = Vector(0.0f, 0.0f, 0.0f);
		_acceleration = Vector(0.0f, 0.0f, 0.0f);
	}

	if (_velocity.Magnitude() < 0.035f)
	{
		_velocity = Vector(0.0f, 0.0f, 0.0f);
	}

	if (_velocity.y < -9.81f)
	{
		_velocity = Vector(0.0f, -9.81f, 0.0f);
	}
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

void PhysicsModel::AddForce(const Vector& force)
{
	_netForce = _netForce + force;
}

Vector PhysicsModel::GravityForce()
{
	Vector gravityForce = _gravity * _mass;

	return gravityForce;
}

Vector PhysicsModel::Dragforce()
{
	float dragForce = 0.5f * _airCoefficient * _fluidDensity * _objectArea * _velocity.Magnitude() * _velocity.
		Magnitude();

	Vector drag = _velocity.Normalise() * -1.0f;

	return drag *= dragForce;
}

Vector PhysicsModel::FrictionForce()
{
	float frictionForceMagnitude = _generalGroundCoefficient * _mass;

	Vector friction = _velocity.Normalise() * -frictionForceMagnitude;

	friction *= _velocity.Magnitude();

	return friction;
}