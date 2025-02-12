#pragma once

#pragma region Includes
// Include{s}
#include "Structures.h"
#include "Appearance.h"
#include "RigidBodyModel.h"
#include "ParticleModel.h"
#pragma endregion

/// <summary>
/// Holds all the components and information for a game object.
/// </summary>
class GameObject
{
public:
#pragma region Constructors
	/// <summary>
	/// Game object constructor override for geometry information.
	/// </summary>
	/// <param name="type">The type of the game object.</param>
	/// <param name="geometry">The geometry data.</param>
	/// <param name="material">The material properties.</param>
	/// <param name="textureRV">The texture resource view.</param>
	/// <param name="position">The position of the game object.</param>
	/// <param name="scale">The scale of the game object.</param>
	/// <param name="rotation">The rotation of the game object.</param>
	/// <param name="mass">The mass of the game object.</param>
	/// <param name="isparticle">Indicates if the game object is a particle.</param>
	GameObject(const string& type, const Geometry& geometry, const Material& material,
		ID3D11ShaderResourceView* textureRV, const Vector& position,
		const Vector& scale, const Vector& rotation, float mass, bool isparticle);

	/// <summary>
	/// Game object constructor override for mesh-path information.
	/// </summary>
	/// <param name="type">The type of the game object.</param>
	/// <param name="meshpath">The path to the mesh file.</param>
	/// <param name="material">The material properties.</param>
	/// <param name="textureRV">The texture resource view.</param>
	/// <param name="device">The Direct3D device.</param>
	/// <param name="position">The position of the game object.</param>
	/// <param name="scale">The scale of the game object.</param>
	/// <param name="rotation">The rotation of the game object.</param>
	/// <param name="mass">The mass of the game object.</param>
	/// <param name="isparticle">Indicates if the game object is a particle.</param>
	GameObject(const string& type, const string& meshpath, const Material& material,
		ID3D11ShaderResourceView* textureRV, ID3D11Device& device,
		const Vector& position, const Vector& scale, const Vector& rotation, float mass, bool isparticle);

	/// <summary>
	/// Game object destructor which destroys the components of the game object.
	/// </summary>
	~GameObject();
#pragma endregion

#pragma region Getters
	/// <summary>
	/// Gets the transform of the game object.
	/// </summary>
	/// <returns>The transform of the game object.</returns>
	Transform* GetTransform() const
	{
		return _transform;
	}

	/// <summary>
	/// Gets the appearance of the game object.
	/// </summary>
	/// <returns>The appearance of the game object.</returns>
	Appearance* GetAppearance() const
	{
		return _appearance;
	}

	/// <summary>
	/// Gets the physics model of the game object.
	/// </summary>
	/// <returns>The physics model of the game object.</returns>
	PhysicsModel* GetPhysicsModel() const
	{
		return _physicsModel;
	}

	/// <summary>
	/// Gets the type of the game object.
	/// </summary>
	/// <returns>The type of the game object.</returns>
	string GetType() const { return _type; }
#pragma endregion

#pragma region Setters
	/// <summary>
	/// Sets the parent of the game object.
	/// </summary>
	/// <param name="parent">The parent game object.</param>
	void SetParent(GameObject* parent) { _parent = parent; }
#pragma endregion

#pragma region Update Methods
	/// <summary>
	/// Updates the game object.
	/// </summary>
	/// <param name="dt">The delta time.</param>
	void Update(float dt) const;
#pragma endregion

#pragma region Collision Methods
	/// <summary>
	/// Handles AABB to AABB collision.
	/// </summary>
	/// <param name="collisionData">The collision data.</param>
	void HandleAABBABBB(const AABBAABBCollisionManifold& collisionData) const;

	/// <summary>
	/// Handles sphere to AABB collision.
	/// </summary>
	/// <param name="collisionData">The collision data.</param>
	void HandleSphereAABB(const SPHEREAABBCollisionManifold& collisionData) const;

	/// <summary>
	/// Handles sphere to sphere collision.
	/// </summary>
	/// <param name="collisionData">The collision data.</param>
	void HandleSphereSphere(const SPHERESPHERECollisionManifold& collisionData) const;
#pragma endregion

#pragma region Member Variables
	bool _objectHasCollided = false;
	bool _collisionEnabled = true;

protected:
	GameObject* _parent = nullptr;
	string _type;
	Transform* _transform = nullptr;
	Appearance* _appearance = nullptr;
	PhysicsModel* _physicsModel = nullptr;
	float _mass;

private:
	const float _restitution = 0.2f;
#pragma endregion
};