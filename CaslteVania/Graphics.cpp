#include "Graphics.h"

//Constructor
Graphics::Graphics()
{
	pDirect3d = NULL;
	pDevice3d = NULL;
	fullscreen = false;
	width = GAME_WIDTH;
	height = GAME_HEIGHT;
}

//Destructor
Graphics::~Graphics()
{
	if (pDevice3d != NULL)
		pDevice3d->Release();
	if (pDirect3d != NULL)
		pDirect3d->Release();
}

//Khoi tao DirectX graphics
bool Graphics::Initialize(HWND hw)
{
	color = D3DCOLOR_XRGB(0, 0, 0);

	hWnd = hw;
	fullscreen = FULLSCREEN;

	// Khoi tao doi tuong DirectX 3D
	pDirect3d = Direct3DCreate9(D3D_SDK_VERSION);
	if (pDirect3d == NULL)
		MessageBox(NULL, "Error initializing Direct 3D", "Error", MB_OK);

	//Khoi tao D3D presentation parameter
	InitD3Dpp();

	//Xac dinh card man hinh do hoa duoc ho tro
	D3DCAPS9 caps;
	DWORD behavior;
	result = pDirect3d->GetDeviceCaps(D3DADAPTER_DEFAULT,
		D3DDEVTYPE_HAL, &caps);

	if ((caps.DevCaps & D3DDEVCAPS_HWTRANSFORMANDLIGHT) == 0 ||
		caps.VertexShaderVersion < D3DVS_VERSION(1, 1))
		// Use software-only processing
		behavior = D3DCREATE_SOFTWARE_VERTEXPROCESSING;
	else
		// Use hardware-only processing
		behavior = D3DCREATE_HARDWARE_VERTEXPROCESSING;

	//Tao doi tuong Direct3D Device
	result = pDirect3d->CreateDevice(
		D3DADAPTER_DEFAULT,
		D3DDEVTYPE_HAL,
		hWnd,
		behavior,
		&d3dpp,
		&pDevice3d);
	if (FAILED(result))
	{
		MessageBox(NULL, "Error creating Direct3D device", "Error", MB_OK);
		return false;
	}
	return true;
}

//Tao D3D presentation parameters
void Graphics::InitD3Dpp()
{
	try {
		//Dien tat ca cac gia tri ban dau la 0
		ZeroMemory(&d3dpp, sizeof(d3dpp));

		//Dien cac tham so can thiet
		d3dpp.BackBufferWidth = width;
		d3dpp.BackBufferHeight = height;
		if (fullscreen) // Neu fullscreen
			d3dpp.BackBufferFormat = D3DFMT_X8R8G8B8;// 24-bit color
		else
			d3dpp.BackBufferFormat = D3DFMT_UNKNOWN; // Su dung thiet lap cua desktop
		d3dpp.BackBufferCount = 1;
		d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
		d3dpp.hDeviceWindow = hWnd;
		d3dpp.Windowed = (!fullscreen);
	}
	catch (...)
	{
		MessageBox(NULL, "Error initializing D3D presentation parameters", "Error", MB_OK);
	}
}

void Graphics::Clear()
{
	// 2nd param is set to NULL so that it clears the entire back buffer
	pDevice3d->Clear(0, NULL, D3DCLEAR_TARGET, color, 1.0f, 0);
}

void Graphics::Begin()
{
	pDevice3d->BeginScene();
}

void Graphics::End()
{
	pDevice3d->EndScene();
}

void Graphics::Present()
{
	// Present our scene to the window
	pDevice3d->Present(NULL, NULL, NULL, NULL);
}