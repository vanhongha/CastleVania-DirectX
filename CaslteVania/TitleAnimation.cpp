#include "TitleAnimation.h"



TitleAnimation::TitleAnimation()
{
}


TitleAnimation::~TitleAnimation()
{
}

bool TitleAnimation::Initialize(LPDIRECT3DDEVICE9 gDevice, const char * file, float x, float y)
{
	if (!sprite)
	{
		sprite = new Sprite(x, y);
		if (!sprite->Initialize(gDevice, file))
			return false;
	}

	anim = new Animation();
	anim->Initialize(sprite, 158, 123, 0.1f);

	std::vector<D3DXVECTOR2> initAnimation;
	initAnimation.push_back(D3DXVECTOR2(0, 0));
	initAnimation.push_back(D3DXVECTOR2(1, 0));
	initAnimation.push_back(D3DXVECTOR2(2, 0));
	initAnimation.push_back(D3DXVECTOR2(3, 0));
	initAnimation.push_back(D3DXVECTOR2(4, 0));
	initAnimation.push_back(D3DXVECTOR2(0, 1));
	initAnimation.push_back(D3DXVECTOR2(1, 1));
	initAnimation.push_back(D3DXVECTOR2(2, 1));
	initAnimation.push_back(D3DXVECTOR2(3, 1));
	initAnimation.push_back(D3DXVECTOR2(4, 1));
	initAnimation.push_back(D3DXVECTOR2(0, 2));
	initAnimation.push_back(D3DXVECTOR2(1, 2));
	initAnimation.push_back(D3DXVECTOR2(2, 2));
	initAnimation.push_back(D3DXVECTOR2(3, 2));
	initAnimation.push_back(D3DXVECTOR2(4, 2));

	std::vector<D3DXVECTOR2> flyAnimation;
	initAnimation.push_back(D3DXVECTOR2(4, 2));
	initAnimation.push_back(D3DXVECTOR2(3, 2));
	initAnimation.push_back(D3DXVECTOR2(2, 2));
	initAnimation.push_back(D3DXVECTOR2(3, 2));

	anim->AddAnimation(0, initAnimation);
	anim->AddAnimation(1, flyAnimation);

	action = 0;
	return true;
}

void TitleAnimation::Render(ViewPort * viewPort)
{
	anim->DoAnimation(action);
	anim->Render(sprite, true, viewPort);
}

void TitleAnimation::Update(float gameTime)
{
	if (!anim->CheckDoAllFrame())
		anim->Update(gameTime);
}