#include "Simon.h"


int Simon::score = 0;

Simon::Simon(int hp, int damage, int point) : Enemy(hp, damage, point)
{
	respawnTime = 0;
	respawnTimeNeo = 0;
}


Simon::~Simon()
{
	SAFE_DELETE(anim);
}

bool Simon::Initialize(LPDIRECT3DDEVICE9 _gDevice, const char* _file, float _x, float _y, int tag)
{
	if (!GameObject::Initialize(_gDevice, _file, _x, _y, tag))
		return false;
	isLeft = false;

	isCollideLeftWall = false;
	isCollideRightWall = false;

	isCollideWith25 = false;
	isCollideWith_25 = false;
	isCollideWith22 = false;
	isCollideWith_22 = false;
	isOnStair = false;
	simonSpeed = SPEED;
	currentDirection = 0;
	currentWeapon = ITEM_CROSS;

	canControlKeyboard = true;
	
	doorCollideDirection = 0;
	isGoingThrowDoor = false;

	deadTime = 0;

	this->CreateAnimation();

	isInvincible = false;
	invincibleTime = INVINCIBLE_TIME;
	nextStage = false;

	whip = new Whip();
	if (!whip->Initialize(_gDevice, WHIP_SPRITE, 0, 0, TAG_WHIP))
		return false;

	//////////////////////////// INIT SUB WEAPON ///////////////////////////
	for (int i = 0; i < 3; i++)
	{
		axe[i] = new Axe();
		if (!axe[i]->Initialize(_gDevice, "", 0, 0, TAG_AXE))
			return false;

		cross[i] = new Cross();
		if (!cross[i]->Initialize(_gDevice, "", 0, 0, TAG_CROSS))
			return false;

		knife[i] = new Knife();
		if (!knife[i]->Initialize(_gDevice, "", 0, 0, TAG_KNIFE))
			return false;

		holyWater[i] = new HolyWater();
		if (!holyWater[i]->Initialize(_gDevice, "", 0, 0, TAG_HOLY_WATER))
			return false;

		stopWatch[i] = new StopWatch();
		if (!stopWatch[i]->Initialize(_gDevice, "", 0, 0, ITEM_STOP_WATCH))
			return false;
		
		subWeapon[i] = cross[i];
	}

	noSubWeapon = 1;
	/*subWeapon = axe;
	subWeapon->SetEnable(false);
	*/
	////////////////////////////////////////////////////////////////////////

	prevHP = hp;
	life = 3;
	typeOfSubWeapon = 0;
	energy = 0;
	typeOfWhip = 1;

	return true;
}

void Simon::Reload()
{
	prevHP = hp;
	isInvincible = false;
	invincibleTime = INVINCIBLE_TIME;
	nextStage = false;

	isGravity = true;
	isGrounded = false;
	this->isLeft = false;

	isCollideLeftWall = false;
	isCollideRightWall = false;

	isCollideWith25 = false;
	isCollideWith_25 = false;
	isCollideWith22 = false;
	isCollideWith_22 = false;
	isOnStair = false;

	SetSize(sprite->GetWidth(), sprite->GetHeight());
	SetPosition(sprite->GetPosition());
	SetRegion(0, width, 0, -height);
	canControlKeyboard = true;
}

void Simon::Reset(D3DXVECTOR3 pos)
{
	isLeft = false;
	isGrounded = false;
	velocity.x = 0;
	action = STAND;
	isOnStair = false;
	isDead = false;
	SetPosition(pos.x, pos.y + 100);
	SetEnable(true);
	isInvincible = false;
	invincibleTime = INVINCIBLE_TIME;
	SetHP(16);
	prevHP = hp;
	canControlKeyboard = true;
}

void Simon::OnStairHandle(float gameTime)
{
	if (isCollideWith25)
	{
		if (IsKeyDown(DIK_UP))
		{
			if (this->sprite->GetPosition().x + 60 > stairCollidePos.x + 32 && !isOnStair)
			{
				action = MOVE;
				isLeft = true;
				velocity.x = -simonSpeed;
			}
			else {
				isOnStair = true;
				directionX = 1;
				directionY = 1;
			}
		}
	}
	else if (isCollideWith_25)
	{
		if (IsKeyDown(DIK_UP))
		{
			if (this->sprite->GetPosition().x < stairCollidePos.x && !isOnStair)
			{
				action = MOVE;
				isLeft = false;
				velocity.x = simonSpeed;
			}
			else {
				isOnStair = true;
				directionX = -1;
				directionY = 1;
			}
		}
	}
	
	if (isCollideWith22)
	{
		
		if (IsKeyDown(DIK_DOWN))
		{
			if (this->sprite->GetPosition().x + 25 > stairCollidePos.x && !isOnStair)
			{
				action = MOVE;
				isLeft = true;
				velocity.x = -simonSpeed;
			}
			else
			{
				isOnStair = true;
				directionX = -1;
				directionY = -1;
			}
		}
	}
	else if (isCollideWith_22)
	{
		if (IsKeyDown(DIK_DOWN))
		{
			if (this->sprite->GetPosition().x + 60 - 30 < stairCollidePos.x + 32 && !isOnStair)
			{
				action = MOVE;
				isLeft = false;
				velocity.x = simonSpeed;
			}
			else
			{
				isOnStair = true;
				directionX = 1;
				directionY = -1;
			}
		}
	}

	if (isOnStair)
	{
		if (directionX * directionY > 0)
		{
			if (IsKeyDown(DIK_DOWN))
			{
				action = DOWN_STAIR;
				isLeft = true;
				velocity.x = -simonSpeed / 2;
				velocity.y = -simonSpeed / 2;
			}
			else if (IsKeyDown(DIK_UP))
			{
				action = UP_STAIR;
				isLeft = false;
				velocity.x = simonSpeed / 2;
				velocity.y = simonSpeed / 2;
			}
		}
		else if (directionX * directionY < 0)
		{
			if (IsKeyDown(DIK_DOWN))
			{
				action = DOWN_STAIR;
				isLeft = false;
				velocity.x = simonSpeed / 2;
				velocity.y = -simonSpeed / 2;
			}
			else if (IsKeyDown(DIK_UP))
			{
				action = UP_STAIR;
				isLeft = true;
				velocity.x = -simonSpeed / 2;
				velocity.y = simonSpeed / 2;
			}
		}
	}

	if ((IsKeyRelease(DIK_DOWN) || IsKeyRelease(DIK_UP)) && isOnStair)
	{
		if (velocity.y < 0)
			action = STANDING_DOWN;
		else
			action = STANDING_UP;
		velocity.x = 0;
		velocity.y = 0;
	}

}

void Simon::SetInvincible(int time)
{
	invincibleTime = time;
	isInvincible = true;
}

void Simon::CreateAnimation()
{
	anim = new Animation();
	anim->Initialize(sprite, 60.0f, 66.0f, 0.15f);

	// Khoi tao animation dung yen
	std::vector<D3DXVECTOR2> standAnimation;
	standAnimation.push_back(D3DXVECTOR2(0, 0));

	//Khoi tao animation luc roi
	std::vector<D3DXVECTOR2> fallAnimation;
	fallAnimation.push_back(D3DXVECTOR2(0, 0));

	// Khoi tao animation luc di chuyen
	std::vector<D3DXVECTOR2> moveAnimation;
	moveAnimation.push_back(D3DXVECTOR2(0, 0));
	moveAnimation.push_back(D3DXVECTOR2(1, 0));
	moveAnimation.push_back(D3DXVECTOR2(2, 0));
	moveAnimation.push_back(D3DXVECTOR2(3, 0));

	//// Khoi tao animation luc ngoi
	std::vector<D3DXVECTOR2> sitAnimation;
	sitAnimation.push_back(D3DXVECTOR2(4, 0));

	//// Khoi tao animation luc nhay
	std::vector<D3DXVECTOR2> jumpAnimation;
	jumpAnimation.push_back(D3DXVECTOR2(4, 0));

	////Khoi tao animation luc danh roi khi dung
	std::vector<D3DXVECTOR2> hitStandAnimation;
	hitStandAnimation.push_back(D3DXVECTOR2(5, 0));
	hitStandAnimation.push_back(D3DXVECTOR2(6, 0));
	hitStandAnimation.push_back(D3DXVECTOR2(7, 0));

	std::vector<D3DXVECTOR2> throwSubWeapon;
	throwSubWeapon.push_back(D3DXVECTOR2(5, 0));
	throwSubWeapon.push_back(D3DXVECTOR2(6, 0));
	throwSubWeapon.push_back(D3DXVECTOR2(7, 0));

	////Khoi tao animation luc danh roi khi ngoi
	std::vector<D3DXVECTOR2> hitSitAnimation;
	hitSitAnimation.push_back(D3DXVECTOR2(7, 1));
	hitSitAnimation.push_back(D3DXVECTOR2(0, 2));
	hitSitAnimation.push_back(D3DXVECTOR2(1, 2));

	std::vector<D3DXVECTOR2> goUpStair;
	goUpStair.push_back(D3DXVECTOR2(4, 1));
	goUpStair.push_back(D3DXVECTOR2(5, 1));

	std::vector<D3DXVECTOR2> goDownStair;
	goDownStair.push_back(D3DXVECTOR2(2, 1));
	goDownStair.push_back(D3DXVECTOR2(3, 1));

	std::vector<D3DXVECTOR2> hitUpStair;
	hitUpStair.push_back(D3DXVECTOR2(5, 2));
	hitUpStair.push_back(D3DXVECTOR2(6, 2));
	hitUpStair.push_back(D3DXVECTOR2(7, 2));

	std::vector<D3DXVECTOR2> hitDownStair;
	hitDownStair.push_back(D3DXVECTOR2(2, 2));
	hitDownStair.push_back(D3DXVECTOR2(3, 2));
	hitDownStair.push_back(D3DXVECTOR2(4, 2));

	// Khoi tao animation nhay lui
	std::vector<D3DXVECTOR2> hurtedAnimation;
	hurtedAnimation.push_back(D3DXVECTOR2(0, 1));

	std::vector<D3DXVECTOR2> deadAnimation;
	deadAnimation.push_back(D3DXVECTOR2(4, 3));

	//Khoi tao animation dung yen khi dang len cau thang
	std::vector<D3DXVECTOR2> standingUpStairAnimation;
	standingUpStairAnimation.push_back(D3DXVECTOR2(4, 1));

	//Khoi tao animation dung yen khi di xuong cau thang
	std::vector<D3DXVECTOR2> standingDownStairAnimation;
	standingDownStairAnimation.push_back(D3DXVECTOR2(2, 1));

	std::vector<D3DXVECTOR2> invisibleAnimation;
	invisibleAnimation.push_back(D3DXVECTOR2(3, 4));

	std::vector<D3DXVECTOR2> eatItemAnimation;
	eatItemAnimation.push_back(D3DXVECTOR2(0, 3));
	eatItemAnimation.push_back(D3DXVECTOR2(1, 3));
	eatItemAnimation.push_back(D3DXVECTOR2(2, 3));

	std::vector<D3DXVECTOR2> standBack;
	standBack.push_back(D3DXVECTOR2(1, 1));

	// Them cac animation
	anim->AddAnimation(STAND, standAnimation);
	anim->AddAnimation(MOVE, moveAnimation);
	anim->AddAnimation(SIT, sitAnimation);
	anim->AddAnimation(JUMP, jumpAnimation);
	anim->AddAnimation(HIT_STAND, hitStandAnimation);
	anim->AddAnimation(HIT_SIT, hitSitAnimation);
	anim->AddAnimation(FALL_DOWN, fallAnimation);
	anim->AddAnimation(UP_STAIR, goUpStair);
	anim->AddAnimation(DOWN_STAIR, goDownStair);
	anim->AddAnimation(HURTED, hurtedAnimation);
	anim->AddAnimation(STANDING_UP, standingUpStairAnimation);
	anim->AddAnimation(STANDING_DOWN, standingDownStairAnimation);
	anim->AddAnimation(THROW_SUB_WEAPON, throwSubWeapon);
	anim->AddAnimation(HIT_UP_STAIR, hitUpStair);
	anim->AddAnimation(HIT_DOWN_STAIR, hitDownStair);
	anim->AddAnimation(DEAD, deadAnimation);
	anim->AddAnimation(INVISIBLE, invisibleAnimation);
	anim->AddAnimation(EAT_ITEM, eatItemAnimation);
	anim->AddAnimation(STAND_BACK, standBack);

	action = STAND;
}

void Simon::Update(float gameTime)
{
	SetRegion(0, width, 0, -height);

	if (velocity.y > 0)
		currentDirection = 1;
	else if (velocity < 0)
		currentDirection = -1;

	OnStairHandle(gameTime);

	if (!isOnStair)
		GravityHandle(gameTime);

	if(action != SIT && action != HIT_SIT && action != JUMP)
		Enemy::Setbox(30, 0, -15, 0);
	else
		Enemy::Setbox(30, 16, -15, 16);

	if (!isDead)
	{
		if (isEnable == true)
		{
			if (hp < prevHP) {
				prevHP = hp;
				isInvincible = true;
			}

			if (hp <= 0)
			{
				isDead = true;
				respawnTime = 0.0f;
			}

			sprite->SetPosition(sprite->GetPosition().x + velocity.x * gameTime,
				sprite->GetPosition().y + velocity.y  * gameTime);

			anim->DoAnimation(action);
			anim->Update(gameTime);

			SetPosition(sprite->GetPosition());
			SetRegion(0, width, 0, -height);

		}
	}
	else
	{
		if (isGrounded)
		{
			isInvincible = false;
			canControlKeyboard = false;
			action = DEAD;
			respawnTime += gameTime;
			if (respawnTime > 2)
			{
				respawnTime = 0.0f;
				isDead = false;
				isEnable = false;
			}
			if (Sound::GetInstance()->IsPLaying(eSoundID::STAGE_01_VAMPIRE_KILLER))
			{
				Sound::GetInstance()->Stop(STAGE_01_VAMPIRE_KILLER);
			}
			if(Sound::GetInstance()->IsPLaying(eSoundID::STAGE_04_STALKER))
			{
				Sound::GetInstance()->Stop(STAGE_04_STALKER);
			}
			if (!Sound::GetInstance()->IsPLaying(eSoundID::LIFE_LOST))
			{
				Sound::GetInstance()->Play(eSoundID::LIFE_LOST);
			}
		}
		else
		{
			sprite->SetPosition(sprite->GetPosition().x + velocity.x * gameTime,
				sprite->GetPosition().y + velocity.y  * gameTime);

		}
		anim->DoAnimation(action);
		anim->Update(gameTime);
	}

	whip->Update(gameTime);

	if (action == HIT_SIT || action == HIT_STAND || action == HIT_UP_STAIR)
		isFighting = !anim->CheckDoAllFrame();

	for (int i = 0;i < noSubWeapon;i++)
	{
		if (!subWeapon[i])
			subWeapon[i] = subWeapon[0];
		if (subWeapon[i]->IsEnable())
			subWeapon[i]->Update(gameTime);
	}

	SetRegion(0, anim->frameWidth, 0, -anim->frameHeight);

	if (isInvincible)
		isSplashing = isSplashing ? false : true;
	else
		isSplashing = false;
}

void Simon::Render(ViewPort *viewPort)
{
	if (!isSplashing)
	{
		if (isFighting && (action == HIT_SIT || action == HIT_STAND || action == HIT_UP_STAIR || action == HIT_DOWN_STAIR))
		{
			whip->Render(viewPort);
		}
		anim->Render(sprite, isLeft, viewPort);
		for (int i = 0;i < noSubWeapon;i++)
			if (subWeapon[i]->IsEnable())
				subWeapon[i]->Render(viewPort);
	}
}

D3DXVECTOR3 Simon::GetVelocity()
{
	return velocity;
}

void Simon::UpdateKeyboard(float gameTime)
{
	if (canControlKeyboard)
	{
		if ((action != HURTED || isGrounded == true) && !(action == HIT_STAND && isGrounded == false))
		{
			if (IsKeyPress(DIK_SPACE) && isGrounded)
			{
				if (action == SIT || action == JUMP)
					return;
				action = JUMP;
				Jump();
				isGrounded = false;
			}

			if (isGrounded && velocity.y > 0 && action != JUMP && action != HURTED && !isCollideWith25)
			{
				isOnStair = false;
				Stand();
			}
			else if (isGrounded && velocity.y == 0)
			{
				isOnStair = false;
				Stand();
			}

			if (IsKeyDown(DIK_DOWN) && !isOnStair)
			{
				if (action == JUMP || action == HURTED)
					return;
				action = SIT;
				Sit();
			}

			if (IsKeyDown(DIK_LCONTROL))
			{
				if (isOnStair)
				{
					if (currentDirection == 1)
						action = HIT_UP_STAIR;
					else action = HIT_DOWN_STAIR;
				}
				else if (action == SIT)
					action = HIT_SIT;
				else
					action = HIT_STAND;
				Fight();
				return;
			}

			if (IsKeyPress(DIK_RCONTROL) || IsKeyPress(DIK_0))
			{
				for (int i = 0;i < noSubWeapon;i++)
					if (!subWeapon[i]->IsEnable() && (energy - subWeapon[i]->GetUseEnergy() >= 0))
					{
						energy -= subWeapon[i]->GetUseEnergy();
						isThrowing = true;
						action = THROW_SUB_WEAPON;
						break;
					}
			}

			if (isGrounded == true)
			{
				velocity.x = 0;
			}

			if (IsKeyDown(DIK_LEFT) && !isOnStair)
			{
				if (action == JUMP && isGrounded == false)
					return;
				isLeft = true;
				if (action != SIT)
				{
					action = MOVE;
					Move();
				}
			}

			if (IsKeyDown(DIK_RIGHT) && !isOnStair)
			{
				if (action == JUMP && isGrounded == false)
					return;
				isLeft = false;
				if (action != SIT)
				{
					action = MOVE;
					Move();
				}
			}
		}
	}
}

void Simon::Throwing()
{
	if (anim->GetCurrentFrame() == 2)
	{
		for (int i = 0;i < noSubWeapon;i++)
		{
			if (!subWeapon[i]->IsEnable())
			{
				subWeapon[i]->SetEnable(true);
				subWeapon[i]->Threw(isLeft, this->sprite->GetPosition().x + 60 / 2, this->sprite->GetPosition().y);
				action = STAND;
				break;
			}
		}
	}
}

void Simon::Move()
{
	action = MOVE;
	velocity.x = ((isLeft) ? -1 : 1) * simonSpeed;
}

void Simon::Jump()
{
	velocity.y = JUMP_FORCE;
}

void Simon::Sit()
{
	velocity.x = 0;
	velocity.y = 0;
}

void Simon::Stand()
{
	//if (action != HURTED)
	if(action != STAND)
		velocity.x = 0;
	action = STAND;
	velocity.y = 0;
	whip->SetType(NORMAL_WHIP);
}

void Simon::Fight()
{
	isFighting = true;
	whip->SetType(typeOfWhip);
	Fighting();
	Sound::GetInstance()->Play(USING_WHIP);
}

void Simon::Fighting()
{
	if (!isFighting)
	{
		if (action == HIT_SIT)
		{
			action = SIT;
			Sit();
		}
		else 
		{
			if (!isOnStair)
			{
				action = STAND;
				Stand();
			}
			else
			{
				if (currentDirection > 0)
					action = STANDING_UP;
				else action = STANDING_DOWN;
			}
				
		}
		whip->SetType(NORMAL_WHIP);
	}
	else
	{
		if(action == HIT_SIT)
			whip->SetPosition(D3DXVECTOR3(this->GetPosition().x, this->GetPosition().y, 0), false);
		else
			whip->SetPosition(D3DXVECTOR3(this->GetPosition().x, this->GetPosition().y, 0), true);
		whip->SetIsLeft(isLeft);
		if (isGrounded)
			velocity.x = 0;
	}
}

void Simon::StandOnStair()
{
	velocity.x = 0;
	velocity.y = 0;
}

void Simon::KeyBoardHandle(float gameTime)
{
	if (action == HIT_SIT || action == HIT_STAND || action == HIT_UP_STAIR || action == HIT_DOWN_STAIR)
		Fighting();
	else if (action == THROW_SUB_WEAPON)
		Throwing();
	else
		UpdateKeyboard(gameTime);
}

void Simon::Hurted()
{
	velocity.y = JUMP_FORCE - 100;
	velocity.x = velocity.x = ((isLeft) ? 1 : -1) * simonSpeed;	
	sprite->SetPosition(sprite->GetPosition().x, sprite->GetPosition().y + 1);
	Sound::GetInstance()->Play(eSoundID::SIMON_HURTED);
}

void Simon::CheckCollider(float gameTime, std::vector<GameObject*> *listGameObject)
{
	this->CheckCollideWithStair(gameTime, listGameObject);
	this->CheckCollideWithGround(gameTime, listGameObject);
	this->CheckColliderWithMovableGround(gameTime, listGameObject);
	this->CheckCollideWithWall(gameTime, listGameObject);
	this->CheckCollieWithTopStair(gameTime, listGameObject);

	if (isInvincible == false && !isDead)
		this->CheckCollideWithEnemy(gameTime, listGameObject);
	else
	{
		invincibleTime -= gameTime;
		if (invincibleTime <= 0)
		{
			isInvincible = false;
			invincibleTime = INVINCIBLE_TIME;
		}
	}

	this->WhipCheckCollider(gameTime, listGameObject);
}

void Simon::CheckCollideWithGround(float gameTime, std::vector<GameObject*>* listGameObject)
{
	float normalX = 0;
	float normalY = 0;

	for (std::vector<GameObject*>::iterator i = listGameObject->begin(); i != listGameObject->end(); i++)
	{
		if ((*i)->GetCollider()->GetTag() == TAG_GROUND || (*i)->GetCollider()->GetTag() == TAG_STAIR_TOP)
		{
			Box tempBox = (*i)->GetCollider()->GetBox();
			if (collider->AABBCheck(collider->GetBox(), (*i)->GetCollider()->GetBox()))
			{
				isGrounded = false;
			}
			Box broadphaseBox = collider->GetSweptBoardphaseBox(collider->GetBox(), gameTime);
			if (collider->AABBCheck(broadphaseBox, (*i)->GetCollider()->GetBox()))
			{
				Box tempBox = (*i)->GetCollider()->GetBox();
				timeCollideGround = collider->SweptAABB(gameTime, collider->GetBox(), (*i)->GetCollider()->GetBox(), normalX, normalY);
				if ((timeCollideGround >= 0.0f && timeCollideGround < 1.0f))
				{
					if (normalY == 1)
					{
						isGrounded = true;
						if ((isGrounded && !isOnStair) ||	// Neu nhu cham dat va ko o tren cau thang
							(isOnStair  && isGrounded && (isCollideWith25 || isCollideWith_25)))	// Neu nhu dang o tren cau thang, di xuong cham chan cau thang
						{	
							if (action != SIT && action != HIT_SIT && action != JUMP)
								sprite->SetPosition(sprite->GetPosition().x,
									collider->GetBox().top + gameTime * timeCollideGround * velocity.y + 0.1);
							else
								sprite->SetPosition(sprite->GetPosition().x,
									collider->GetBox().top + gameTime * timeCollideGround * velocity.y + 0.1 + 16);
							velocity.y = 0;
						}
				
						return;
					}
				}
			}
			else
				isGrounded = false;
		}
	}
}

void Simon::CheckCollideWithGround(float gameTime, GameObject *gameObject)
{
	float normalX = 0;
	float normalY = 0;
	float timeCollide;
		Box tempBox = gameObject->GetCollider()->GetBox();
		if (collider->AABBCheck(collider->GetBox(), gameObject->GetCollider()->GetBox()))
		{
			isGrounded = false;
		}
		Box broadphaseBox = collider->GetSweptBoardphaseBox(collider->GetBox(), gameTime);
		if (collider->AABBCheck(broadphaseBox, gameObject->GetCollider()->GetBox()))
		{
			Box tempBox = gameObject->GetCollider()->GetBox();
			timeCollide = collider->SweptAABB(gameTime, collider->GetBox(), gameObject->GetCollider()->GetBox(), normalX, normalY);
			if ((timeCollide >= 0.0f && timeCollide < 1.0f))
			{
				if (normalY == 1)
				{
					sprite->SetPosition(sprite->GetPosition().x,
						collider->GetBox().top + gameTime * timeCollide * velocity.y + 0.1);
					velocity.y = 0;
					isGrounded = true;
					return;
				}
			}
		}
		else
			isGrounded = false;
}

void Simon::CheckCollideWithStair(float gameTime, std::vector<GameObject*>* listGameObject)
{
	float normalX = 0;
	float normalY = 0;
	float timeCollide;

	for (std::vector<GameObject*>::iterator i = listGameObject->begin(); i != listGameObject->end(); i++)
	{
		if ((*i)->GetCollider()->GetTag() == 25 || (*i)->GetCollider()->GetTag() == -25) // cham bot
		{
			Box tempBox = (*i)->GetCollider()->GetBox();
			if (collider->AABBCheck(collider->GetBox(), (*i)->GetCollider()->GetBox()))
			{
				if ((*i)->GetCollider()->GetTag() == 25)
				{
					isCollideWith25 = true;
					isCollideWith_25 = false;
					isCollideWith22 = false;
				}
				else if ((*i)->GetCollider()->GetTag() == -25)
				{
					isCollideWith25 = false;
					isCollideWith_25 = true;
					isCollideWith22 = false;
					isCollideWith_22 = false;
				}
				stairCollidePos = (*i)->GetPosition();
				return;
			}
			Box broadphaseBox = collider->GetSweptBoardphaseBox(collider->GetBox(), gameTime);
			if (collider->AABBCheck(broadphaseBox, (*i)->GetCollider()->GetBox()))
			{
				Box tempBox = (*i)->GetCollider()->GetBox();
				timeCollide = collider->SweptAABB(gameTime, collider->GetBox(), (*i)->GetCollider()->GetBox(), normalX, normalY);
				if ((timeCollide >= 0.0f && timeCollide < 1.0f))
				{
					if ((*i)->GetCollider()->GetTag() == 25)
					{
						isCollideWith25 = true;
						isCollideWith_25 = false;
						isCollideWith22 = false;
						isCollideWith_22 = false;					
					}
					else if ((*i)->GetCollider()->GetTag() == -25)
					{
						isCollideWith25 = false;
						isCollideWith_25 = true;
						isCollideWith22 = false;
						isCollideWith_22 = false;
					}
					stairCollidePos = (*i)->GetPosition();
					return;
				}
			}
			else
			{
				isCollideWith25 = false;
				isCollideWith_25 = false;
			}
		}
		else if ((*i)->GetCollider()->GetTag() == 22 || (*i)->GetCollider()->GetTag() == -22) // cham top
		{
			Box tempBox = (*i)->GetCollider()->GetBox();
			if (collider->AABBCheck(collider->GetBox(), (*i)->GetCollider()->GetBox()))
			{
				if (action == UP_STAIR)
					isOnStair = false;
				if ((*i)->GetCollider()->GetTag() == 22)
				{
					isCollideWith22 = true;
					isCollideWith_22 = false;
					isCollideWith25 = false;
					isCollideWith_25 = false;
				}
				else if ((*i)->GetCollider()->GetTag() == -22)
				{
					isCollideWith22 = false;
					isCollideWith_22 = true;
					isCollideWith25 = false;
					isCollideWith_25 = false;
				}
				stairCollidePos = (*i)->GetPosition();
				return;
			}
			Box broadphaseBox = collider->GetSweptBoardphaseBox(collider->GetBox(), gameTime);
			if (collider->AABBCheck(broadphaseBox, (*i)->GetCollider()->GetBox()))
			{
				Box tempBox = (*i)->GetCollider()->GetBox();
				timeCollide = collider->SweptAABB(gameTime, collider->GetBox(), (*i)->GetCollider()->GetBox(), normalX, normalY);
				if ((timeCollide >= 0.0f && timeCollide < 1.0f))
				{
					if (action == UP_STAIR)
						isOnStair = false;
					if ((*i)->GetCollider()->GetTag() == 22)
					{
						isCollideWith22 = true;
						isCollideWith_22 = false;
						isCollideWith25 = false;
						isCollideWith_25 = false;
					}
					else if ((*i)->GetCollider()->GetTag() == -22)
					{
						isCollideWith22 = false;
						isCollideWith_22 = true;
						isCollideWith25 = false;
						isCollideWith_25 = false;
					}
					stairCollidePos = (*i)->GetPosition();
					return;
				}
			}
			else
			{
				isCollideWith22 = false;
				isCollideWith_22 = false;
			}
		}
	}
}

void Simon::CheckCollideWithWall(float gameTime, std::vector<GameObject*>* listGameObject)
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
					if (normalX == 1 && !isOnStair)
					{
						sprite->SetPosition(collider->GetBox().left + gameTime * timeCollide * velocity.x - 15 + 0.1,
							sprite->GetPosition().y);
						return;
						
					}
					if (normalX == -1 && !isOnStair)
					{
						sprite->SetPosition(collider->GetBox().left + gameTime * timeCollide * velocity.x - 15 - 0.1,
							sprite->GetPosition().y);
						return;
					}
				}
			}
		}
	}
}

void Simon::CheckCollideWithEnemy(float gameTime, std::vector<GameObject*>* listGameObject)
{
	{
		float normalX = 0;
		float normalY = 0;
		float timeCollide;

		for (std::vector<GameObject*>::iterator i = listGameObject->begin(); i != listGameObject->end(); i++)
		{
			if ((((*i)->GetCollider()->GetTag() > 0 && (*i)->GetCollider()->GetTag() < 10) || (*i)->GetTag() == 2001 || (*i)->GetTag() == 2000 || (*i)->GetCollider()->GetTag() == -3 || (*i)->GetTag() == TAG_CRUSHER) && !(*i)->IsDead() && (*i)->IsEnable())
			{
				Box broadphaseBox = collider->GetSweptBoardphaseBox(collider->GetBox(), gameTime);
				if (collider->AABBCheck(collider->GetBox(), (*i)->GetCollider()->GetBox()))
				{
					if ((*i)->GetTag() == TAG_CRUSHER)
					{
						this->SetDead(true);
						hp = 0;
						isFighting = false;
						break;
					}
					
					if (hp > 2)
						hp -= 2;
					else
						hp = 0;

					if ((*i)->GetTag() == TAG_BAT)
						((Enemy*)(*i))->SetDead(true);

					isGrounded = false;

					if (!isOnStair)
					{
						action = HURTED;
						Hurted();
					}
					isFighting = false;
					break;
				}
				else if (collider->AABBCheck(broadphaseBox, (*i)->GetCollider()->GetBox()))
				{
					Box tempBox = (*i)->GetCollider()->GetBox();
					timeCollide = collider->SweptAABB(gameTime, collider->GetBox(), (*i)->GetCollider()->GetBox(), normalX, normalY);
					if ((timeCollide > 0.0f && timeCollide < 1.0f))
					{
						if ((*i)->GetTag() == TAG_CRUSHER)
						{
							this->SetDead(true);
							hp = 0;
							isFighting = false;
							break;
						}

						if (hp > 2)
							hp -= 2;
						else
							hp = 0;
						

						if ((*i)->GetTag() == TAG_BAT)
							((Enemy*)(*i))->SetDead(true);

						isGrounded = false;

						if (!isOnStair)
						{
							action = HURTED;
							Hurted();
						}
						isFighting = false;
						break;
					}
				}
			}
		}
	}

}

void Simon::CheckColliderWithMovableGround(float gameTime, std::vector<GameObject*>* listGameObject)
{
	float normalX = 0;
	float normalY = 0;
	float timeCollide;

	for (std::vector<GameObject*>::iterator i = listGameObject->begin(); i != listGameObject->end(); i++)
	{
		if ((*i)->GetCollider()->GetTag() == TAG_MOVEABLE_GROUND)
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
					if (normalY == 1)
					{
						if (action != SIT && action != HIT_SIT && action != JUMP)
							sprite->SetPosition(sprite->GetPosition().x + (*i)->GetVelocity().x * gameTime,
								collider->GetBox().top + gameTime * timeCollide * velocity.y + 0.1);
						else

							sprite->SetPosition(sprite->GetPosition().x + (*i)->GetVelocity().x * gameTime,
								collider->GetBox().top + gameTime * timeCollide * velocity.y + 0.1 + 16);
						velocity.y = 0;

						velocity.y = 0;
						isGrounded = true;
						return;
					}
				}
			}
		}
	}
}

bool Simon::IsCollideWith(float gameTime, GameObject * object)
{
	return Enemy::IsCollideWith(gameTime, object);
}

void Simon::CheckColliderWith(float gameTime, GameObject *object)
{
	float normalX = 0;
	float normalY = 0;
	float timeCollide;
	
	if (object->GetTag() == TAG_GROUND)
	{
		Box broadphaseBox = collider->GetSweptBoardphaseBox(collider->GetBox(), gameTime);
		if (collider->AABBCheck(broadphaseBox, object->GetCollider()->GetBox()))
		{
			Box tempBox = object->GetCollider()->GetBox();
			timeCollide = collider->SweptAABB(gameTime, collider->GetBox(), object->GetCollider()->GetBox(), normalX, normalY);
			if ((timeCollide >= 0.0f && timeCollide < 1.0f))
			{
				if (normalX == 1 && !isOnStair)
				{
					sprite->SetPosition(collider->GetBox().left + gameTime * timeCollide * velocity.x - 15,
						sprite->GetPosition().y);
				}
				if (normalX == -1 && !isOnStair)
				{
					sprite->SetPosition(collider->GetBox().left + gameTime * timeCollide * velocity.x - 15,
						sprite->GetPosition().y);
				}
			}
		}
	}

	if (object->GetTag() == 2000)
	{
		Box broadphaseBox = collider->GetSweptBoardphaseBox(collider->GetBox(), gameTime);
		if (collider->AABBCheck(collider->GetBox(), object->GetCollider()->GetBox()))
		{
			SetHP(GetHP() - 2);
			isGrounded = false;
			action = HURTED;
			Hurted();
			((Batman*)object)->isCollided = true;
		}
		else if (collider->AABBCheck(broadphaseBox, object->GetCollider()->GetBox()))
		{
			Box tempBox = object->GetCollider()->GetBox();
			timeCollide = collider->SweptAABB(gameTime, collider->GetBox(), object->GetCollider()->GetBox(), normalX, normalY);
			if ((timeCollide > 0.0f && timeCollide < 1.0f))
			{
				SetHP(GetHP() - 2);
				isGrounded = false;
				action = HURTED;
				Hurted();
				((Batman*)object)->isCollided = true;
			}
		}
	}
}

void Simon::WhipCheckCollider(float gameTime, std::vector<GameObject*>* listGameObject)
{
	if (isFighting && whip->GetCurrentFrameOfWhip() == 2)
	{
		if(typeOfWhip < 3)
			whip->SetBox(whip->GetPosition().x + (isLeft ? 50 : 145), whip->GetPosition().y - 22, 45, 5, 0, 0);
		else
			whip->SetBox(whip->GetPosition().x + (isLeft ? 20 : 140), whip->GetPosition().y - 22, 70, 5, 0, 0);
		whip->CheckCollider(gameTime, listGameObject);
	}
}

void Simon::CheckColliderWithDoor(float gameTime, std::vector<GameObject*>* listGameObject)
{
	float normalX = 0;
	float normalY = 0;
	float timeCollide;

	for (std::vector<GameObject*>::iterator i = listGameObject->begin(); i != listGameObject->end(); i++)
	{
		if ((*i)->GetCollider()->GetTag() == TAG_DOOR)
		{
			Box broadphaseBox = collider->GetSweptBoardphaseBox(collider->GetBox(), gameTime);
			if (collider->AABBCheck(broadphaseBox, (*i)->GetCollider()->GetBox()))
			{
				Box tempBox = (*i)->GetCollider()->GetBox();
				timeCollide = collider->SweptAABB(gameTime, collider->GetBox(), (*i)->GetCollider()->GetBox(), normalX, normalY);
				if ((timeCollide >= 0.0f && timeCollide < 1.0f))
				{
					collideDoor = (*i)->GetPosition();
					doorCollideDirection = normalX;
					door = (Door*)(*i);
				}
			}
		}
	}
}

void Simon::CollideWithDoorHandle(float gameTime, std::vector<GameObject*>* listGameObject, ViewPort* viewPort)
{
	CheckColliderWithDoor(gameTime, listGameObject);
	if (doorCollideDirection == -1 && !door->IsOccurred())
	{
		if (viewPort->GetCameraPosition().x + GAME_WIDTH / 2 < collideDoor.x)
		{
			isGoingThrowDoor = true;
			Stand();
			canControlKeyboard = false;
			viewPort->SetCameraPosition(viewPort->GetCameraPosition().x + 120 * gameTime,
				viewPort->GetCameraPosition().y);
		}
		else
		{
			door->isLeft = isLeft;
			door->SetSpriteXPosition(30);
			viewPort->SetCameraPosition(viewPort->GetCameraPosition().x,
				viewPort->GetCameraPosition().y);

			if (!door->IsDoAllFrame())
			{
				door->SetDrawable(true);
				door->UpdateAnim(gameTime);
				if (!Sound::GetInstance()->IsPLaying(OPEN_DOOR))
					Sound::GetInstance()->Play(OPEN_DOOR);
			}
			else
			{

				if (sprite->GetPosition().x < collideDoor.x + 110)
				{
					action = MOVE;
					SetPosition(GetPosition().x + 75 * gameTime, GetPosition().y);
				}
				else
				{
					Stand();
					door->SetAction(1);
					
					if (!door->IsDoAllFrame())
					{
						door->UpdateAnim(gameTime);
						if (!Sound::GetInstance()->IsPLaying(OPEN_DOOR))
							Sound::GetInstance()->Play(OPEN_DOOR);
					}
					else
					{
						door->SetDrawable(false);
						if (viewPort->GetCameraPosition().x < collideDoor.x + 16)
							viewPort->SetCameraPosition(viewPort->GetCameraPosition().x + 120 * gameTime,
								viewPort->GetCameraPosition().y);
						else
						{
							viewPort->SetCameraPosition(viewPort->GetCameraPosition().x,
								viewPort->GetCameraPosition().y);
							canControlKeyboard = true;
							doorCollideDirection = 0;
							door->SetIsOccurred(true);
							isGoingThrowDoor = false;
						}
					}
				}
			}
		}
	}
	else if (doorCollideDirection == 1 && !door->IsOccurred())
	{
		if (viewPort->GetCameraPosition().x + GAME_WIDTH / 2 > collideDoor.x)
		{
			isGoingThrowDoor = true;
			Stand();
			canControlKeyboard = false;
			viewPort->SetCameraPosition(viewPort->GetCameraPosition().x - 120 * gameTime,
				viewPort->GetCameraPosition().y);
		}
		else
		{
			door->isLeft = isLeft;
			door->SetSpriteXPosition(-30);
			viewPort->SetCameraPosition(viewPort->GetCameraPosition().x,
				viewPort->GetCameraPosition().y);
			
			if (!door->IsDoAllFrame())
			{
				door->UpdateAnim(gameTime);
				door->SetDrawable(true);
				if (!Sound::GetInstance()->IsPLaying(OPEN_DOOR))
					Sound::GetInstance()->Play(OPEN_DOOR);
			}
			else
			{
				if (sprite->GetPosition().x > collideDoor.x - 120)
				{
					action = MOVE;
					SetPosition(GetPosition().x - 75 * gameTime, GetPosition().y);
				}
				else
				{
					Stand();
					door->SetAction(1);
					
					if (!door->IsDoAllFrame())
					{
						door->UpdateAnim(gameTime);
						if (!Sound::GetInstance()->IsPLaying(OPEN_DOOR))
							Sound::GetInstance()->Play(OPEN_DOOR);
					}
					else
					{
						door->SetDrawable(false);
						if (viewPort->GetCameraPosition().x + GAME_WIDTH > collideDoor.x - 2)
							viewPort->SetCameraPosition(viewPort->GetCameraPosition().x - 120 * gameTime,
								viewPort->GetCameraPosition().y);
						else
						{
							viewPort->SetCameraPosition(viewPort->GetCameraPosition().x,
								viewPort->GetCameraPosition().y);
							canControlKeyboard = true;
							doorCollideDirection = 0;
							door->SetIsOccurred(true);
							isGoingThrowDoor = false;
						}
					}
				}
			}
		}
	}
}

bool Simon::IsColliderWithCheckPoint(float gameTime, std::vector<GameObject*>* listGameObject)
{
	float normalX = 0;
	float normalY = 0;
	float timeCollide;

	for (std::vector<GameObject*>::iterator i = listGameObject->begin(); i != listGameObject->end(); i++)
	{
		if ((*i)->GetCollider()->GetTag() == TAG_CHECK_POINT)
		{
			Box tempBox = (*i)->GetCollider()->GetBox();
			if (collider->AABBCheck(collider->GetBox(), (*i)->GetCollider()->GetBox()))
			{
				return true;
			}
			Box broadphaseBox = collider->GetSweptBoardphaseBox(collider->GetBox(), gameTime);
			if (collider->AABBCheck(broadphaseBox, (*i)->GetCollider()->GetBox()))
			{
				Box tempBox = (*i)->GetCollider()->GetBox();
				timeCollide = collider->SweptAABB(gameTime, collider->GetBox(), (*i)->GetCollider()->GetBox(), normalX, normalY);
				if ((timeCollide >= 0.0f && timeCollide < 1.0f))
				{
					return true;
				}
			}
		}
	}
	return false;
}

void Simon::CheckCollieWithTopStair(float gameTime, std::vector<GameObject*>* listGameObject)
{
	float normalX = 0;
	float normalY = 0;
	float timeCollide;

	for (std::vector<GameObject*>::iterator i = listGameObject->begin(); i != listGameObject->end(); i++)
	{
		if ((*i)->GetCollider()->GetTag() == TAG_STAIR_TOP || (*i)->GetCollider()->GetTag() == TAG_STAIR_TOP_)
		{
			Box tempBox = (*i)->GetCollider()->GetBox();
			if (collider->AABBCheck(collider->GetBox(), (*i)->GetCollider()->GetBox()))
			{
				/*if (normalY == 1)
					velocity.y = 0;*/
			}
			Box broadphaseBox = collider->GetSweptBoardphaseBox(collider->GetBox(), gameTime);
			if (collider->AABBCheck(broadphaseBox, (*i)->GetCollider()->GetBox()))
			{
				Box tempBox = (*i)->GetCollider()->GetBox();
				timeCollide = collider->SweptAABB(gameTime, collider->GetBox(), (*i)->GetCollider()->GetBox(), normalX, normalY);
				if ((timeCollide >= 0.0f && timeCollide < 1.0f)){}
					/*if (normalY == 1)
						velocity.y = 0;*/
			}
		}
	}
}

void Simon::ChangeSenceStairCheck(float gameTime, std::vector<GameObject*>* listGameObject, ViewPort *viewPort)
{
	float normalX = 0;
	float normalY = 0;
	float timeCollide;

	for (std::vector<GameObject*>::iterator i = listGameObject->begin(); i != listGameObject->end(); i++)
	{
		if ((*i)->GetCollider()->GetTag() == TAG_CHECK_STAIR)
		{
			Box broadphaseBox = collider->GetSweptBoardphaseBox(collider->GetBox(), gameTime);
			if (collider->AABBCheck(broadphaseBox, (*i)->GetCollider()->GetBox()))
			{
				Box tempBox = (*i)->GetCollider()->GetBox();
				timeCollide = collider->SweptAABB(gameTime, collider->GetBox(), (*i)->GetCollider()->GetBox(), normalX, normalY);
				if ((timeCollide >= 0.0f && timeCollide < 1.0f))
				{
					if (normalY == 1)
					{
						if (action == DOWN_STAIR)
						{
							if (velocity.x < 0)
							{
								sprite->SetPosition(sprite->GetPosition().x,
									sprite->GetPosition().y - 30);
								viewPort->SetCameraPosition(viewPort->GetCameraPosition().x,
									viewPort->GetCameraPosition().y - 186);
							}
							else if (velocity.x > 0)
							{
								sprite->SetPosition(sprite->GetPosition().x + 10,
									sprite->GetPosition().y - 30);
								viewPort->SetCameraPosition(viewPort->GetCameraPosition().x,
									viewPort->GetCameraPosition().y - 186);
							}
						}
					}
					else if (normalY == -1)
					{
						if (action == UP_STAIR)
						{
							if (velocity.x < 0)
							{
								velocity.x = 0;
								sprite->SetPosition(sprite->GetPosition().x - 28,
									sprite->GetPosition().y + 66);
								viewPort->SetCameraPosition(viewPort->GetCameraPosition().x,
									viewPort->GetCameraPosition().y + 373);
							}
							else if (velocity.x > 0)
							{
								velocity.x = 0;
								sprite->SetPosition(sprite->GetPosition().x,
									sprite->GetPosition().y + 66);
								viewPort->SetCameraPosition(viewPort->GetCameraPosition().x,
									viewPort->GetCameraPosition().y + 385);
							}
						}
					}
				}
			}
		}
	}
}

void Simon::SetTypeOfSubWeapon(int subType)
{
	for (int i = 0;i < 3;i++)
	{
		switch (subType)
		{
		case ITEM_AXE:
			subWeapon[i] = axe[i];
			currentWeapon = subType;
			break;
		case ITEM_CROSS:
			subWeapon[i] = cross[i];
			currentWeapon = subType;
			break;
		case ITEM_HOLY_WATER:
			subWeapon[i] = holyWater[i];
			currentWeapon = subType;
			break;
		case ITEM_KNIFE:
			subWeapon[i] = knife[i];
			currentWeapon = subType;
			break;
		case ITEM_STOP_WATCH:
			subWeapon[i] = stopWatch[i];
			currentWeapon = subType;
			break;
		default:
			break;
		}
	}
}