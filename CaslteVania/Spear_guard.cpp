#include "Spear_guard.h"



Spear_guard::Spear_guard(int hp, int damage, int point) : Enemy(hp, damage, point)
{
}

void Spear_guard::InitAnim()
{
	anim = new Animation();
	anim->Initialize(sprite, 32, 64, 0.1);

	std::vector<D3DXVECTOR2> moveAnimation;
	moveAnimation.push_back(D3DXVECTOR2(0, 0));
	moveAnimation.push_back(D3DXVECTOR2(1, 0));
	moveAnimation.push_back(D3DXVECTOR2(2, 0));
	moveAnimation.push_back(D3DXVECTOR2(3, 0));

	anim->AddAnimation(MOVE, moveAnimation);
}

Spear_guard::~Spear_guard()
{
}

bool Spear_guard::Initialize(LPDIRECT3DDEVICE9 gDevice, const char * file, float x, float y, int tag)
{
	if (!Enemy::Initialize(gDevice, file, x, y, tag))
		return false;

	InitAnim();

	action = MOVE;

	isLeft = true;

	velocity.x = -70.0f;
	velocity.y = 0;

	return true;
}

void Spear_guard::Update(float gameTime)
{
	timeDelay += gameTime;
	if (timeDelay > 1.0f)
	{
		isLeft = isLeft ? false : true;
		timeDelay = 0.0f;
	}
	velocity.x = isLeft ? -70 : 70;
	Enemy::GravityHandle(gameTime);
	Enemy::Setbox(0, 0, 0, 0);
	Enemy::Update(gameTime);
}

void Spear_guard::CheckCollider(float gameTime, std::vector<GameObject*>* listObject)
{
	Enemy::CheckCollider(gameTime, listObject);
	this->CheckColliderWithWall(gameTime, listObject);
}

void Spear_guard::CheckColliderWithWall(float gameTime, std::vector<GameObject*> *listObject)
{
	float normalX = 0;
	float normalY = 0;
	float timeCollide;

	for (std::vector<GameObject*>::iterator i = listObject->begin(); i != listObject->end(); i++)
	{
		if ((*i)->GetCollider()->GetTag() == TAG_GROUND)
		{
			Box broadphaseBox = collider->GetSweptBoardphaseBox(collider->GetBox(), gameTime);
			if (collider->AABBCheck(broadphaseBox, (*i)->GetCollider()->GetBox()))
			{
				Box tempBox = (*i)->GetCollider()->GetBox();
				timeCollide = collider->SweptAABB(gameTime, collider->GetBox(), (*i)->GetCollider()->GetBox(), normalX, normalY);
				if ((timeCollide >= 0.0f && timeCollide < 1.0f))
				{
					if (normalX == 1)
					{
						sprite->SetPosition(collider->GetBox().left + gameTime * timeCollide * velocity.x + 0.1,
							sprite->GetPosition().y);
						isLeft = isLeft ? false : true;
						timeDelay = 0.0f;
						return;

					}
					if (normalX == -1)
					{
						sprite->SetPosition(collider->GetBox().left + gameTime * timeCollide * velocity.x - 0.1,
							sprite->GetPosition().y);
						isLeft = isLeft ? false : true;
						timeDelay = 0.0f;
						return;
					}
				}
			}
		}
	}
}