#pragma once
#include "Structures.h"

class Transform
{
public:
	Transform(const Vector& position, const Vector& rotation, const Vector& scale, const std::string& objectType);

	void Update();

	Vector GetPosition() const { return _position; }
	void SetPosition(const Vector& newPosition) { _position = newPosition; }

	void SetPosition(const float x, const float y, const float z)
	{
		_position.x = x;
		_position.y = y;
		_position.z = z;
	}

	Vector GetScale() const { return _scale; }
	void SetScale(const Vector& newScale) { _scale = newScale; }

	void SetScale(const float x, const float y, const float z)
	{
		_scale.x = x;
		_scale.y = y;
		_scale.z = z;
	}

	Vector GetRotation() { return MakeEulerAnglesFromQ(_orientation); }
	void SetRotation(const Vector& newRotation) { _orientation = MakeQFromEulerAngles(newRotation.x, newRotation.y, newRotation.z); }
	void SetRotation(const float x, const float y, const float z){	_orientation = MakeQFromEulerAngles(x, y, z);}

	void SetOrientation(Quaternion orientation) { _orientation = orientation; }
	Quaternion GetOrientation() {return _orientation; }

	void Move(const Vector& direction, float deltaTime, float moveSpeed);
	void Rotate(const Vector& rotation, float deltaTime, float moveSpeed);
	void Scale(const Vector& scale, float deltaTime, float moveSpeed);
	void Reset();

	XMFLOAT4X4* GetWorldFloat4X4() { return &_world; }
	XMMATRIX GetWorldMatrix() const { return XMLoadFloat4x4(&_world); }
	XMMATRIX GetPreviousWorldMatrix() const { return XMLoadFloat4x4(&_previousWorld); }

private:
	Vector _position;
	Vector _scale;
	Quaternion _orientation;

	Vector _originPosition;
	Vector _originScale;
	Vector _originRotation;

	std::string _objectType;

	XMFLOAT4X4 _world;
	XMFLOAT4X4 _previousWorld;
};
