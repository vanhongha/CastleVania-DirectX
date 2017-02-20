#include "GamePlayStateTwo.h"



GamePlayStateTwo::GamePlayStateTwo()
{
}


GamePlayStateTwo::~GamePlayStateTwo()
{
}

bool GamePlayStateTwo::Initialize(Graphics *graphics)
{
	this->graphics = graphics;
	this->gDevice = graphics->GetDevice();

	timeKillAll = 0;
	choose = false;

	simon->SetCanControlKeyboard(true);

	viewPort = new ViewPort(0, GAME_HEIGHT + 370);

	sprite = new Sprite();
	if (!sprite->Initialize(gDevice, LEVEL_TWO_BG))
		return false;

	tileMap = new TileMap(5632, 768, sprite, 64, 64);
	tileMap->LoadListTileFromFile(LEVEL_TWO_MATRIX_BG);

	cameraObject = new GameObject();
	cameraObject->Initialize(gDevice, SIMON_SPRITE, viewPort->GetCameraPosition().x, viewPort->GetCameraPosition().y, 5);

	leftCamera = new Ground(2, 450);
	leftCamera->Initialize(gDevice, "Resource\\sprites\\Ground\\0.png", viewPort->GetCameraPosition().x - 2, viewPort->GetCameraPosition().y, TAG_GROUND);

	rightCamera = new Ground(2, 450);
	rightCamera->Initialize(gDevice, "Resource\\sprites\\Ground\\0.png", viewPort->GetCameraPosition().x + GAME_WIDTH, viewPort->GetCameraPosition().y, TAG_GROUND);

	quadTree = new QuadTree(gDevice, viewPort);
	quadTree->Load("Resource\\sprites\\QuadTree\\level2.txt");

	list = new std::vector<GameObject*>();

	//simon = staticSimon; 5, 600
	simonIsFallIntoWater = false;
	simon->SetPosition(5, 600);
	simon->Reload();

	#pragma region Init enemy 

	listEnemy = new std::vector<GameObject*>();
	InitZombie(gDevice);
	InitPanther(gDevice);
	InitBat(gDevice);
	InitMerman(gDevice);

	#pragma endregion

	#pragma region Init water effect

	for (int i = 0; i < 3; i++)
	{
		waterEffect[i] = new WaterAnimation();
		waterEffect[i]->Initialzie(gDevice, 0, 0);
		waterEffect[i]->SetEnable(false);
	}

	#pragma endregion

	RECT *area = new RECT();
	*area = { (LONG)viewPort->GetCameraPosition().x + 5600 - GAME_WIDTH, (LONG)viewPort->GetCameraPosition().y - 116, (LONG)viewPort->GetCameraPosition().x + 5600, (LONG)viewPort->GetCameraPosition().y - 240 };

	batman = new Batman(32, 2, 3000);
	batman->Initialize(gDevice, BATMAN_SPRITE, 5632 - GAME_WIDTH / 2 - 50, 768 - 50, 2000); // Tag ???
	batman->Restrict(area);

	changeState = false;
	inBoss = false;

	Item::TakeSimonPointer(simon);
	Item::TakeListEnemyPointer(listEnemy);
	listItem = new std::vector<Item*>();


	ui = new UI();
	ui->Initialize(gDevice, simon, batman->GetHP());
	time = 500;
	score = 0;

	return true;
}

void GamePlayStateTwo::Update(float gameTime)
{
	GameState::Update(gameTime);

	time -= gameTime;
	ui->Update(batman->GetHP(), (int)time, 3, 1);

	#pragma region Init cameraObject, left camera, right camera

	cameraObject->SetPosition(viewPort->GetCameraPosition());
	cameraObject->SetRegion(0, GAME_WIDTH, 0, -GAME_HEIGHT);

	//khong cho simon di ra khoi camera
	leftCamera->SetPosition(viewPort->GetCameraPosition().x - 2, viewPort->GetCameraPosition().y);
	leftCamera->SetBox(leftCamera->GetPosition().x, leftCamera->GetPosition().y, 2, GAME_HEIGHT, 0, 0);

	rightCamera->SetPosition(viewPort->GetCameraPosition().x + GAME_WIDTH, viewPort->GetCameraPosition().y);
	rightCamera->SetBox(rightCamera->GetPosition().x, rightCamera->GetPosition().y, 2, GAME_HEIGHT, 0, 0);

#pragma endregion

	//////////////////////////////////--------- LIST ITEM ---------///////////////////////////////////////
	// * list = list of object, include [ground, candle, ...]
	// * listItem = list of drop item

	list->clear();
	quadTree->GetObjectList(list, cameraObject->GetRegion());

	if (inBoss == true)
	{
		if (batman->IsEnable() == true)
		{
			batman->Aim(simon, gameTime);
			/*batman->Update(gameTime);*/
			list->push_back(batman);
		}
		else
		{
			if (batman->isDropItem == false)
			{
				Item *ball = new Item();
				ball->Initialize(gDevice, batman->GetPosition().x, batman->GetPosition().y, ITEM_BALL, 1000);
				listItem->push_back(ball);
				batman->isDropItem = true;
			}
		}
	}

	for (auto i = list->begin(); i != list->end(); i++)
	{
		if (!(*i)->IsEnable())
		{
			// Drop item from hitting object
			if ((*i)->isDropItem == false)
			{
				Item *item = new Item();
				item->Initialize(gDevice, (*i)->GetPosition().x, (*i)->GetPosition().y);
				//item->Initialize(gDevice, (*i)->GetPosition().x, (*i)->GetPosition().y, ITEM_ROSARY, 10);
				listItem->push_back(item);
				(*i)->isDropItem = true;
			}
			quadTree->Remove((*i));
		}
		else
		{
			if ((*i)->GetTag() == TAG_DOOR && (*i)->GetID() == 1256)
				((Door*)(*i))->SetIsOccurred(true);
			(*i)->Update(gameTime);
		}
	}

	for (auto i = listItem->begin(); i != listItem->end(); i++)
	{
		(*i)->Update(gameTime);
		(*i)->CheckCollider(gameTime, list);
	}

	/*---------------------------------------  END LIST ITEM  ------------------------------------------*/


	listEnemy->clear();

	#pragma region Check if simon is going throw the door dont let any object update

	if (!simon->IsGoingThrowDoor())
	{
		UpdateZombie(gameTime);
		UpdatePanther(gameTime);
		UpdateBat(gameTime);
		UpdateMerman(gameTime);
		UpdateWaterEffect(gameTime);
	}

#pragma endregion

	CheckPointHandle(gameTime, simon, list);

	simon->KeyBoardHandle(gameTime);
	simon->Update(gameTime);

	simon->ChangeSenceStairCheck(gameTime, list, viewPort);
	CameraFollowHandle(gameTime);

	simon->CheckCollider(gameTime, list);
	simon->CheckColliderWith(gameTime, leftCamera);
	simon->CheckColliderWith(gameTime, rightCamera);
	simon->CollideWithDoorHandle(gameTime, list, viewPort);

	for (int i = 0; i < simon->GetNoSubWeapon();i++)
	{
		if (simon->subWeapon[i]->GetTag() == TAG_CROSS && !simon->subWeapon[i]->IsEnable())
			((Cross*)(simon->subWeapon[i]))->SetDefaultProperties();
		
		simon->subWeapon[i]->CheckCollider(gameTime, list);

		if (simon->subWeapon[i]->GetPosition().x < viewPort->GetCameraPosition().x ||
			simon->subWeapon[i]->GetPosition().x > viewPort->GetCameraPosition().x + GAME_WIDTH)
			simon->subWeapon[i]->SetEnable(false);

		if (simon->subWeapon[i]->GetTag() == TAG_CROSS)
		{
			((Cross*)(simon->subWeapon[i]))->CheckTurnBack(gameTime, viewPort);
			if (simon->subWeapon[i]->CheckColliderWith(gameTime, simon) && ((Cross*)(simon->subWeapon[i]))->IsTurnBack())
				simon->subWeapon[i]->SetEnable(false);
		}
		
	}

	if (time < 0)
	{
		simon->SetEnable(false);
		time = 500;
	}

	if (simon->GetHP() <= 0 && inBoss) {
		inBoss = false;
		batman->Reset();
		if (Sound::GetInstance()->IsPLaying(BOSS_BATTLE_POISON_MIND))
			Sound::GetInstance()->Stop(BOSS_BATTLE_POISON_MIND);
	}

#pragma region Simon falling into water effect handler
	/*Check if simon fall into water*/
	/*Water effect will be updated*/

	if (simon->GetPosition().y <= 150)
	{
		if (!waterEffect[2]->IsEnable())
			simonIsFallIntoWater = true;
		waterEffect[2]->SetEnable(true);
	}
	if (simonIsFallIntoWater)
	{
		waterEffect[2]->SetPosition(simon->GetPosition().x, 100);
		waterEffect[2]->SetDefaultForce();
		simonIsFallIntoWater = false;
		Sound::GetInstance()->Play(eSoundID::FALLING_DOWN_WATER_SUFACE);
	}

	/******************************/
#pragma endregion

	if (simon->GetPosition().y < viewPort->GetCameraPosition().y - GAME_HEIGHT)
		simon->SetHP(0);

	if (IsKeyPress(DIK_M))
		SetChangingState(true);
}

	#pragma region Init enemies

void GamePlayStateTwo::InitZombie(LPDIRECT3DDEVICE9 gDevice)
{
	//Init zombie
	for (int i = 0; i < 3; i++)
	{
		zombies[i] = new Zombie(1, 2, 100);
		zombies[i]->Initialize(gDevice, ZOMBIE_SPRITE, GAME_WIDTH + (i + 1) * 100, 510, TAG_ZOMBIE);
		zombies[i]->SetEnable(true);
	}
}

void GamePlayStateTwo::InitPanther(LPDIRECT3DDEVICE9 gDevice)
{
	//Init panther
	for (int i = 0; i < 3; i++)
	{
		panther[i] = new Panther(1, 2, 150);
		panther[i]->Initialize(gDevice, PANTHER_SPRITE, 1400 + i * 300, 800, TAG_PANTHER);
		panther[i]->SetEnable(true);
	}

	//Set lai position cho panther
	panther[0]->SetPosition(1700, 646);
	panther[1]->SetPosition(1400, 578);
	panther[2]->SetPosition(1900, 578);
}

void GamePlayStateTwo::InitBat(LPDIRECT3DDEVICE9 gDevice)
{
	//Init bat
	bat = new Bat(1, 2, 100);
	bat->Initialize(gDevice, BAT_SPRITE, 0, 0, TAG_BAT);
	bat->SetEnable(false);
}

void GamePlayStateTwo::InitMerman(LPDIRECT3DDEVICE9 gDevice)
{
	//Init merman
	for (int i = 0; i < 2; i++)
	{
		merman[i] = new Merman(1, 2, 100);
		merman[i]->Initialize(gDevice, MERMAN_SPRITE, 3100 + rand() % 810, 200, TAG_MERMAN);
		merman[i]->SetEnable(false);
	}
}

	#pragma endregion

	#pragma region Update enemies

	void GamePlayStateTwo::UpdateZombie(float gameTime)
	{
		//neu nhu simon di vao vung hoat dong cua zombie
		if ((simon->GetPosition().x > 0 && simon->GetPosition().x < 890)
			|| (simon->GetPosition().x > 2220 && simon->GetPosition().x < 2950)
			|| (simon->GetPosition().x > 4125 && simon->GetPosition().x < 4950))
		{
			for (int i = 0; i < 3; i++)
			{
				//neu zombie disable va thoi gian ngu cua no lon hon 1
				if (!zombies[i]->IsEnable() && zombies[i]->respawnTime > 1.0f)
				{
					//neu simon nam trong vai vung dac biet thi cho no di chuyen tu trai qua
					if ((simon->GetPosition().x > 0 && simon->GetPosition().x < 630)
						|| (simon->GetPosition().x > 2220 && simon->GetPosition().x < 2700)
						|| (simon->GetPosition().x > 4125 && simon->GetPosition().x < 4870))
					{
						zombies[i]->SetPosition(viewPort->GetCameraPosition().x + GAME_WIDTH - 1, simon->GetPosition().y);
						zombies[i]->SetIsLeft(true);
					}
					else
					{
						zombies[i]->SetPosition(viewPort->GetCameraPosition().x + 1, simon->GetPosition().y);
						zombies[i]->SetIsLeft(false);
					}

					zombies[i]->SetEnable(true);
					zombies[i]->Respawn();

					//doi thoi gian cua cac zombie sau con zombie trc 1 thoi khoang thoi gian
					if (i == 0)
					{
						zombies[1]->respawnTime = 0.5f;
						zombies[2]->respawnTime = 0.5f;
					}

					if (i == 1)
						zombies[2]->respawnTime = 0.5f;
				}
			}
		}

		for (int i = 0; i < 3; i++)
		{
			//neu zombie khong nam trong camera thi disable
			if (!IsInCamera(zombies[i]->GetPosition().x, zombies[i]->GetPosition().y))
				zombies[i]->SetEnable(false);

			zombies[i]->Update(gameTime);
			zombies[i]->CheckCollider(gameTime, list);

			//neu zombie enable thi update va dua vao list
			if (zombies[i]->IsEnable())
			{
				list->push_back(zombies[i]);
				listEnemy->push_back(zombies[i]);
			}
		}
	}

	void GamePlayStateTwo::UpdatePanther(float gameTime)
	{
		//update and check collision for panther and add panther to list collision
		for (int i = 0; i < sizeof(panther) / sizeof(*panther); i++)
		{
			//neu nhu panther di ra khoi camera va simon dang trong vung hoat dong cua no
			//thi enable = false
			if (!IsInCamera(panther[i]->GetPosition().x, panther[i]->GetPosition().y)
				&& (simon->GetPosition().x < 2100 && simon->GetPosition().x > 1100) && panther[i]->GetAction() != SIT)
			{
				panther[i]->SetEnable(false);
			}

			//neu camera o ngoai vung hoat dong cua no thi dua no ve vi tri cu va enable = true
			if (viewPort->GetCameraPosition().x > 2100 || viewPort->GetCameraPosition().x < 740)
			{
				panther[i]->SetEnable(true);
				panther[i]->SetHP(1);
				panther[i]->SetAction(SIT);
				if (i == 0)
					panther[0]->SetPosition(1700, 646);
				else if (i == 1)
					panther[1]->SetPosition(1400, 578);
				else
					panther[2]->SetPosition(1900, 578);
				//panther[i]->Respawn();
				panther[i]->SetIsLeft(!simon->GetIsLeft());
			}

			//neu simon di lai gan panther thi cho panther di chuyen
			if ((abs(simon->GetPosition().x - panther[i]->GetPosition().x) < 120) && panther[i]->GetAction() == SIT)
			{
				panther[i]->SetAction(MOVE);
			}

			//neu panther nam trong camera va no enable thi update va dua vao list de check collider
			if (IsInCamera(panther[i]->GetPosition().x, panther[i]->GetPosition().y) && panther[i]->IsEnable())
			{
				panther[i]->Update(gameTime);
				panther[i]->CheckCollider(gameTime, list);
				list->push_back(panther[i]);
				listEnemy->push_back(panther[i]);
			}
		}
	}

	void GamePlayStateTwo::UpdateBat(float gameTime)
	{
		if (!IsInCamera(bat->GetPosition().x, bat->GetPosition().y))
			bat->SetEnable(false);

		//neu simon di vao vung hoat dong cua bat
		if (simon->GetPosition().x > 3185 && simon->GetPosition().x < 4000
			&& simon->GetPosition().y > 423)
		{
			if (!bat->IsEnable() && bat->respawnTime > 3)
			{
				if (simon->GetPosition().x > 3185 && simon->GetPosition().x < 3270)
				{
					bat->SetPosition(viewPort->GetCameraPosition().x + GAME_WIDTH - 1, simon->GetPosition().y);
					bat->SetIsLeft(true);
				}
				else if (simon->GetPosition().x > 3950 && simon->GetPosition().x < 4000)
				{
					bat->SetPosition(viewPort->GetCameraPosition().x + 1, simon->GetPosition().y);
					bat->SetIsLeft(false);
				}
				else if (simon->GetIsLeft())
				{
					bat->SetPosition(viewPort->GetCameraPosition().x + 1, simon->GetPosition().y);
					bat->SetIsLeft(false);
				}
				else
				{
					bat->SetPosition(viewPort->GetCameraPosition().x + GAME_WIDTH - 1, simon->GetPosition().y);
					bat->SetIsLeft(true);
				}
				bat->SetY(simon->GetPosition().y);
				/*bat->SetHP(1);
				bat->SetEnable(true);*/
				bat->Respawn();
			}
		}

		bat->Update(gameTime);

		if (bat->IsEnable())
		{
			list->push_back(bat);
			listEnemy->push_back(bat);
		}
	}

	void GamePlayStateTwo::UpdateMerman(float gameTime)
	{
		//update and check collision for merman and add merman to list collision
		for (int i = 0; i < 2; i++)
		{
			//neu merman rot xuong nuoc thi cho no disable
			if (merman[i]->GetPosition().y < 100 && merman[i]->IsEnable())
			{
				waterEffect[i]->SetEnable(merman[i]->IsEnable());
				if (waterEffect[i]->IsEnable())
				{
					waterEffect[i]->SetDefaultForce();
					waterEffect[i]->SetPosition(merman[i]->GetPosition().x, merman[i]->GetPosition().y - 30);
					Sound::GetInstance()->Play(eSoundID::FALLING_DOWN_WATER_SUFACE);
				}
				merman[i]->SetEnable(false);
			}

			//neu merman ra khoi camera thi set enable = false
			if(!IsInCamera(merman[i]->GetPosition().x, merman[i]->GetPosition().y))
				merman[i]->SetEnable(false);

			merman[i]->Update(gameTime);
			merman[i]->CheckCollider(gameTime, list);

			if (merman[i]->IsEnable())
			{
				list->push_back(merman[i]);
				listEnemy->push_back(merman[i]);
			}
			else
			{
				if (merman[i]->respawnTime >= 4
					&& (simon->GetPosition().x > 3080 && simon->GetPosition().x < 4080
						&& simon->GetPosition().y > 0 && simon->GetPosition().y < 330))
				{
					//random lai toa do cho merman
					merman[i]->SetPosition(viewPort->GetCameraPosition().x + rand() % 641, 100);
					//neu toa do x cua merman khac voi simon thi moi cho enable cach xa simon khoang 50 moi duoc
					if (merman[i]->GetPosition().x + 34 < simon->GetPosition().x - 50
						|| merman[i]->GetPosition().x > simon->GetPosition().x + 60 + 50)
					{
						merman[i]->SetJump();
						merman[i]->SetEnable(true);
						//merman[i]->SetHP(1);
						merman[i]->Respawn();
						waterEffect[i]->SetEnable(merman[i]->IsEnable());
						if (waterEffect[i]->IsEnable())
						{
							waterEffect[i]->SetDefaultForce();
							waterEffect[i]->SetPosition(merman[i]->GetPosition().x, merman[i]->GetPosition().y - 30);
							Sound::GetInstance()->Play(eSoundID::SLASH_WATER);
						}
					}
				}
			}

			//neu vien dan ra khoi camera thi disable
			if (merman[i]->GetProjectile()->GetPosition().x < viewPort->GetCameraPosition().x ||
				merman[i]->GetProjectile()->GetPosition().x > viewPort->GetCameraPosition().x + GAME_WIDTH)
				merman[i]->GetProjectile()->SetEnable(false);

			//neu vien dan enable thi dua no vao list kiem tra va cham
			if (merman[i]->GetProjectile()->IsEnable())
			{
				merman[i]->GetProjectile()->Update(gameTime);
				list->push_back(merman[i]->GetProjectile());
				listEnemy->push_back(merman[i]->GetProjectile());
			}
		}
	}

	#pragma endregion

void GamePlayStateTwo::UpdateWaterEffect(float gameTime)
{
	for (int i = 0; i < 3; i++)
	{
		if (waterEffect[i]->IsEnable())
			waterEffect[i]->Update(gameTime);
	}
}

void GamePlayStateTwo::Render()
{
	tileMap->Draw(viewPort);
	for (auto i = list->begin(); i != list->end(); i++)
	{
		(*i)->isAdded = false;
		(*i)->Render(viewPort);
	}
	for (auto i = listItem->begin(); i != listItem->end(); i++)
	{
		(*i)->Render(viewPort);
	}

	for (int i = 0; i < 3; i++)
		if (waterEffect[i]->IsEnable())
			waterEffect[i]->Render(viewPort);

	simon->Render(viewPort);
	ui->Render();
}

bool GamePlayStateTwo::GetChangingState()
{
	return GameState::GetChangingState();
}

void GamePlayStateTwo::SetChangingState(bool status)
{
	GameState::SetChangingState(status);
}

	#pragma region camera handle

bool GamePlayStateTwo::CameraFollowHandle(float gameTime)
{
	if (simon->nextStage)
	{
		SetChangingState(true);
		simon->nextStage = false;
	}
	
	if (simon->GetPosition().x >= GAME_WIDTH / 2 && simon->GetPosition().x < 3078 - GAME_WIDTH / 2)
	{
		D3DXVECTOR3 currentCamera = viewPort->GetCameraPosition();
		currentCamera.x = simon->GetPosition().x - GAME_WIDTH / 2;
		viewPort->SetCameraPosition(currentCamera.x, currentCamera.y);
	}
	else if (simon->GetPosition().x >= 3078 && simon->GetPosition().x < 4096 - GAME_WIDTH / 2)
	{
		D3DXVECTOR3 currentCamera = viewPort->GetCameraPosition();
		if (simon->GetPosition().x >= 3078 + GAME_WIDTH / 2 && simon->GetPosition().x <= 4096 - GAME_WIDTH / 2)
			currentCamera.x = simon->GetPosition().x - GAME_WIDTH / 2;
		viewPort->SetCameraPosition(currentCamera.x, currentCamera.y);
	}
	else if (simon->GetPosition().x >= 4096 && simon->GetPosition().x <= 5662 - GAME_WIDTH / 2)
	{
		{
			if (viewPort->GetCameraPosition().x + GAME_WIDTH < 5662 - 32)
			{
				if (simon->GetPosition().x >= 4096 + GAME_WIDTH / 2 && simon->GetPosition().x <= 5662 - GAME_WIDTH / 2)
				{
					D3DXVECTOR3 currentCamera = viewPort->GetCameraPosition();
					currentCamera.x = simon->GetPosition().x - GAME_WIDTH / 2;
					viewPort->SetCameraPosition(currentCamera.x, currentCamera.y);
				}
			}
			else
			{
				// Do something here
				inBoss = true;
				Sound::GetInstance()->Stop(STAGE_01_VAMPIRE_KILLER);
				if (!Sound::GetInstance()->IsPLaying(BOSS_BATTLE_POISON_MIND) && simon->CanControllKeyboard())
					Sound::GetInstance()->PlayLoop(BOSS_BATTLE_POISON_MIND);
			}
		}
	}


	return false;
}

	bool GamePlayStateTwo::IsInCamera(float x, float y)
	{
		return (x > viewPort->GetCameraPosition().x
			&& x < viewPort->GetCameraPosition().x + GAME_WIDTH
			&& y < viewPort->GetCameraPosition().y
			&& y > viewPort->GetCameraPosition().y - GAME_HEIGHT);
	}

	#pragma endregion

void GamePlayStateTwo::DestroyAll()
{
	//delete(simon);
	delete(tileMap);
	delete(ground);
	delete(cameraObject);
	delete(listObject);
	delete(viewPort);
	delete(listItem);
	delete(list);
}