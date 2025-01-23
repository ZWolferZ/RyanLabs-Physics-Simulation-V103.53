#include "DX11PhysicsFramework.h"
#include "SphereCollider.h"
#include "AABBCollider.h"

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	if (ImGui::GetCurrentContext() != nullptr)
	{
		ImGuiIO& io = ImGui::GetIO();
		switch (message)
		{
		case WM_LBUTTONDOWN:
		case WM_LBUTTONDBLCLK:
			io.MouseDown[0] = true;
			return 0;
		case WM_RBUTTONDOWN:
		case WM_RBUTTONDBLCLK:
			io.MouseDown[1] = true;
			return 0;
		case WM_MBUTTONDOWN:
		case WM_MBUTTONDBLCLK:
			io.MouseDown[2] = true;
			return 0;
		case WM_LBUTTONUP:
			io.MouseDown[0] = false;
			return 0;
		case WM_RBUTTONUP: io.MouseDown[1] = false;
			return 0;
		case WM_MBUTTONUP: io.MouseDown[2] = false;
			return 0;
		case WM_MOUSEWHEEL: io.MouseWheel += GET_WHEEL_DELTA_WPARAM(wParam) / WHEEL_DELTA;
			return 0;
		case WM_MOUSEHWHEEL: io.MouseWheelH += GET_WHEEL_DELTA_WPARAM(wParam) / WHEEL_DELTA;
			return 0;
		case WM_MOUSEMOVE: io.MousePos.x = static_cast<float>(GET_X_LPARAM(lParam));
			io.MousePos.y = static_cast<float>(GET_Y_LPARAM(lParam));
			return 0;
		default:;
		}
	}

	switch (message)
	{
	case WM_PAINT:
	{
		PAINTSTRUCT ps;
		HDC hdc = BeginPaint(hWnd, &ps);
		EndPaint(hWnd, &ps);
		break;
	}
	case WM_DESTROY:
		PostQuitMessage(0);
		break;

	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}

	// YOU WILL BE MAXIMIZED
	ShowWindow(hWnd, SW_MAXIMIZE);
	ShowCursor(TRUE);

	return 0;
}

HRESULT DX11PhysicsFramework::Initialise(HINSTANCE hInstance, int nShowCmd)
{
	HRESULT hr = S_OK;

	hr = CreateWindowHandle(hInstance, nShowCmd);
	if (FAILED(hr)) return E_FAIL;

	hr = CreateD3DDevice();
	if (FAILED(hr)) return E_FAIL;

	hr = CreateSwapChainAndFrameBuffer();
	if (FAILED(hr)) return E_FAIL;

	InitGUI();

	hr = InitShadersAndInputLayout();
	if (FAILED(hr)) return E_FAIL;

	hr = InitVertexIndexBuffers();
	if (FAILED(hr)) return E_FAIL;

	hr = InitPipelineStates();
	if (FAILED(hr)) return E_FAIL;

	hr = InitRunTimeData();
	if (FAILED(hr)) return E_FAIL;

	return hr;
}

HRESULT DX11PhysicsFramework::CreateWindowHandle(HINSTANCE hInstance, int nCmdShow)
{
	nCmdShow = 0;

	auto windowName = L"RyanLabs Proprietary Real-Time Physics Framework";

	WNDCLASSW wndClass = {};
	wndClass.style = 0;
	wndClass.lpfnWndProc = WndProc;
	wndClass.cbClsExtra = 0;
	wndClass.cbWndExtra = 0;
	wndClass.hInstance = nullptr;
	wndClass.hCursor = nullptr;
	wndClass.hbrBackground = nullptr;
	wndClass.lpszMenuName = nullptr;
	wndClass.lpszClassName = windowName;
	wndClass.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(1)); // Load the RyanLabs Icon

	RegisterClassW(&wndClass);

	_windowHandle = CreateWindowExW(0, windowName, windowName, WS_OVERLAPPEDWINDOW | WS_VISIBLE, CW_USEDEFAULT,
		CW_USEDEFAULT, WindowWidth, WindowHeight, nullptr, nullptr, hInstance, nullptr);

	return S_OK;
}

HRESULT DX11PhysicsFramework::CreateD3DDevice()
{
	HRESULT hr = S_OK;

	D3D_FEATURE_LEVEL featureLevels[] = {
		D3D_FEATURE_LEVEL_11_1, D3D_FEATURE_LEVEL_11_0,
		D3D_FEATURE_LEVEL_10_1, D3D_FEATURE_LEVEL_10_0,
	};

	ID3D11Device* baseDevice;
	ID3D11DeviceContext* baseDeviceContext;

	DWORD createDeviceFlags = 0;
#ifdef _DEBUG
	createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif
	hr = D3D11CreateDevice(nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr,
		D3D11_CREATE_DEVICE_BGRA_SUPPORT | createDeviceFlags, featureLevels,
		ARRAYSIZE(featureLevels), D3D11_SDK_VERSION, &baseDevice, nullptr, &baseDeviceContext);
	if (FAILED(hr)) return hr;

	///////////////////////////////////////////////////////////////////////////////////////////////

	hr = baseDevice->QueryInterface(__uuidof(ID3D11Device), reinterpret_cast<void**>(&_device));
	hr = baseDeviceContext->QueryInterface(__uuidof(ID3D11DeviceContext), reinterpret_cast<void**>(&_immediateContext));

	baseDevice->Release();
	baseDeviceContext->Release();

	///////////////////////////////////////////////////////////////////////////////////////////////

	hr = _device->QueryInterface(__uuidof(IDXGIDevice), reinterpret_cast<void**>(&_dxgiDevice));
	if (FAILED(hr)) return hr;

	IDXGIAdapter* dxgiAdapter;
	hr = _dxgiDevice->GetAdapter(&dxgiAdapter);
	hr = dxgiAdapter->GetParent(__uuidof(IDXGIFactory2), reinterpret_cast<void**>(&_dxgiFactory));
	dxgiAdapter->Release();

	return S_OK;
}

HRESULT DX11PhysicsFramework::CreateSwapChainAndFrameBuffer()
{
	HRESULT hr = S_OK;
	DXGI_SWAP_CHAIN_DESC1 swapChainDesc = {};

	swapChainDesc.Width = 1920; // Defer to WindowWidth
	swapChainDesc.Height = 1080; // Defer to WindowHeight
	swapChainDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM; //FLIP* modes don't support sRGB backbuffer
	swapChainDesc.Stereo = FALSE;
	swapChainDesc.SampleDesc.Count = 1;
	swapChainDesc.SampleDesc.Quality = 0;
	swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swapChainDesc.BufferCount = 2;
	swapChainDesc.Scaling = DXGI_SCALING_NONE;
	swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_SEQUENTIAL;
	swapChainDesc.AlphaMode = DXGI_ALPHA_MODE_UNSPECIFIED;
	swapChainDesc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_TEARING;

	hr = _dxgiFactory->CreateSwapChainForHwnd(_device, _windowHandle, &swapChainDesc, nullptr, nullptr, &_swapChain);
	if (FAILED(hr)) return hr;

	///////////////////////////////////////////////////////////////////////////////////////////////

	ID3D11Texture2D* frameBuffer = nullptr;
	hr = _swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<void**>(&frameBuffer));
	if (FAILED(hr)) return hr;

	D3D11_RENDER_TARGET_VIEW_DESC framebufferDesc = {};
	framebufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB; //sRGB render target enables hardware gamma correction
	framebufferDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
	hr = _device->CreateRenderTargetView(frameBuffer, &framebufferDesc, &_frameBufferView);
	frameBuffer->Release();

	D3D11_TEXTURE2D_DESC depthBufferDesc;
	frameBuffer->GetDesc(&depthBufferDesc); // copy from framebuffer properties
	depthBufferDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthBufferDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;

	if (_depthStencilBuffer == nullptr || _depthStencilBuffer != nullptr)
	{
		hr = _device->CreateTexture2D(&depthBufferDesc, nullptr, &_depthStencilBuffer);
		hr = _device->CreateDepthStencilView(_depthStencilBuffer, nullptr, &_depthBufferView);
	}

	return hr;
}

void DX11PhysicsFramework::InitGUI() const
{
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();
	(void)io;
	ImGui::StyleColorsDark();
	ImGui_ImplWin32_Init(GetWindowHandle());
	ImGui_ImplDX11_Init(GetDevice(), GetDeviceContext());

	io.IniFilename = nullptr;
}

HRESULT DX11PhysicsFramework::InitShadersAndInputLayout()
{
	HRESULT hr = S_OK;
	ID3DBlob* errorBlob;

	DWORD dwShaderFlags = D3DCOMPILE_ENABLE_STRICTNESS;
#ifdef _DEBUG
	// Set the D3DCOMPILE_DEBUG flag to embed debug information in the shaders.
	// Setting this flag improves the shader debugging experience, but still allows
	// the shaders to be optimized and to run exactly the way they will run in
	// the release configuration of this program.
	dwShaderFlags |= D3DCOMPILE_DEBUG;
#endif

	ID3DBlob* vsBlob;

	// Compile the vertex shader
	hr = D3DCompileFromFile(L"SimpleShaders.hlsl", nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE, "VS_main", "vs_5_0",
		dwShaderFlags, 0, &vsBlob, &errorBlob);
	if (FAILED(hr))
	{
		MessageBoxA(_windowHandle, static_cast<char*>(errorBlob->GetBufferPointer()), nullptr, ERROR);
		errorBlob->Release();
		return hr;
	}

	// Create the vertex shader
	hr = _device->CreateVertexShader(vsBlob->GetBufferPointer(), vsBlob->GetBufferSize(), nullptr, &_vertexShader);

	if (FAILED(hr))
	{
		vsBlob->Release();
		return hr;
	}

	// Compile the pixel shader
	ID3DBlob* psBlob;
	hr = D3DCompileFromFile(L"SimpleShaders.hlsl", nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE, "PS_main", "ps_5_0",
		dwShaderFlags, 0, &psBlob, &errorBlob);
	if (FAILED(hr))
	{
		MessageBoxA(_windowHandle, static_cast<char*>(errorBlob->GetBufferPointer()), nullptr, ERROR);
		errorBlob->Release();
		return hr;
	}

	// Create the pixel shader
	hr = _device->CreatePixelShader(psBlob->GetBufferPointer(), psBlob->GetBufferSize(), nullptr, &_pixelShader);

	if (FAILED(hr))
		return hr;

	// Define the input layout
	D3D11_INPUT_ELEMENT_DESC inputElementDesc[] =
	{
		{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 24, D3D11_INPUT_PER_VERTEX_DATA, 0},
	};

	// Create the input layout
	hr = _device->CreateInputLayout(inputElementDesc, ARRAYSIZE(inputElementDesc), vsBlob->GetBufferPointer(),
		vsBlob->GetBufferSize(), &_inputLayout);

	vsBlob->Release();
	psBlob->Release();
	errorBlob->Release();

	return hr;
}

HRESULT DX11PhysicsFramework::InitVertexIndexBuffers()
{
	// Create vertex buffer
	SimpleVertex vertices[] =
	{
		{XMFLOAT3(-1.0f, 1.0f, -1.0f), XMFLOAT3(0, 1.0f, 0), XMFLOAT2(1.0f, 0.0f)},
		{XMFLOAT3(1.0f, 1.0f, -1.0f), XMFLOAT3(0, 1.0f, 0), XMFLOAT2(0.0f, 0.0f)},
		{XMFLOAT3(1.0f, 1.0f, 1.0f), XMFLOAT3(0, 1.0f, 0), XMFLOAT2(0.0f, 1.0f)},
		{XMFLOAT3(-1.0f, 1.0f, 1.0f), XMFLOAT3(0, 1.0f, 0), XMFLOAT2(1.0f, 1.0f)},

		{XMFLOAT3(-1.0f, -1.0f, -1.0f), XMFLOAT3(0, -1.0f, 0), XMFLOAT2(0.0f, 0.0f)},
		{XMFLOAT3(1.0f, -1.0f, -1.0f), XMFLOAT3(0, -1.0f, 0), XMFLOAT2(1.0f, 0.0f)},
		{XMFLOAT3(1.0f, -1.0f, 1.0f), XMFLOAT3(0, -1.0f, 0), XMFLOAT2(1.0f, 1.0f)},
		{XMFLOAT3(-1.0f, -1.0f, 1.0f), XMFLOAT3(0, -1.0f, 0), XMFLOAT2(0.0f, 1.0f)},

		{XMFLOAT3(-1.0f, -1.0f, 1.0f), XMFLOAT3(-1.0f, 0, 0), XMFLOAT2(0.0f, 1.0f)},
		{XMFLOAT3(-1.0f, -1.0f, -1.0f), XMFLOAT3(-1.0f, 0, 0), XMFLOAT2(1.0f, 1.0f)},
		{XMFLOAT3(-1.0f, 1.0f, -1.0f), XMFLOAT3(-1.0f, 0, 0), XMFLOAT2(1.0f, 0.0f)},
		{XMFLOAT3(-1.0f, 1.0f, 1.0f), XMFLOAT3(-1.0f, 0, 0), XMFLOAT2(0.0f, 0.0f)},

		{XMFLOAT3(1.0f, -1.0f, 1.0f), XMFLOAT3(1.0f, 0, 0), XMFLOAT2(1.0f, 1.0f)},
		{XMFLOAT3(1.0f, -1.0f, -1.0f), XMFLOAT3(1.0f, 0, 0), XMFLOAT2(0.0f, 1.0f)},
		{XMFLOAT3(1.0f, 1.0f, -1.0f), XMFLOAT3(1.0f, 0, 0), XMFLOAT2(0.0f, 0.0f)},
		{XMFLOAT3(1.0f, 1.0f, 1.0f), XMFLOAT3(1.0f, 0, 0), XMFLOAT2(1.0f, 0.0f)},

		{XMFLOAT3(-1.0f, -1.0f, -1.0f), XMFLOAT3(0, 0, -1.0f), XMFLOAT2(0.0f, 1.0f)},
		{XMFLOAT3(1.0f, -1.0f, -1.0f), XMFLOAT3(0, 0, -1.0f), XMFLOAT2(1.0f, 1.0f)},
		{XMFLOAT3(1.0f, 1.0f, -1.0f), XMFLOAT3(0, 0, -1.0f), XMFLOAT2(1.0f, 0.0f)},
		{XMFLOAT3(-1.0f, 1.0f, -1.0f), XMFLOAT3(0, 0, -1.0f), XMFLOAT2(0.0f, 0.0f)},

		{XMFLOAT3(-1.0f, -1.0f, 1.0f), XMFLOAT3(0, 0, 1.0f), XMFLOAT2(1.0f, 1.0f)},
		{XMFLOAT3(1.0f, -1.0f, 1.0f), XMFLOAT3(0, 0, 1.0f), XMFLOAT2(0.0f, 1.0f)},
		{XMFLOAT3(1.0f, 1.0f, 1.0f), XMFLOAT3(0, 0, 1.0f), XMFLOAT2(0.0f, 0.0f)},
		{XMFLOAT3(-1.0f, 1.0f, 1.0f), XMFLOAT3(0, 0, 1.0f), XMFLOAT2(1.0f, 0.0f)},
	};

	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(SimpleVertex) * 24;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = 0;

	D3D11_SUBRESOURCE_DATA InitData;
	ZeroMemory(&InitData, sizeof(InitData));
	InitData.pSysMem = vertices;

	HRESULT hr = _device->CreateBuffer(&bd, &InitData, &_cubeVertexBuffer);

	if (FAILED(hr))
		return hr;

	// Create index buffer
	WORD indices[] =
	{
		3, 1, 0,
		2, 1, 3,

		6, 4, 5,
		7, 4, 6,

		11, 9, 8,
		10, 9, 11,

		14, 12, 13,
		15, 12, 14,

		19, 17, 16,
		18, 17, 19,

		22, 20, 21,
		23, 20, 22
	};

	ZeroMemory(&bd, sizeof(bd));

	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(WORD) * 36;
	bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
	bd.CPUAccessFlags = 0;

	ZeroMemory(&InitData, sizeof(InitData));
	InitData.pSysMem = indices;
	hr = _device->CreateBuffer(&bd, &InitData, &_cubeIndexBuffer);

	if (FAILED(hr))
		return hr;

	// Create vertex buffer
	SimpleVertex planeVertices[] =
	{
		{XMFLOAT3(-1.0f, -1.0f, 0.0f), XMFLOAT3(0.0f, 0.0f, -1.0f), XMFLOAT2(0.0f, 5.0f)},
		{XMFLOAT3(1.0f, -1.0f, 0.0f), XMFLOAT3(0.0f, 0.0f, -1.0f), XMFLOAT2(5.0f, 5.0f)},
		{XMFLOAT3(1.0f, 1.0f, 0.0f), XMFLOAT3(0.0f, 0.0f, -1.0f), XMFLOAT2(5.0f, 0.0f)},
		{XMFLOAT3(-1.0f, 1.0f, 0.0f), XMFLOAT3(0.0f, 0.0f, -1.0f), XMFLOAT2(0.0f, 0.0f)},
	};

	ZeroMemory(&bd, sizeof(bd));
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(SimpleVertex) * 4;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = 0;

	ZeroMemory(&InitData, sizeof(InitData));
	InitData.pSysMem = planeVertices;

	hr = _device->CreateBuffer(&bd, &InitData, &_planeVertexBuffer);

	if (FAILED(hr))
		return hr;

	// Create plane index buffer
	WORD planeIndices[] =
	{
		0, 3, 1,
		3, 2, 1,
	};

	ZeroMemory(&bd, sizeof(bd));
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(WORD) * 6;
	bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
	bd.CPUAccessFlags = 0;

	ZeroMemory(&InitData, sizeof(InitData));
	InitData.pSysMem = planeIndices;
	hr = _device->CreateBuffer(&bd, &InitData, &_planeIndexBuffer);

	if (FAILED(hr))
		return hr;

	return S_OK;
}

HRESULT DX11PhysicsFramework::InitPipelineStates()
{
	HRESULT hr = S_OK;

	_immediateContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	_immediateContext->IASetInputLayout(_inputLayout);

	// Rasterizer
	D3D11_RASTERIZER_DESC cmdesc;
	ZeroMemory(&cmdesc, sizeof(D3D11_RASTERIZER_DESC));
	cmdesc.FillMode = D3D11_FILL_SOLID;
	cmdesc.CullMode = D3D11_CULL_NONE;
	hr = _device->CreateRasterizerState(&cmdesc, &RSCullNone);

	ZeroMemory(&cmdesc, sizeof(D3D11_RASTERIZER_DESC));
	cmdesc.FillMode = D3D11_FILL_SOLID;
	cmdesc.CullMode = D3D11_CULL_BACK;
	cmdesc.FrontCounterClockwise = true;
	hr = _device->CreateRasterizerState(&cmdesc, &CCWcullMode);

	cmdesc.FrontCounterClockwise = false;
	hr = _device->CreateRasterizerState(&cmdesc, &CWcullMode);

	_immediateContext->RSSetState(CWcullMode);

	D3D11_DEPTH_STENCIL_DESC dssDesc;
	ZeroMemory(&dssDesc, sizeof(D3D11_DEPTH_STENCIL_DESC));
	dssDesc.DepthEnable = true;
	dssDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	dssDesc.DepthFunc = D3D11_COMPARISON_LESS_EQUAL;

	_device->CreateDepthStencilState(&dssDesc, &DSLessEqual);

	_immediateContext->OMSetDepthStencilState(DSLessEqual, 0);

	D3D11_SAMPLER_DESC bilinearSamplerdesc = {};
	bilinearSamplerdesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	bilinearSamplerdesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	bilinearSamplerdesc.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;
	bilinearSamplerdesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	bilinearSamplerdesc.MaxLOD = static_cast<float>(INT_MAX);
	bilinearSamplerdesc.MinLOD = 0;

	hr = _device->CreateSamplerState(&bilinearSamplerdesc, &_samplerLinear);
	if (FAILED(hr)) return hr;

	return S_OK;
}

HRESULT DX11PhysicsFramework::InitRunTimeData()
{
	HRESULT hr = S_OK;

	D3D11_BUFFER_DESC constantBufferDesc = {};
	constantBufferDesc.ByteWidth = sizeof(ConstantBuffer);
	constantBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	constantBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	constantBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

	_viewport = { 0.0f, 0.0f, static_cast<float>(WindowWidth), static_cast<float>(WindowHeight), 0.0f, 1.0f };
	_immediateContext->RSSetViewports(1, &_viewport);

	hr = _device->CreateBuffer(&constantBufferDesc, nullptr, &_constantBuffer);
	if (FAILED(hr)) { return hr; }

	hr = CreateDDSTextureFromFile(_device, L"Resources\\Textures\\stone.dds", nullptr, &StoneTextureRV);
	hr = CreateDDSTextureFromFile(_device, L"Resources\\Textures\\floor.dds", nullptr, &GroundTextureRV);
	hr = CreateDDSTextureFromFile(_device, L"Resources\\Textures\\Selected.dds", nullptr, &SelectedTexture);
	if (FAILED(hr)) { return hr; }

	// Setup Camera
	_camera = new Camera();

	LoadSceneCameraVariables();

	// Setup the scene's light
	basicLight.AmbientLight = XMFLOAT4(0.2f, 0.2f, 0.2f, 1.0f);
	basicLight.DiffuseLight = XMFLOAT4(0.6f, 0.6f, 0.6f, 1.0f);
	basicLight.SpecularLight = XMFLOAT4(0.8f, 0.8f, 0.8f, 1.0f);
	basicLight.SpecularPower = 10.0f;
	basicLight.LightVecW = XMFLOAT3(0.0f, 0.5f, -1.0f);

	Geometry cubeGeometry = {};
	cubeGeometry.indexBuffer = _cubeIndexBuffer;
	cubeGeometry.vertexBuffer = _cubeVertexBuffer;
	cubeGeometry.numberOfIndices = 36;
	cubeGeometry.vertexBufferOffset = 0;
	cubeGeometry.vertexBufferStride = sizeof(SimpleVertex);

	Geometry planeGeometry = {};
	planeGeometry.indexBuffer = _planeIndexBuffer;
	planeGeometry.vertexBuffer = _planeVertexBuffer;
	planeGeometry.numberOfIndices = 6;
	planeGeometry.vertexBufferOffset = 0;
	planeGeometry.vertexBufferStride = sizeof(SimpleVertex);

	Material shinyMaterial = {};
	shinyMaterial.ambient = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	shinyMaterial.diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	shinyMaterial.specular = XMFLOAT4(0.5f, 0.5f, 0.5f, 1.0f);

	Material noSpecMaterial = {};
	noSpecMaterial.ambient = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	noSpecMaterial.diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	noSpecMaterial.specular = XMFLOAT4(0.0f, 0.0f, 0.0f, 0.0f);

	auto gameObject = new GameObject("Floor", planeGeometry, noSpecMaterial, GroundTextureRV,
		Vector(0.0f, 0.0f, 0.0f),
		Vector(15.0f, 15.0f, 15.0f), Vector(90.0f, 0.0f, 0.0f), 1,
		false);

	Vector floorMinPoints = gameObject->GetTransform()->GetPosition() - Vector(15.0f, 0.1f, 15.0f);
	Vector floorMaxPoints = gameObject->GetTransform()->GetPosition() + Vector(15.0f, 0.1f, 15.0f);

	Collider* floorCollider = new AABBCollider(gameObject->GetTransform(), floorMinPoints, floorMaxPoints);
	gameObject->GetPhysicsModel()->SetCollider(floorCollider);

	gameObject->GetPhysicsModel()->_simulateDrag = true;
	gameObject->GetPhysicsModel()->_simulateFriction = true;

	_gameObjects.push_back(gameObject);

	for (auto i = 0; i < 4; i++)
	{
		gameObject = new GameObject("Cube", cubeGeometry, shinyMaterial, StoneTextureRV,
			Vector(-2.0f + (i * 2.5f), 1.0f, 10.0f),
			Vector(1.0f, 1.0f, 1.0f), Vector(0.0f, 0.0f, 0.0f), 1, false);

		Vector minPoints = gameObject->GetTransform()->GetPosition() - Vector(1.0f, 1.0f, 1.0f);
		Vector maxPoints = gameObject->GetTransform()->GetPosition() + Vector(1.0f, 1.0f, 1.0f);

		Collider* collider = new AABBCollider(gameObject->GetTransform(), minPoints, maxPoints);

		gameObject->GetPhysicsModel()->SetCollider(collider);

		gameObject->GetPhysicsModel()->_simulateGravity = true;
		gameObject->GetPhysicsModel()->_simulateDrag = true;
		gameObject->GetPhysicsModel()->_simulateFriction = true;

		_gameObjects.push_back(gameObject);
	}

	gameObject = new GameObject("Donut", "Resources\\OBJ\\donut.obj", shinyMaterial, StoneTextureRV, *_device,
		Vector(-5.0f, 1.0f, 10.0f),
		Vector(1.0f, 1.0f, 1.0f),
		Vector(0.0f, 0.0f, 0.0f), 1.0f, false);

	Collider* donutCollider = new SphereCollider(gameObject->GetTransform(), 1.0f);

	gameObject->GetPhysicsModel()->SetCollider(donutCollider);

	gameObject->GetPhysicsModel()->_simulateGravity = true;
	gameObject->GetPhysicsModel()->_simulateDrag = true;
	gameObject->GetPhysicsModel()->_simulateFriction = true;

	_gameObjects.push_back(gameObject);

	gameObject = new GameObject("Donut", "Resources\\OBJ\\donut.obj", shinyMaterial, StoneTextureRV, *_device,
		Vector(-9.0f, 1.0f, 10.0f),
		Vector(1.0f, 1.0f, 1.0f),
		Vector(0.0f, 0.0f, 0.0f), 1.0f, false);

	Collider* donutCollider2 = new SphereCollider(gameObject->GetTransform(), 1.0f);

	gameObject->GetPhysicsModel()->SetCollider(donutCollider2);

	gameObject->GetPhysicsModel()->_simulateGravity = true;
	gameObject->GetPhysicsModel()->_simulateDrag = true;
	gameObject->GetPhysicsModel()->_simulateFriction = true;

	_gameObjects.push_back(gameObject);

	_gameObjectSize = _gameObjects.size();

	return S_OK;
}

void DX11PhysicsFramework::LoadSceneCameraVariables()
{
	std::ifstream file("JSON Files\\Scene Camera Variables.json");

	// Check if the file is open
	if (!file.is_open())
	{
		std::cerr << "Failed to open JSON file." << std::endl;
	}

	// Load the JSON file
	file >> m_sceneCameraVariables;

	//Camera
	const int aspect = WindowWidth / WindowHeight;

	//////////////////////////////////////

	_camera->SetPosition(
		m_sceneCameraVariables["SceneCameraVariables"]["DebugCamera"]["Position"]["x"].get<float>(),
		m_sceneCameraVariables["SceneCameraVariables"]["DebugCamera"]["Position"]["y"].get<float>(),
		m_sceneCameraVariables["SceneCameraVariables"]["DebugCamera"]["Position"]["z"].get<
		float>());

	_camera->SetRotation(
		m_sceneCameraVariables["SceneCameraVariables"]["DebugCamera"]["Rotation"]["x"].get<float>(),
		m_sceneCameraVariables["SceneCameraVariables"]["DebugCamera"]["Rotation"]["y"].get<float>(),
		m_sceneCameraVariables["SceneCameraVariables"]["DebugCamera"]["Rotation"]["z"].get<
		float>());

	_camera->SetProjectionValues(
		m_sceneCameraVariables["SceneCameraVariables"]["DebugCamera"]["ProjectionValues"]["fov"].get<float>(), aspect,
		m_sceneCameraVariables["SceneCameraVariables"]["DebugCamera"]["ProjectionValues"]["near"].get<float>(),
		m_sceneCameraVariables["SceneCameraVariables"]["DebugCamera"]["ProjectionValues"]["far"].get<float>());

	_camera->m_startingPosition = _camera->GetPosition();
	// Close the file
	file.close();
}

void DX11PhysicsFramework::BasicObjectMovement(float deltaTime, int objectSelected) const
{
	if (GetAsyncKeyState(VK_NUMPAD5) & 0xFFFF)
	{
		_gameObjects[objectSelected]->GetTransform()->Move(Vector(0.0f, 0.0f, -1.0f), deltaTime, _objectMoveSpeed);
	}
	if (GetAsyncKeyState(VK_NUMPAD8) & 0xFFFF)
	{
		_gameObjects[objectSelected]->GetTransform()->Move(Vector(0.0f, 0.0f, 1.0f), deltaTime, _objectMoveSpeed);
	}
	if (GetAsyncKeyState(VK_NUMPAD4) & 0xFFFF)
	{
		_gameObjects[objectSelected]->GetTransform()->Move(Vector(-1.0f, 0.0f, 0.0f), deltaTime, _objectMoveSpeed);
	}
	if (GetAsyncKeyState(VK_NUMPAD6) & 0xFFFF)
	{
		_gameObjects[objectSelected]->GetTransform()->Move(Vector(1.0f, 0.0f, 0.0f), deltaTime, _objectMoveSpeed);
	}

	if (GetAsyncKeyState(VK_NUMPAD7) & 0xFFFF)
	{
		_gameObjects[objectSelected]->GetTransform()->Move(Vector(0, 1.0f, 0), deltaTime, _objectMoveSpeed);
	}
	if (GetAsyncKeyState(VK_NUMPAD9) & 0xFFFF)
	{
		_gameObjects[objectSelected]->GetTransform()->Move(Vector(0, -1.0f, 0), deltaTime, _objectMoveSpeed);
	}

	if (GetAsyncKeyState(VK_NUMPAD1) & 0xFFFF)
	{
		_gameObjects[objectSelected]->GetTransform()->Rotate(Vector(1.0f, 0, 0), deltaTime, _objectRotateSpeed);
	}

	if (GetAsyncKeyState(VK_NUMPAD2) & 0xFFFF)
	{
		_gameObjects[objectSelected]->GetTransform()->Rotate(Vector(0, 1.0f, 0.0f), deltaTime, _objectRotateSpeed);
	}
	if (GetAsyncKeyState(VK_NUMPAD3) & 0xFFFF)
	{
		_gameObjects[objectSelected]->GetTransform()->Rotate(Vector(0, 0, 1.0f), deltaTime, _objectRotateSpeed);
	}
	if (GetAsyncKeyState(VK_ADD) & 0xFFFF)
	{
		_gameObjects[objectSelected]->GetTransform()->Scale(
			Vector(1.0f, 1.0f, 1.0f), deltaTime, _objectScaleSpeed);
	}

	if (GetAsyncKeyState(VK_SUBTRACT) & 0xFFFF)
	{
		_gameObjects[objectSelected]->GetTransform()->Scale(
			Vector(-1.0f, -1.0f, -1.0f), deltaTime, _objectScaleSpeed);
	}

	if (GetAsyncKeyState(VK_NUMPAD0) & 0xFFFF)
	{
		_gameObjects[objectSelected]->GetTransform()->Reset();
	}
}

DX11PhysicsFramework::~DX11PhysicsFramework()
{
	delete _camera;
	for each(GameObject * go in _gameObjects)
	{
		delete go;
	}

	if (_immediateContext)_immediateContext->Release();

	if (_frameBufferView)_frameBufferView->Release();
	if (_depthBufferView)_depthBufferView->Release();
	if (_depthStencilBuffer)_depthStencilBuffer->Release();
	if (_swapChain)_swapChain->Release();
	if (CWcullMode)CWcullMode->Release();
	if (CCWcullMode)CCWcullMode->Release();
	if (_vertexShader)_vertexShader->Release();
	if (_inputLayout)_inputLayout->Release();
	if (_pixelShader)_pixelShader->Release();
	if (_constantBuffer)_constantBuffer->Release();

	if (_cubeVertexBuffer)_cubeVertexBuffer->Release();
	if (_cubeIndexBuffer)_cubeIndexBuffer->Release();
	if (_planeVertexBuffer)_planeVertexBuffer->Release();
	if (_planeIndexBuffer)_planeIndexBuffer->Release();
	if (_objMeshData.IndexBuffer) _objMeshData.IndexBuffer->Release();
	if (_objMeshData.VertexBuffer)_objMeshData.VertexBuffer->Release();

	if (DSLessEqual) DSLessEqual->Release();
	if (RSCullNone) RSCullNone->Release();

	if (_samplerLinear)_samplerLinear->Release();
	if (StoneTextureRV)StoneTextureRV->Release();
	if (GroundTextureRV)GroundTextureRV->Release();
	if (SelectedTexture)SelectedTexture->Release();

	if (_dxgiDevice)_dxgiDevice->Release();
	if (_dxgiFactory)_dxgiFactory->Release();
	if (_device)_device->Release();

	ImGui_ImplDX11_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();
}

void DX11PhysicsFramework::Update()
{
	//Static initializes this value only once
	static ULONGLONG frameStart = GetTickCount64();

	ULONGLONG frameNow = GetTickCount64();
	deltaTime = (frameNow - frameStart) / 1000.0f;
	frameStart = frameNow;

	_runtimeTimer += deltaTime;

	static Timer frame_timer;

	_accumulator += frame_timer.GetDeltaTime();

#ifdef _DEBUG
	if (_accumulator > 1.0f) // assume back from breakpoint
		_accumulator = FPS60;
#endif

	while (_accumulator >= FPS60)
	{
		// Update physics
		PhysicsUpdate();
		_accumulator -= FPS60;
		frame_timer.Tick();
	}

	// Update the general game loop
	GeneralUpdate(deltaTime);

	const double alpha = _accumulator / FPS60;

	Draw(alpha);
}

void DX11PhysicsFramework::PhysicsUpdate() const
{
	DetectCollisions();

	for (auto gameObject : _gameObjects)
	{
		gameObject->Update(FPS60);
	}
}

// This function killed me to figure out, but essentially it's a nested for loop that checks for collisions between all objects,
// Once a collision is detected it will store the pair of objects that collided in a vector, which is sorted out after the collision detection loop.
// If you don't store them in a pair and resolve them this way, the multiple collisions that happen at the same time will spit out garbage normal data for some reason.
void DX11PhysicsFramework::DetectCollisions() const
{
	// Set all collision bools to false as we gotta recheck them
	for (GameObject* gameObject : _gameObjects)
	{
		if (gameObject->GetPhysicsModel()->IsCollideable())
		{
			gameObject->_objectHasCollided = false;
		}
	}

	// Super based Standard library coming in clutch
	std::vector<std::pair<GameObject*, GameObject*>> collisions;

	// Nested for loop Hell
	for (int i = 0; i < _gameObjects.size(); i++)
	{
		// Move the loop ahead by one so we don't check the same object against itself
		for (int j = i + 1; j < _gameObjects.size(); j++)
		{
			if (_gameObjects[i]->GetPhysicsModel()->IsCollideable() &&
				_gameObjects[j]->GetPhysicsModel()->IsCollideable() &&
				_gameObjects[i]->GetPhysicsModel()->GetCollider()->CollidesWith(
					*_gameObjects[j]->GetPhysicsModel()->GetCollider()))
			{
				if (!_gameObjects[i]->_collisionEnabled) continue;
				if (!_gameObjects[j]->_collisionEnabled) continue;

				// Just a flag I use for the UI
				_gameObjects[i]->_objectHasCollided = true;
				_gameObjects[j]->_objectHasCollided = true;

				collisions.push_back(std::make_pair(_gameObjects[i], _gameObjects[j]));
			}
		}
	}

	// Handle collisions in pairs instead of all at once at random
	for (const pair<GameObject*, GameObject*>& collisionPair : collisions)
	{
		GameObject* gameObjectA = collisionPair.first;
		GameObject* gameObjectB = collisionPair.second;

		gameObjectA->GetPhysicsModel()->GetCollider()->HandleCollision(gameObjectA, gameObjectB);
		gameObjectB->GetPhysicsModel()->GetCollider()->HandleCollision(gameObjectB, gameObjectA);
	}
}

void DX11PhysicsFramework::GeneralUpdate(float deltaTime)
{
	_camera->HandleMovement(deltaTime);

	for (int i = 0; i < _gameObjectSize; i++)
	{
		if (GetAsyncKeyState('0' + i) & 0x0001)
		{
			for (bool& j : objectSelected) { j = false; }
			objectSelected[i] = true;
			Debug::Debug_WriteString("Object " + std::to_string(i) + " Selected");
		}
	}

	if (GetAsyncKeyState(VK_MULTIPLY) & 0x0001)
	{
		for (bool& j : objectSelected) { j = false; }
		Debug::Debug_WriteString("Objects Deselected");
	}

	if (GetAsyncKeyState(VK_ESCAPE) & 0x0001)
	{
		PostQuitMessage(0);
	}

	for (int i = 0; i < _gameObjectSize; i++)
	{
		if (objectSelected[i])
		{
			BasicObjectMovement(deltaTime, i);
			_gameObjects[i]->GetAppearance()->SetTextureRV(SelectedTexture);
		}
		else
		{
			if (_gameObjects[i]->GetType() == "Floor")
			{
				_gameObjects[i]->GetAppearance()->SetTextureRV(GroundTextureRV);
			}
			else
			{
				_gameObjects[i]->GetAppearance()->SetTextureRV(StoneTextureRV);
			}
		}
	}
}

void DX11PhysicsFramework::DrawObjectMovementControlWindow(float deltaTime, int objectSelected)
{
	// Object Movement Control Window
	ImGui::SetNextWindowPos(ImVec2(1315, 10), ImGuiCond_FirstUseEver);
	ImGui::SetNextWindowSize(ImVec2(600, 600), ImGuiCond_FirstUseEver);

	ImGui::Begin("Object Movement Control");
	ImGui::Separator();
	ImGui::Text("Object Statistics:");
	ImGui::Separator();
	ImGui::Text("VELOCITY:");
	ImGui::Text("Velocity X: %s",
		std::to_string(_gameObjects[objectSelected]->GetPhysicsModel()->GetVelocity().x).c_str());
	ImGui::Text("Velocity Y: %s",
		std::to_string(_gameObjects[objectSelected]->GetPhysicsModel()->GetVelocity().y).c_str());
	ImGui::Text("Velocity Z: %s",
		std::to_string(_gameObjects[objectSelected]->GetPhysicsModel()->GetVelocity().z).c_str());
	ImGui::Separator();
	ImGui::Text("POSITION:");
	ImGui::Text("Position x: %s",
		std::to_string(_gameObjects[objectSelected]->GetTransform()->GetPosition().x).c_str());
	ImGui::Text("Position y: %s",
		std::to_string(_gameObjects[objectSelected]->GetTransform()->GetPosition().y).c_str());
	ImGui::Text("Position z: %s",
		std::to_string(_gameObjects[objectSelected]->GetTransform()->GetPosition().z).c_str());
	ImGui::Separator();
	ImGui::Text("ROTATION:");
	ImGui::Text("Rotation x: %s",
		std::to_string(_gameObjects[objectSelected]->GetTransform()->GetRotation().x).c_str());
	ImGui::Text("Rotation y: %s",
		std::to_string(_gameObjects[objectSelected]->GetTransform()->GetRotation().y).c_str());
	ImGui::Text("Rotation z: %s",
		std::to_string(_gameObjects[objectSelected]->GetTransform()->GetRotation().z).c_str());
	ImGui::Separator();
	ImGui::Checkbox("Enable Collisions", &_gameObjects[objectSelected]->_collisionEnabled);
	if (_gameObjects[objectSelected]->_objectHasCollided)
	{
		ImGui::Text("Collided State: True");
	}
	else
	{
		ImGui::Text("Collided State: False");
	}

	ImGui::Separator();
	ImGui::Text("Control the transform speeds of the selected object:");

	ImGui::SliderFloat("Move Speed", &_objectMoveSpeed, 0.1f, 10.0f);
	ImGui::SliderFloat("Rotate Speed", &_objectRotateSpeed, 50.0f, 100.0f);
	ImGui::SliderFloat("Scale Speed", &_objectScaleSpeed, 0.1f, 10.0f);

	ImGui::Separator();

	if (objectSelected >= 0 && objectSelected < _gameObjectSize)
	{
		ImGui::Text("Reset Controls:");
		if (ImGui::Button("Reset Transform"))
		{
			_gameObjects[objectSelected]->GetTransform()->Reset();
		}
		if (ImGui::Button("Reset Velocity"))
		{
			_gameObjects[objectSelected]->GetPhysicsModel()->SetVelocity(Vector(0.0f, 0.0f, 0.0f));
		}
		if (ImGui::Button("Reset Acceleration"))
		{
			_gameObjects[objectSelected]->GetPhysicsModel()->SetAcceleration(Vector(0.0f, 0.0f, 0.0f));
		}

		ImGui::Text("Movement Controls: (Does NOT work with Collision Response)");

		ImGui::Button("Move Forward (Z-)");
		if (ImGui::IsItemActive())
		{
			_gameObjects[objectSelected]->GetTransform()->Move(Vector(0.0f, 0.0f, -1.0f), deltaTime, _objectMoveSpeed);
		}

		ImGui::Button("Move Backward (Z+)");
		if (ImGui::IsItemActive())
		{
			_gameObjects[objectSelected]->GetTransform()->Move(Vector(0.0f, 0.0f, 1.0f), deltaTime, _objectMoveSpeed);
		}

		ImGui::Button("Move Left (X-)");
		if (ImGui::IsItemActive())
		{
			_gameObjects[objectSelected]->GetTransform()->Move(Vector(-1.0f, 0.0f, 0.0f), deltaTime, _objectMoveSpeed);
		}

		ImGui::Button("Move Right (X+)");
		if (ImGui::IsItemActive())
		{
			_gameObjects[objectSelected]->GetTransform()->Move(Vector(1.0f, 0.0f, 0.0f), deltaTime, _objectMoveSpeed);
		}

		ImGui::Button("Move Up (Y+)");
		if (ImGui::IsItemActive())
		{
			_gameObjects[objectSelected]->GetTransform()->Move(Vector(0.0f, 1.0f, 0.0f), deltaTime, _objectMoveSpeed);
		}

		ImGui::Button("Move Down (Y-)");
		if (ImGui::IsItemActive())
		{
			_gameObjects[objectSelected]->GetTransform()->Move(Vector(0.0f, -1.0f, 0.0f), deltaTime, _objectMoveSpeed);
		}

		ImGui::Separator();

		ImGui::Text("Rotate Controls:");
		ImGui::Button("Rotate X");
		if (ImGui::IsItemActive())
		{
			_gameObjects[objectSelected]->GetTransform()->Rotate(Vector(1.0f, 0.0f, 0.0f), deltaTime,
				_objectRotateSpeed);
		}

		ImGui::Button("Rotate Y");
		if (ImGui::IsItemActive())
		{
			_gameObjects[objectSelected]->GetTransform()->Rotate(Vector(0.0f, 1.0f, 0.0f), deltaTime,
				_objectRotateSpeed);
		}

		ImGui::Button("Rotate Z");
		if (ImGui::IsItemActive())
		{
			_gameObjects[objectSelected]->GetTransform()->Rotate(Vector(0.0f, 0.0f, 1.0f), deltaTime,
				_objectRotateSpeed);
		}

		ImGui::Separator();

		ImGui::Text("Scale Controls:");
		ImGui::Button("Scale Up");
		if (ImGui::IsItemActive())
		{
			_gameObjects[objectSelected]->GetTransform()->Scale(Vector(1.0f, 1.0f, 1.0f), deltaTime, _objectScaleSpeed);
		}

		ImGui::Button("Scale Down");
		if (ImGui::IsItemActive())
		{
			_gameObjects[objectSelected]->GetTransform()->Scale(Vector(-1.0f, -1.0f, -1.0f), deltaTime,
				_objectScaleSpeed);
		}

		ImGui::Separator();

		ImGui::Text("Constant Velocity Controls:");
		if (ImGui::Button("Set Velocity Forward (Z-)"))
		{
			_gameObjects[objectSelected]->GetPhysicsModel()->SetVelocity(Vector(0.0f, 0.0f, -0.1f));
		}

		if (ImGui::Button("Set Velocity Backward (Z+)"))
		{
			_gameObjects[objectSelected]->GetPhysicsModel()->SetVelocity(Vector(0.0f, 0.0f, 0.1f));
		}

		if (ImGui::Button("Set Velocity Left (X-)"))
		{
			_gameObjects[objectSelected]->GetPhysicsModel()->SetVelocity(Vector(-0.1f, 0.0f, 0.0f));
		}

		if (ImGui::Button("Set Velocity Right (X+)"))
		{
			_gameObjects[objectSelected]->GetPhysicsModel()->SetVelocity(Vector(0.1f, 0.0f, 0.0f));
		}

		if (ImGui::Button("Set Velocity Up (Y+)"))
		{
			_gameObjects[objectSelected]->GetPhysicsModel()->SetVelocity(Vector(0.0f, 0.1f, 0.0f));
		}

		if (ImGui::Button("Set Velocity Down (Y-)"))
		{
			_gameObjects[objectSelected]->GetPhysicsModel()->SetVelocity(Vector(0.0f, -0.1f, 0.0f));
		}

		ImGui::Separator();

		ImGui::Text("Constant Acceleration Controls:");
		ImGui::Checkbox("Switch On Constant Acceleration",
			&_gameObjects[objectSelected]->GetPhysicsModel()->_constantAcceleration);
		if (ImGui::Button("Set Acceleration Forward (Z-)"))
		{
			_gameObjects[objectSelected]->GetPhysicsModel()->SetAcceleration(Vector(0.0f, 0.0f, -0.1f));
		}

		if (ImGui::Button("Set Acceleration Backward (Z+)"))
		{
			_gameObjects[objectSelected]->GetPhysicsModel()->SetAcceleration(Vector(0.0f, 0.0f, 0.1f));
		}

		if (ImGui::Button("Set Acceleration Left (X-)"))
		{
			_gameObjects[objectSelected]->GetPhysicsModel()->SetAcceleration(Vector(-0.1f, 0.0f, 0.0f));
		}

		if (ImGui::Button("Set Acceleration Right (X+)"))
		{
			_gameObjects[objectSelected]->GetPhysicsModel()->SetAcceleration(Vector(0.1f, 0.0f, 0.0f));
		}

		if (ImGui::Button("Set Acceleration Up (Y+)"))
		{
			_gameObjects[objectSelected]->GetPhysicsModel()->SetAcceleration(Vector(0.0f, 0.1f, 0.0f));
		}

		if (ImGui::Button("Set Acceleration Down (Y-)"))
		{
			_gameObjects[objectSelected]->GetPhysicsModel()->SetAcceleration(Vector(0.0f, -0.1f, 0.0f));
		}

		ImGui::Separator();

		ImGui::Text("Add Force Controls:");
		ImGui::SliderFloat("Force to Add", &_addForceNumber, 10.0f, 20.0f);
		if (ImGui::Button("Add Force Forward (Z-)"))
		{
			_gameObjects[objectSelected]->GetPhysicsModel()->AddForce(Vector(0.0f, 0.0f, -_addForceNumber));
		}
		if (ImGui::Button("Add Force Backward (Z+)"))
		{
			_gameObjects[objectSelected]->GetPhysicsModel()->AddForce(Vector(0.0f, 0.0f, _addForceNumber));
		}
		if (ImGui::Button("Add Force Left (X-)"))
		{
			_gameObjects[objectSelected]->GetPhysicsModel()->AddForce(Vector(-_addForceNumber, 0.0f, 0.0f));
		}
		if (ImGui::Button("Add Force Right (X+)"))
		{
			_gameObjects[objectSelected]->GetPhysicsModel()->AddForce(Vector(_addForceNumber, 0.0f, 0.0f));
		}
		if (ImGui::Button("Add Force Up (Y+)"))
		{
			_gameObjects[objectSelected]->GetPhysicsModel()->AddForce(Vector(0.0f, _addForceNumber, 0.0f));
		}
		if (ImGui::Button("Add Force Down (Y-)"))
		{
			_gameObjects[objectSelected]->GetPhysicsModel()->AddForce(Vector(0.0f, -_addForceNumber, 0.0f));
		}

		ImGui::Separator();
		ImGui::Text("Simulate Forces:");
		ImGui::SliderFloat("Gravity Force", &_gameObjects[objectSelected]->GetPhysicsModel()->_gravity.y, -9.81f, 0);
		ImGui::Checkbox("Simulate Gravity", &_gameObjects[objectSelected]->GetPhysicsModel()->_simulateGravity);
		ImGui::Checkbox("Simulate Drag", &_gameObjects[objectSelected]->GetPhysicsModel()->_simulateDrag);
		ImGui::Checkbox("Simulate Friction", &_gameObjects[objectSelected]->GetPhysicsModel()->_simulateFriction);
		ImGui::Separator();

		ImGui::Text("Rotational Force: (Kinda Works)");
		ImGui::Button("Add Relative Force");
		if (ImGui::IsItemActive())
		{
			_gameObjects[objectSelected]->GetPhysicsModel()->AddRelativeForce(Vector(0, 0, -10), Vector(0, 1, -1), deltaTime);
		}
	}

	ImGui::End();
}

void DX11PhysicsFramework::DrawCameraWindow() const
{
	// Camera Window
	ImGui::SetNextWindowPos(ImVec2(635, 10), ImGuiCond_FirstUseEver);
	ImGui::SetNextWindowSize(ImVec2(300, 208), ImGuiCond_FirstUseEver);

	ImGui::Begin("Camera Statistics");
	ImGui::Text("Camera POSITION:");
	ImGui::NewLine();
	ImGui::Text("Camera Position X: %s", std::to_string(_camera->GetPosition().x).c_str());
	ImGui::Text("Camera Position Y: %s", std::to_string(_camera->GetPosition().y).c_str());
	ImGui::Text("Camera Position Z: %s", std::to_string(_camera->GetPosition().z).c_str());
	ImGui::Separator();
	ImGui::NewLine();
	ImGui::Text("Camera ROTATION:");
	ImGui::NewLine();
	// Why is this flipped (I am not going to fix it)
	ImGui::Text("Camera Rotation X: %s", std::to_string(_camera->GetRotation().y).c_str());
	ImGui::Text("Camera Rotation Y: %s", std::to_string(_camera->GetRotation().x).c_str());

	ImGui::End();
}

void DX11PhysicsFramework::DrawIntegrationWindow(int objectSelected) const
{
	// Integration Window
	ImGui::SetNextWindowPos(ImVec2(10, 520), ImGuiCond_FirstUseEver);
	ImGui::SetNextWindowSize(ImVec2(420, 220), ImGuiCond_FirstUseEver);
	ImGui::Begin("Integration Controls");
	ImGui::Text("Use this Window To Control Integration!");
	ImGui::NewLine();
	ImGui::Text("Current Integration Method: %s", _gameObjects[objectSelected]->GetPhysicsModel()->GetIntegrationMethodName().c_str());
	ImGui::Text("Select Integration Method:");

	const char* integrationMethods[] = { "Explicit Euler", "Semi-Implicit Euler", "Verlet", "Stormer-Verlet", "RK4" };
	static int selectedMethod = 5; // Default is RK4

	if (ImGui::Combo("Integration Method", &selectedMethod, integrationMethods, IM_ARRAYSIZE(integrationMethods)))
	{
		_gameObjects[objectSelected]->GetPhysicsModel()->SetIntegrationMethod(selectedMethod);
	}

	ImGui::End();
}

void DX11PhysicsFramework::DrawObjectSelectWindow()
{
	// Object Selection Window
	ImGui::SetNextWindowPos(ImVec2(10, 10), ImGuiCond_FirstUseEver);
	ImGui::SetNextWindowSize(ImVec2(300, 500), ImGuiCond_FirstUseEver);

	ImGui::Begin("Object Selection");
	ImGui::Text("Use this Window To Select Objects!");
	ImGui::NewLine();

	for (int i = 0; i < _gameObjectSize; i++)
	{
		ImGui::Text("Select Object: %d", i);

		std::string label = _gameObjects[i]->GetType() + " " + std::to_string(i);

		if (objectSelected[i])
		{
			// Green when selected
			ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.0f, 1.0f, 0.0f, 1.0f));
			ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.0f, 0.5f, 0.0f, 1.0f));
			ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.0f, 0.1f, 0.0f, 1.0f));
		}
		else
		{
			// Red when not selected
			ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(1.0f, 0.0f, 0.0f, 1.0f));
			ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.5f, 0.0f, 0.0f, 1.0f));
			ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.1f, 0.0f, 0.0f, 1.0f));
		}

		if (ImGui::Button(label.c_str(), ImVec2(150, 50)))
		{
			if (objectSelected[i])
			{
				objectSelected[i] = false;
			}
			else
			{
				for (bool& object : objectSelected)
				{
					object = false;
				}
				objectSelected[i] = true;
			}
		}

		ImGui::PopStyleColor(3);
	}

	ImGui::End();
}

void DX11PhysicsFramework::DrawStatsWindow() const
{
	// Statistics Window
	ImGui::SetNextWindowPos(ImVec2(320, 10), ImGuiCond_FirstUseEver);
	ImGui::SetNextWindowSize(ImVec2(300, 130), ImGuiCond_FirstUseEver);

	ImGui::Begin("App Statistics");
	ImGui::Text("Application Statistics:");
	ImGui::NewLine();
	ImGui::Text("Physics Frame Time: %s", std::to_string(_accumulator).c_str());
	ImGui::Text("Application Runtime: %s", std::to_string(_runtimeTimer).c_str());

	ImGui::Text("Total Objects: %d", _gameObjectSize);

	ImGui::End();
}

void DX11PhysicsFramework::DrawUI()
{
	ImGui_ImplDX11_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();

	DrawObjectSelectWindow();

	DrawStatsWindow();

	DrawCameraWindow();

	for (int i = 0; i < _gameObjectSize; i++)
	{
		if (objectSelected[i])
		{
			DrawObjectMovementControlWindow(deltaTime, i);
			DrawIntegrationWindow(i);
		}
	}

	ImGui::Render();
	ImDrawData* draw_data = ImGui::GetDrawData();
	ImGui_ImplDX11_RenderDrawData(draw_data);
}

void DX11PhysicsFramework::Draw(const double alphaScalar)
{
	// Clear buffers
	float ClearColor[4] = { 0.25f, 0.55f, 1.0f, 1.0f }; // red, green, blue, alpha
	_immediateContext->OMSetRenderTargets(1, &_frameBufferView, _depthBufferView);
	_immediateContext->ClearRenderTargetView(_frameBufferView, ClearColor);
	_immediateContext->ClearDepthStencilView(_depthBufferView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);

	// Setup buffers and render scene
	_immediateContext->VSSetShader(_vertexShader, nullptr, 0);
	_immediateContext->PSSetShader(_pixelShader, nullptr, 0);

	_immediateContext->VSSetConstantBuffers(0, 1, &_constantBuffer);
	_immediateContext->PSSetConstantBuffers(0, 1, &_constantBuffer);
	_immediateContext->PSSetSamplers(0, 1, &_samplerLinear);

	// Use alpha scalar to interpolate matrix to get a smoother movement
	XMMATRIX previousView = _camera->GetPreviousViewMatrix();
	XMMATRIX currentView = _camera->GetViewMatrix();

	XMMATRIX predictedView = alphaScalar * (currentView - previousView);
	XMMATRIX interpolatedView = previousView + predictedView;

	_cbData.View = XMMatrixTranspose(interpolatedView);
	_cbData.Projection = XMMatrixTranspose(_camera->GetProjectionMatrix());

	_cbData.light = basicLight;
	_cbData.EyePosW = _camera->GetPosition();

	// Render all scene objects
	for (auto gameObject : _gameObjects)
	{
		// Use alpha scalar to interpolate matrix to get a smoother movement
		XMMATRIX previousWorld = gameObject->GetTransform()->GetPreviousWorldMatrix();
		XMMATRIX currentWorld = gameObject->GetTransform()->GetWorldMatrix();

		XMMATRIX predictedMatrix = alphaScalar * (currentWorld - previousWorld);
		XMMATRIX interpolatedMatrix = previousWorld + predictedMatrix;

		_cbData.World = XMMatrixTranspose(interpolatedMatrix);

		Material material = gameObject->GetAppearance()->GetMaterial();

		// Copy material to shader
		_cbData.surface.AmbientMtrl = material.ambient;
		_cbData.surface.DiffuseMtrl = material.diffuse;
		_cbData.surface.SpecularMtrl = material.specular;

		// Set texture
		if (gameObject->GetAppearance()->_hasTexture)
		{
			_immediateContext->PSSetShaderResources(0, 1, gameObject->GetAppearance()->GetTextureRV());
			_cbData.HasTexture = 1.0f;
		}
		else
		{
			_cbData.HasTexture = 0.0f;
		}

		// Write constant buffer data onto GPU
		D3D11_MAPPED_SUBRESOURCE mappedSubresource;
		_immediateContext->Map(_constantBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedSubresource);
		memcpy(mappedSubresource.pData, &_cbData, sizeof(_cbData));
		_immediateContext->Unmap(_constantBuffer, 0);

		// Draw object
		gameObject->GetAppearance()->Draw(_immediateContext);
	}

	DrawUI();

	// Present our back buffer to our front buffer
	_swapChain->Present(0, DXGI_PRESENT_ALLOW_TEARING);
}