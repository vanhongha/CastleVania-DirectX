#include "MedusaHead.h"



MedusaHead::MedusaHead(int hp, int damage, int point) : Enemy(hp, damage, point)
{
}


MedusaHead::~MedusaHead()
{
}


void MedusaHead::InitAnim()
{
	anim = new Animation();
	anim->Initialize(sprite, 32, 32, 0.15);

	std::vector<D3DXVECTOR2> initAnimation;
	initAnimation.push_back(D3DXVECTOR2(0, 0));

	std::vector<D3DXVECTOR2> moveAnimation;
	moveAnimation.push_back(D3DXVECTOR2(0, 0));
	moveAnimation.push_back(D3DXVECTOR2(1, 0));

	anim->AddAnimation(0, initAnimation);
	anim->AddAnimation(MOVE, moveAnimation);
}


bool MedusaHead::Initialize(LPDIRECT3DDEVICE9 gDevice, const char* file, float x, float y, int tag)
{
	if (!Enemy::Initialize(gDevice, file, x, y, tag))
		return false;

	InitAnim();

	isLeft = true;

	velocity.x = -120.0f * 1.25;
	velocity.y = 0.0f;

	action = MOVE;
}

void MedusaHead::Update(float gameTime)
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
				if (stopTime <= 0) {
					D3DXVECTOR3 pos = sprite->GetPosition();

					if (isLeft)
					{
						pos.x += velocity.x * gameTime;
						pos.y = std::sin(pos.x * 0.03) * 95.5 + y;
					}
					else
					{
						pos.x -= velocity.x * gameTime;
						pos.y = std::sin(pos.x * 0.03) * 95.5 + y;
					}
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