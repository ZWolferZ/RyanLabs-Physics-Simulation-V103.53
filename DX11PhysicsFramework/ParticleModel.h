#pragma once
#include "PhysicsModel.h"

class ParticleModel : public PhysicsModel
{
public:
	explicit ParticleModel(Transform* transform, float mass) : PhysicsModel(transform, mass)
	{
		_transform = transform;
		_mass = mass;
	}

	void AddRelativeForce(const Vector& force, const Vector& point) override;
};
