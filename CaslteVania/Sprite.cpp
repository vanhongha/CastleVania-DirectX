#include "Sprite.h"

Sprite::Sprite()
{
	color = D3DCOLOR_ARGB(255, 255, 255, 255);
	initialized = false;
	position.x = 0.0f;
	position.y = 0.0f;
	position.z = 0.0f;

	spriteHandle = NULL;
	texture = NULL;
}

Sprite::Sprite(float x, float y)
{
	color = D3DCOLOR_ARGB(255, 255, 255, 255);
	initialized = false;
	position.x = x;
	position.y = y;
	position.z = 0;
	spriteHandle = NULL;
	texture = NULL;
}

Sprite::~Sprite()
{
	if (spriteHandle)
	{
		spriteHandle->Release();
		spriteHandle = 0;
	}

	if (texture)
	{
		texture->Release();
		texture = 0;
	}
}

//Khoi tao sprite tu file
bool Sprite::Initialize(LPDIRECT3DDEVICE9 device, const char *file)
{
	
	if (LoadTexture(device, file) == NULL)
	{
		LPCSTR file_ = file;
		MessageBox(NULL, file_, "Error", MB_OK);
		return initialized;
	}

	HRESULT result;

	//create sprite
	result = D3DXCreateSprite(device, &spriteHandle);
	if (FAILED(result))
	{
		MessageBox(NULL, "Error creating Direct 3D sprite", "Error", MB_OK);
		return initialized;
	}

	initialized = true;
	return initialized;
}

bool Sprite::Initialize(LPDIRECT3DDEVICE9 device, const char *file, int width, int height)
{

	if (LoadTexture(device, file, width, height) == NULL)
	{
		MessageBox(NULL, "Could not load image file", "Error", MB_OK);
		return initialized;
	}

	HRESULT result;

	//create sprite
	result = D3DXCreateSprite(device, &spriteHandle);
	if (FAILED(result))
	{
		MessageBox(NULL, "Error creating Direct 3D sprite", "Error", MB_OK);
		return initialized;
	}

	initialized = true;
	return initialized;
}

//load Texture
LPDIRECT3DTEXTURE9 Sprite::LoadTexture(LPDIRECT3DDEVICE9 device, const char *file)
{
	D3DXIMAGE_INFO info;
	HRESULT result;

	result = D3DXGetImageInfoFromFile(file, &info);
	if (result != D3D_OK)
	{
		return NULL;
	}

	width = info.Width;
	height = info.Height;

	//create texture
 	result = D3DXCreateTextureFromFileEx(
		device,
		file,
		info.Width,
		info.Height,
		1,									//Mipmap levels
		D3DUSAGE_DYNAMIC,
		D3DFMT_UNKNOWN,
		D3DPOOL_DEFAULT,
		D3DX_DEFAULT,
		D3DX_DEFAULT,
		D3DCOLOR_XRGB(255, 0, 255),			//Transparent color
		&info,								//Image infomation
		NULL,
		&texture							//result
		);

	if (FAILED(result))
		return NULL;
 	return texture;
}

LPDIRECT3DTEXTURE9 Sprite::LoadTexture(LPDIRECT3DDEVICE9 device, const char *file, int width, int height)
{
	HRESULT result;
	this->width = width;
	this->height = height;
	//create texture
	result = D3DXCreateTextureFromFileEx(
		device,
		file,
		width,
		height,
		1,									//Mipmap levels
		D3DUSAGE_DYNAMIC,
		D3DFMT_UNKNOWN,
		D3DPOOL_DEFAULT,
		D3DX_DEFAULT,
		D3DX_DEFAULT,
		D3DCOLOR_XRGB(255, 0, 255),			//Transparent color
		NULL,							
		NULL,
		&texture							
	);

	if (FAILED(result))
		return NULL;

	return texture;
}

//Kiem tra xem sprite da duoc khoi tao hay chua
bool Sprite::IsInitialized()
{
	return initialized;
}

//Ve sprite, ve nguyen ca hinh
void Sprite::Render(ViewPort *viewPort)
{
 	if (spriteHandle && texture)
	{
		spriteHandle->Begin(D3DXSPRITE_ALPHABLEND);

		spriteHandle->Draw(texture, NULL, NULL, &viewPort->SetPositionInViewPort(position), color);

		spriteHandle->End();
	}
}

void Sprite::Render(D3DXVECTOR3 pos, RECT rect)
{
	if (spriteHandle && texture)
	{
		spriteHandle->Begin(D3DXSPRITE_ALPHABLEND);

		spriteHandle->Draw(texture, &rect, NULL, &pos, color);

		spriteHandle->End();
	}
}

//Ve 1 frame cua sprite
void Sprite::Render(RECT rect, bool isLeft, int width, int height, ViewPort *viewPort)
{
	if (spriteHandle && texture)
	{
		D3DXVECTOR3 pos = viewPort->SetPositionInViewPort(position);
		D3DXMATRIX oldTransform;
		spriteHandle->GetTransform(&oldTransform);

		D3DXMATRIX newTransform;
		D3DXVECTOR2 center = D3DXVECTOR2(pos.x + width / 2, pos.y + height / 2);
		D3DXVECTOR2 rotate = D3DXVECTOR2(isLeft ? 1 : -1, 1);

		D3DXMatrixTransformation2D(&newTransform, &center, 0.0f, &rotate, NULL, 0.0f, NULL);

		D3DXMATRIX finalTransform = newTransform * oldTransform;
		spriteHandle->SetTransform(&finalTransform);

		spriteHandle->Begin(D3DXSPRITE_ALPHABLEND);

		spriteHandle->Draw(texture, &rect, NULL, &pos, color);

		spriteHandle->End();

		spriteHandle->SetTransform(&oldTransform);
	}
}

void Sprite::Render(D3DXVECTOR3 position, D3DCOLOR color, ViewPort *viewPort)
{
	if (spriteHandle && texture)
	{
		spriteHandle->Begin(D3DXSPRITE_ALPHABLEND);
		spriteHandle->Draw(texture, NULL, NULL, &viewPort->SetPositionInViewPort(position), color);
		spriteHandle->End();
	}
	
}

void Sprite::Render(D3DXVECTOR3 pos)
{
	if (spriteHandle && texture)
	{
		spriteHandle->Begin(D3DXSPRITE_ALPHABLEND);

		spriteHandle->Draw(texture, NULL, NULL, &pos, color);

		spriteHandle->End();
	}
}


void Sprite::SetPosition(float x, float y)
{
	position.x = x;
	position.y = y;
}

D3DXVECTOR3 Sprite::GetPosition()
{
	return position;
}

void Sprite::SetPosition(D3DXVECTOR3 _position)
{
	position = _position;
}

float Sprite::GetWidth()
{
	return width;
}

float Sprite::GetHeight()
{
	return height;
}


