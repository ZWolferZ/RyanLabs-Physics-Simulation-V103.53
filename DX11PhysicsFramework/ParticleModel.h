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

	void AddRelativeForce(const Vector& force, const Vector& point, float deltaTime) override;

	void Update(float deltaTime) override;

	void Reset();

	void SetResetTime(float time) override { _resetTime = time; }

	float _resetTime = 10.0f;

private:

	float _timeAlive = 0.0f;
};
