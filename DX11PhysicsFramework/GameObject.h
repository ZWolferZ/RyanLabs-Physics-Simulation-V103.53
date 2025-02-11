#pragma once
#include "Structures.h"
#include "Appearance.h"
#include "RigidBodyModel.h"
#include "ParticleModel.h"

class GameObject
{
public:
	GameObject(const string& type, const Geometry& geometry, const Material& material,
		ID3D11ShaderResourceView* textureRV, const Vector& position,
		const Vector& scale, const Vector& rotation, float mass, bool isparticle);

	GameObject(const string& type, const string& meshpath, const Material& material,
		ID3D11ShaderResourceView* textureRV, ID3D11Device& device,
		const Vector& position, const Vector& scale, const Vector& rotation, float mass, bool isparticle);

	~GameObject();

	Transform* GetTransform() const
	{
		return _transform;
	}

	Appearance* GetAppearance() const
	{
		return _appearance;
	}

	PhysicsModel* GetPhysicsModel() const
	{
		return _physicsModel;
	}

	string GetType() const { return _type; }

	void SetParent(GameObject* parent) { _parent = parent; }

	void Update(float dt) const;

	void HandleAABBABBB(const AABBAABBCollisionManifold& collisionData) const;

	void HandleSphereAABB(const SPHEREAABBCollisionManifold& collisionData) const;

	void HandleSphereSphere(const SPHERESPHERECollisionManifold& collisionData) const;

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
};
