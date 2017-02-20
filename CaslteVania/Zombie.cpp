#include "Zombie.h"



Zombie::Zombie(int hp, int damage, int point) : Enemy(hp, damage, point)
{
}


Zombie::~Zombie()
{
}

bool Zombie::Initialize(LPDIRECT3DDEVICE9 gDevice, const char* file, float x, float y, int tag)
{
	if (!Enemy::Initialize(gDevice, file, x, y, tag))
		return false;

	InitAnimation();

	action = MOVE;

	isLeft = true;

	velocity.x = -70.0f;
	velocity.y = 0;

	return true;
}

void Zombie::InitAnimation()
{
	anim = new Animation();
	anim->Initialize(sprite, 34, 64, 0.1);

	std::vector<D3DXVECTOR2> moveAnimation;
	moveAnimation.push_back(D3DXVECTOR2(0, 0));
	moveAnimation.push_back(D3DXVECTOR2(1, 0));

	anim->AddAnimation(MOVE, moveAnimation);
}

void Zombie::Update(float gameTime)
{
	velocity.x = isLeft ? -70 : 70;
	Enemy::GravityHandle(gameTime);
	Enemy::Setbox(0, 0, 0, 0);
	Enemy::Update(gameTime);
}

void Zombie::CheckCollider(float gameTime, std::vector<GameObject*> *listObject)
{
	Enemy::CheckCollider(gameTime, listObject);
	this->CheckColliderWithWall(gameTime, listObject);
}

void Zombie::CheckColliderWithWall(float gameTime, std::vector<GameObject*>* listGameObject)
{
	float normalX = 0;
	float normalY = 0;
	float timeCollide;

	for (std::vector<GameObject*>::iterator i = listGameObject->begin(); i != listGameObject->end(); i++)
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
					if (normalX == 1 || normalX == -1)
					{
						isLeft = isLeft ? false : true;
						return;
					}
				}
			}
		}
	}
}