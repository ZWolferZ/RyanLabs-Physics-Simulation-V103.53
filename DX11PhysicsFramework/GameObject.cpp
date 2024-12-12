#include "GameObject.h"

GameObject::GameObject(const string& type, const string& meshpath, const Material& material,
	ID3D11ShaderResourceView* textureRV, ID3D11Device& device,
	const Vector& position, const Vector& scale, const Vector& rotation) : _type(type)

{
	_parent = nullptr;
	_transform = new Transform(position, rotation, scale, type);
	_appearance = new Appearance(meshpath, device, textureRV, material);
}

GameObject::GameObject(const string& type, const Geometry& geometry, const Material& material,
	ID3D11ShaderResourceView* textureRV, const Vector& position,
	const Vector& scale, const Vector& rotation) : _type(type)
{
	_parent = nullptr;
	_transform = new Transform(position, rotation, scale, type);
	_appearance = new Appearance(geometry, textureRV, material);
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
}

void GameObject::Update(float dt) const
{
	_transform->Update();

	if (_parent != nullptr)
	{
		XMStoreFloat4x4(GetTransform()->GetWorldMatrix(),
			this->GetTransform()->GetWorldMatrix4X4() * _parent->GetTransform()->GetWorldMatrix4X4());
	}
}