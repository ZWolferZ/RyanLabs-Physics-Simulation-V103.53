#pragma once
#pragma region Includes
// Include{s}
#include "Structures.h"
#include "OBJLoader.h"
#pragma endregion

/// <summary>
/// Manages the appearance of objects, including geometry, texture, and material.
/// </summary>
class Appearance
{
public:
#pragma region Constructors
	/// <summary>
	/// Initializes a new instance of the Appearance class with specified geometry, texture, and material.
	/// </summary>
	/// <param name="geometry">The geometry data.</param>
	/// <param name="textureRV">The texture resource view.</param>
	/// <param name="material">The material properties.</param>
	Appearance(Geometry geometry, ID3D11ShaderResourceView* textureRV, Material material);

	/// <summary>
	/// Initializes a new instance of the Appearance class by loading a mesh from a file.
	/// </summary>
	/// <param name="meshpath">The path to the mesh file.</param>
	/// <param name="device">The Direct3D device.</param>
	/// <param name="textureRV">The texture resource view.</param>
	/// <param name="material">The material properties.</param>
	Appearance(const string& meshpath, ID3D11Device& device, ID3D11ShaderResourceView* textureRV, Material material);
#pragma endregion

#pragma region Public Methods
	/// <summary>
	/// Loads a mesh from a file.
	/// </summary>
	/// <param name="meshpath">The path to the mesh file.</param>
	/// <param name="device">The Direct3D device.</param>
	void LoadMesh(string meshpath, ID3D11Device& device);

	/// <summary>
	/// Draws the appearance using the specified device context.
	/// </summary>
	/// <param name="pImmediateContext">The device context.</param>
	void Draw(ID3D11DeviceContext* pImmediateContext) const;

	/// <summary>
	/// Sets the texture resource view.
	/// </summary>
	/// <param name="textureRV">The texture resource view.</param>
	void SetTextureRV(ID3D11ShaderResourceView* textureRV);

	/// <summary>
	/// Gets the texture resource view.
	/// </summary>
	/// <returns>The texture resource view.</returns>
	ID3D11ShaderResourceView* const* GetTextureRV() const { return &_textureRV; }

	/// <summary>
	/// Gets the geometry data.
	/// </summary>
	/// <returns>The geometry data.</returns>
	Geometry GetGeometryData() const { return _geometry; }

	/// <summary>
	/// Gets the material properties.
	/// </summary>
	/// <returns>The material properties.</returns>
	Material GetMaterial() const { return _material; }
#pragma endregion

#pragma region Public Member Variables
	/// <summary>
	/// Indicates whether the appearance has a texture.
	/// </summary>
	bool _hasTexture;
#pragma endregion

private:
#pragma region Private Member Variables
	/// <summary>
	/// The geometry data.
	/// </summary>
	Geometry _geometry;

	/// <summary>
	/// The texture resource view.
	/// </summary>
	ID3D11ShaderResourceView* _textureRV;

	/// <summary>
	/// The material properties.
	/// </summary>
	Material _material;
#pragma endregion
};
