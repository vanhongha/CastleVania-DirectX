#pragma once
#include "Enemy.h"
#include "Input.h"
class WaterAnimation
{
private:
	D3DXVECTOR3 position;
	D3DXVECTOR3 velocity[3];
	Sprite *waterSprite[3];
	bool wasOnTop[3];
	bool isEnable;
public:
	WaterAnimation();
	~WaterAnimation();
	void Initialzie(LPDIRECT3DDEVICE9 gDevice, int x, int y);
	void SetPosition(float x, float y);
	D3DXVECTOR3 GetPosition() { return position; }
	void Render(ViewPort *viewPort);
	void Update(float gameTime);
	void SetDefaultForce();
	float PositionYOfMidWater() { return waterSprite[1]->GetPosition().y; }
	bool IsEnable() { return isEnable; }
	void SetEnable(bool _isEnable) { isEnable = _isEnable; }
};

