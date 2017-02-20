#include "HolyWater.h"

HolyWater::HolyWater()
{
}


HolyWater::~HolyWater()
{
}

bool HolyWater::Initialize(LPDIRECT3DDEVICE9 gDevice, const char * file, float x, float y, int tag)
{
	if (!SubWeapon::Initialize(gDevice, HOLY_WATER_SPRITE_ACTION, x, y, tag))
		return false;
	InitAnim();
	isFiring = false;
	damage = 2;
	return true;
}

void HolyWater::InitAnim()
{
	anim = new Animation();
	anim->Initialize(sprite, 32, 26, 0.25f);

	std::vector<D3DXVECTOR2> initAnim;
	initAnim.push_back(D3DXVECTOR2(0, 0));
	
	std::vector<D3DXVECTOR2> burnAnim;
	burnAnim.push_back(D3DXVECTOR2(1, 0));
	burnAnim.push_back(D3DXVECTOR2(2, 0));

	anim->AddAnimation(0, initAnim);
	anim->AddAnimation(1, burnAnim);

	action = 0;
}

void HolyWater::Threw(bool isLeft, float x, float y)
{
	action = 0;
	timeDelay = 0;
	SetPosition(x, y);
	SetVelocity(D3DXVECTOR3(((isLeft) ? -1 : 1) * 300, 500, 0));
}

void HolyWater::Update(float gameTime)
{
	if (isFiring)
		isFiring = !anim->CheckDoAllFrame();
	SubWeapon::Update(gameTime);
	velocity.y += EARTH_ACCELERATION * gameTime;
	anim->DoAnimation(action);
	anim->Update(gameTime);
	GameObject::SetBox(sprite->GetPosition().x,
		sprite->GetPosition().y,
		anim->frameWidth,
		anim->frameHeight,
		velocity.x,
		velocity.y);
	if (action == 1)
		if (timeDelay > 1.0f)
		{
			timeDelay = 0;
			this->SetEnable(false);
		}
		else
			timeDelay += gameTime;
}

void HolyWater::Render(ViewPort *viewPort)
{
	anim->Render(sprite, false, viewPort);
}

void HolyWater::CheckCollider(float gameTime, std::vector<GameObject*> *listGameObject)
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
					action = 1;
					velocity.x = 0;
					velocity.y = 0;
					(*i)->GetCollideEffect()->SetVisible(true);
				}
			}
			else if ((*i)->GetTag() == 2000 && ((Batman*)(*i))->isHurted == false)
			{
				if (IsEnable())
				{
					((Enemy*)(*i))->SetHP(((Enemy*)(*i))->GetHP() - GetDamage());
					action = 1;
					velocity.x = 0;
					velocity.y = 0;
					(*i)->GetCollideEffect()->SetVisible(true);
				}
			}
			else if ((*i)->GetTag() == 2001 && ((Medusa*)(*i))->isHurted == false)
			{
				if (IsEnable())
				{
					((Enemy*)(*i))->SetHP(((Enemy*)(*i))->GetHP() - GetDamage());
					action = 1;
					velocity.x = 0;
					velocity.y = 0;
					(*i)->GetCollideEffect()->SetVisible(true);
				}
			}
			/*if (((*i)->GetTag() < 10 && (*i)->GetTag() >= 0) || (*i)->GetTag() >= 2000)
			{
				if (IsEnable())
				{
					((Enemy*)(*i))->SetHP(((Enemy*)(*i))->GetHP() - 2);
					action = 1;
					velocity.x = 0;
					velocity.y = 0;
					(*i)->GetCollideEffect()->SetVisible(true);
				}
			}*/
			else
				if ((*i)->GetTag() == TAG_GROUND)
				{
					if (IsEnable())
					{
						action = 1;
						velocity.x = 0;
						velocity.y = 0;
						if(!Sound::GetInstance()->IsPLaying(eSoundID::HOLY_WATER))
							Sound::GetInstance()->Play(HOLY_WATER);
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
							action = 1;
							velocity.x = 0;
							velocity.y = 0;
							(*i)->GetCollideEffect()->SetVisible(true);
						}
					}
					else if ((*i)->GetTag() == 2000 && ((Batman*)(*i))->isHurted == false)
					{
						if (IsEnable())
						{
							((Enemy*)(*i))->SetHP(((Enemy*)(*i))->GetHP() - GetDamage());
							action = 1;
							velocity.x = 0;
							velocity.y = 0;
							(*i)->GetCollideEffect()->SetVisible(true);
						}
					}
					else if ((*i)->GetTag() == 2001 && ((Medusa*)(*i))->isHurted == false)
					{
						if (IsEnable())
						{
							((Enemy*)(*i))->SetHP(((Enemy*)(*i))->GetHP() - GetDamage());
							action = 1;
							velocity.x = 0;
							velocity.y = 0;
							(*i)->GetCollideEffect()->SetVisible(true);
						}
					}
					/*if (((*i)->GetTag() < 10 && (*i)->GetTag() >= 0) || (*i)->GetTag() >= 2000)
					{
						((Enemy*)(*i))->SetHP(((Enemy*)(*i))->GetHP() - 2);
						action = 1;
						velocity.x = 0;
						velocity.y = 0;
						(*i)->GetCollideEffect()->SetVisible(true);
					}*/
					else
						if ((*i)->GetTag() == TAG_GROUND)
						{
							if (IsEnable())
							{
								action = 1;
								velocity.x = 0;
								velocity.y = 0;
								if (!Sound::GetInstance()->IsPLaying(eSoundID::HOLY_WATER))
									Sound::GetInstance()->Play(HOLY_WATER);
							}
						}
				}
			}
		}
	}
}