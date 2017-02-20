#pragma once
#include "GameObject.h"
#include "Animation.h"
#include "Sound.h"

#define E_INVINCIBLE_TIME 0.4

class Enemy : public GameObject
{
protected:
	Animation *anim;
	D3DXVECTOR3 velocity;
	float acceleration;

	int action;
	bool isLeft;

	int hp;

	int e_prevHP;
	float e_invincibleTime;

	int hpC;
	int damage;
	int point;

	bool isGravity;
	bool isGrounded;

	static int score;
	void AddScore();

public:

	bool e_isInvincible;
	static float stopTime;

	Enemy() {}
	Enemy(int hp, int damage, int point);
	~Enemy();

	virtual bool Initialize(LPDIRECT3DDEVICE9 gDevice, const char* file, float x, float y, int tag);
	virtual void Render(ViewPort *viewPort);
	virtual void Update(float gameTime);
	virtual void Setbox(float offsetX, float offsetY, float width, float height);
	virtual void CheckCollider(float gameTime, std::vector<GameObject*> *listGameObject);
	virtual bool IsCollideWith(float gameTime, GameObject *object);

	virtual int GetHP() { return this->hp; }
	virtual void SetHP(int hp) { this->hp = hp; }
	int GetPoint() { return point; }

	void Reload();
	void Respawn();

	float respawnTime;
	float respawnTimeNeo;
	bool isIn;

	void GravityHandle(float gameTime);

	static int GetScore();
};

