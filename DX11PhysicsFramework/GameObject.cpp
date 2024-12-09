#include "GameObject.h"

GameObject::GameObject(string type, Geometry geometry, Material material) : _geometry(geometry), _type(type), _material(material)
{
	_parent = nullptr;
	_transform = new Transform;
	_textureRV = nullptr;
}

GameObject::~GameObject()
{
	_parent = nullptr;
	_textureRV = nullptr;
	_geometry.indexBuffer = nullptr;
	_geometry.vertexBuffer = nullptr;
}


void GameObject::Update(float dt)
{
	_transform->Update();

	if (_parent != nullptr)
	{
		XMStoreFloat4x4(&GetTranform()->GetWorldMatrix(), this->GetTranform()->GetWorldMatrix4X4() * _parent->GetTranform()->GetWorldMatrix4X4());
	}
}


void GameObject::Draw(ID3D11DeviceContext * pImmediateContext)
{
	// We are assuming that the constant buffers and all other draw setup has already taken place

	// Set vertex and index buffers
	pImmediateContext->IASetVertexBuffers(0, 1, &_geometry.vertexBuffer, &_geometry.vertexBufferStride, &_geometry.vertexBufferOffset);
	pImmediateContext->IASetIndexBuffer(_geometry.indexBuffer, DXGI_FORMAT_R16_UINT, 0);

	pImmediateContext->DrawIndexed(_geometry.numberOfIndices, 0, 0);
}
