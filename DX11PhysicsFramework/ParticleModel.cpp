#pragma region Includes
// Include{s}
#include "ParticleModel.h"
#pragma endregion

#pragma region ParticleModel Methods
void ParticleModel::AddRelativeForce(const Vector& force, const Vector& point, float deltaTime)
{
	AddForce(force);
}

void ParticleModel::Update(float deltaTime)
{
	_timeAlive += deltaTime;

	if (_timeAlive > _resetTime)
	{
		Reset();
	}

	// Get a random force with a limit of 10
	float randomX = ((rand() % 100) / 100.0f - 0.5f) * 10.0f;
	float randomY = ((rand() % 100) / 100.0f - 0.5f) * 10.0f;
	float randomZ = ((rand() % 100) / 100.0f - 0.5f) * 10.0f;

	// Add the random force to the particle
	AddForce(Vector{ randomX,randomY,randomZ });

	// Update the particle
	PhysicsModel::Update(deltaTime);
}

void ParticleModel::Reset()
{
	_transform->Reset();
	_velocity = Vector(0.0f, 0.0f, 0.0f);
	_acceleration = Vector(0.0f, 0.0f, 0.0f);
	_timeAlive = 0.0f;
}
#pragma endregion