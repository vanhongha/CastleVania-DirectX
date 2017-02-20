#include "Medusa.h"



Medusa::Medusa(int hp, int damage, int point) : Enemy(hp, damage, point)
{
	std::srand(time(0));
	isCollided = false;
	atSpot = true;
	aimable = true;
	initialized = false;
	waitTime = 0;
	hurtTime = HURT_TIME;
	isHurted = false;
}


Medusa::~Medusa()
{
	delete anim;
}

void Medusa::InitAnim()
{
	anim = new Animation();
	anim->Initialize(sprite, width, height, 0.15);

	std::vector<D3DXVECTOR2> sleepAnimation;
	sleepAnimation.push_back(D3DXVECTOR2(4, 0));

	std::vector<D3DXVECTOR2> flyAnimation;
	flyAnimation.push_back(D3DXVECTOR2(0, 0));
	flyAnimation.push_back(D3DXVECTOR2(1, 0));
	flyAnimation.push_back(D3DXVECTOR2(2, 0));
	flyAnimation.push_back(D3DXVECTOR2(3, 0));

	std::vector<D3DXVECTOR2> hurtedAnimation;
	hurtedAnimation.push_back(D3DXVECTOR2(2, 0));
	hurtedAnimation.push_back(D3DXVECTOR2(3, 0));

	anim->AddAnimation(MEDUSA_SLEEP, sleepAnimation);
	anim->AddAnimation(MEDUSA_FLY, flyAnimation);
	anim->AddAnimation(MEDUSA_HURTED, hurtedAnimation);
}

bool Medusa::Initialize(LPDIRECT3DDEVICE9 gDevice, const char* file, float x, float y, int tag)
{
	if (!Enemy::Initialize(gDevice, file, x, y, tag))
		return false;

	width = 65;
	height = 64;

	InitAnim();

	isLeft = true;

	velocity = { 100, 100, 0 };
	direction = { 0, 0 };

	snake_1 = new Snake(100, 2, 1);
	snake_2 = new Snake(100, 2, 1);

	snake_1->Initialize(gDevice, SNAKE_SPRITE, 30, 300, TAG_ZOMBIE);
	snake_2->Initialize(gDevice, SNAKE_SPRITE, 100, 300, TAG_ZOMBIE);

	snake_1->SetEnable(false);
	snake_2->SetEnable(false);

	this->y = y;

	action = MEDUSA_SLEEP;
	previousHP = hp;

	introTime = 5;
	chaseTime = CHASE_TIME;
	respawnSnakeTime = RESPAWN_TIME;
	initialized = true;

	simonPos.x = -1; // Flag - meaning it's not initialized
	distance = -1; // Flag - meaning it's not initialized
}

void Medusa::Reset()
{

	atSpot = true;
	aimable = true;

	hurtTime = HURT_TIME;
	isHurted = false;

	action = MEDUSA_SLEEP;
	hp = hpC;
	sprite->SetPosition(positionC);
	Enemy::Setbox(0, 0, 0, 0);

	respawnSnakeTime = RESPAWN_TIME;
	chaseTime = CHASE_TIME;

	snake_1->SetEnable(false);
	snake_1->SetPosition(D3DXVECTOR3(-100, 0, 0));
	snake_1->Setbox(0, 0, 0, 0);

	snake_2->SetEnable(false);
	snake_2->SetPosition(D3DXVECTOR3(-100, 0, 0));
	snake_2->Setbox(0, 0, 0, 0);

	previousHP = hp;

	introTime = 5;

	simonPos.x = -1; // Flag - meaning it's not initialized
	distance = -1; // Flag - meaning it's not initialized

}

void Medusa::Aim(GameObject *object, float gameTime)
{
	// Usually check once
	if (simonPos.x < 0) {
		simonPos = { object->GetPosition().x, object->GetPosition().y };
	}
	else if (chaseTime > 0) {
		chaseTime -= gameTime;
		simonPos = { object->GetPosition().x, object->GetPosition().y };
	}

	if (aimable == true)
	{
		distance = std::sqrt(
			std::pow(simonPos.x - GetPosition().x, 2) +
			std::pow(simonPos.y - GetPosition().y, 2));

		direction.x = (simonPos.x - GetPosition().x) / distance;
		direction.y = (simonPos.y - GetPosition().y) / distance;
	}
}

void Medusa::Sleep()
{
	direction = { 0, 0 };
	action = MEDUSA_SLEEP;
}

void Medusa::Fly(float gameTime)
{
	// We create a new position that should happen if everything is fine
	D3DXVECTOR3 pos = sprite->GetPosition();
	pos.x += direction.x * velocity.x * gameTime;
	pos.y += direction.y * velocity.y * gameTime;;

	// If the distance from pos to new spot > distance from before move to new pos
	// - which impossible in real life, that's mean the pos is move away from new pos
	if (std::sqrt(std::pow(simonPos.x - pos.x, 2) + std::pow(simonPos.y - pos.y, 2)) >= distance)
	{
		aimable = false;	// We aldready move to simon pos (maybe)
		pos.x = simonPos.x; // Then we set pos = simon pos
		pos.y = simonPos.y;
	}

	sprite->SetPosition(pos.x, pos.y); // So we setup (valid) new position

	action = MEDUSA_FLY; // Medusa's flying so action = fly
}

void Medusa::Restrict(RECT* area)
{
	restriction = area;
	restriction->right -= width;
	restriction->bottom += height;
}

void Medusa::Update(float gameTime)
{
	if (isEnable == true)  // If medusa exist
	{
		if (Intro(gameTime) == true) // If medusa's still in intro
			return;	// Then she do nothing

		CheckHPChange(); // Check for HP change

		if (isHurted == true) // Only happen when there's HP change
			CheckHurtTime(gameTime);
		// If Medusa's hurted then she can't move and we can't hit her
		// Also there's an anim she's hurted

		else // If she's not hurted ~ there's no HP change
		{
			if (aimable == true) {
				if (waitTime > 0)
					waitTime -= gameTime;
				else
					Fly(gameTime);
			}
			else {
				if (atSpot == false) // If Medusa's not on random spot
					FlyToRandomSpot(gameTime); // She move to that spot
				else // atSpot == true ~ If she's already on random spot 
					RandomSpot(); // We random new spot for medusa to fly, atspot = false	
			}

			CreateChildren(2, gameTime);
		}

		Enemy::Setbox(0, 0, 0, 0);
		anim->DoAnimation(action);
		anim->Update(gameTime);
	}
	collideEffect->SetPosition(sprite->GetPosition().x, sprite->GetPosition().y - 20);
	collideEffect->Update(gameTime);
}

void Medusa::Render(ViewPort *viewPort)
{
	if (isEnable == true)
		anim->Render(sprite, isLeft, viewPort);
	collideEffect->Render(viewPort);
}

bool Medusa::Intro(float gameTime)
{
	if (introTime > 0) // If medusa just introduce itself, it can't hit or be hit
	{
		if (introTime > 2.5)
			action = MEDUSA_SLEEP; // Sleep head ~ iron head
		else
			action = MEDUSA_HURTED; // Turn into a crazy head

		anim->DoAnimation(action);
		anim->Update(gameTime);
		introTime -= gameTime;
		return true;
	}
	else
		return false;
}

void Medusa::CheckHPChange()
{
	if (previousHP != hp) // Check for HP change
	{
		if (hp <= 0) // If there's a change and HP <= 0
		{
			Sound::GetInstance()->Play(HIT_SOUND);
			isEnable = false; // Then disable Medusa
			return; // Break the loop (Can't access still Medusa enable - which is never)
		}
		else // If ther's a change but Medusa's HP still on
		{
			isHurted = true; // Then Medusa's only hurted
			previousHP = hp; // Set previousHP and HP equal to reset the change
			return; // Break the loop (Can access the loop but pass the check HP change - b/c they're equal now)
		}
	}
}

void Medusa::CheckHurtTime(float gameTime)
{
	action = MEDUSA_HURTED; // Hurted action for medusa
	hurtTime -= gameTime; // hurtTime = hurtTime - elapsed time 
	if (hurtTime <= 0) // If Medusa's out of hurtTime
	{
		isHurted = false; // Then she's not hurt anymore
		hurtTime = HURT_TIME; // And we reset the time
		action = MEDUSA_FLY; // She's no longer hurted, so we return her flying-action
	}
}

void Medusa::FlyToRandomSpot(float gameTime)
{
	// Calculate the distance from current position to the new spot
	distance = std::sqrt(
		std::pow(random.x - GetPosition().x, 2) +
		std::pow(random.y - GetPosition().y, 2));

	// We get direction vector here
	direction.x = (random.x - GetPosition().x) / distance;
	direction.y = (random.y - GetPosition().y) / distance;

	// We create a new position that should happen if everything is fine
	D3DXVECTOR3 pos = sprite->GetPosition();
	pos.x += direction.x * velocity.x * gameTime;
	pos.y += direction.y * velocity.y * gameTime;;

	// If the distance from pos to new spot > distance from before move to new pos
	// - which impossible in real life, that's mean the pos is move away from new pos
	if (std::sqrt(std::pow(random.x - pos.x, 2) + std::pow(random.y - pos.y, 2)) >= distance)
	{
		chaseTime = CHASE_TIME;
		aimable = true;
		atSpot = true; // We aldready move to random spot, so atSpot = true
		pos.x = random.x; // Then we set pos = random pos
		pos.y = random.y;
	}

	sprite->SetPosition(pos.x, pos.y); // So we setup (valid) new position

	action = MEDUSA_FLY; // Medusa's flying so action = fly
}

void Medusa::RandomSpot()
{
	// Random a new spot for Medusa to fly to
	random.x = std::rand() % (restriction->right - restriction->left) + restriction->left;
	random.y = std::rand() % (restriction->top - restriction->bottom) + restriction->bottom;

	// Everytime we random spot, that mean Medusa is default not on that's spot 
	// (even if practically luckily she is on spot, we stil consider as she's not)
	waitTime = WAIT_TIME;
	atSpot = false;
}

void Medusa::CreateChildren(int number, float gameTime)
{
	if (snake_1->IsEnable() == false && snake_2->IsEnable() == false)
	{
		snake_1->SetEnable(true);
		snake_1->SetPosition(sprite->GetPosition().x, sprite->GetPosition().y);
		snake_1->SetHP(2);

		snake_2->SetEnable(true);
		snake_2->SetPosition(sprite->GetPosition().x, sprite->GetPosition().y);
		snake_2->SetIsLeft(false);
		snake_2->SetHP(2);
	}
}

