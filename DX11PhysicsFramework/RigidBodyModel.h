#pragma once

#pragma region Includes
// Include{s}
#include "PhysicsModel.h"
#pragma endregion

/// <summary>
/// Represents a rigid body model for physics simulation.
/// </summary>
class RigidBodyModel : public PhysicsModel
{
public:
#pragma region Member Variables
	XMFLOAT3X3 _inertiaTensor;
	Vector _angularVelocity = { 0, 0, 0 };
	bool _tensorSet = false;
#pragma endregion

#pragma region Constructor
	/// <summary>
	/// Initializes a new instance of the RigidBodyModel class.
	/// </summary>
	/// <param name="transform">The transform of the rigid body.</param>
	/// <param name="mass">The mass of the rigid body.</param>
	explicit RigidBodyModel(Transform* transform, float mass) : PhysicsModel(transform, mass)
	{
		_transform = transform;
		_mass = mass;

		// This is funny to me
		_inertiaTensor = { 0, 0, 0, 0, 0, 0, 0, 0, 0 };
	}
#pragma endregion

#pragma region RigidBody Methods
	/// <summary>
	/// Adds a relative force to the rigid body at a specific point.
	/// </summary>
	/// <param name="force">The force to add.</param>
	/// <param name="point">The point at which the force is applied.</param>
	/// <param name="deltaTime">The time between frames</param>
	void AddRelativeForce(const Vector& force, const Vector& point, float deltaTime) override;

	/// <summary>
	/// Calculates the angular velocity of the rigid body.
	/// </summary>
	/// <param name="torque">The torque applied to the rigid body.</param>
	/// <param name="deltaTime">The time between frames</param>
	/// <returns>The calculated angular velocity.</returns>
	Vector CalculateAngularVelocity(Vector torque, float deltaTime) const;

#pragma endregion

#pragma region Setters
	/// <summary>
	/// Sets the inertia tensor of the rigid body to either a sphere tensor or cube tensor.
	/// </summary>
	void SetInertiaTensor();

	/// <summary>
	/// Unimplemented.
	/// </summary>
	/// <param name="time">The reset time.</param>
	void SetResetTime(float time) override {};
#pragma endregion
};