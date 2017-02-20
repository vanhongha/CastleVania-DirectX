#include "Cross.h"

Cross::Cross()
{
}


Cross::~Cross()
{
}

bool Cross::Initialize(LPDIRECT3DDEVICE9 gDevice, const char * file, float x, float y, int tag)
{
	if (!SubWeapon::Initialize(gDevice, CROSS_SPRITE_ACTION, x, y, tag))
		return false;
	InitAnim();
	isEnable = false;
	damage = 4;
	SetDefaultProperties();
	return true;
}

void Cross::InitAnim()
{
	anim = new Animation();
	anim->Initialize(sprite, 28, 28, 0.15f);

	std::vector<D3DXVECTOR2> animation;
	animation.push_back(D3DXVECTOR2(0, 0));
	animation.push_back(D3DXVECTOR2(1, 0));
	animation.push_back(D3DXVECTOR2(2, 0));

	anim->AddAnimation(0, animation);

	action = 0;
}

void Cross::Threw(bool isLeft, float x, float y)
{
	left = isLeft;
	isTurnBack = false;

	simonPos.x = x;
	simonPos.y = y;
	simonPos.z = 0;

	SetPosition(x, y);
	SetVelocity(D3DXVECTOR3(((left) ? -1 : 1 )* 300.0f, 0.0f, 0.0f));
}

void Cross::Update(float gameTime)
{
	SubWeapon::Update(gameTime);
	anim->Update(gameTime);

	GameObject::SetBox(sprite->GetPosition().x,
		sprite->GetPosition().y,
		anim->frameWidth,
		anim->frameHeight,
		velocity.x,
		velocity.y);
}

void Cross::Render(ViewPort *viewPort)
{
	anim->Render(sprite, false, viewPort);
}

void Cross::SetDefaultProperties()
{
	isCollidedWithEnemy = false;
	isTurnBack = false;
}

void Cross::CheckCollider(float gameTime, std::vector<GameObject*> *listGameObject)
{
	float normalX = 0;
	float normalY = 0;
	float timeCollide;

	for (std::vector<GameObject*>::iterator i = listGameObject->begin(); i != listGameObject->end(); i++)
	{
		Box tempBox = (*i)->GetCollider()->GetBox();
		if (collider->AABBCheck(collider->GetBox(), (*i)->GetCollider()->GetBox()))
		{
			/*if ((*i)->GetTag() < 10 && (*i)->GetTag() >= 0 && ((Enemy*)(*i))->e_isInvincible == false)
			{
				if (IsEnable())
				{
					((Enemy*)(*i))->SetHP(((Enemy*)(*i))->GetHP() - GetDamage());
					isCollidedWithEnemy = true;
					(*i)->GetCollideEffect()->SetVisible(true);
				}
			}
			else if ((*i)->GetTag() == 2000 && ((Batman*)(*i))->isHurted == false)
			{
				if (IsEnable())
				{
					((Enemy*)(*i))->SetHP(((Enemy*)(*i))->GetHP() - GetDamage());
					isCollidedWithEnemy = true;
					(*i)->GetCollideEffect()->SetVisible(true);
				}
			}
			else if ((*i)->GetTag() == 2001 && ((Medusa*)(*i))->isHurted == false)
			{
				if (IsEnable())
				{
					((Enemy*)(*i))->SetHP(((Enemy*)(*i))->GetHP() - GetDamage());
					isCollidedWithEnemy = true;
					(*i)->GetCollideEffect()->SetVisible(true);
				}
			}*/
		}
		else
		{
			Box broadphaseBox = collider->GetSweptBoardphaseBox(collider->GetBox(), gameTime);
			if (collider->AABBCheck(broadphaseBox, (*i)->GetCollider()->GetBox()))
			{
				Box tempBox = (*i)->GetCollider()->GetBox();
				timeCollide = collider->SweptAABB(gameTime, collider->GetBox(), (*i)->GetCollider()->GetBox(), normalX, normalY);
				if ((timeCollide >= 0.0f && timeCollide < 1.0f))
				{
					if ((*i)->GetTag() < 10 && (*i)->GetTag() >= 0 && ((Enemy*)(*i))->e_isInvincible == false)
					{
						if (IsEnable())
						{
							((Enemy*)(*i))->SetHP(((Enemy*)(*i))->GetHP() - GetDamage());
							isCollidedWithEnemy = true;
							(*i)->GetCollideEffect()->SetVisible(true);
						}
					}
					else if ((*i)->GetTag() == 2000 && ((Batman*)(*i))->isHurted == false)
					{
						if (IsEnable())
						{
							((Enemy*)(*i))->SetHP(((Enemy*)(*i))->GetHP() - GetDamage());
							isCollidedWithEnemy = true;
							(*i)->GetCollideEffect()->SetVisible(true);
						}
					}
					else if ((*i)->GetTag() == 2001 && ((Medusa*)(*i))->isHurted == false)
					{
						if (IsEnable())
						{
							((Enemy*)(*i))->SetHP(((Enemy*)(*i))->GetHP() - GetDamage());
							isCollidedWithEnemy = true;
							(*i)->GetCollideEffect()->SetVisible(true);
						}
					}
				}
			}
		}
	}
}

bool Cross::CheckColliderWith(float gameTime, GameObject * gameObject)
{
	return SubWeapon::CheckColliderWith(gameTime, gameObject);
}

void Cross::CheckTurnBack(float gameTime, ViewPort *viewPort)
{
	if (isCollidedWithEnemy &&
		!isTurnBack &&
		(sprite->GetPosition().x >= viewPort->GetCameraPosition().x + GAME_WIDTH - 15 ||
			sprite->GetPosition().x <= viewPort->GetCameraPosition().x + 15))
	{
		isTurnBack = true;
		SetVelocity(D3DXVECTOR3(((left) ? 1 : -1) * 300.0f, 0.0f, 0.0f));
	}
}
