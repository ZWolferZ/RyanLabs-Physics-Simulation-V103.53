#pragma once
#include <cstring>
#include <d3d11.h>
#include <directxmath.h>
#include<string>
#include <nlohmann/json.hpp> // Using nlohmann.json Library (Not Mine!)
#include <iostream>
#include <fstream>
#include <thread>
#include <windows.h>
#include <d3d11_1.h>
#include <vector>
#include <directxcolors.h>
#include "DDSTextureLoader.h"
#include "resource.h"
#include <d3dcompiler.h>

using namespace DirectX;
using namespace std;

struct SurfaceInfo
{
	XMFLOAT4 AmbientMtrl;
	XMFLOAT4 DiffuseMtrl;
	XMFLOAT4 SpecularMtrl;
};

struct Light
{
	XMFLOAT4 AmbientLight;
	XMFLOAT4 DiffuseLight;
	XMFLOAT4 SpecularLight;

	float SpecularPower;
	XMFLOAT3 LightVecW;
};

struct ConstantBuffer
{
	XMMATRIX World;
	XMMATRIX View;
	XMMATRIX Projection;

	SurfaceInfo surface;

	Light light;

	XMFLOAT3 EyePosW;
	float HasTexture;
};

struct Geometry
{
	ID3D11Buffer* vertexBuffer;
	ID3D11Buffer* indexBuffer;
	UINT numberOfIndices;

	UINT vertexBufferStride;
	UINT vertexBufferOffset;
};

struct Material
{
	XMFLOAT4 diffuse;
	XMFLOAT4 ambient;
	XMFLOAT4 specular;
};

struct SimpleVertex
{
	XMFLOAT3 Pos;
	XMFLOAT3 Normal;
	XMFLOAT2 TexC;

	bool operator<(const SimpleVertex other) const
	{
		return memcmp(this, &other, sizeof(SimpleVertex)) > 0;
	};
};

struct MeshData
{
	ID3D11Buffer* VertexBuffer;
	ID3D11Buffer* IndexBuffer;
	UINT VBStride;
	UINT VBOffset;
	UINT IndexCount;
};
