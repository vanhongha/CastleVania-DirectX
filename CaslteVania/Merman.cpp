#include "Merman.h"



Merman::Merman(int hp, int damage, int point) : Enemy(hp, damage, point)
{
}


Merman::~Merman()
{
}

void Merman::InitAnim()
{
	anim = new Animation();
	anim->Initialize(sprite, 32, 64, 0.15f);

	std::vector<D3DXVECTOR2> fightAnimation;
	fightAnimation.push_back(D3DXVECTOR2(0, 0));

	std::vector<D3DXVECTOR2> moveAnimation;
	moveAnimation.push_back(D3DXVECTOR2(1, 0));
	moveAnimation.push_back(D3DXVECTOR2(2, 0));

	std::vector<D3DXVECTOR2> jumpAnimation;
	jumpAnimation.push_back(D3DXVECTOR2(1, 0));

	anim->AddAnimation(FIGHT, fightAnimation);
	anim->AddAnimation(MOVE, moveAnimation);
	anim->AddAnimation(JUMP, jumpAnimation);

}

bool Merman::Initialize(LPDIRECT3DDEVICE9 gDevice, const char* file, float x, float y, int tag)
{
	if (!Enemy::Initialize(gDevice, file, x, y, tag))
		return false;

	projectile = new Projectile();
	projectile->Initialize(gDevice, FIREBALL_SPRITE, x, y, TAG_PROJECTILE);
	projectile->SetEnable(false);

	InitAnim();

	isLeft = false;

	velocity.x = 0.0f;
	velocity.y = 700.0f;

	timeDelay = 0.0f;

	action = JUMP;
}

void Merman::Render(ViewPort *viewPort)
{
	Enemy::Render(viewPort);
	if (projectile->IsEnable())
		projectile->Render(viewPort);
}

void Merman::Update(float gameTime)
{
	if (isEnable == true && !isDead && stopTime <= 0)
	{
		timeDelay += gameTime;
		if (timeDelay > 2.0f && action != JUMP)
		{
			action = FIGHT;
			velocity.x = 0.0f;
			timeDelay = 0.0f;
			projectile->SetIsLeft(isLeft);
			projectile->SetPosition(sprite->GetPosition().x, sprite->GetPosition().y - 10.0f);
			projectile->SetEnable(true);
		}
		if (action == FIGHT && timeDelay > 1.0f)
		{
			action = MOVE;
			timeDelay = 0.0f;
			isLeft = isLeft ? false : true;
		}
		if (action == JUMP && isGrounded && velocity.y == 0)
		{
			action = MOVE;
		}
		if (action == MOVE)
		{
			velocity.x = isLeft ? -100.0f : 100.0f;
		}
		GravityHandle(gameTime);
		Enemy::Setbox(0, 0, 0, 0);
	}
	Enemy::Update(gameTime);
}

void Merman::CheckCollider(float gameTime, std::vector<GameObject*> *listObject)
{
	if (isEnable == true)
		Enemy::CheckCollider(gameTime, listObject);
}

void Merman::SetJump()
{
	action = JUMP;
	velocity.y = 900.0f;
	velocity.x = 0.0f;
}
