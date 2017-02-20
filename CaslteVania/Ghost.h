#pragma once
#include "Enemy.h"
class Ghost : public Enemy
{
private:
	D3DXVECTOR3 targetPosition;
	D3DXVECTOR3 vectorDirection;
	D3DXVECTOR3 horizontalAxisVector;
	float targetDistance;
	float cosAngle;

	///////////////////////////////////
	float distance;
	D3DXVECTOR2 direction;

public:
	Ghost(int hp, int damage, int point);
	~Ghost();
	bool Initialize(LPDIRECT3DDEVICE9 gDevice, const char* file, float x, float y, int tag);
	void InitAnim();
	void Update(float gameTime);
	void CalculateVectorFollowTarget();
	void GetTargetPosition(D3DXVECTOR3 pos) { targetPosition = pos; }
	void SetIsLeft(bool _isLeft) { isLeft = _isLeft; }
};

