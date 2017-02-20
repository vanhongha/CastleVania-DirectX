#include "DeadAnimation.h"



DeadAnimation::DeadAnimation()
{
}


DeadAnimation::~DeadAnimation()
{
}

bool DeadAnimation::Initialize(LPDIRECT3DDEVICE9 gDevice, const char * file, float x, float y)
{
	if (!sprite)
	{
		sprite = new Sprite(x, y);
		if (!sprite->Initialize(gDevice, file))
			return false;
	}

	anim = new Animation();
	anim->Initialize(sprite, 42.6f, 44.0f, 0.15f);

	std::vector<D3DXVECTOR2> deadAnimation;
	deadAnimation.push_back(D3DXVECTOR2(0, 0));
	deadAnimation.push_back(D3DXVECTOR2(1, 0));
	deadAnimation.push_back(D3DXVECTOR2(2, 0));

	anim->AddAnimation(0, deadAnimation);
	return true;
}

void DeadAnimation::Render(ViewPort * viewPort)
{
	anim->Render(sprite, true, viewPort);
}

void DeadAnimation::Update(float gameTime)
{
	anim->Update(gameTime);
}

void DeadAnimation::SetPosition(float x, float y)
{
	sprite->SetPosition(D3DXVECTOR3(x, y, 0));
}

bool DeadAnimation::CheckDoAllFrame()
{
	return anim->CheckDoAllFrame();
}