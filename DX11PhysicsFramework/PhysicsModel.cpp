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

Vector PhysicsModel::GetIntegratedPosition(const float deltaTime) const
{
	Vector integratedPosition;

	switch (GetIntegrationMethod())
	{
	case ExplicitEuler:
		integratedPosition = CalculateExplicitEuler(deltaTime);
		break;
	case SemiImplicitEuler:
		integratedPosition = CalculateSemiImplicitEuler(deltaTime);
		break;
	case Verlet:
		integratedPosition = CalculateVerlet(deltaTime);
		break;
	case StormerVerlet:
		integratedPosition = CalculateStormerVerlet(deltaTime);
		break;
	case RK4:
		integratedPosition = CalculateRK4(deltaTime);
		break;

	default: break;
	}

	return integratedPosition;
}

string PhysicsModel::GetIntegrationMethodName() const
{
	switch (_integrationMethod)
	{
	case ExplicitEuler: return "Explicit Euler";
	case SemiImplicitEuler: return "Semi-Implicit Euler";
	case Verlet: return "Verlet";
	case StormerVerlet: return "Stormer Verlet";
	case RK4: return "RK4";
	default: return "Unknown";
	}
}

void PhysicsModel::SetIntegrationMethod(int method)
{
	switch (method)
	{
	case 0: _integrationMethod = ExplicitEuler; break;
	case 1: _integrationMethod = SemiImplicitEuler; break;
	case 2: _integrationMethod = Verlet; break;
	case 3: _integrationMethod = StormerVerlet; break;
	case 4: _integrationMethod = RK4; break;
	default: break;
	}
}

Vector PhysicsModel::CalculateExplicitEuler(float deltaTime) const
{
	return _transform->GetPosition() + GetVelocity() * deltaTime;
}

Vector PhysicsModel::CalculateSemiImplicitEuler(float deltaTime) const
{
	Vector newVelocity = GetVelocity() + (GetAcceleration() * deltaTime);

	return _transform->GetPosition() + newVelocity * deltaTime;
}

Vector PhysicsModel::CalculateVerlet(float deltaTime) const
{
	return _transform->GetPosition() * 2 - _transform->GetPreviousPosition() + (_acceleration * deltaTime * deltaTime);
}

Vector PhysicsModel::CalculateStormerVerlet(float deltaTime) const
{
	Vector newVelocity = GetVelocity() + (GetAcceleration() * deltaTime * 0.5f);
	Vector newPosition = _transform->GetPosition() + (newVelocity * deltaTime);
	newVelocity += (GetAcceleration() * deltaTime * 0.5f);

	return newPosition;
}

Vector PhysicsModel::CalculateRK4(const float deltaTime) const
{
	Vector currentVelocity = GetVelocity();
	Vector currentAcceleration = GetAcceleration();

	Vector K1 = currentVelocity * deltaTime;
	Vector K2 = (currentVelocity + currentAcceleration * deltaTime / 2.0f) * deltaTime;
	Vector K3 = (currentVelocity + currentAcceleration * deltaTime / 2.0f) * deltaTime;
	Vector K4 = (currentVelocity + currentAcceleration * deltaTime) * deltaTime;

	return _transform->GetPosition() + (K1 + (K2 * 2.0f) + (K3 * 2.0f) + K4) / 6.0f;
}