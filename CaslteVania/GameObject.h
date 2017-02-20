#pragma once
#include "Sprite.h"
#include "Graphics.h"
#include "Collider.h"
#include "DeadAnimation.h"
#include "CollideEffect.h"

class GameObject
{
private:
	int tag;
	int id;
protected:
	Sprite* sprite;
	Collider* collider;
	RECT *region;
	float width;
	float height;
	D3DXVECTOR3 position;
	bool isEnable;
	bool isDead;
	bool isMoveable;

	DeadAnimation *deadEffect;
	CollideEffect *collideEffect;

public:
	bool isInCamera;
	bool trigger;
	RECT *regionC;
	D3DXVECTOR3 positionC;
	bool isAdded;

	virtual bool Initialize(LPDIRECT3DDEVICE9 gDevice, const char* file, float x, float y, int tag);
	virtual bool InitSprite(LPDIRECT3DDEVICE9 gDevice, const char* file, float x, float y);
	virtual void Render(ViewPort *viewPort);
	virtual void Update(float gameTime);

	int GetTag();
	Collider* GetCollider();

	virtual void SetBox(float x, float y, float w, float h, float vx, float vy);

	virtual D3DXVECTOR3 GetVelocity();

	// For interact with quadTree
	virtual bool IsInRegion(RECT *region);
	virtual bool IsInRegionC(RECT *region);
	virtual void SetRegion(int left, int right, int top, int bottom);
	virtual RECT *GetRegion() { return region; }
	virtual void CheckCollider(float gameTime, std::vector<GameObject*> *listGameObject) {}

	float GetWidth() { return width; }
	float GetHeight() { return height; }
	D3DXVECTOR3 GetPosition() { return sprite->GetPosition(); }
	void SetSize(float width, float height);
	void SetPosition(D3DXVECTOR3 position);
	void SetPosition(float x, float y);

	void SetEnable(bool enable);
	bool IsEnable() { return isEnable; }

	void SetDead(bool _isDead) { isDead = _isDead; }
	bool IsDead() { return isDead; }

	void UpdateEffect(float gameTime);

	CollideEffect* GetCollideEffect() { return collideEffect; }

	void SetID(int id) { this->id = id; }
	int GetID() { return id; }
	GameObject();
	~GameObject();

	void SetMoveable(bool isMoveable) { this->isMoveable = isMoveable; }
	bool IsMoveable() { return this->isMoveable; }
	virtual void Reload();

	bool isDropItem;

	void SetColor(D3DCOLOR color) { sprite->SetColor(color); }
};

