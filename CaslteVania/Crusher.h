#pragma once
#include "GameObject.h"

class Crusher : public GameObject
{
private:
	D3DXVECTOR3 position;
	float height;

	GameObject *surface;
	GameObject *rope[20];

	D3DXVECTOR3 velocity;

	bool surfaceOnTop, surfaceOnBottom;
	bool enableRope[20];
public:
	Crusher();
	~Crusher();

	void Initialize(LPDIRECT3DDEVICE9 gDevice, float x, float y);

	void Render(ViewPort *viewport);
	void Update(float gameTime);

	void SurfaceUpdate(float gameTime);
	void RopeUpdate();

	GameObject *GetSurface() { return surface; }

	void SetHeight(int height) { this->height = height; }

	void Setposition(GameObject *object, D3DXVECTOR3 v, float gameTime);
};

