#pragma once
#include"Transform.h"
#include "Vector.h"

class PhysicsModel
{
public:
	PhysicsModel(Transform* transform);
	void Update(float deltaTime);

	Vector GetVelocity() const;
	Vector SetVelocity(const Vector& newVelocity);
	Vector GetAcceleration() const;
	Vector SetAcceleration(const Vector& newAcceleration);

protected:
	Transform* _transform;
	Vector _velocity;
	Vector _acceleration;
};
