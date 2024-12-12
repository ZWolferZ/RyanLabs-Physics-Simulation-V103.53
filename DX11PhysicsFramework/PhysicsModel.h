#pragma once
#include"Transform.h"
#include "Vector.h"
class PhysicsModel
{
public:

	PhysicsModel(Transform* transform);
	void Update(float deltatime);

	Vector GetVelocity();
	Vector SetVelocity(Vector newVelocity);
	Vector GetAccerlation();
	Vector SetAccerlation(Vector newAccerlation);

protected:

	Transform* _transform;
	Vector _velocity;
	Vector _acceleration;

};

