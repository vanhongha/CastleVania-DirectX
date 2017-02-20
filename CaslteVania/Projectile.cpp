#include "Projectile.h"



Projectile::Projectile()
{
}


Projectile::~Projectile()
{
}

bool Projectile::Initialize(LPDIRECT3DDEVICE9 gDevice, const char* file, float x, float y, int tag)
{
	velocity.x = -100.0f;
	velocity.y = 0.0f;

	if (!GameObject::Initialize(gDevice, file, x, y, tag))
		return false;

	anim = new Animation();
	anim->Initialize(sprite, 14, 12, 0.1f);

	std::vector<D3DXVECTOR2> animation;
	animation.push_back(D3DXVECTOR2(0, 0));

	anim->AddAnimation(0, animation);
	return true;
}

void Projectile::Update(float gameTime)
{
	anim->Update(gameTime);
	velocity.x = isLeft ?  -abs(velocity.x) : abs(velocity.x);
	if (isEnable) {
		SetBox(sprite->GetPosition().x, sprite->GetPosition().y, 14, 12, velocity.x, velocity.y);
		sprite->SetPosition(sprite->GetPosition().x + velocity.x * gameTime,
			sprite->GetPosition().y + velocity.y * gameTime);
	}
}

void Projectile::Render(ViewPort *viewPort)
{
	if (isEnable)
		anim->Render(sprite, isLeft, viewPort);
}

void Projectile::SetIsLeft(bool _isLeft)
{
	isLeft = _isLeft;
}

void Projectile::SetVelocity(float vx, float vy)
{
	velocity.x = vx;
	velocity.y = vy;
}
