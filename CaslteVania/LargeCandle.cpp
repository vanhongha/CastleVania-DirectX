#include "LargeCandle.h"



void LargeCandle::InitAnim()
{
	anim = new Animation();
	anim->Initialize(sprite, 32, 64, 0.25);


	std::vector<D3DXVECTOR2> animation;
	animation.push_back(D3DXVECTOR2(0, 0));
	animation.push_back(D3DXVECTOR2(1, 0));

	anim->AddAnimation(0, animation);
}

bool LargeCandle::Initialize(LPDIRECT3DDEVICE9 gDevice, const char * file, float x, float y, int tag)
{
	if (!Enemy::Initialize(gDevice, file, x, y, tag))
		return false;

	InitAnim();
	return true;
}

void LargeCandle::Render(ViewPort *viewPort)
{
	if (!isDead && isEnable)
		anim->Render(sprite, false, viewPort);
	else
		deadEffect->Render(viewPort);
	collideEffect->Render(viewPort);
}

void LargeCandle::Update(float gameTime)
{
	if (!isDead)
	{
		SetBox(sprite->GetPosition().x,
			sprite->GetPosition().y,
			anim->frameWidth,
			anim->frameHeight,
			0,
			0);
		anim->Update(gameTime);
	}
	else
		UpdateEffect(gameTime);
	collideEffect->Update(gameTime);
}

LargeCandle::LargeCandle()
{
}


LargeCandle::~LargeCandle()
{
}
