#pragma once
#include"Transform.h"
#include "Vector.h"

class PhysicsModel
{
public:

	PhysicsModel(Transform* transform);

	PhysicsModel(Transform* transform, float mass);

	virtual void Update(float deltaTime);

	virtual Vector GetVelocity() const;
	virtual Vector SetVelocity(const Vector& newVelocity);
	virtual Vector GetAcceleration() const;
	virtual Vector SetAcceleration(const Vector& newAcceleration);
	virtual void AddForce(const Vector& force);
	virtual Vector GravityForce();
	virtual Vector Dragforce();
	virtual Vector FrictionForce();

	bool _simulateGravity = false;
	bool _simulateDrag = false;
	bool _simulateFriction = false;
	bool _constantAcceleration = false;
	Vector _gravity = { 0,-9.81f,0 };
	float _frictionScalar = 0.01f; //This scalar modify the force of friction it more gradual. (Easier to test)

protected:
	Transform* _transform;
	Vector _velocity;
	Vector _acceleration;
	Vector _netForce;
	float _mass = 1.0f;
	float _generalGroundCoefficient = 0.47f;
	float _airCoefficient = 0.1f;
	float _objectDensity = 1.225f;
	float _objectArea = 1.0f;
};
