#pragma once
#include "Structures.h"
#include "OBJLoader.h"

class Appearance
{
public:
	Appearance(Geometry geometry, ID3D11ShaderResourceView* textureRV, Material material);
	Appearance(const string& meshpath, ID3D11Device& device, ID3D11ShaderResourceView* textureRV, Material material);

	void LoadMesh(string meshpath, ID3D11Device& device);
	void Draw(ID3D11DeviceContext* pImmediateContext) const;

	void SetTextureRV(ID3D11ShaderResourceView* textureRV);
	ID3D11ShaderResourceView* const* GetTextureRV() const { return &_textureRV; }

	Geometry GetGeometryData() const { return _geometry; }
	Material GetMaterial() const { return _material; };

	bool _hasTexture;

private:
	Geometry _geometry;
	ID3D11ShaderResourceView* _textureRV;
	Material _material;
};
