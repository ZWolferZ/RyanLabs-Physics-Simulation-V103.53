#pragma once
#include "PhysicsModel.h"

class RigidBodyModel : public PhysicsModel
{
public:
	explicit RigidBodyModel(Transform* transform, float mass) : PhysicsModel(transform, mass)
	{
		_transform = transform;
		_mass = mass;
	}
};
