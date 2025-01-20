#include "Transform.h"

Transform::Transform(const Vector& position, const Vector& rotation, const Vector& scale, const std::string& objectType)
{
	_position = position;
	SetRotation(rotation);
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
	if (GetRotation().x >= 360.0f)
	{
		SetRotation(GetRotation().x - 360.0f, GetRotation().y, GetRotation().z);
	}
	else if (GetRotation().x < 0.0f)
	{
		SetRotation(GetRotation().x + 360.0f, GetRotation().y, GetRotation().z);
	}

	if (GetRotation().y >= 360.0f)
	{
		SetRotation(GetRotation().x, GetRotation().y - 360.0f, GetRotation().z);
	}
	else if (GetRotation().y < 0.0f)
	{
		SetRotation(GetRotation().x, GetRotation().y + 360.0f, GetRotation().z);
	}

	if (GetRotation().z >= 360.0f)
	{
		SetRotation(GetRotation().x, GetRotation().y, GetRotation().z - 360.0f);
	}
	else if (GetRotation().z < 0.0f)
	{
		SetRotation(GetRotation().x, GetRotation().y, GetRotation().z + 360.0f);
	}

	XMMATRIX scale = XMMatrixScaling(_scale.x, _scale.y, _scale.z);
	XMMATRIX rotation = XMMatrixRotationRollPitchYaw(GetRotation().x, GetRotation().y, GetRotation().z);
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
	SetRotation(GetRotation() += (Vector::Normalise(rotation) * moveSpeed) * deltaTime);
}

void Transform::Scale(const Vector& scale, float deltaTime, float moveSpeed)
{
	_scale += (Vector::Normalise(scale) * moveSpeed) * deltaTime;
	Debug::Debug_WriteVector(_objectType, "Scale", _scale);
}

void Transform::Reset()
{
	_position = _originPosition;
	SetRotation(_originRotation);
	_scale = _originScale;

	Debug::Debug_WriteString(_objectType + " Reset");
}