#include "Transform.h"

Transform::Transform(const Vector& position, const Vector& rotation, const Vector& scale, const std::string& objectType)
{
	_position = position;
	_rotation = rotation;
	_scale = scale;

	_originPosition = position;
	_originRotation = rotation;
	_originScale = scale;

	_objectType = objectType;
}

void Transform::Update()
{
	_previousWorld = _world;

	// Reset the rotation if it goes over 360 degrees or under 0
	if (_rotation.x >= 360.0f)
	{
		_rotation.x -= 360.0f;
	}
	else if (_rotation.x < 0.0f)
	{
		_rotation.x += 360.0f;
	}

	if (_rotation.y >= 360.0f)
	{
		_rotation.y -= 360.0f;
	}
	else if (_rotation.y < 0.0f)
	{
		_rotation.y += 360.0f;
	}

	if (_rotation.z >= 360.0f)
	{
		_rotation.z -= 360.0f;
	}
	else if (_rotation.z < 0.0f)
	{
		_rotation.z += 360.0f;
	}

	XMMATRIX scale = XMMatrixScaling(_scale.x, _scale.y, _scale.z);
	XMMATRIX rotation = XMMatrixRotationRollPitchYaw(_rotation.x, _rotation.y, _rotation.z);
	XMMATRIX translation = XMMatrixTranslation(_position.x, _position.y, _position.z);

	XMStoreFloat4x4(&_world, scale * rotation * translation);
}

void Transform::Move(const Vector& direction, float deltaTime, float moveSpeed)
{
	_position += (Vector::Normalise(direction) * moveSpeed) * deltaTime;
	Debug::Debug_WriteVector(_objectType, "Position", _position);
}

void Transform::Rotate(const Vector& rotation, float deltaTime, float moveSpeed)
{
	_rotation += (Vector::Normalise(rotation) * moveSpeed) * deltaTime;

	Debug::Debug_WriteVector(_objectType, "Rotation", _rotation);
}

void Transform::Scale(const Vector& scale, float deltaTime, float moveSpeed)
{
	_scale += (Vector::Normalise(scale) * moveSpeed) * deltaTime;
	Debug::Debug_WriteVector(_objectType, "Scale", _scale);
}

void Transform::Reset()
{
	_position = _originPosition;
	_rotation = _originRotation;
	_scale = _originScale;

	Debug::Debug_WriteString(_objectType + " Reset");
}