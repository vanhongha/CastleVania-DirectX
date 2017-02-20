#include "SubWeapon.h"




SubWeapon::SubWeapon()
{
}


SubWeapon::~SubWeapon()
{
}

bool SubWeapon::Initialize(LPDIRECT3DDEVICE9 gDevice, const char * file, float x, float y, int tag)
{

	if (!GameObject::Initialize(gDevice, file, x, y, tag))
		return false;
	useEnergy = 1;
	isEnable = false;
	InitAnim();
	return true;
}

void SubWeapon::Render(ViewPort * viewPort)
{
	GameObject::Render(viewPort);
}

void SubWeapon::Update(float gameTime)
{
	sprite->SetPosition(sprite->GetPosition().x + velocity.x * gameTime,
		sprite->GetPosition().y + velocity.y  * gameTime);
}


void SubWeapon::CheckCollider(float gameTime, std::vector<GameObject*> *listGameObject)
{
	float normalX = 0;
	float normalY = 0;
	float timeCollide;

	for (std::vector<GameObject*>::iterator i = listGameObject->begin(); i != listGameObject->end(); i++)
	{
		Box tempBox = (*i)->GetCollider()->GetBox();
		if (collider->AABBCheck(collider->GetBox(), (*i)->GetCollider()->GetBox()))
		{
			if ((*i)->GetTag() < 10 && (*i)->GetTag() >= 0)
			{
				if (((Enemy*)(*i))->e_isInvincible == false)
				{
					((Enemy*)(*i))->SetHP(((Enemy*)(*i))->GetHP() - GetDamage());
					anim->DoAnimation(1);
					this->sprite->SetPosition(-1000, -1000);
					(*i)->GetCollideEffect()->SetVisible(true);
				}
			}
			else if ((*i)->GetTag() == 2000)
			{
				if (((Batman*)(*i))->isHurted == false)
				{
					((Enemy*)(*i))->SetHP(((Enemy*)(*i))->GetHP() - GetDamage());
					//((Batman*)(*i))->isHurted = true;
					anim->DoAnimation(1);
					this->sprite->SetPosition(-1000, -1000);
					(*i)->GetCollideEffect()->SetVisible(true);
				}
			}
			else if ((*i)->GetTag() == 2001)
			{
				if (((Medusa*)(*i))->isHurted == false)
				{
					((Enemy*)(*i))->SetHP(((Enemy*)(*i))->GetHP() - GetDamage());
					anim->DoAnimation(1);
					this->sprite->SetPosition(-1000, -1000);
					(*i)->GetCollideEffect()->SetVisible(true);
				}
			}
			/*if ((*i)->GetTag() < 10 && (*i)->GetTag() >= 0)
			{
				((Enemy*)(*i))->SetHP(((Enemy*)(*i))->GetHP() - 2);
				anim->DoAnimation(1);
				this->sprite->SetPosition(-1000, -1000);
				(*i)->GetCollideEffect()->SetVisible(true);
			}*/
			else
				if ((*i)->GetTag() == TAG_GROUND)
				{
					anim->DoAnimation(1);
					this->sprite->SetPosition(-1000, -1000);
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
					if ((*i)->GetTag() < 10 && (*i)->GetTag() >= 0)
					{
						if (((Enemy*)(*i))->e_isInvincible == false)
						{
							((Enemy*)(*i))->SetHP(((Enemy*)(*i))->GetHP() - GetDamage());
							anim->DoAnimation(1);
							this->sprite->SetPosition(-1000, -1000);
							(*i)->GetCollideEffect()->SetVisible(true);
						}
					}
					else if ((*i)->GetTag() == 2000)
					{
						if (((Batman*)(*i))->isHurted == false)
						{
							((Enemy*)(*i))->SetHP(((Enemy*)(*i))->GetHP() - GetDamage());
							//((Batman*)(*i))->isHurted = true;
							anim->DoAnimation(1);
							this->sprite->SetPosition(-1000, -1000);
							(*i)->GetCollideEffect()->SetVisible(true);
						}
					}
					else if ((*i)->GetTag() == 2001)
					{
						if (((Medusa*)(*i))->isHurted == false)
						{
							((Enemy*)(*i))->SetHP(((Enemy*)(*i))->GetHP() - GetDamage());
							anim->DoAnimation(1);
							this->sprite->SetPosition(-1000, -1000);
							(*i)->GetCollideEffect()->SetVisible(true);
						}
					}
					/*if ((*i)->GetTag() < 10 && (*i)->GetTag() >= 0)
					{
						((Enemy*)(*i))->SetHP(((Enemy*)(*i))->GetHP() - 2);
						anim->DoAnimation(1);
						this->sprite->SetPosition(-1000, -1000);
						(*i)->GetCollideEffect()->SetVisible(true);
					}*/
					else
						if ((*i)->GetTag() == TAG_GROUND)
						{
							anim->DoAnimation(1);
							this->sprite->SetPosition(-1000, -1000);
						}
				}
			}
		}
	}
}

bool SubWeapon::CheckColliderWith(float gameTime, GameObject * gameObject)
{
	float normalX = 0;
	float normalY = 0;
	float timeCollide;

	Box tempBox = gameObject->GetCollider()->GetBox();
	if (collider->AABBCheck(collider->GetBox(), gameObject->GetCollider()->GetBox()))
	{
		return true;
	}
	else
	{
		Box broadphaseBox = collider->GetSweptBoardphaseBox(collider->GetBox(), gameTime);
		if (collider->AABBCheck(broadphaseBox, gameObject->GetCollider()->GetBox()))
		{
			Box tempBox = gameObject->GetCollider()->GetBox();
			timeCollide = collider->SweptAABB(gameTime, collider->GetBox(), gameObject->GetCollider()->GetBox(), normalX, normalY);
			if ((timeCollide >= 0.0f && timeCollide < 1.0f))
			{
				return true;
			}
		}
	}
	return false;
}

void SubWeapon::CheckColliderWithBoss(float gameTime, GameObject *object)
{
	if (object->IsEnable() == true && ((Batman*)object)->isHurted == false)
	{
		float normalX = 0;
		float normalY = 0;
		float timeCollide;

		/*Box tempBox = (object)->GetCollider()->GetBox();
		if (collider->AABBCheck(collider->GetBox(), object->GetCollider()->GetBox()))
		{
			((Enemy*)object)->SetHP(((Enemy*)object)->GetHP() - 2);
			((Batman*)object)->isHurted = true;
		}*/

		Box broadphaseBox = collider->GetSweptBoardphaseBox(collider->GetBox(), gameTime);
		if (collider->AABBCheck(broadphaseBox, object->GetCollider()->GetBox()))
		{
			Box tempBox = object->GetCollider()->GetBox();
			timeCollide = collider->SweptAABB(gameTime, collider->GetBox(), object->GetCollider()->GetBox(), normalX, normalY);
			if ((timeCollide >= 0.0f && timeCollide < 1.0f))
			{
				((Enemy*)object)->SetHP(((Enemy*)object)->GetHP() - 2);
				((Batman*)object)->isHurted = true;
				object->GetCollideEffect()->SetVisible(true);
			}
		}
	}
}