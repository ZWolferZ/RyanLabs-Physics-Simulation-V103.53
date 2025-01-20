#pragma once
#include"Transform.h"
#include "Vector.h"

class Collider;
class SphereCollider;

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
	Vector GravityForce();
	virtual Vector Dragforce();
	virtual Vector FrictionForce();
	void ApplyImpulse(Vector impulse) { _velocity += impulse; }
	bool IsCollideable() const { return _collider != nullptr; }
	Collider* GetCollider() const { return _collider; }
	Collider* SetCollider(Collider* collider) { return _collider = collider; }
	float GetMass() { return _mass; }
	virtual void AddRelativeForce(const Vector& force, const Vector& point) = 0;

	bool _simulateGravity = false;
	bool _simulateDrag = false;
	bool _simulateFriction = false;
	bool _constantAcceleration = false;
	Vector _gravity = { 0, -9.81f, 0 };
	float _frictionScalar = 0.1f; //This scalar modify the force of friction it more gradual. (Easier to test)

protected:
	Transform* _transform = nullptr;
	Collider* _collider = nullptr;
	Vector _velocity;
	Vector _acceleration;
	Vector _netForce;
	float _mass = 1.0f; // Mass of the object unless changed when object is instantiated
	float _generalGroundCoefficient = 0.65f; // Stone on Stone friction coefficient
	float _airCoefficient = 0.3f; // Air drag coefficient
	float _fluidDensity = 1.225f; // Air density
	float _objectArea = 1.0f; // Cross-section area of the object
};
