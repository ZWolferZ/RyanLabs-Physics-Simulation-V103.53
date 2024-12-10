#include "GameObject.h"

GameObject::GameObject(const string& type, const string& meshpath, const Material& material, ID3D11Device& device, XMFLOAT3 position, XMFLOAT3 scale, XMFLOAT3 rotation) : _type(type),
_material(material)
{
	_parent = nullptr;
	_transform = new Transform;
	LoadMesh(meshpath, device);
	_transform->SetPositon(position);
	_transform->SetScale(scale);
	_transform->SetRotation(rotation);
	_textureRV = nullptr;
}

GameObject::GameObject(const string& type, const Geometry& geometry, const Material& material, XMFLOAT3 position, XMFLOAT3 scale, XMFLOAT3 rotation) : _type(type),
_geometry(geometry), _material(material)
{
	_parent = nullptr;
	_transform = new Transform;
	_transform->SetPositon(position);
	_transform->SetScale(scale);
	_transform->SetRotation(rotation);
	_textureRV = nullptr;
}

GameObject::~GameObject()
{
	_parent = nullptr;
	_textureRV = nullptr;
	_geometry.indexBuffer = nullptr;
	_geometry.vertexBuffer = nullptr;
}

void GameObject::LoadMesh(string meshpath, ID3D11Device& device)
{
	MeshData _objMeshData = OBJLoader::Load(meshpath.c_str(), &device);
	_geometry.indexBuffer = _objMeshData.IndexBuffer;
	_geometry.vertexBuffer = _objMeshData.VertexBuffer;
	_geometry.numberOfIndices = _objMeshData.IndexCount;
	_geometry.vertexBufferOffset = 0;
	_geometry.vertexBufferStride = sizeof(SimpleVertex);
}

void GameObject::Update(float dt)
{
	_transform->Update();

	if (_parent != nullptr)
	{
		XMStoreFloat4x4(GetTranform()->GetWorldMatrix(),
			this->GetTranform()->GetWorldMatrix4X4() * _parent->GetTranform()->GetWorldMatrix4X4());
	}
}

void GameObject::Draw(ID3D11DeviceContext* pImmediateContext) const
{
	// We are assuming that the constant buffers and all other draw setup has already taken place

	// Set vertex and index buffers
	pImmediateContext->IASetVertexBuffers(0, 1, &_geometry.vertexBuffer, &_geometry.vertexBufferStride,
		&_geometry.vertexBufferOffset);
	pImmediateContext->IASetIndexBuffer(_geometry.indexBuffer, DXGI_FORMAT_R16_UINT, 0);

	pImmediateContext->DrawIndexed(_geometry.numberOfIndices, 0, 0);
}