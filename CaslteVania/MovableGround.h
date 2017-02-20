#pragma once
#include "GameObject.h"
class MovableGround : public GameObject
{
private:
	D3DXVECTOR3 velocity;
public:
	bool Initialize(LPDIRECT3DDEVICE9 gDevice, const char * file, float x, float y, int tag);
	void Update(float gameTime);
	void CheckCollideWithGround(float gameTime, std::vector<GameObject*> *listGameObject);
	D3DXVECTOR3 GetVelocity() { return velocity; }
	MovableGround();
	~MovableGround();
};

