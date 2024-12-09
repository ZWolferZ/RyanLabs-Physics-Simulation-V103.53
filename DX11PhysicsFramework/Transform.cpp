#include "Transform.h"


Transform::Transform()
{
	_position = XMFLOAT3();
	_rotation = XMFLOAT3();
	_scale = XMFLOAT3(1.0f, 1.0f, 1.0f);
	_world = XMFLOAT4X4();
}

void Transform::Move(XMFLOAT3 direction)
{
	_position.x += direction.x;
	_position.y += direction.y;
	_position.z += direction.z;
}



void Transform::Update()
{
	if (!dirtyMatrix) return;

	// Calculate world matrix
	XMMATRIX scale = XMMatrixScaling(_scale.x, _scale.y, _scale.z);
	XMMATRIX rotation = XMMatrixRotationX(_rotation.x) * XMMatrixRotationY(_rotation.y) * XMMatrixRotationZ(_rotation.z);
	XMMATRIX translation = XMMatrixTranslation(_position.x, _position.y, _position.z);

	XMStoreFloat4x4(&_world, scale * rotation * translation);

}