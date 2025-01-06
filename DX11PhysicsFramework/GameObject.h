#pragma once
#include "Structures.h"
#include "Appearance.h"
#include "ParticleModel.h"

class GameObject
{
public:
	GameObject(const string& type, const string& meshpath, const Material& material,
		ID3D11ShaderResourceView* textureRV, ID3D11Device& device,
		const Vector& position, const Vector& scale, const Vector& rotation);
	GameObject(const string& type, const Geometry& geometry, const Material& material,
		ID3D11ShaderResourceView* textureRV, const Vector& position,
		const Vector& scale, const Vector& rotation);
	GameObject(const string& type, const Geometry& geometry, const Material& material,
		ID3D11ShaderResourceView* textureRV, const Vector& position,
		const Vector& scale, const Vector& rotation, float mass);

	~GameObject();

	Transform* GetTransform() const
	{
		if (_transform == nullptr) { return nullptr; }
		return _transform;
	}

	Appearance* GetAppearance() const
	{
		if (_appearance == nullptr) { return nullptr; }
		return _appearance;
	}

	PhysicsModel* GetPhysicsModel() const
	{
		if (_physicsModel == nullptr) { return nullptr; }
		return _physicsModel;
	}

	string GetType() const { return _type; }

	void SetParent(GameObject* parent) { _parent = parent; }

	void Update(float dt) const;

private:
	GameObject* _parent = nullptr;

	string _type;

	Transform* _transform = nullptr;

	Appearance* _appearance = nullptr;

	PhysicsModel* _physicsModel = nullptr;
};
