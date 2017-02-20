#include "BatAnimation.h"



BatAnimation::BatAnimation()
{
}


BatAnimation::~BatAnimation()
{
}

bool BatAnimation::Initialize(LPDIRECT3DDEVICE9 gDevice, const char * file, float x, float y)
{
	if (!sprite)
	{
		sprite = new Sprite(x, y);
		if (!sprite->Initialize(gDevice, file))
			return false;
	}

	anim = new Animation();
	anim->Initialize(sprite, 16, 16, 0.15f);

	std::vector<D3DXVECTOR2> batAnimation;
	batAnimation.push_back(D3DXVECTOR2(0, 0));
	batAnimation.push_back(D3DXVECTOR2(1, 0));

	anim->AddAnimation(0, batAnimation);

	return true;
}

void BatAnimation::Render(ViewPort * viewPort)
{
	anim->Render(sprite, true, viewPort);
}

void BatAnimation::Update(float gameTime, float vx, float vy)
{
	sprite->SetPosition(sprite->GetPosition().x + vx * gameTime, sprite->GetPosition().y + vy * gameTime);
	anim->Update(gameTime);
}

void BatAnimation::SetPosition(float x, float y)
{
	sprite->SetPosition(D3DXVECTOR3(x, y, 0));
}



