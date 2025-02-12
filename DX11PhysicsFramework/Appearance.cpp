#pragma region Includes
// Include{s}
#include "Appearance.h"
#pragma endregion

#pragma region Constructors

Appearance::Appearance(const Geometry& geometry, ID3D11ShaderResourceView* textureRV, const Material& material)
{
	_geometry = geometry;

	SetTextureRV(textureRV);

	_material = material;
}

Appearance::Appearance(const string& meshpath, ID3D11Device& device, ID3D11ShaderResourceView* textureRV,
	const Material& material)
{
	LoadMesh(meshpath, device);

	SetTextureRV(textureRV);

	_material = material;
}

#pragma endregion

#pragma region Mesh Loading

void Appearance::LoadMesh(const string& meshpath, ID3D11Device& device)
{
	// Load the OBJ file into the geometry data
	MeshData _objMeshData = OBJLoader::Load(meshpath.c_str(), &device);
	_geometry.indexBuffer = _objMeshData.IndexBuffer;
	_geometry.vertexBuffer = _objMeshData.VertexBuffer;
	_geometry.numberOfIndices = _objMeshData.IndexCount;
	_geometry.vertexBufferOffset = 0;
	_geometry.vertexBufferStride = sizeof(SimpleVertex);
}

#pragma endregion

#pragma region Drawing

void Appearance::Draw(ID3D11DeviceContext* pImmediateContext) const
{
	// We are assuming that the constant buffers and all other draw setup has already taken place

	// Set vertex and index buffers
	pImmediateContext->IASetVertexBuffers(0, 1, &_geometry.vertexBuffer, &_geometry.vertexBufferStride,
		&_geometry.vertexBufferOffset);
	pImmediateContext->IASetIndexBuffer(_geometry.indexBuffer, DXGI_FORMAT_R16_UINT, 0);

	pImmediateContext->DrawIndexed(_geometry.numberOfIndices, 0, 0);
}

#pragma endregion

#pragma region Texture Management

void Appearance::SetTextureRV(ID3D11ShaderResourceView* textureRV)
{
	// I just use this to set the texture resource view back and forth between the selected texture and regular texture
	if (textureRV == nullptr)
	{
		_textureRV = nullptr;
		_hasTexture = false;
	}
	else
	{
		_textureRV = textureRV;
		_hasTexture = true;
	}
}

#pragma endregion