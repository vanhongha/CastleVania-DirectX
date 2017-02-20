#include "Batman.h"



Batman::Batman(int hp, int damage, int point) : Enemy(hp, damage, point)
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

void Batman::Reset()
{
	atSpot = true;
	aimable = true;

	hurtTime = HURT_TIME;
	isHurted = false;

	action = BATMAN_SLEEP;
	hp = hpC;
	sprite->SetPosition(positionC);
	Enemy::Setbox(0, 0, 0, 0);

	previousHP = hp;

	introTime = 5;
	chaseTime = CHASE_TIME;

	simonPos.x = -1; // Flag - meaning it's not initialized
	distance = -1; // Flag - meaning it's not initialized
}


Batman::~Batman()
{
	delete anim;
}

void Batman::InitAnim()
{
	anim = new Animation();
	anim->Initialize(sprite, width, height, 0.15);

	std::vector<D3DXVECTOR2> sleepAnimation;
	sleepAnimation.push_back(D3DXVECTOR2(0, 0));

	std::vector<D3DXVECTOR2> flyAnimation;
	flyAnimation.push_back(D3DXVECTOR2(1, 0));
	flyAnimation.push_back(D3DXVECTOR2(2, 0));

	std::vector<D3DXVECTOR2> hurtedAnimation;
	hurtedAnimation.push_back(D3DXVECTOR2(1, 0));

	anim->AddAnimation(BATMAN_SLEEP, sleepAnimation);
	anim->AddAnimation(BATMAN_FLY, flyAnimation);
	anim->AddAnimation(BATMAN_HURTED, hurtedAnimation);
}

bool Batman::Initialize(LPDIRECT3DDEVICE9 gDevice, const char* file, float x, float y, int tag)
{
	if (!Enemy::Initialize(gDevice, file, x, y, tag))
		return false;

	width = 100;
	height = 46;

	InitAnim();

	isLeft = true;

	velocity = { 125, 120, 0 };
	direction = { 0, 0 };

	this->y = y;

	action = BATMAN_SLEEP;
	previousHP = hp;

	introTime = 5;
	initialized = true;

	simonPos.x = -1; // Flag - meaning it's not initialized
	distance = -1; // Flag - meaning it's not initialized
}

void Batman::Aim(GameObject *object, float gameTime)
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

void Batman::Sleep()
{
	direction = { 0, 0 };
	action = BATMAN_SLEEP;
}

void Batman::Fly(float gameTime)
{
	// We create a new position that should happen if everything is fine
	D3DXVECTOR3 pos = sprite->GetPosition();
	pos.x += direction.x * velocity.x * gameTime;
	pos.y += direction.y * velocity.y * gameTime;
	//pos.y = 3 * (pos.x - positionC.x) * (pos.x - positionC.x) + positionC.y;

	// If the distance from pos to new spot > distance from before move to new pos
	// - which impossible in real life, that's mean the pos is move away from new pos
	if (std::sqrt(std::pow(simonPos.x - pos.x, 2) + std::pow(simonPos.y - pos.y, 2)) >= distance)
	{
		aimable = false;	// We aldready move to simon pos (maybe)
		pos.x = simonPos.x; // Then we set pos = simon pos
		pos.y = simonPos.y;
	}

	sprite->SetPosition(pos.x, pos.y); // So we setup (valid) new position

	action = BATMAN_FLY; // Batman's flying so action = fly
}

void Batman::Restrict(RECT* area)
{
	restriction = area;
	restriction->right -= width;
	restriction->bottom += height;
}

void Batman::Update(float gameTime)
{
	if (isEnable == true)  // If Batman exist
	{
		if (Intro(gameTime) == true) // If Batman's still in intro
			return;	// Then she do nothing

		CheckHPChange(); // Check for HP change

		if (isHurted == true) // Only happen when there's HP change
			CheckHurtTime(gameTime);
		// If Batman's hurted then she can't move and we can't hit her
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
				if (atSpot == false) // If Batman's not on random spot
					FlyToRandomSpot(gameTime); // She move to that spot
				else // atSpot == true ~ If she's already on random spot 
					RandomSpot(); // We random new spot for Batman to fly, atspot = false	
			}
		}

		Enemy::Setbox(0, 0, 0, 0);
		anim->DoAnimation(action);
		anim->Update(gameTime);
	}
	collideEffect->SetPosition(sprite->GetPosition().x, sprite->GetPosition().y - 20);
	collideEffect->Update(gameTime);
}

void Batman::Render(ViewPort *viewPort)
{
	if (isEnable == true)
		anim->Render(sprite, isLeft, viewPort);
	collideEffect->Render(viewPort);
}

bool Batman::Intro(float gameTime)
{
	if (introTime > 0) // If Batman just introduce itself, it can't hit or be hit
	{
		if (introTime > 2.5)
			action = BATMAN_SLEEP; // Sleep head ~ iron head
		else
			action = BATMAN_FLY; // Turn into a crazy head

		anim->DoAnimation(action);
		anim->Update(gameTime);
		introTime -= gameTime;
		return true;
	}
	else
		return false;
}

void Batman::CheckHPChange()
{
	if (previousHP != hp) // Check for HP change
	{
		if (hp <= 0) // If there's a change and HP <= 0
		{
			Sound::GetInstance()->Play(HIT_SOUND);
			isEnable = false; // Then disable Batman
			return; // Break the loop (Can't access still Batman enable - which is never)
		}
		else // If ther's a change but Batman's HP still on
		{
			isHurted = true; // Then Batman's only hurted
			previousHP = hp; // Set previousHP and HP equal to reset the change
							 //aimable = false;
			return; // Break the loop (Can access the loop but pass the check HP change - b/c they're equal now)
		}
	}
}

void Batman::CheckHurtTime(float gameTime)
{
	action = BATMAN_HURTED; // Hurted action for Batman
	hurtTime -= gameTime; // hurtTime = hurtTime - elapsed time 
	if (hurtTime <= 0) // If Batman's out of hurtTime
	{
		isHurted = false; // Then she's not hurt anymore
		hurtTime = HURT_TIME; // And we reset the time
		action = BATMAN_FLY; // She's no longer hurted, so we return her flying-action
	}
}

void Batman::FlyToRandomSpot(float gameTime)
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
		velocity = { 170, 170, 0 };
		chaseTime = CHASE_TIME;

		aimable = true;
		atSpot = true; // We aldready move to random spot, so atSpot = true
		pos.x = random.x; // Then we set pos = random pos
		pos.y = random.y;
	}

	sprite->SetPosition(pos.x, pos.y); // So we setup (valid) new position

	action = BATMAN_FLY; // Batman's flying so action = fly



}

void Batman::RandomSpot()
{
	// Random a new spot for Batman to fly to
	random.x = std::rand() % (restriction->right - restriction->left) + restriction->left;
	random.y = std::rand() % (restriction->top - restriction->bottom) + restriction->bottom;

	// Everytime we random spot, that mean Batman is default not on that's spot 
	// (even if practically luckily she is on spot, we stil consider as she's not)
	atSpot = false;
	waitTime = WAIT_TIME;
	velocity = { 125, 120, 0 };
}

