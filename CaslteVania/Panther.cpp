#include "Panther.h"



Panther::Panther(int hp, int damage, int point) : Enemy(hp, damage, point)
{

}


Panther::~Panther()
{
}

void Panther::InitAnim()
{
	anim = new Animation();
	anim->Initialize(sprite, 64, 32, 0.15);

	std::vector<D3DXVECTOR2> sitAnimation;
	sitAnimation.push_back(D3DXVECTOR2(0, 0));

	std::vector<D3DXVECTOR2> moveAnimation;
	moveAnimation.push_back(D3DXVECTOR2(1, 0));
	moveAnimation.push_back(D3DXVECTOR2(2, 0));
	moveAnimation.push_back(D3DXVECTOR2(3, 0));

	std::vector<D3DXVECTOR2> jumpAnimation;
	jumpAnimation.push_back(D3DXVECTOR2(3, 0));

	anim->AddAnimation(SIT, sitAnimation);
	anim->AddAnimation(MOVE, moveAnimation);
	anim->AddAnimation(JUMP, jumpAnimation);
}

bool Panther::Initialize(LPDIRECT3DDEVICE9 gDevice, const char* file, float x, float y, int tag)
{
	if (!Enemy::Initialize(gDevice, file, x, y, tag))
		return false;

	InitAnim();
	isLeft = true;
	action = SIT;

	velocity.x = 0.0f;
	velocity.y = 0.0f;

	isGroundedInNextFrame = true;
	return true;
}

void Panther::Update(float gameTime)
{
	if (action == SIT)
	{
		velocity.x = 0.0f;
		velocity.y = 0.0f;
		isGroundedInNextFrame = true;
	}
	if (!isGroundedInNextFrame)
		Jump();
	if (isGrounded && action == JUMP)
	{
		action = MOVE;
		isLeft = isLeft ? false : true;
	}
	if (action == MOVE)
		Move();

	Enemy::GravityHandle(gameTime);
	Enemy::Setbox(0, 0, 0, 0);
	Enemy::Update(gameTime);
}

void Panther::SetAction(int action)
{
	this->action = action;
}

void Panther::Jump()
{
	action = JUMP;
	isGrounded = false;
	velocity.x = isLeft ? -450.0f : 450.0f;
	velocity.y = 350.0f;
	isGroundedInNextFrame = true;
}

void Panther::Move()
{
	velocity.x = isLeft ? -200 : 200;
}

void Panther::CheckCollider(float gameTime, std::vector<GameObject*> *listObject)
{
	bool state = isGrounded;
	Enemy::CheckCollider(gameTime, listObject);

	float normalX = 0;
	float normalY = 0;
	float timeCollide;

	Box topBox = collider->GetBox();
	topBox.right = topBox.left + 2;

	for (std::vector<GameObject*>::iterator i = listObject->begin(); i != listObject->end(); i++)
	{
		if ((*i)->GetCollider()->GetTag() == TAG_GROUND)
		{
			if (collider->AABBCheck(topBox, (*i)->GetCollider()->GetBox()))
			{
			}
			else
			{
				Box broadphaseBox = collider->GetSweptBoardphaseBox(topBox, gameTime);
				if (collider->AABBCheck(broadphaseBox, (*i)->GetCollider()->GetBox()))
				{
					timeCollide = collider->SweptAABB(gameTime, topBox, (*i)->GetCollider()->GetBox(), normalX, normalY);
					if ((timeCollide >= 0.0f && timeCollide < 1.0f))
					{
						if (normalY == 1)
						{
							isGroundedInNextFrame = true;
							return;
						}
					}
				}
			}
		}
	}
	if (state)
		isGroundedInNextFrame = false;
}