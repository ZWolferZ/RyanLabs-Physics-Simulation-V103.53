#include "ParticleModel.h"

void ParticleModel::AddRelativeForce(const Vector& force, const Vector& point, float deltaTime)
{
	_mass = _transform->GetMass();
	AddForce(force);
}

void ParticleModel::Update(float deltaTime)
{
	_timeAlive += deltaTime;

	if (_timeAlive > _resetTime)
	{
		Reset();
	}

	float randomX = ((rand() % 100) / 100.0f - 0.5f) * 10.0f;
	float randomY = ((rand() % 100) / 100.0f - 0.5f) * 10.0f;
	float randomZ = ((rand() % 100) / 100.0f - 0.5f) * 10.0f;

	Vector force(randomX, randomY, randomZ);
	AddForce(force);

	PhysicsModel::Update(deltaTime);
}

void ParticleModel::Reset()
{
	_transform->Reset();
	_velocity = Vector(0.0f, 0.0f, 0.0f);
	_acceleration = Vector(0.0f, 0.0f, 0.0f);
	_timeAlive = 0.0f;
}