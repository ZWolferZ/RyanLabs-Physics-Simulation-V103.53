#pragma once
#pragma region Includes
// Include{s}
#include "GameObject.h"
#include "RigidBodyModel.h"
#pragma endregion

/// <summary>
/// Forward Declaration of Sphere Collider Class.
/// </summary>
class SphereCollider;

/// <summary>
/// Forward Declaration of AABB Collider Class.
/// </summary>
class AABBCollider;

/// <summary>
/// Base Class of the Collider.
/// </summary>
class Collider abstract
{
protected:
#pragma region Memember Variables
	// Holds the transform of the game object.
	Transform* _transform;
#pragma endregion

public:
#pragma region Constructor

	/// <summary>
	/// Base Constructor of the base collider class.
	/// </summary>
	/// <param name="tranform">Transform of the game object.</param>
	Collider(Transform* tranform) { _transform = tranform; }

#pragma endregion

#pragma region Collision Dectection
	/// <summary>
	/// Base Collider Overrider.
	/// </summary>
	/// <param name="other">Other collider that has been collied with.</param>
	/// <returns>Returns true if there has been a collision.</returns>
	virtual bool CollidesWith(Collider& other) = 0;

	/// <summary>
	/// Base Collider with the sphere collider class
	/// </summary>
	/// <param name="other">Other Sphere Collider that has been collided with.</param>
	/// <returns>Returns true if there has been a collision.</returns>
	virtual bool CollidesWith(SphereCollider& other) = 0;

	/// <summary>
	/// Base Collider with the AABB collider class
	/// </summary>
	/// <param name="other">Other AABB Collider that has been collided with.</param>
	/// <returns>Returns true if there has been a collision.</returns>
	virtual bool CollidesWith(AABBCollider& other) = 0;

	/// <summary>
	/// Identifies the type of collision that has taken place.
	/// </summary>
	/// <param name="gameObjectA">The first game object collider.</param>
	/// <param name="gameObjectB">The second game object collider.</param>
	void HandleCollision(const GameObject* gameObjectA, const GameObject* gameObjectB);
#pragma endregion

#pragma region Info Getters
	/// <summary>
	/// Returns the collision normal of the collision.
	/// </summary>
	/// <param name="other">Other base collider that gets downcast.</param>
	/// <returns>A vector of the collision normal.</returns>
	virtual Vector GetCollisionNormal(const Collider& other) = 0;

	/// <summary>
	/// Gets which type of collider an object is.
	/// </summary>
	/// <returns>Returns a string of which collider a game object has.</returns>
	virtual std::string GetType() const = 0;

	/// <summary>
	/// Shortcut to get the transform of the collider.
	/// </summary>
	/// <returns>The position of the collider.</returns>
	Vector GetPosition() const { return _transform->GetPosition(); }
#pragma endregion

#pragma region Updaters
	/// <summary>
	/// Handles collider movement.
	/// </summary>
	virtual void Update() = 0;
#pragma endregion
};
