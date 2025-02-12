#pragma once

#pragma region Includes
// Include{s}
#include "Transform.h"
#include "Vector.h"
#pragma endregion

/// <summary>
/// Forward Declaration for the Base Collider Class.
/// </summary>
class Collider;

/// <summary>
/// Forward Declaration for the Sphere Collider Class.
/// </summary>
class SphereCollider;

/// <summary>
/// Represents a physics model for simulation.
/// </summary>
class PhysicsModel
{
public:
#pragma region Constructors
	/// <summary>
	/// Initializes a new instance of the PhysicsModel class as base constructor.
	/// </summary>
	/// <param name="transform">The transform of the object.</param>
	PhysicsModel(Transform* transform);

	/// <summary>
	/// Initializes a new instance of the PhysicsModel class with mass.
	/// </summary>
	/// <param name="transform">The transform of the object.</param>
	/// <param name="mass">The mass of the object.</param>
	PhysicsModel(Transform* transform, float mass);
#pragma endregion

#pragma region Update Methods
	/// <summary>
	/// Updates the physics object.
	/// </summary>
	/// <param name="deltaTime">The time between frames.</param>
	virtual void Update(float deltaTime);
#pragma endregion

#pragma region Physics Methods
	/// <summary>
	/// Adds a force to the object.
	/// </summary>
	/// <param name="force">The force to add.</param>
	virtual void AddForce(const Vector& force);

	/// <summary>
	/// Calculates the gravity force.
	/// </summary>
	/// <returns>The gravity force.</returns>
	Vector GravityForce();

	/// <summary>
	/// Calculates the drag force.
	/// </summary>
	/// <returns>The drag force.</returns>
	virtual Vector Dragforce();

	/// <summary>
	/// Calculates the friction force.
	/// </summary>
	/// <returns>The friction force.</returns>
	virtual Vector FrictionForce();

	/// <summary>
	/// Applies an impulse to the object.
	/// </summary>
	/// <param name="impulse">The impulse to apply.</param>
	void ApplyImpulse(Vector impulse) { _velocity += impulse; }

	/// <summary>
	/// Adds a relative force to the object at a specific point.
	/// </summary>
	/// <param name="force">The force to add.</param>
	/// <param name="point">The point at which the force is applied.</param>
	/// <param name="deltaTime">The time between frames.</param>
	virtual void AddRelativeForce(const Vector& force, const Vector& point, float deltaTime) = 0;
#pragma endregion

#pragma region Getters
	/// <summary>
	/// Gets the integrated position of the object.
	/// </summary>
	/// <param name="deltaTime">The time between frames.</param>
	/// <returns>The integrated position.</returns>
	Vector GetIntegratedPosition(float deltaTime) const;

	/// <summary>
	/// Gets the name of the integration method.
	/// </summary>
	/// <returns>The name of the integration method.</returns>
	string GetIntegrationMethodName() const;

	/// <summary>
	/// Gets the velocity of the object.
	/// </summary>
	/// <returns>The velocity of the object.</returns>
	virtual Vector GetVelocity() const;

	/// <summary>
	/// Gets the acceleration of the object.
	/// </summary>
	/// <returns>The acceleration of the object.</returns>
	virtual Vector GetAcceleration() const;

	/// <summary>
	/// Checks if the model is collideable.
	/// </summary>
	/// <returns>True if the model is collideable, otherwise false.</returns>
	bool IsCollideable() const { return _collider != nullptr; }

	/// <summary>
	/// Gets the collider of the object.
	/// </summary>
	/// <returns>The collider of the object.</returns>
	Collider* GetCollider() const { return _collider; }

	/// <summary>
	/// Gets the mass of the object.
	/// </summary>
	/// <returns>The mass of the object.</returns>
	float GetMass() { return _mass; }

	/// <summary>
	/// Gets the integration method of the object.
	/// </summary>
	/// <returns>The current integration method.</returns>
	IntegrationMethods GetIntegrationMethod() const { return _integrationMethod; }
#pragma endregion

#pragma region Setters
	/// <summary>
	/// Sets the reset time for the object (Particle Override).
	/// </summary>
	/// <param name="time">The reset time.</param>
	virtual void SetResetTime(float time) = 0;

	/// <summary>
	/// Sets the integration method of the object.
	/// </summary>
	/// <param name="method">The integration method to set.</param>
	void SetIntegrationMethod(int method);

	/// <summary>
	/// Sets the velocity of the object.
	/// </summary>
	/// <param name="newVelocity">The new velocity of the object.</param>
	/// <returns>The updated velocity.</returns>
	virtual Vector SetVelocity(const Vector& newVelocity);

	/// <summary>
	/// Sets the acceleration of the object.
	/// </summary>
	/// <param name="newAcceleration">The new acceleration of the object.</param>
	/// <returns>The updated acceleration of the object.</returns>
	virtual Vector SetAcceleration(const Vector& newAcceleration);

	/// <summary>
	/// Sets the collider of the object.
	/// </summary>
	/// <param name="collider">The collider to set.</param>
	/// <returns>The updated collider.</returns>
	Collider* SetCollider(Collider* collider) { return _collider = collider; }
#pragma endregion

#pragma region Member Variables
	bool _simulateGravity = false;
	bool _simulateDrag = false;
	bool _simulateFriction = false;
	bool _constantAcceleration = false;
	Vector _gravity = { 0, -9.81f, 0 };
	float _frictionScalar = 0.1f; // This scalar modifies the force of friction to be more gradual. (Easier to test)

protected:
	Transform* _transform = nullptr;
	Collider* _collider = nullptr;
	Vector _velocity;
	Vector _acceleration;
	Vector _netForce;
	float _mass = 1.0f; // Mass of the object unless changed when the object is instantiated
	float _generalGroundCoefficient = 0.65f; // Stone on Stone friction coefficient
	float _airCoefficient = 0.3f; // Air drag coefficient
	float _fluidDensity = 1.225f; // Air density
	float _objectArea = 1.0f; // Cross-section area of the object
	IntegrationMethods _integrationMethod = RK4; // Set the starting integration method to RK4
#pragma endregion

#pragma region Private Methods
	/// <summary>
	/// Calculates the explicit Euler integration.
	/// </summary>
	/// <param name="deltaTime">The time between frames.</param>
	/// <returns>The integrated position.</returns>
	Vector CalculateExplicitEuler(float deltaTime) const;

	/// <summary>
	/// Calculates the semi-implicit Euler integration.
	/// </summary>
	/// <param name="deltaTime">The time between frames.</param>
	/// <returns>The integrated position.</returns>
	Vector CalculateSemiImplicitEuler(float deltaTime) const;

	/// <summary>
	/// Calculates the Verlet integration.
	/// </summary>
	/// <param name="deltaTime">The time between frames.</param>
	/// <returns>The integrated position.</returns>
	Vector CalculateVerlet(float deltaTime) const;

	/// <summary>
	/// Calculates the Stormer-Verlet integration.
	/// </summary>
	/// <param name="deltaTime">The time between frames.</param>
	/// <returns>The integrated position.</returns>
	Vector CalculateStormerVerlet(float deltaTime) const;

	/// <summary>
	/// Calculates the RK4 integration.
	/// </summary>
	/// <param name="deltaTime">The time between frames.</param>
	/// <returns>The integrated position.</returns>
	Vector CalculateRK4(float deltaTime) const;
#pragma endregion
};