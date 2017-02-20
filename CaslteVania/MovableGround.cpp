#include "MovableGround.h"

bool MovableGround::Initialize(LPDIRECT3DDEVICE9 gDevice, const char * file, float x, float y, int tag)
{
	if (!GameObject::Initialize(gDevice, file, x, y, tag))
		return false;
	velocity.x = 50;
	return true;
}

void MovableGround::Update(float gameTime)
{
	SetBox(sprite->GetPosition().x,
		sprite->GetPosition().y,
		64,
		16,
		this->velocity.x,
		0);
	sprite->SetPosition(D3DXVECTOR3(this->sprite->GetPosition().x + velocity.x*gameTime,
		this->sprite->GetPosition().y,
		0));
}

void MovableGround::CheckCollideWithGround(float gameTime, std::vector<GameObject*> *listGameObject)
{
	float normalX = 0;
	float normalY = 0;
	float timeCollide;

	for (std::vector<GameObject*>::iterator i = listGameObject->begin(); i != listGameObject->end(); i++)
	{
		if ((*i)->GetCollider()->GetTag() == TAG_GROUND)
		{
			Box tempBox = (*i)->GetCollider()->GetBox();
			if (collider->AABBCheck(collider->GetBox(), (*i)->GetCollider()->GetBox()))
			{
				return;
			}
			Box broadphaseBox = collider->GetSweptBoardphaseBox(collider->GetBox(), gameTime);
			if (collider->AABBCheck(broadphaseBox, (*i)->GetCollider()->GetBox()))
			{
				Box tempBox = (*i)->GetCollider()->GetBox();
				timeCollide = collider->SweptAABB(gameTime, collider->GetBox(), (*i)->GetCollider()->GetBox(), normalX, normalY);
				if ((timeCollide >= 0.0f && timeCollide < 1.0f))
				{
					if (normalX == 1)
					{
						sprite->SetPosition(collider->GetBox().left + timeCollide * gameTime * velocity.x + 0.1,
							sprite->GetPosition().y);
						this->velocity.x = 50;
					}
					else if (normalX == -1)
					{
						sprite->SetPosition(collider->GetBox().left + timeCollide * gameTime * velocity.x - 0.1,
							sprite->GetPosition().y);
						this->velocity.x = -50;
					}
					return;
				}
			}
		}
	}
}

MovableGround::MovableGround()
{
}


MovableGround::~MovableGround()
{
}
