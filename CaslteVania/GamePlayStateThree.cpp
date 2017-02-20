#include "GamePlayStateThree.h"



GamePlayStateThree::GamePlayStateThree()
{
}


GamePlayStateThree::~GamePlayStateThree()
{
}

bool GamePlayStateThree::Initialize(Graphics *graphics)
{
	this->graphics = graphics;
	this->gDevice = graphics->GetDevice();

	viewPort = new ViewPort(4094 - GAME_WIDTH, GAME_HEIGHT); // (3750 - 240, GAME_HEIGHT) 516, 1026

	sprite = new Sprite();
	if (!sprite->Initialize(gDevice, LEVEL_THREE_BG))
		return false;

	tileMap = new TileMap(4096, 1536, sprite, 64, 64);
	tileMap->LoadListTileFromFile(LEVEL_THREE_MATRIX_BG);

	cameraObject = new GameObject();
	cameraObject->Initialize(gDevice, SIMON_SPRITE, viewPort->GetCameraPosition().x, viewPort->GetCameraPosition().y, 5);

	leftCamera = new Ground(2, 450);
	leftCamera->Initialize(gDevice, "Resource\\sprites\\Ground\\0.png", viewPort->GetCameraPosition().x - 2, viewPort->GetCameraPosition().y, TAG_GROUND);

	rightCamera = new Ground(2, 450);
	rightCamera->Initialize(gDevice, "Resource\\sprites\\Ground\\0.png", viewPort->GetCameraPosition().x + GAME_WIDTH, viewPort->GetCameraPosition().y, TAG_GROUND);

	quadTree = new QuadTree(gDevice, viewPort);
	quadTree->Load("Resource\\sprites\\QuadTree\\lv3.txt");

	list = new std::vector<GameObject*>();
	listItem = new std::vector<Item*>();
	moveList = new std::vector<GameObject*>();
	listEnemy = new std::vector<GameObject*>();

	InitMovableGround();
	InitBlueBat();
	InitBonePillar();
	InitMedusaHead();
	InitGhost();

	// 3750, 100
	simon->SetPosition(3750, 100);
	simon->Reload();

	changeState = false;
	
	if(Sound::GetInstance()->IsPLaying(BOSS_BATTLE_POISON_MIND))
		Sound::GetInstance()->Stop(BOSS_BATTLE_POISON_MIND);
	if (Sound::GetInstance()->IsPLaying(STAGE_01_VAMPIRE_KILLER))
		Sound::GetInstance()->Stop(STAGE_01_VAMPIRE_KILLER);
	Sound::GetInstance()->PlayLoop(STAGE_04_STALKER);

	RECT* rect = new RECT();
	SetRect(rect, 0, 1536, GAME_WIDTH, 1200);
	medusa = new Medusa(64, 2, 3000);
	medusa->Initialize(gDevice, MEDUSA_SPRITE, 190, 1340, 2001); // tag : 2001
	medusa->Restrict(rect);
	medusa->SetEnable(false);

	ui = new UI();
	ui->Initialize(gDevice, simon, medusa->GetHP());
	time = 0;
	score = 0;
	inBoss = false;
	Item::TakeSimonPointer(simon);
	Item::TakeListEnemyPointer(listEnemy);

	
	return true;
}


void GamePlayStateThree::Update(float gameTime)
{
	GameState::Update(gameTime);

	if (simon->nextStage == true)
	{
		SetChangingState(true);
		simon->nextStage = false;
		return;
	}

	time += gameTime;
	ui->Update(medusa->GetHP(), 1000 - (int)time, 3, 2);

	cameraObject->SetPosition(viewPort->GetCameraPosition());
	cameraObject->SetRegion(0, GAME_WIDTH, 0, -GAME_HEIGHT);

	//khong cho simon di ra khoi camera
	leftCamera->SetPosition(viewPort->GetCameraPosition().x - 2, viewPort->GetCameraPosition().y);
	leftCamera->SetBox(leftCamera->GetPosition().x, leftCamera->GetPosition().y, 2, GAME_HEIGHT, 0, 0);

	rightCamera->SetPosition(viewPort->GetCameraPosition().x + GAME_WIDTH, viewPort->GetCameraPosition().y);
	rightCamera->SetBox(rightCamera->GetPosition().x, rightCamera->GetPosition().y, 2, GAME_HEIGHT, 0, 0);

	//////////////////////////////////////////////////////

	list->clear();

	quadTree->GetObjectList(list, moveList, cameraObject->GetRegion(), cameraObject);

	if (inBoss == true)
	{
		if (medusa->IsEnable() == true)
		{
			medusa->Aim(simon, gameTime);
			list->push_back(medusa);
			list->push_back(medusa->Snake_1());
			list->push_back(medusa->Snake_2());
		}
		else
			if (medusa->isDropItem == false)
			{
				Item *item = new Item();
				item->Initialize(gDevice, medusa->GetPosition().x, medusa->GetPosition().y, ITEM_BALL, 1000);
				listItem->push_back(item);
				medusa->isDropItem = true;
			}
	}

	for (auto i = list->begin(); i != list->end(); i++)
	{
		if (!(*i)->IsEnable())
		{
			if ((*i)->isDropItem == false)
			{
				Item *item = new Item();
				item->Initialize(gDevice, (*i)->GetPosition().x, (*i)->GetPosition().y);
				listItem->push_back(item);
				(*i)->isDropItem = true;
			}
			quadTree->Remove((*i));
		}
		else
		{
			if ((*i)->GetTag() == TAG_DOOR && ((*i)->GetID() == 91 || (*i)->GetID() == 102))
			{
				((Door*)(*i))->SetIsOccurred(true);
			}
			(*i)->Update(gameTime);
			if ((*i)->GetCollider()->GetTag() == TAG_ZOMBIE)
			{
				if ((*i)->IsInRegion(cameraObject->GetRegion()) == false)
					(*i)->SetEnable(false);
				else
					((Zombie*)(*i))->CheckCollider(gameTime, list);
			}
			if ((*i)->GetTag() == TAG_CRUSHER && (*i)->GetID() == 93)
			{
				((Crusher*)(*i))->SetHeight(80);
			}
		}
	}

	listEnemy->clear();
	for (auto i = moveList->begin(); i != moveList->end();)
	{
		if ((*i)->trigger == false)
		{
			if ((*i)->IsInRegion(cameraObject->GetRegion()))
			{
				(*i)->trigger = true;
			}
		}
		else 
		{
			(*i)->Update(gameTime);
			if ((*i)->IsEnable())
			{
				(*i)->CheckCollider(gameTime, list);
				list->push_back(*i);
				listEnemy->push_back(*i);
			}
		}

		if ((*i)->trigger == true && !(*i)->IsInRegion(cameraObject->GetRegion()) && !(*i)->IsInRegionC(cameraObject->GetRegion()))
		{
			((Enemy*)(*i))->Reload();
			((Enemy*)(*i))->isIn = false;
			moveList->erase(i);
			break;
		}
		i++;
	}

	for (auto i = listItem->begin(); i != listItem->end(); i++)
	{
		(*i)->Update(gameTime);
		(*i)->CheckCollider(gameTime, list);
	}

	//////////////////////////////////////////////////////

	

	if (!simon->IsGoingThrowDoor())
	{
		UpdateMovableGround(gameTime);
		UpdateBlueBat(gameTime);
		UpdateBonePillar(gameTime);
		UpdateMedusaHead(gameTime);
		UpdateGhost(gameTime);
	}

	if (simon->GetPosition().y < viewPort->GetCameraPosition().y - GAME_HEIGHT)
		simon->SetHP(0);

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
		medusa->Reset();
		if (Sound::GetInstance()->IsPLaying(BOSS_BATTLE_POISON_MIND))
			Sound::GetInstance()->Stop(BOSS_BATTLE_POISON_MIND);
	}

}

void GamePlayStateThree::Render()
{
	tileMap->Draw(viewPort);
	for (auto i = list->begin(); i != list->end(); i++)
	{
		(*i)->isAdded = false;
		(*i)->Render(viewPort);
	}
	for (auto i = moveList->begin(); i != moveList->end(); i++)
	{
		(*i)->isAdded = false;
		(*i)->Render(viewPort);
	}
	for (auto i = listItem->begin(); i != listItem->end(); i++)
	{
		(*i)->Render(viewPort);
	}
	simon->Render(viewPort);

	ui->Render();
}

bool GamePlayStateThree::GetChangingState()
{
	return GameState::GetChangingState();
}

void GamePlayStateThree::SetChangingState(bool status)
{
	GameState::SetChangingState(status);
}

void GamePlayStateThree::SimonCollideWithDoorHandle(float gameTime)
{
}

bool GamePlayStateThree::CameraFollowHandle(float gameTime)
{
	D3DXVECTOR3 currentCamera = viewPort->GetCameraPosition();

	if (simon->GetPosition().y > 1140 && currentCamera.x <= 5)
	{
		if (inBoss == false)
		{
			medusa->SetEnable(true);
			if (!Sound::GetInstance()->IsPLaying(BOSS_BATTLE_POISON_MIND))
			{
				Sound::GetInstance()->PlayLoop(BOSS_BATTLE_POISON_MIND);
				Sound::GetInstance()->Stop(STAGE_04_STALKER);
			}
		}
		inBoss = true;
		return false;
	}

	if (simon->GetPosition().y < currentCamera.y - GAME_HEIGHT) {
		
	}

	if (simon->GetPosition().y < 350)
	{
		viewPort->SetCameraPosition(currentCamera.x, GAME_HEIGHT);
	}
	else
	{
		if ((simon->GetPosition().x >= 3072 + GAME_WIDTH / 2 && simon->GetPosition().x <= 4094 - GAME_WIDTH / 2) ||
			(simon->GetPosition().x >= 1536 + GAME_WIDTH / 2 && simon->GetPosition().x < 3072 - GAME_WIDTH / 2) ||
			(simon->GetPosition().x >= 0 + GAME_WIDTH / 2 && simon->GetPosition().x < 1536 - GAME_WIDTH / 2))
			currentCamera.x = simon->GetPosition().x - GAME_WIDTH / 2;
		viewPort->SetCameraPosition(currentCamera.x, currentCamera.y);
	}

	return false;
}

bool GamePlayStateThree::IsInCamera(float x, float y)
{
	return (x > viewPort->GetCameraPosition().x
		&& x < viewPort->GetCameraPosition().x + GAME_WIDTH
		&& y < viewPort->GetCameraPosition().y
		&& y > viewPort->GetCameraPosition().y - GAME_HEIGHT);
}


void GamePlayStateThree::InitMovableGround()
{
	for (int i = 0; i < 3; i++)
		movableGround[i] = new MovableGround();

	movableGround[0]->Initialize(gDevice, "Resource\\sprites\\Ground\\9.png", 3200, 603, TAG_MOVEABLE_GROUND);
	movableGround[1]->Initialize(gDevice, "Resource\\sprites\\Ground\\9.png", 2000, 990, TAG_MOVEABLE_GROUND);
	movableGround[2]->Initialize(gDevice, "Resource\\sprites\\Ground\\9.png", 2250, 990, TAG_MOVEABLE_GROUND);
}

void GamePlayStateThree::InitBlueBat()
{
	for (int i = 0; i < 3; i++)
		blueBat[i] = new BlueBat(1, 2, 200);

	blueBat[0]->Initialize(gDevice, BLUE_BAT_SPRITE, 3680, 320, TAG_BAT);
	blueBat[1]->Initialize(gDevice, BLUE_BAT_SPRITE, 3683, 696, TAG_BAT);
	blueBat[2]->Initialize(gDevice, BLUE_BAT_SPRITE, 264, 1029, TAG_BAT);

	for (int i = 0; i < 3; i++)
		blueBat[i]->SetEnable(true);
}

void GamePlayStateThree::InitBonePillar()
{
	for (int i = 0; i < 2; i++)
		bonePillar[i] = new BonePillar(24, 2, 500);
	bonePillar[0]->Initialize(gDevice, BONE_PILLAR_SPRITE, 478, 1060, TAG_BONE_PILLAR);
	bonePillar[1]->Initialize(gDevice, BONE_PILLAR_SPRITE, 608, 1122, TAG_BONE_PILLAR);
	bonePillar[0]->SetEnable(true);
	bonePillar[1]->SetEnable(true);
}

void GamePlayStateThree::InitMedusaHead()
{
	for (int i = 0; i < 2; i++)
	{
		medusaHead[i] = new MedusaHead(2, 2, 100);
		medusaHead[i]->Initialize(gDevice, MEDUSA_HEAD_SPRITE, 0, 0, TAG_MEDUSA_HEAD);
	}
}

void GamePlayStateThree::InitGhost()
{
	ghost = new Ghost(8, 2, 100);
	ghost->Initialize(gDevice, GHOST_SPRITE, 0, 0, TAG_GHOST);
}


void GamePlayStateThree::UpdateMovableGround(float gameTime)
{
	//cap nhat cho moveableGround
	for (int i = 0; i < sizeof(movableGround) / sizeof(*movableGround); i++)
	{
		//neu camera di chuyen ra khoi vung hoat dong cua moveableGround thi disable
		if (!IsInCamera(movableGround[i]->GetPosition().x, movableGround[i]->GetPosition().y))
		{
			movableGround[i]->SetEnable(false);
		}
		else
		{
			movableGround[i]->SetEnable(true);
		}

		//neu moveable duoc enable thi update va dua no vao list xet va cham
		if (movableGround[i]->IsEnable())
		{
			movableGround[i]->Update(gameTime);
			movableGround[i]->CheckCollideWithGround(gameTime, list);
			list->push_back(movableGround[i]);
		}
	}
}

void GamePlayStateThree::UpdateBlueBat(float gameTime)
{
	for (int i = 0;i < 3;i++)
	{
		//neu nhu blue khong nam trong camera va simon dang trong vung hoat dong cua no
		//thi enable = false
		if (!IsInCamera(blueBat[i]->GetPosition().x, blueBat[i]->GetPosition().y))
		{
			switch (i)
			{
			case 0:
				if (simon->GetPosition().x > 3574 &&
					simon->GetPosition().x < 4050 &&
					simon->GetPosition().y > 0 &&
					simon->GetPosition().y < 480)
				{
					blueBat[i]->SetEnable(false);
				}
				break;
			case 1:
				if (simon->GetPosition().x > 3062 &&
					simon->GetPosition().x < 4050 &&
					simon->GetPosition().y > 380 &&
					simon->GetPosition().y < 700)
				{
					blueBat[i]->SetEnable(false);
				}
				break;
			case 2:
				if (simon->GetPosition().x > 60 &&
					simon->GetPosition().x < 400 &&
					simon->GetPosition().y > 770 &&
					simon->GetPosition().y < 1440)
				{
					blueBat[i]->SetEnable(false);
				}
			}
		}

		//neu camera di chuyen ra ngoai vung hoat dong cua bluebat nao thi enable no lai vi tri cu
		if (!blueBat[i]->IsEnable())
		{
			switch (i)
			{
			case 0:
				if (viewPort->GetCameraPosition().x > 4050 ||
					viewPort->GetCameraPosition().x + GAME_WIDTH < 3574 ||
					viewPort->GetCameraPosition().y > 480 ||
					viewPort->GetCameraPosition().y - GAME_HEIGHT < 0)
				{
					blueBat[i]->Respawn();
					blueBat[i]->SetAction(HANG);
					blueBat[i]->SetPosition(3680, 320);
				}
				break;
			case 1:
				if (viewPort->GetCameraPosition().x > 4050 ||
					viewPort->GetCameraPosition().x + GAME_WIDTH < 3062 ||
					viewPort->GetCameraPosition().y > 960 ||
					viewPort->GetCameraPosition().y - GAME_HEIGHT < 380)
				{
					blueBat[i]->Respawn();
					blueBat[i]->SetAction(HANG);
					blueBat[i]->SetPosition(3683, 696);
				}
				break;
			case 2:
				if (viewPort->GetCameraPosition().x > 1000 ||
					viewPort->GetCameraPosition().x + GAME_WIDTH < 60 ||
					viewPort->GetCameraPosition().y > 1440 ||
					viewPort->GetCameraPosition().y - GAME_HEIGHT < 770)
				{
					blueBat[i]->Respawn();
					blueBat[i]->SetAction(HANG);
					blueBat[i]->SetPosition(264, 1029);
				}
			}
		}

		//neu bluebat nam trong camera va no enable thi update cho no
		if (blueBat[i]->IsEnable() && IsInCamera(blueBat[i]->GetPosition().x, blueBat[i]->GetPosition().y))
		{
			if (((abs(simon->GetPosition().x - blueBat[i]->GetPosition().x) < 100) &&
				(abs(simon->GetPosition().y - blueBat[i]->GetPosition().y) < 50)) &&
				blueBat[i]->GetAction() == HANG)
			{
				if (simon->GetPosition().x > blueBat[i]->GetPosition().x)
					blueBat[i]->SetIsLeft(false);
				else
					blueBat[i]->SetIsLeft(true);
				blueBat[i]->SetPosition(blueBat[i]->GetPosition().x, simon->GetPosition().y);
				blueBat[i]->SetAction(FLY);
			}
			blueBat[i]->Update(gameTime);
			list->push_back(blueBat[i]);
			listEnemy->push_back(blueBat[i]);
		}
	}

}

void GamePlayStateThree::UpdateBonePillar(float gameTime)
{
	//cap nhat cho bonepillar
	for (int i = 0; i < 2; i++)
	{
		//neu bonePillar khong nam trong camera thi cho no hoi sinh
		if (!IsInCamera(bonePillar[i]->GetPosition().x, bonePillar[i]->GetPosition().y))
		{
			bonePillar[i]->Respawn();
		}

		//neu bonePillar nam trong camera va no enable thi update cho no
		if (bonePillar[i]->IsEnable() && IsInCamera(bonePillar[i]->GetPosition().x, bonePillar[i]->GetPosition().y))
		{
			bonePillar[i]->Update(gameTime);
			list->push_back(bonePillar[i]);
			listEnemy->push_back(bonePillar[i]);
			if (bonePillar[i]->GetPosition().x < simon->GetPosition().x)
				bonePillar[i]->SetIsLeft(false);
			else
				bonePillar[i]->SetIsLeft(true);
		}

		//neu vien dan ra khoi camera thi disable
		for (int j = 0;j < 2; j++)
		{
			if (!IsInCamera(bonePillar[i]->GetProjectile()[j]->GetPosition().x, bonePillar[i]->GetProjectile()[j]->GetPosition().y))
				bonePillar[i]->GetProjectile()[j]->SetEnable(false);

			//neu vien dan enable thi dua no vao list kiem tra va cham
			if (bonePillar[i]->GetProjectile()[j]->IsEnable())
			{
				bonePillar[i]->GetProjectile()[j]->Update(gameTime);
				list->push_back(bonePillar[i]->GetProjectile()[j]);
				listEnemy->push_back(bonePillar[i]->GetProjectile()[j]);
			}
		}
	}
}

void GamePlayStateThree::UpdateMedusaHead(float gameTime)
{
	//neu nhu simon di vao vung hoat dong cua medusahead
	if ((simon->GetPosition().x > 2500 && simon->GetPosition().x < 3000
		&& simon->GetPosition().y > 396 && simon->GetPosition().y < 736)
		|| (simon->GetPosition().x > 1730 && simon->GetPosition().x < 2170
			&& simon->GetPosition().y > 806 && simon->GetPosition().y < 956)
		|| (simon->GetPosition().x > 1645 && simon->GetPosition().x < 2010
			&& simon->GetPosition().y > 996 && simon->GetPosition().y < 1120)
		|| (simon->GetPosition().x > 680 && simon->GetPosition().x < 1285
			&& simon->GetPosition().y > 1286 && simon->GetPosition().y < 1463))
	{
		for (int i = 0; i < 2; i++)
			if (!medusaHead[i]->IsEnable() && medusaHead[i]->respawnTime > 3)
			{
				if (simon->GetIsLeft())
				{
					medusaHead[i]->SetPosition(viewPort->GetCameraPosition().x + 1, simon->GetPosition().y);
					medusaHead[i]->SetIsLeft(false);
				}
				else
				{
					medusaHead[i]->SetPosition(viewPort->GetCameraPosition().x + GAME_WIDTH - 1, simon->GetPosition().y);
					medusaHead[i]->SetIsLeft(true);
				}
				medusaHead[i]->SetY(simon->GetPosition().y);
				medusaHead[i]->Respawn();

				//neu medusaHead dau tien enable thi con thu hai phai doi 1 thoi gian
				if (i == 0)
					medusaHead[1]->respawnTime = 0.0f;
			}
	}

	for (int i = 0; i < 2; i++)
	{
		//neu medusaHead khong nam trong camera thi disable
		if (!IsInCamera(medusaHead[i]->GetPosition().x, medusaHead[i]->GetPosition().y))
			medusaHead[i]->SetEnable(false);

		medusaHead[i]->Update(gameTime);

		//neu medusaHead enable thi update va dua vao list
		if (medusaHead[i]->IsEnable())
		{
			list->push_back(medusaHead[i]);
			listEnemy->push_back(medusaHead[i]);
		}
	}
}

void GamePlayStateThree::UpdateGhost(float gameTime)
{
	if ((simon->GetPosition().x > 640 && simon->GetPosition().x < 810
		&& simon->GetPosition().y > 866 && simon->GetPosition().y < 986)
		|| (simon->GetPosition().x > 820 && simon->GetPosition().x < 1150
			&& simon->GetPosition().y > 1060 && simon->GetPosition().y < 1120))
	{
		if (!ghost->IsEnable() && ghost->respawnTime > 1)
		{
			if (simon->GetIsLeft())
			{
				ghost->SetPosition(viewPort->GetCameraPosition().x + 1, simon->GetPosition().y - 60);
				ghost->SetIsLeft(false);
			}
			else
			{
				ghost->SetPosition(viewPort->GetCameraPosition().x + GAME_WIDTH - 1, simon->GetPosition().y - 60);
				ghost->SetIsLeft(true);
			}
			ghost->Respawn();
		}
	}

	if (!IsInCamera(ghost->GetPosition().x, ghost->GetPosition().y))
		ghost->SetEnable(false);

	
	ghost->GetTargetPosition(simon->GetPosition());
	ghost->Update(gameTime);

	//neu medusaHead enable thi update va dua vao list
	if (ghost->IsEnable())
	{
		list->push_back(ghost);
		listEnemy->push_back(ghost);
	}
}

void GamePlayStateThree::DestroyAll()
{
	delete(simon);
	delete(tileMap);
	delete(ground);
	delete(cameraObject);
	delete(listObject);
	delete(viewPort);
}