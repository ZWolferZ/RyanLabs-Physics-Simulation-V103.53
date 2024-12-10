#include "Transform.h"

Transform::Transform()
{
	_position = XMFLOAT3();
	_rotation = XMFLOAT3();
	_scale = XMFLOAT3(1.0f, 1.0f, 1.0f);
	dirtyMatrix = false;
	_world = nullptr;
}

void Transform::Move(XMFLOAT3 direction)
{
	_position.x += direction.x;
	_position.y += direction.y;
	_position.z += direction.z;
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