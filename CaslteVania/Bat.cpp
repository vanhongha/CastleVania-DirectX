#include "Bat.h"



Bat::Bat(int hp, int damage, int point) : Enemy(hp, damage, point)
{
}


Bat::~Bat()
{
}


void Bat::InitAnim()
{
	anim = new Animation();
	anim->Initialize(sprite, 32, 32, 0.15);

	std::vector<D3DXVECTOR2> sitAnimation;
	sitAnimation.push_back(D3DXVECTOR2(0, 0));

	std::vector<D3DXVECTOR2> moveAnimation;
	moveAnimation.push_back(D3DXVECTOR2(1, 0));
	moveAnimation.push_back(D3DXVECTOR2(2, 0));
	moveAnimation.push_back(D3DXVECTOR2(3, 0));

	anim->AddAnimation(SIT, sitAnimation);
	anim->AddAnimation(MOVE, moveAnimation);
}


bool Bat::Initialize(LPDIRECT3DDEVICE9 gDevice, const char* file, float x, float y, int tag)
{
	if (!Enemy::Initialize(gDevice, file, x, y, tag))
		return false;

	InitAnim();

	isLeft = true;

	velocity.x = -100.0f;
	velocity.y = 0.0f;

	action = MOVE;
}

void Bat::Update(float gameTime)
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
					D3DXVECTOR3 pos = sprite->GetPosition();

					if (isLeft)
					{
						pos.x += velocity.x * gameTime;
						pos.y = std::sin(pos.x * 0.03) * 40 + y;
					}
					else
					{
						pos.x -= velocity.x * gameTime;
						pos.y = std::sin(pos.x * 0.03) * 40 + y;
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