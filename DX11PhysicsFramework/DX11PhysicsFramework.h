#pragma once

#pragma region Includes
// Include{s}
#include "Structures.h"
#include "Camera.h"
#include "GameObject.h"
#pragma endregion

// Frame-time that the process should run at
constexpr auto FPS60 = 1.0 / 60.0f;

using namespace DirectX;

/// <summary>
/// Controls the main framework of the application, calls the draw and update functions.
/// </summary>
class DX11PhysicsFramework
{
#pragma region Member Variables
	// Window dimensions
	int WindowWidth = 1920;
	int WindowHeight = 1080;

	// Object movement parameters
	float _objectMoveSpeed = 4.0f;
	float _objectRotateSpeed = 50.0f;
	float _objectScaleSpeed = 1.0f;
	float _addForceNumber = 10.0f;

	// Direct3D device and context
	HWND _windowHandle = nullptr;
	ID3D11DeviceContext* _immediateContext = nullptr;
	ID3D11Device* _device = nullptr;
	IDXGIDevice* _dxgiDevice = nullptr;
	IDXGIFactory2* _dxgiFactory = nullptr;
	ID3D11RenderTargetView* _frameBufferView = nullptr;
	IDXGISwapChain1* _swapChain = nullptr;
	D3D11_VIEWPORT _viewport = {};
	ID3D11DepthStencilView* _depthBufferView = nullptr;
	ID3D11Texture2D* _depthStencilBuffer = nullptr;
	ID3D11SamplerState* _samplerLinear = nullptr;
	Light basicLight = {};
	ConstantBuffer _cbData = {};
	ID3D11DepthStencilState* DSLessEqual = nullptr;
	ID3D11RasterizerState* RSCullNone = nullptr;
	ID3D11RasterizerState* CCWcullMode = nullptr; // Counter Clockwise
	ID3D11RasterizerState* CWcullMode = nullptr; // Clockwise

	// Shaders and input layout
	ID3D11VertexShader* _vertexShader = nullptr;
	ID3D11InputLayout* _inputLayout = nullptr;
	ID3D11PixelShader* _pixelShader = nullptr;
	ID3D11Buffer* _constantBuffer = nullptr;
	ID3D11Buffer* _cubeVertexBuffer = nullptr;
	ID3D11Buffer* _cubeIndexBuffer = nullptr;

	// Plane buffers
	ID3D11Buffer* _planeVertexBuffer = nullptr;
	ID3D11Buffer* _planeIndexBuffer = nullptr;

	// Shader resource views
	ID3D11ShaderResourceView* StoneTextureRV = nullptr;
	ID3D11ShaderResourceView* GroundTextureRV = nullptr;
	ID3D11ShaderResourceView* SelectedTexture = nullptr;

	// Mesh data
	MeshData _objMeshData = {};
	vector<GameObject*> _gameObjects = {};
	vector<GameObject*> _particles = {};

	// Camera variables
	Camera* _camera = nullptr;
	nlohmann::json m_sceneCameraVariables = nullptr;

	// Time-step variables
	float _accumulator = 0.0f;
	float _runtimeTimer = 0.0f;
	float deltaTime = 0.0f;

	// Object selection and collision
	bool _objectSelected[8] = {};
	bool _floorCollision = false;
	int _gameObjectSize = 0;

	// Broad phase detection
	float _broadPhaseDetectionRadius = 4.0f;
	bool _toggleBroadPhase = true;

	// Particle system toggles
	bool _toggleParticleGravity = false;
	bool _toggleParticleFriction = false;
	bool _toggleParticleDrag = false;
	bool _toggleParticleSystem = false;
	float _particleTimeAlive = 5.0f;

	// Matrix interpolation
	bool _gameobjectsMatrixInterpolation = true;
	bool _cameraMatrixInterpolation = true;

	// Main menu toggle
	bool _mainMenu = true;
#pragma endregion

#pragma region Initialisation Methods
	/// <summary>
	/// Creates the window handle.
	/// </summary>
	/// <param name="hInstance">Handle to the instance.</param>
	/// <param name="nCmdShow">Command show parameter.</param>
	/// <returns>HRESULT indicating success or failure.</returns>
	HRESULT CreateWindowHandle(HINSTANCE hInstance, int nCmdShow);

	/// <summary>
	/// Creates the Direct3D device.
	/// </summary>
	/// <returns>HRESULT indicating success or failure.</returns>
	HRESULT CreateD3DDevice();

	/// <summary>
	/// Creates the swap chain and frame buffer.
	/// </summary>
	/// <returns>HRESULT indicating success or failure.</returns>
	HRESULT CreateSwapChainAndFrameBuffer();

	/// <summary>
	/// Initializes the IMGUI.
	/// </summary>
	void InitGUI() const;

	/// <summary>
	/// Initializes the shaders and input layout.
	/// </summary>
	/// <returns>HRESULT indicating success or failure.</returns>
	HRESULT InitShadersAndInputLayout();

	/// <summary>
	/// Initializes the vertex and index buffers.
	/// </summary>
	/// <returns>HRESULT indicating success or failure.</returns>
	HRESULT InitVertexIndexBuffers();

	/// <summary>
	/// Initializes the pipeline states.
	/// </summary>
	/// <returns>HRESULT indicating success or failure.</returns>
	HRESULT InitPipelineStates();

	/// <summary>
	/// Initializes the runtime data.
	/// </summary>
	/// <returns>HRESULT indicating success or failure.</returns>
	HRESULT InitRunTimeData();

	/// <summary>
	/// Loads the scene camera variables.
	/// </summary>
	void LoadSceneCameraVariables();
#pragma endregion

#pragma region Object Movement
	/// <summary>
	/// Handles basic object movement.
	/// </summary>
	/// <param name="deltaTime">Time elapsed since the last frame.</param>
	/// <param name="objectSelected">Index of the selected object.</param>
	void BasicObjectMovement(float deltaTime, int objectSelected) const;
#pragma endregion

public:
#pragma region Destructor
	/// <summary>
	/// Destructor for the scene
	/// </summary>
	~DX11PhysicsFramework();
#pragma endregion

#pragma region Main Functions

	/// <summary>
	/// Initializes the DX11framework.
	/// </summary>
	/// <param name="hInstance">Handle to the instance.</param>
	/// <param name="nCmdShow">Command show parameter.</param>
	/// <returns>HRESULT indicating success or failure.</returns>
	HRESULT Initialise(HINSTANCE hInstance, int nCmdShow);

	/// <summary>
	/// Updates the time-step.
	/// </summary>
	void Update();

	/// <summary>
	/// Updates the physics for the objects.
	/// </summary>
	void PhysicsUpdate() const;

	/// <summary>
	/// Detects collisions between objects.
	/// </summary>
	void DetectCollisions() const;

	/// <summary>
	/// Performs general updates that don't need to be updating inside the physics loop.
	/// </summary>
	/// <param name="deltaTime">Time elapsed since the last frame.</param>
	void GeneralUpdate(float deltaTime);

	/// <summary>
	/// Draws the object select window.
	/// </summary>
	void DrawObjectSelectWindow();

	/// <summary>
	/// Draws the stats window.
	/// </summary>
	void DrawStatsWindow() const;

	/// <summary>
	/// Draws the object movement control window.
	/// </summary>
	/// <param name="deltaTime">Time elapsed since the last frame.</param>
	/// <param name="objectSelected">Index of the selected object.</param>
	void DrawObjectMovementControlWindow(float deltaTime, int objectSelected);

	/// <summary>
	/// Draws the camera window.
	/// </summary>
	void DrawCameraWindow();

	/// <summary>
	/// Draws the integration window.
	/// </summary>
	/// <param name="objectSelected">Index of the selected object.</param>
	void DrawIntegrationWindow(int objectSelected) const;

	/// <summary>
	/// Draws the matrix interpolation window.
	/// </summary>
	void DrawMatrixInterpolationWindow();

	/// <summary>
	/// Draws the particle system window.
	/// </summary>
	void DrawParticleSystemWindow();

	/// <summary>
	/// Draws the main menu UI.
	/// </summary>
	void DrawMainMenuUI();

	/// <summary>
	/// Draws the back to main menu window.
	/// </summary>
	void DrawBackToMainMenuWindow();

	/// <summary>
	/// Draws the UI by calling all the UI functions.
	/// </summary>
	void DrawUI();

	/// <summary>
	/// Draws the scene to the camera.
	/// </summary>
	/// <param name="alphaScalar">Alpha scalar for interpolation.</param>
	void Draw(double alphaScalar);
#pragma endregion

#pragma region Getters
	/// <summary>
	/// Gets the window handle.
	/// </summary>
	/// <returns>Handle to the window.</returns>
	HWND GetWindowHandle() const { return _windowHandle; }

	/// <summary>
	/// Gets the Direct3D device.
	/// </summary>
	/// <returns>Pointer to the Direct3D device.</returns>
	ID3D11Device* GetDevice() const { return _device; }

	/// <summary>
	/// Gets the Direct3D device context.
	/// </summary>
	/// <returns>Pointer to the Direct3D device context.</returns>
	ID3D11DeviceContext* GetDeviceContext() const { return _immediateContext; }
#pragma endregion
};