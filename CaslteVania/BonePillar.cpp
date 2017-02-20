#include "BonePillar.h"



BonePillar::BonePillar(int hp, int damage, int point) : Enemy(hp, damage, point)
{
}


BonePillar::~BonePillar()
{
}


void BonePillar::InitAnim()
{
	anim = new Animation();
	anim->Initialize(sprite, 32, 64, 0.15f);

	std::vector<D3DXVECTOR2> initAnimation;
	initAnimation.push_back(D3DXVECTOR2(0, 0));

	anim->AddAnimation(0, initAnimation);
}

bool BonePillar::Initialize(LPDIRECT3DDEVICE9 gDevice, const char* file, float x, float y, int tag)
{
	if (!Enemy::Initialize(gDevice, file, x, y, tag))
		return false;

	for (int i = 0; i < 2; i++)
	{
		projectile[i] = new Projectile();
		projectile[i]->Initialize(gDevice, FIREBALL_SPRITE, x, y, TAG_PROJECTILE);
		projectile[i]->SetEnable(false);
		projectile[i]->SetVelocity(200.0f, 0);
	}

	InitAnim();

	isLeft = false;

	velocity.x = 0.0f;
	velocity.y = 0.0f;

	timeDelay = 0.0f;

	action = 0;
}

void BonePillar::Render(ViewPort *viewPort)
{
	if (!isDead && isEnable)
		anim->Render(sprite, isLeft, viewPort);
	else if (isDead)
		deadEffect->Render(viewPort);
	if (isEnable)
		collideEffect->Render(viewPort);
}

void BonePillar::Update(float gameTime)
{
	Enemy::Setbox(0, 0, 0, 0);

	if (isEnable && stopTime <= 0)
	{
		for (int i = 0; i < 2; i++)
		{
			if (!projectile[i]->IsEnable())
			{
				timeDelay += gameTime;
				if (timeDelay > 0.6 && timeDelay < 0.8f)
					this->SetColor(D3DCOLOR_ARGB(200, 255, 66, 66));
				else if (timeDelay > 1.0f)
				{
					this->SetColor(D3DCOLOR_ARGB(255, 255, 255, 255));
					if (i == 0)
						timeDelay = 0.8f;
					else
						timeDelay = 0.0f;
					projectile[i]->SetIsLeft(isLeft);
					projectile[i]->SetPosition(sprite->GetPosition().x, sprite->GetPosition().y - 15.0f);
					projectile[i]->SetEnable(true);
				}
			}
		}
	}
	Enemy::Update(gameTime);
}

void BonePillar::CheckCollider(float gameTime, std::vector<GameObject*> *listObject)
{
	if (isEnable)
		Enemy::CheckCollider(gameTime, listObject);
}