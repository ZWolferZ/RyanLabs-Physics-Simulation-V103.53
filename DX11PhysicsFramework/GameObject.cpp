#include "GameObject.h"

GameObject::GameObject(const string& type, const string& meshpath, const Material& material,
	ID3D11ShaderResourceView* textureRV, ID3D11Device& device,
	const Vector& position, const Vector& scale, const Vector& rotation) : _type(type)

{
	_parent = nullptr;
	_transform = new Transform(position, rotation, scale, type);
	_appearance = new Appearance(meshpath, device, textureRV, material);
	_physicsModel = new PhysicsModel(_transform);
}

GameObject::GameObject(const string& type, const Geometry& geometry, const Material& material,
	ID3D11ShaderResourceView* textureRV, const Vector& position,
	const Vector& scale, const Vector& rotation) : _type(type)
{
	_parent = nullptr;
	_transform = new Transform(position, rotation, scale, type);
	_appearance = new Appearance(geometry, textureRV, material);
	_physicsModel = new PhysicsModel(_transform);
}

GameObject::~GameObject()
{
	if (_transform != nullptr)
	{
		delete _transform;
		_transform = nullptr;
	}

	if (_appearance != nullptr)
	{
		delete _appearance;
		_appearance = nullptr;
	}

	if (_parent != nullptr)
	{
		delete _parent;
		_parent = nullptr;
	}

	if (_physicsModel != nullptr)
	{
		delete _physicsModel;
		_physicsModel = nullptr;
	}
}

void GameObject::Update(const float dt) const
{
	_physicsModel->Update(dt);
	_transform->Update();

	if (_parent != nullptr)
	{
		XMStoreFloat4x4(GetTransform()->GetWorldFloat4X4(),
			this->GetTransform()->GetWorldMatrix() * _parent->GetTransform()->GetWorldMatrix());
	}
}