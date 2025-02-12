#pragma once

#pragma region Includes
// Include{s}
#include "PhysicsModel.h"
#pragma endregion

/// <summary>
/// Represents a particle for physics simulation.
/// </summary>
class ParticleModel : public PhysicsModel
{
public:
#pragma region Constructor
	/// <summary>
	/// Initializes a new instance of the ParticleModel class.
	/// </summary>
	/// <param name="transform">The transform of the particle.</param>
	/// <param name="mass">The mass of the particle.</param>
	explicit ParticleModel(Transform* transform, float mass) : PhysicsModel(transform, mass)
	{
		_transform = transform;
		_mass = mass;
	}
#pragma endregion

#pragma region Update Methods

	/// <summary>
	/// Updates the particle.
	/// </summary>
	/// <param name="deltaTime">The time between frames.</param>
	void Update(float deltaTime) override;

#pragma endregion

#pragma region Particle Methods
	/// <summary>
	/// Adds a relative force to the particle at a specific point.
	/// </summary>
	/// <param name="force">The force to add.</param>
	/// <param name="point">The point at which the force is applied.</param>
	/// <param name="deltaTime">The time between frames.</param>
	void AddRelativeForce(const Vector& force, const Vector& point, float deltaTime) override;

	/// <summary>
	/// Resets the particle back to its original point and stops its velocity.
	/// </summary>
	void Reset();

	/// <summary>
	/// Sets the reset time for the particle.
	/// </summary>
	/// <param name="time">The reset time.</param>
	void SetResetTime(float time) override { _resetTime = time; }
#pragma endregion

#pragma region Member Variables
	float _resetTime = 10.0f;

private:
	float _timeAlive = 0.0f;
#pragma endregion
};