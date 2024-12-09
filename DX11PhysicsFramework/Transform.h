#pragma once
#include "Structures.h"

class Transform
{
public:

	Transform();
	
	void Update();

	// Setters and Getters for position/rotation/scale

	void SetPositon(XMFLOAT3 newposition) { _position = newposition; }
	void SetPosition(float x, float y, float z) { _position.x = x; _position.y = y; _position.z = z; }
	XMFLOAT3 GetPosition() const { return _position; }

	void SetScale(XMFLOAT3 scale) { _scale = scale; }
	void SetScale(float x, float y, float z) { _scale.x = x; _scale.y = y; _scale.z = z; }
	XMFLOAT3 GetScale() const { return _scale; }

	void SetRotation(XMFLOAT3 rotation) { _rotation = rotation; }
	void SetRotation(float x, float y, float z) { _rotation.x = x; _rotation.y = y; _rotation.z = z; }
	XMFLOAT3 GetRotation() const { return _rotation; }

	void Move(XMFLOAT3 direction);

	XMFLOAT4X4 GetWorldMatrix() const { return _world; }
	XMMATRIX GetWorldMatrix4X4() const { return XMLoadFloat4x4(&_world); }
private:
	XMFLOAT3 _position;
	XMFLOAT3 _scale;
	XMFLOAT3 _rotation;
	XMFLOAT4X4 _world;
	bool dirtyMatrix;
};

