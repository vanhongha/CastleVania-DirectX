#include "BlueBat.h"



BlueBat::BlueBat(int hp, int damage, int point):Enemy(hp,damage,point)
{
}


void BlueBat::InitAnim()
{
	anim = new Animation();
	anim->Initialize(sprite, 32, 32, 0.15);

	std::vector<D3DXVECTOR2> hangAnamation;
	hangAnamation.push_back(D3DXVECTOR2(0, 0));

	std::vector<D3DXVECTOR2> flyAnimation;
	flyAnimation.push_back(D3DXVECTOR2(1, 0));
	flyAnimation.push_back(D3DXVECTOR2(2, 0));
	flyAnimation.push_back(D3DXVECTOR2(3, 0));

	std::vector<D3DXVECTOR2> jumpAnimation;
	jumpAnimation.push_back(D3DXVECTOR2(3, 0));

	anim->AddAnimation(HANG, hangAnamation);
	anim->AddAnimation(FLY, flyAnimation);
}

BlueBat::~BlueBat()
{
}

bool BlueBat::Initialize(LPDIRECT3DDEVICE9 gDevice, const char * file, float x, float y, int tag)
{
	if (!Enemy::Initialize(gDevice, file, x, y, tag))
		return false;

	InitAnim();
	isLeft = true;
	action = HANG;

	velocity.x = 0.0f;
	velocity.y = 0.0f;
	return true;
}

void BlueBat::Update(float gameTime)
{
	if (action == HANG)
	{
		velocity.x = 0.0f;
		velocity.y = 0.0f;
	}
	else
	{
		isLeft = isLeft ? true : false;
		velocity.x = isLeft ? -120 : 120;
	}

	Enemy::Setbox(0, 0, 0, 0);
	Enemy::Update(gameTime);
}
