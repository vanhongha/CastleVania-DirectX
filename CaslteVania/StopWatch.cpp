#include "StopWatch.h"



StopWatch::StopWatch()
{
}


StopWatch::~StopWatch()
{
}

bool StopWatch::Initialize(LPDIRECT3DDEVICE9 gDevice, const char * file, float x, float y, int tag)
{
	if (!SubWeapon::Initialize(gDevice, STOP_WATCH_ACTION, x, y, tag))
		return false;

	useEnergy = 0;
	damage = 0;
	/*InitAnim();*/
	return true;
}

void StopWatch::InitAnim()
{
	anim = new Animation();
	anim->Initialize(sprite, 30, 32, 0.25f);

	std::vector<D3DXVECTOR2> initAnim;
	initAnim.push_back(D3DXVECTOR2(0, 0));

	anim->AddAnimation(0, initAnim);

	action = 0;
}

void StopWatch::Threw(bool isLeft, float x, float y)
{
	Enemy::stopTime = 10;
	action = 0;
	/*SetPosition(x, y);
	SetVelocity(D3DXVECTOR3(((isLeft) ? -1 : 1) * 300, 500, 0));*/
}

void StopWatch::Update(float gameTime)
{
	SubWeapon::Update(gameTime);
	/*velocity.y += EARTH_ACCELERATION * gameTime;
	anim->DoAnimation(action);
	anim->Update(gameTime);
	GameObject::SetBox(sprite->GetPosition().x,
	sprite->GetPosition().y,
	anim->frameWidth,
	anim->frameHeight,
	velocity.x,
	velocity.y);*/
}

void StopWatch::Render(ViewPort *viewPort)
{
	/*anim->Render(sprite, false, viewPort);*/
}

