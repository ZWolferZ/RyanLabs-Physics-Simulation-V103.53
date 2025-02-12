#pragma once
#pragma region Include Hell
// The examiners when they see the includes on this file: https://www.youtube.com/watch?v=-nQn9UkHS4w
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
#include "Vector.h" // My own vector library!
#include "Debug.h"
#include "Timer.h"
#include "imgui.h"
#include "imgui_impl_win32.h"
#include "imgui_impl_dx11.h"
#include <windowsx.h>
#include "Quaternion.h"
#pragma endregion

using namespace DirectX;
using namespace std;

/// <summary>
/// Holds the surface lighting information for the surface.
/// </summary>
struct SurfaceInfo
{
	XMFLOAT4 AmbientMtrl;
	XMFLOAT4 DiffuseMtrl;
	XMFLOAT4 SpecularMtrl;
};

/// <summary>
/// Holds light data.
/// </summary>
struct Light
{
	XMFLOAT4 AmbientLight;
	XMFLOAT4 DiffuseLight;
	XMFLOAT4 SpecularLight;

	float SpecularPower;
	XMFLOAT3 LightVecW;
};

/// <summary>
/// Holds a buffer of information to pass into the shader.
/// </summary>
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

/// <summary>
/// Holds the geometry information for the object.
/// </summary>
struct Geometry
{
	ID3D11Buffer* vertexBuffer;
	ID3D11Buffer* indexBuffer;
	UINT numberOfIndices;

	UINT vertexBufferStride;
	UINT vertexBufferOffset;
};

/// <summary>
/// Holds the surface lighting information for the object.
/// </summary>
struct Material
{
	XMFLOAT4 diffuse;
	XMFLOAT4 ambient;
	XMFLOAT4 specular;
};

/// <summary>
/// Hold the basic vertex information.
/// </summary>
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

/// <summary>
/// Hold the meshdata for a object.
/// </summary>
struct MeshData
{
	ID3D11Buffer* VertexBuffer;
	ID3D11Buffer* IndexBuffer;
	UINT VBStride;
	UINT VBOffset;
	UINT IndexCount;
};

/// <summary>
/// Stores the types of integration methods.
/// </summary>
enum IntegrationMethods
{
	ExplicitEuler,
	SemiImplicitEuler,
	Verlet,
	StormerVerlet,
	RK4
};

/// <summary>
/// Stores the collision data for a AABB-AABB collision.
/// </summary>
struct AABBAABBCollisionManifold
{
	Vector CollisionNormal;
	float ObjectBMass;
	Vector ObjectBVelocity;
	Vector ObjectAMinPoints;
	Vector ObjectAMaxPoints;
	Vector ObjectBMinPoints;
	Vector ObjectBMaxPoints;
};

/// <summary>
/// Stores the collision data for a Sphere-AABB collision.
/// </summary>
struct SPHEREAABBCollisionManifold
{
	Vector CollisionNormal;
	float ObjectBMass;
	Vector ObjectBVelocity;
	Vector SphereCenter;
	float SphereRadius;
	Vector ObjectBMinPoints;
	Vector ObjectBMaxPoints;
};

/// <summary>
/// Stores the collision data for a Sphere-Sphere collision.
/// </summary>
struct SPHERESPHERECollisionManifold
{
	Vector CollisionNormal;
	float ObjectBMass;
	Vector ObjectBVelocity;
	float SphereARadius;
	float SphereBRadius;
	Vector SphereACenter;
	Vector SphereBCenter;
};
