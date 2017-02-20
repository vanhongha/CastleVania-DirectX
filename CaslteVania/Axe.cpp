#include "Axe.h"



Axe::Axe()
{
}


Axe::~Axe()
{
}

bool Axe::Initialize(LPDIRECT3DDEVICE9 gDevice, const char * file, float x, float y, int tag)
{
	if (!SubWeapon::Initialize(gDevice, AXE_ACTION, x, y, tag))
		return false;
	InitAnim();
	isFiring = false;
	damage = 4;
	return true;
}

void Axe::InitAnim()
{
	anim = new Animation();
	anim->Initialize(sprite, 30, 28, 0.2f);

	std::vector<D3DXVECTOR2> initAnim;
	initAnim.push_back(D3DXVECTOR2(0, 0));
	initAnim.push_back(D3DXVECTOR2(1, 0));
	initAnim.push_back(D3DXVECTOR2(2, 0));
	initAnim.push_back(D3DXVECTOR2(3, 0));

	anim->AddAnimation(0, initAnim);

	action = 0;
}

void Axe::Threw(bool isLeft, float x, float y)
{
	action = 0;
	SetPosition(x, y);
	SetVelocity(D3DXVECTOR3(((isLeft) ? -1 : 1) * 300, 450, 0));
}

void Axe::Update(float gameTime)
{
	if (isFiring)
		isFiring = !anim->CheckDoAllFrame();
	SubWeapon::Update(gameTime);
	velocity.y += -1200 * gameTime;
	anim->DoAnimation(action);
	anim->Update(gameTime);
	GameObject::SetBox(sprite->GetPosition().x,
		sprite->GetPosition().y,
		anim->frameWidth,
		anim->frameHeight,
		velocity.x,
		velocity.y);
}

void Axe::Render(ViewPort *viewPort)
{
	anim->Render(sprite, false, viewPort);
}

void Axe::CheckCollider(float gameTime, std::vector<GameObject*> *listGameObject)
{
	float normalX = 0;
	float normalY = 0;
	float timeCollide;

	for (std::vector<GameObject*>::iterator i = listGameObject->begin(); i != listGameObject->end(); i++)
	{
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
			else
				if ((*i)->GetTag() == TAG_GROUND)
				{
					if (IsEnable() && normalY == 1)
						SetEnable(false);
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
					else
						if ((*i)->GetTag() == TAG_GROUND)
						{
							if (IsEnable() && normalY == 1)
								SetEnable(false);
						}
				}
			}
		}
	}
}

void Axe::CheckColliderWithBoss(float gameTime, GameObject *object)
{
	SubWeapon::CheckColliderWithBoss(gameTime, object);
}