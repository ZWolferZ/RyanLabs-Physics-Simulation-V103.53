#include "Transform.h"

Transform::Transform(XMFLOAT3 position, XMFLOAT3 rotation, XMFLOAT3 scale)
{
	_position = position;
	_rotation = rotation;
	_scale = scale;

	_originPosition = position;
	_originRotation = rotation;
	_originScale = scale;
	dirtyMatrix = false;

	_world = nullptr;
}

void Transform::Move(XMFLOAT3 direction)
{
	_position.x += direction.x;
	_position.y += direction.y;
	_position.z += direction.z;
}

void Transform::Rotate(XMFLOAT3 rotation)
{
	_rotation.x += rotation.x;
	_rotation.y += rotation.y;
	_rotation.z += rotation.z;
}

void Transform::Scale(XMFLOAT3 scale)
{
	_scale.x += scale.x;
	_scale.y += scale.y;
	_scale.z += scale.z;
}

void Transform::Reset()
{
	_position = _originPosition;
	_rotation = _originRotation;
	_scale = _originScale;
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

void Transform::Update()
{
	if (!dirtyMatrix) return;

	// Calculate world matrix
	XMMATRIX scale = XMMatrixScaling(_scale.x, _scale.y, _scale.z);
	XMMATRIX rotation = XMMatrixRotationX(_rotation.x) * XMMatrixRotationY(_rotation.y) *
		XMMatrixRotationZ(_rotation.z);
	XMMATRIX translation = XMMatrixTranslation(_position.x, _position.y, _position.z);

	XMStoreFloat4x4(_world, scale * rotation * translation);
}