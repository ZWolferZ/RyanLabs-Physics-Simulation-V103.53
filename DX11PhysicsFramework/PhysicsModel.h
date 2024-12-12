#pragma once
#include"Transform.h"
#include "Vector.h"
class PhysicsModel
{
public:

	PhysicsModel(Transform* transform);
	void Update(float deltaTime);

	Vector GetVelocity();
	Vector SetVelocity(const Vector& newVelocity);
	Vector GetAcceleration();
	Vector SetAcceleration(const Vector& newAcceleration);

protected:

	Transform* _transform;
	Vector _velocity;
	Vector _acceleration;
};
