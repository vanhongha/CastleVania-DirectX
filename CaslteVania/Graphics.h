#pragma once
#ifndef _GRAPHICS_H // Prevent multiple definitions if this
#define _GRAPHICS_H // file is included in more than one place

#include <d3d9.h>
#include <d3dx9.h>
#include "define.h"

class Graphics
{
private:
	LPDIRECT3DDEVICE9 pDevice3d;
	LPDIRECT3D9 pDirect3d;
	D3DPRESENT_PARAMETERS d3dpp;

	HRESULT result;
	HWND hWnd;
	bool fullscreen;
	int width;
	int height;

	D3DCOLOR color;

	//Khoi tao D3D presentation parameters
	void InitD3Dpp();

	// Shorthand for IDirect3DDevice9 *device
public:
	Graphics();
	~Graphics();

	//Khoi tao doi tuong DirectX graphics
	bool Initialize(HWND hw);

	//Cac phuong thuc ve len device
	void Clear();
	void Begin();
	void End();
	void Present();

	//Tra ve device3d
	LPDIRECT3DDEVICE9 GetDevice() { return pDevice3d; }
	// Shorthand for IDirect3DDevice9 *device

	void SetColor(D3DCOLOR color) { this->color = color; }
};

#endif