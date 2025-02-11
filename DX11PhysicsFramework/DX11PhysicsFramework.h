#pragma once
#include "Structures.h"
#include "Camera.h"
#include "GameObject.h"

constexpr auto FPS60 = 1.0 / 60.0f;

using namespace DirectX;

class DX11PhysicsFramework
{
	int WindowWidth = 1920;
	int WindowHeight = 1080;

	float _objectMoveSpeed = 4.0f;
	float _objectRotateSpeed = 50.0f;
	float _objectScaleSpeed = 1.0f;

	float _addForceNumber = 10.0f;

	HWND _windowHandle = nullptr;

	ID3D11DeviceContext* _immediateContext = nullptr;
	ID3D11Device* _device = nullptr;
	IDXGIDevice* _dxgiDevice = nullptr;
	IDXGIFactory2* _dxgiFactory = nullptr;
	ID3D11RenderTargetView* _frameBufferView = nullptr;
	IDXGISwapChain1* _swapChain = nullptr;
	D3D11_VIEWPORT _viewport = {};

	ID3D11VertexShader* _vertexShader = nullptr;
	ID3D11InputLayout* _inputLayout = nullptr;
	ID3D11PixelShader* _pixelShader = nullptr;;
	ID3D11Buffer* _constantBuffer = nullptr;;
	ID3D11Buffer* _cubeVertexBuffer = nullptr;;
	ID3D11Buffer* _cubeIndexBuffer = nullptr;;

	ID3D11Buffer* _planeVertexBuffer = nullptr;;
	ID3D11Buffer* _planeIndexBuffer = nullptr;;

	ID3D11DepthStencilView* _depthBufferView = nullptr;
	ID3D11Texture2D* _depthStencilBuffer = nullptr;

	ID3D11ShaderResourceView* StoneTextureRV = nullptr;
	ID3D11ShaderResourceView* GroundTextureRV = nullptr;
	ID3D11ShaderResourceView* SelectedTexture = nullptr;

	ID3D11SamplerState* _samplerLinear = nullptr;

	Light basicLight = {};

	MeshData _objMeshData = {};
	vector<GameObject*> _gameObjects = {};

	Camera* _camera = nullptr;
	nlohmann::json m_sceneCameraVariables = nullptr;

	ConstantBuffer _cbData = {};

	ID3D11DepthStencilState* DSLessEqual = nullptr;
	ID3D11RasterizerState* RSCullNone = nullptr;;

	ID3D11RasterizerState* CCWcullMode = nullptr; //Counter Clockwise
	ID3D11RasterizerState* CWcullMode = nullptr; //Clockwise

	float _accumulator = 0.0f;

	float _runtimeTimer = 0.0f;

	bool _objectSelected[8] = {};
	bool _floorCollision = false;
	int _gameObjectSize = 0;
	float deltaTime = 0.0f;

	float _broadPhaseDetectionRadius = 4.0f;
	bool _toggleBroadPhase = true;

	bool _gameobjectsMatrixInterpolation = true;
	bool _cameraMatrixInterpolation = true;

	HRESULT CreateWindowHandle(HINSTANCE hInstance, int nCmdShow);
	HRESULT CreateD3DDevice();
	HRESULT CreateSwapChainAndFrameBuffer();
	void InitGUI() const;
	HRESULT InitShadersAndInputLayout();
	HRESULT InitVertexIndexBuffers();
	HRESULT InitPipelineStates();
	HRESULT InitRunTimeData();
	void LoadSceneCameraVariables();

	void BasicObjectMovement(float deltaTime, int objectSelected) const;

public:
	~DX11PhysicsFramework();

	HRESULT Initialise(HINSTANCE hInstance, int nCmdShow);

	void Update();
	void PhysicsUpdate() const;
	void DetectCollisions() const;
	void GeneralUpdate(float deltaTime);
	void DrawObjectSelectWindow();
	void DrawStatsWindow() const;
	void DrawObjectMovementControlWindow(float deltaTime, int objectSelected);
	void DrawCameraWindow();
	void DrawIntegrationWindow(int objectSelected) const;
	void DrawMatrixInterpolationWindow();
	void DrawUI();
	void Draw(double alphaScalar);

	HWND GetWindowHandle() const { return _windowHandle; }
	ID3D11Device* GetDevice() const { return _device; }
	ID3D11DeviceContext* GetDeviceContext() const { return _immediateContext; }
};
