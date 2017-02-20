#include "Knife.h"


Knife::Knife()
{
}


Knife::~Knife()
{
}

bool Knife::Initialize(LPDIRECT3DDEVICE9 gDevice, const char * file, float x, float y, int tag)
{
	if (!SubWeapon::Initialize(gDevice, KNIFE_ACTION, x, y, tag))
		return false;
	InitAnim();
	isLeft = false;
	damage = 3;
	return true;
}

void Knife::InitAnim()
{
	anim = new Animation();
	anim->Initialize(sprite, 32, 18, 0.1f);

	std::vector<D3DXVECTOR2> animation;
	animation.push_back(D3DXVECTOR2(0, 0));

	anim->AddAnimation(0, animation);

	action = 0;
}

void Knife::Threw(bool isLeft, float x, float y)
{
	simonPos.x = x;
	simonPos.y = y;
	simonPos.z = 0;

	this->isLeft = isLeft;

	SetPosition(x, y);
	SetVelocity(D3DXVECTOR3(((isLeft) ? -1 : 1)* 300.0f, 0.0f, 0.0f));
}

void Knife::Update(float gameTime)
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

void Knife::Render(ViewPort *viewPort)
{
	anim->Render(sprite, isLeft, viewPort);
}

void Knife::CheckCollider(float gameTime, std::vector<GameObject*> *listGameObject)
{
	float normalX = 0;
	float normalY = 0;
	float timeCollide;

	for (std::vector<GameObject*>::iterator i = listGameObject->begin(); i != listGameObject->end(); i++)
	{
		/*if (((*i)->GetTag() < 10 && (*i)->GetTag() >= 0) || (*i)->GetTag() >= 2000)
		{
			Box tempBox = (*i)->GetCollider()->GetBox();
			if (collider->AABBCheck(collider->GetBox(), (*i)->GetCollider()->GetBox()))
			{
				if (IsEnable())
				{
					((Enemy*)(*i))->SetHP(((Enemy*)(*i))->GetHP() - 2);
					SetEnable(false);
					(*i)->GetCollideEffect()->SetVisible(true);
				}
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
						if (IsEnable())
						{
							((Enemy*)(*i))->SetHP(((Enemy*)(*i))->GetHP() - 2);
							SetEnable(false);
							(*i)->GetCollideEffect()->SetVisible(true);
						}
					}
				}
			}
		}*/
		Box tempBox = (*i)->GetCollider()->GetBox();
		if (collider->AABBCheck(collider->GetBox(), (*i)->GetCollider()->GetBox()))
		{
			if ((*i)->GetTag() < 10 && (*i)->GetTag() >= 0 && ((Enemy*)(*i))->e_isInvincible == false)
			{
				if (IsEnable())
				{
					((Enemy*)(*i))->SetHP(((Enemy*)(*i))->GetHP() - GetDamage());
					SetEnable(false);
					(*i)->GetCollideEffect()->SetVisible(true);
				}
			}
			else if ((*i)->GetTag() == 2000 && ((Batman*)(*i))->isHurted == false)
			{
				if (IsEnable())
				{
					((Enemy*)(*i))->SetHP(((Enemy*)(*i))->GetHP() - GetDamage());
					SetEnable(false);
					(*i)->GetCollideEffect()->SetVisible(true);
				}
			}
			else if ((*i)->GetTag() == 2001 && ((Medusa*)(*i))->isHurted == false)
			{
				if (IsEnable())
				{
					((Enemy*)(*i))->SetHP(((Enemy*)(*i))->GetHP() - GetDamage());
					SetEnable(false);
					(*i)->GetCollideEffect()->SetVisible(true);
				}
			}
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
							SetEnable(false);
							(*i)->GetCollideEffect()->SetVisible(true);
						}
					}
					else if ((*i)->GetTag() == 2000 && ((Batman*)(*i))->isHurted == false)
					{
						if (IsEnable())
						{
							((Enemy*)(*i))->SetHP(((Enemy*)(*i))->GetHP() - GetDamage());
							SetEnable(false);
							(*i)->GetCollideEffect()->SetVisible(true);
						}
					}
					else if ((*i)->GetTag() == 2001 && ((Medusa*)(*i))->isHurted == false)
					{
						if (IsEnable())
						{
							((Enemy*)(*i))->SetHP(((Enemy*)(*i))->GetHP() - GetDamage());
							SetEnable(false);
							(*i)->GetCollideEffect()->SetVisible(true);
						}
					}
				}
			}
		}
	}
}

bool Knife::CheckColliderWith(float gameTime, GameObject * gameObject)
{
	return SubWeapon::CheckColliderWith(gameTime, gameObject);
}
