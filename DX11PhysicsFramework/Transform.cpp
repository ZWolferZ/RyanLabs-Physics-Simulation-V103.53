#include "Transform.h"

Transform::Transform(const Vector& position, const Vector& rotation, const Vector& scale, const std::string& objectType)
{
	_position = position;
	_rotation = rotation;
	_scale = scale;

	_originPosition = position;
	_originRotation = rotation;
	_originScale = scale;
	dirtyMatrix = false;

	_objectType = objectType;

	_world = nullptr;
}

void Transform::Move(const Vector& direction)
{
	_position += direction;
	Debug::Debug_WriteVector(_objectType, "Position", _position);
}

void Transform::Rotate(const Vector& rotation)
{
	_rotation += rotation;
	Debug::Debug_WriteVector(_objectType, "Rotation", _rotation);
}

void Transform::Scale(const Vector& scale)
{
	_scale += scale;
	Debug::Debug_WriteVector(_objectType, "Scale", _scale);
}

void Transform::Reset()
{
	_position = _originPosition;
	_rotation = _originRotation;
	_scale = _originScale;

	Debug::Debug_WriteString(_objectType + " Reset");
}

XMMATRIX Transform::GetWorldMatrix4X4()
{
	if (!dirtyMatrix)
	{
		_world = new XMFLOAT4X4();
		dirtyMatrix = true;
	}

	return XMLoadFloat4x4(_world);
}

XMFLOAT4X4* Transform::GetWorldMatrix()
{
	if (!dirtyMatrix)
	{
		_world = new XMFLOAT4X4();
		dirtyMatrix = true;
	}
	return _world;
}

void Transform::Update() const
{
	if (!dirtyMatrix) return;

	// Calculate world matrix
	XMMATRIX scale = XMMatrixScaling(_scale.x, _scale.y, _scale.z);
	XMMATRIX rotation = XMMatrixRotationX(_rotation.x) * XMMatrixRotationY(_rotation.y) *
		XMMatrixRotationZ(_rotation.z);
	XMMATRIX translation = XMMatrixTranslation(_position.x, _position.y, _position.z);

	XMStoreFloat4x4(_world, scale * rotation * translation);
}