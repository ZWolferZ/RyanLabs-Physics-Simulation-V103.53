#pragma once
#include "Structures.h"
#include "Transform.h"
#include "Appearance.h"

class GameObject
{
public:
	GameObject(const string& type, const string& meshpath, const Material& material,
		ID3D11ShaderResourceView* textureRV, ID3D11Device& device,
		XMFLOAT3 position, XMFLOAT3 scale, XMFLOAT3 rotation);
	GameObject(const string& type, const Geometry& geometry, const Material& material,
		ID3D11ShaderResourceView* textureRV, XMFLOAT3 position,
		XMFLOAT3 scale, XMFLOAT3 rotation);

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

	string GetType() const { return _type; }

	void SetParent(GameObject* parent) { _parent = parent; }

	void Update(float dt) const;

private:
	GameObject* _parent = nullptr;

	string _type;

	Transform* _transform;

	Appearance* _appearance = nullptr;
};
