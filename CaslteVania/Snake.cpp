#include "Snake.h"



Snake::Snake(int hp, int damage, int point) : Enemy(hp, damage, point)
{
}


Snake::~Snake()
{
}

bool Snake::Initialize(LPDIRECT3DDEVICE9 gDevice, const char* file, float x, float y, int tag)
{
	if (!Enemy::Initialize(gDevice, file, x, y, tag))
		return false;

	InitAnimation();

	action = MOVE;

	isLeft = true;

	velocity.x = -130;
	velocity.y = 0;

	return true;
}

void Snake::InitAnimation()
{
	anim = new Animation();
	anim->Initialize(sprite, 32, 20, 0.1);

	std::vector<D3DXVECTOR2> moveAnimation;
	moveAnimation.push_back(D3DXVECTOR2(0, 0));
	moveAnimation.push_back(D3DXVECTOR2(1, 0));

	anim->AddAnimation(MOVE, moveAnimation);
}

void Snake::Update(float gameTime)
{
	velocity.x = isLeft ? -70 : 70;
	Enemy::GravityHandle(gameTime);
	Enemy::Setbox(0, 0, 0, 0);
	Enemy::Update(gameTime);
}

void Snake::CheckCollider(float gameTime, std::vector<GameObject*> *listObject)
{
	Enemy::CheckCollider(gameTime, listObject);
}


