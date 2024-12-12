#pragma once
#include "Structures.h"

class Transform
{
public:
	Transform(const Vector& position, const Vector& rotation, const Vector& scale, const std::string& objectType);

	void Update() const;

	// Setters and Getters for position/rotation/scale

	void SetPositon(const Vector& newposition) { _position = newposition; }

	void SetPosition(const float x, const float y, const float z)
	{
		_position.x = x;
		_position.y = y;
		_position.z = z;
	}

	Vector GetPosition() const { return _position; }

	void SetScale(const Vector& scale) { _scale = scale; }

	void SetScale(const float x, const float y, const float z)
	{
		_scale.x = x;
		_scale.y = y;
		_scale.z = z;
	}

	Vector GetScale() const { return _scale; }

	void SetRotation(const Vector& rotation) { _rotation = rotation; }

	void SetRotation(const float x, const float y, const float z)
	{
		_rotation.x = x;
		_rotation.y = y;
		_rotation.z = z;
	}

	Vector GetRotation() const { return _rotation; }

	void Move(const Vector& direction);
	void Rotate(const Vector& rotation);
	void Scale(const Vector& scale);
	void Reset();

	XMFLOAT4X4* GetWorldMatrix();
	XMMATRIX GetWorldMatrix4X4();

private:
	Vector _position;
	Vector _scale;
	Vector _rotation;

	Vector _originPosition;
	Vector _originScale;
	Vector _originRotation;

	std::string _objectType;

	XMFLOAT4X4* _world;
	bool dirtyMatrix;
};
