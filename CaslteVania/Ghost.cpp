#include "Ghost.h"



Ghost::Ghost(int hp, int damage, int point) : Enemy(hp, damage, point)
{
}


Ghost::~Ghost()
{
}

void Ghost::InitAnim()
{
	anim = new Animation();
	anim->Initialize(sprite, 28, 28, 0.15);

	std::vector<D3DXVECTOR2> initAnimation;
	initAnimation.push_back(D3DXVECTOR2(0, 0));
	initAnimation.push_back(D3DXVECTOR2(1, 0));

	anim->AddAnimation(0, initAnimation);
}


bool Ghost::Initialize(LPDIRECT3DDEVICE9 gDevice, const char* file, float x, float y, int tag)
{
	if (!Enemy::Initialize(gDevice, file, x, y, tag))
		return false;

	horizontalAxisVector = D3DXVECTOR3(1, 0, 0);

	InitAnim();

	isLeft = true;

	velocity.x = 50.0f;
	velocity.y = 50.0f;

	action = 0;
}

void Ghost::Update(float gameTime)
{
	Enemy::Setbox(0, 0, 0, 0);
	if (!isDead)
	{
		if (isEnable == true)
		{
			respawnTime = 0;

			if (hp < e_prevHP) {
				Sound::GetInstance()->Play(HIT_SOUND);
				e_isInvincible = true;
				e_prevHP = hp;
			}

			if (hp <= 0)
			{
				isDead = true;
				AddScore();
				return;
			}

			if (e_isInvincible == true) {
				e_invincibleTime -= gameTime;
				if (e_invincibleTime <= 0) {
					e_invincibleTime = E_INVINCIBLE_TIME;
					e_isInvincible = false;
				}
			}
			else {
				if (stopTime <= 0 || GetTag() == TAG_SIMON) {
					distance = std::sqrt(
						std::pow(targetPosition.x - GetPosition().x, 2) +
						std::pow(targetPosition.y - GetPosition().y, 2));

					direction.x = (targetPosition.x - GetPosition().x) / distance;
					direction.y = (targetPosition.y - GetPosition().y) / distance;

					D3DXVECTOR3 pos = sprite->GetPosition();
					pos.x += direction.x * velocity.x * gameTime;
					pos.y += direction.y * velocity.y * gameTime;

					sprite->SetPosition(pos.x, pos.y);

					anim->DoAnimation(action);
					anim->Update(gameTime);
				}
				else {
					stopTime -= gameTime;
				}

				SetPosition(sprite->GetPosition());
				SetRegion(0, width, 0, -height);
			}

			deadEffect->SetPosition(sprite->GetPosition().x, sprite->GetPosition().y);
		}
		else
			respawnTime += gameTime;
	}
	else
		UpdateEffect(gameTime);

	collideEffect->SetPosition(sprite->GetPosition().x, sprite->GetPosition().y - 20);
	collideEffect->Update(gameTime);
}

void Ghost::CalculateVectorFollowTarget()
{
	vectorDirection = D3DXVECTOR3(targetPosition.x - sprite->GetPosition().x,
		targetPosition.y - sprite->GetPosition().y,
		0);

	targetDistance = sqrt(pow(vectorDirection.x, 2) + pow(vectorDirection.y, 2));

	// cos(a, b) = (a1.b1 + a2.b2)/sqrt(a1^2 + a2^2)*sqrt(b1^2 + b2^2)
	// Cong thuc tim goc cua 2 vector
	cosAngle = (vectorDirection.x * horizontalAxisVector.x + vectorDirection.y * horizontalAxisVector.y) /
		(sqrt(pow(vectorDirection.x, 2) + pow(vectorDirection.y, 2)) *
			sqrt(pow(horizontalAxisVector.x, 2) + pow(horizontalAxisVector.y, 2)));

	velocity.x = targetDistance * cosAngle;
	velocity.y = targetDistance * sqrt(1 - pow(cosAngle, 2)); // sin^2 + cos^2 = 1
}