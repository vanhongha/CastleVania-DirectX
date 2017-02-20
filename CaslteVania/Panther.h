#pragma once
#include "Enemy.h"
class Panther : public Enemy
{
private:
	void InitAnim();
	float delayTime;
	bool isGroundedInNextFrame;
public:
	Panther(int hp, int damage, int point);
	~Panther();

	bool Initialize(LPDIRECT3DDEVICE9 gDevice, const char* file, float x, float y, int tag);
	void Update(float gameTime);
	void Jump();
	void Move();
	void SetAction(int action);
	int GetAction() { return action; }
	void CheckCollider(float gameTime, std::vector<GameObject*> *listObject);
	void SetIsLeft(bool _isLeft) { isLeft = _isLeft; }
};

