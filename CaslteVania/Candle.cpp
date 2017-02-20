#include "Candle.h"



void Candle::InitAnim()
{
	anim = new Animation();
	anim->Initialize(sprite, 16, 32, 0.25);


	std::vector<D3DXVECTOR2> animation;
	animation.push_back(D3DXVECTOR2(0, 0));
	animation.push_back(D3DXVECTOR2(1, 0));


	anim->AddAnimation(0, animation);

}

bool Candle::Initialize(LPDIRECT3DDEVICE9 gDevice, const char * file, float x, float y, int tag)
{
	if (!Enemy::Initialize(gDevice, file, x, y, tag))
		return false;

	InitAnim();
	return true;
}

void Candle::Render(ViewPort *viewPort)
{
	if (!isDead && isEnable)
		anim->Render(sprite, false, viewPort);
	else
		deadEffect->Render(viewPort);
	collideEffect->Render(viewPort);
}

void Candle::Update(float gameTime)
{
	if (!isDead)
	{
		SetBox(sprite->GetPosition().x,
			sprite->GetPosition().y,
			anim->frameWidth,
			anim->frameHeight,
			0,
			0);
		anim->DoAnimation(0);
		anim->Update(gameTime);
	}
	else
		UpdateEffect(gameTime);
	collideEffect->Update(gameTime);
}

Candle::Candle()
{
}


Candle::~Candle()
{
}
