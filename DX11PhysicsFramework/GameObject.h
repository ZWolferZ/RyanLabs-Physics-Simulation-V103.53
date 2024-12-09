#pragma once
#include "Structures.h"
#include "Transform.h"

class GameObject
{
public:
	GameObject(string type, Geometry geometry, Material material);
	~GameObject();

	Transform* GetTranform() { if (_transform != nullptr) { return _transform; } }

	string GetType() const { return _type; }

	void SetParent(GameObject * parent) { _parent = parent; }

	// Rendering information
	Geometry GetGeometryData() const { return _geometry; }
	Material GetMaterial() const { return _material; }


	void SetTextureRV(ID3D11ShaderResourceView * textureRV) { _textureRV = textureRV; }
	ID3D11ShaderResourceView* const* GetTextureRV() { return &_textureRV; }  
	bool HasTexture() const { return _textureRV ? true : false; }

	void Update(float dt);
	
	void Draw(ID3D11DeviceContext * pImmediateContext);

private:
	GameObject* _parent = nullptr;

	string _type;


	Transform* _transform;


	Geometry _geometry;
	Material _material;

	ID3D11ShaderResourceView* _textureRV = nullptr;
};

