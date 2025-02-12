#pragma once

#pragma region Includes
// Include{s}
#include "Collider.h"
#pragma endregion

/// <summary>
/// Represents a sphere collider for collision detection.
/// </summary>
class SphereCollider : public Collider
{
public:
#pragma region Constructor
	/// <summary>
	/// Initializes a new instance of the SphereCollider class.
	/// </summary>
	/// <param name="transform">The transform of the sphere collider.</param>
	/// <param name="r">The radius of the sphere collider.</param>
	SphereCollider(Transform* transform, float r) : Collider(transform) { radius = r; }
#pragma endregion

#pragma region Collision Methods
	/// <summary>
	/// Checks for collision with another collider.
	/// </summary>
	/// <param name="other">The other collider.</param>
	/// <returns>True if there is a collision, otherwise false.</returns>
	bool CollidesWith(Collider& other) override { return other.CollidesWith(*this); }

	/// <summary>
	/// Checks for collision with another sphere collider.
	/// </summary>
	/// <param name="other">The other sphere collider.</param>
	/// <returns>True if there is a collision, otherwise false.</returns>
	bool CollidesWith(SphereCollider& other) override;

	/// <summary>
	/// Checks for collision with an AABB collider.
	/// </summary>
	/// <param name="other">The AABB collider.</param>
	/// <returns>True if there is a collision, otherwise false.</returns>
	bool CollidesWith(AABBCollider& other) override;

	/// <summary>
	/// Gets the collision normal with another collider.
	/// </summary>
	/// <param name="other">The other collider.</param>
	/// <returns>The collision normal vector.</returns>
	Vector GetCollisionNormal(const Collider& other) override;
#pragma endregion

#pragma region Getters
	/// <summary>
	/// Gets the type of the collider.
	/// </summary>
	/// <returns>The type of the collider as a string.</returns>
	std::string GetType() const override { return "SphereCollider"; }

	/// <summary>
	/// Gets the radius of the sphere collider.
	/// </summary>
	/// <returns>The radius of the sphere collider.</returns>
	float GetRadius() const { return radius; }
#pragma endregion

#pragma region Update Methods
	/// <summary>
	/// Updates the sphere collider.
	/// </summary>
	void Update() override {}
#pragma endregion

private:
#pragma region Member Variables
	float radius = 1.0f;
#pragma endregion
};