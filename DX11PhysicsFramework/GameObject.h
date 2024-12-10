#pragma once
#include "Structures.h"
#include "Transform.h"
#include "OBJLoader.h"

class GameObject
{
public:
	GameObject(const string& type, const string& meshpath, const Material& material, ID3D11Device& device, XMFLOAT3 position, XMFLOAT3 scale, XMFLOAT3 rotation);
	GameObject(const string& type, const Geometry& geometry, const Material& material, XMFLOAT3 position, XMFLOAT3 scale, XMFLOAT3 rotation);

	~GameObject();

	Transform* GetTranform()
	{
		if (_transform == nullptr) { return nullptr; }
		return _transform;
	}

	string GetType() const { return _type; }

	void SetParent(GameObject* parent) { _parent = parent; }

	// Rendering information
	void LoadMesh(string meshpath, ID3D11Device& device);
	Geometry GetGeometryData() const { return _geometry; }
	Material GetMaterial() const { return _material; }

	void SetTextureRV(ID3D11ShaderResourceView* textureRV) { _textureRV = textureRV; }
	ID3D11ShaderResourceView* const* GetTextureRV() { return &_textureRV; }
	bool HasTexture() const { return _textureRV ? true : false; }

	void Update(float dt);

	void Draw(ID3D11DeviceContext* pImmediateContext) const;

private:
	GameObject* _parent = nullptr;

	string _type;

	Transform* _transform;

	Geometry _geometry = {};

	Material _material;
	ID3D11ShaderResourceView* _textureRV = nullptr;
};
