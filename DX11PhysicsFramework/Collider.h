#pragma once
#pragma region Includes
// Include{s}
#include "GameObject.h"
#include "RigidBodyModel.h"
#pragma endregion

/// <summary>
/// Foward Declaration of Sphere Collider Class
/// </summary>
class SphereCollider;

/// <summary>
/// Forward Declartion of AABB Collider Class
/// </summary>
class AABBCollider;

/// <summary>
/// Base Class of the Collider
/// </summary>
class Collider abstract
{
protected:
#pragma region Memember Variables
	// Holds the transform of the game object
	Transform* _transform;
#pragma endregion

public:
#pragma region Constructor

	/// <summary>
	/// Base Constructor of the base collider class
	/// </summary>
	/// <param name="tranform">Transform of the gameobject</param>
	Collider(Transform* tranform) { _transform = tranform; }

#pragma endregion

#pragma region Collision Dectection
	/// <summary>
	/// Base Collider Overrider
	/// </summary>
	/// <param name="other">Other collider that has been collied with</param>
	/// <returns>Returns true if there has been a collision</returns>
	virtual bool CollidesWith(Collider& other) = 0;

	/// <summary>
	/// Base Collider with the sphere collider class
	/// </summary>
	/// <param name="other">Other Sphere Collider that has been collided with</param>
	/// <returns>Returns true if there has been a collision</returns>
	virtual bool CollidesWith(SphereCollider& other) = 0;

	/// <summary>
	/// Base Collider with the AABB collider class
	/// </summary>
	/// <param name="other">Other AABB Collider that has been collided with</param>
	/// <returns>Returns true if there has been a collision</returns>
	virtual bool CollidesWith(AABBCollider& other) = 0;

	/// <summary>
	/// Identifies the type of collsion that has taken place
	/// </summary>
	/// <param name="gameObjectA">The first gameobject collider</param>
	/// <param name="gameObjectB">The second gameobject collider</param>
	void HandleCollision(const GameObject* gameObjectA, const GameObject* gameObjectB);
#pragma endregion

#pragma Info Getters
	/// <summary>
	/// Returns the collision normal of the collision
	/// </summary>
	/// <param name="other">Other base collider that gets downcasted</param>
	/// <returns>A vector of the collsion normal</returns>
	virtual Vector GetCollisionNormal(const Collider& other) = 0;

	/// <summary>
	/// Gets which type of collider a object is
	/// </summary>
	/// <returns>Returns a string of which collider a gameobject has</returns>
	virtual std::string GetType() const = 0;

	/// <summary>
	/// Shortcut to get the transform of the collider
	/// </summary>
	/// <returns>The position of the collider</returns>
	Vector GetPosition() const { return _transform->GetPosition(); }
#pragma endregion

#pragma region Updaters
	/// <summary>
	/// Handles collider movement
	/// </summary>
	virtual void Update() = 0;
#pragma endregion
};
