#include "GamePlayStateOne.h"



GamePlayStateOne::GamePlayStateOne()
{
}


GamePlayStateOne::~GamePlayStateOne()
{
}

bool GamePlayStateOne::Initialize(Graphics *graphics)
{
	this->gDevice = graphics->GetDevice();

	viewPort = new ViewPort(0, GAME_HEIGHT);

	sprite = new Sprite();
	if (!sprite->Initialize(gDevice, LEVEL_ONE_BG))
		return false;	

	castleSprite = new Sprite(1408, 347);
	if (!castleSprite->Initialize(gDevice, CASTLE_SPRITE))
		return false;
	
	tileMap = new TileMap(1536, 392, sprite, 64, 64);
	tileMap->LoadListTileFromFile(LEVEL_ONE_MATRIX_BG);

	cameraObject = new GameObject();
	cameraObject->Initialize(gDevice, SIMON_SPRITE, viewPort->GetCameraPosition().x, viewPort->GetCameraPosition().y, 5);

	leftCamera = new Ground(2, 450);
	leftCamera->Initialize(gDevice, "Resource\\sprites\\Ground\\0.png", viewPort->GetCameraPosition().x - 2, viewPort->GetCameraPosition().y, TAG_GROUND);

	rightCamera = new Ground(2, 450);
	rightCamera->Initialize(gDevice, "Resource\\sprites\\Ground\\0.png", viewPort->GetCameraPosition().x + GAME_WIDTH, viewPort->GetCameraPosition().y, TAG_GROUND);

	quadTree = new QuadTree(gDevice, viewPort);
	quadTree->Load("Resource\\sprites\\QuadTree\\lv1.txt");

	list = new std::vector<GameObject*>();

	simon->SetPosition(100, 150);
	simon->Reload();

	changeState = false;

	Sound::GetInstance()->Stop(GAME_START_PROLOGUE);

	if (!Sound::GetInstance()->IsPLaying(STAGE_01_VAMPIRE_KILLER))
		Sound::GetInstance()->PlayLoop(STAGE_01_VAMPIRE_KILLER);

	Item::TakeSimonPointer(simon);

	listItem = new std::vector<Item*>();

	ui = new UI();
	ui->Initialize(gDevice, simon, 16);
	time = 0;

	return true;
}


void GamePlayStateOne::Update(float gameTime)
{
	time += gameTime;
	ui->Update(16, 1000 - (int)time, 3, 1);

	if (simon->GetPosition().x >= GAME_WIDTH / 2 && simon->GetPosition().x <= 1536 - GAME_WIDTH / 2 - 32)
	{
		D3DXVECTOR3 currentCamera = viewPort->GetCameraPosition();
		currentCamera.x += simon->GetVelocity().x * gameTime;
		viewPort->SetCameraPosition(currentCamera.x, currentCamera.y);
	}

	cameraObject->SetPosition(viewPort->GetCameraPosition());
	cameraObject->SetRegion(0, GAME_WIDTH, 0, -GAME_HEIGHT);

	//khong cho simon di ra khoi camera
	leftCamera->SetPosition(viewPort->GetCameraPosition().x - 2, viewPort->GetCameraPosition().y);
	leftCamera->SetBox(leftCamera->GetPosition().x, leftCamera->GetPosition().y, 2, GAME_HEIGHT, 0, 0);

	rightCamera->SetPosition(viewPort->GetCameraPosition().x + GAME_WIDTH, viewPort->GetCameraPosition().y);
	rightCamera->SetBox(rightCamera->GetPosition().x, rightCamera->GetPosition().y, 2, GAME_HEIGHT, 0, 0);

	list->clear();
	quadTree->GetObjectList(list, cameraObject->GetRegion());
	for (auto i = list->begin(); i != list->end(); i++)
	{
		if (!(*i)->IsEnable())
		{
			if ((*i)->isDropItem == false)
			{
				Item *item = new Item();
				item->Initialize(gDevice, (*i)->GetPosition().x, (*i)->GetPosition().y);
				//item->Initialize(gDevice, (*i)->GetPosition().x, (*i)->GetPosition().y, ITEM_UPGRADE_1, 10);
				listItem->push_back(item);
				(*i)->isDropItem = true;
			}
			quadTree->Remove((*i));
		}
		else
			(*i)->Update(gameTime);
	}

	for (auto i = listItem->begin(); i != listItem->end(); i++)
	{
		(*i)->Update(gameTime);
		(*i)->CheckCollider(gameTime, list);
	}

	for (int i = 0; i < simon->GetNoSubWeapon();i++)
	{
		simon->subWeapon[i]->CheckCollider(gameTime, list);
		if (simon->subWeapon[i]->GetPosition().x < viewPort->GetCameraPosition().x ||
			simon->subWeapon[i]->GetPosition().x > viewPort->GetCameraPosition().x + GAME_WIDTH)
			simon->subWeapon[i]->SetEnable(false);

		if (simon->subWeapon[i]->CheckColliderWith(gameTime, simon) && simon->subWeapon[i]->GetTag() == TAG_CROSS)
		{
			if (((Cross*)simon->subWeapon[i])->IsTurnBack())
				simon->subWeapon[i]->SetEnable(false);
		}
	}

	simon->KeyBoardHandle(gameTime);
	simon->Update(gameTime);
	simon->CheckCollider(gameTime, list);
	simon->CheckColliderWith(gameTime, leftCamera);
	simon->CheckColliderWith(gameTime, rightCamera);

	if (IsKeyPress(DIK_M))
		SetChangingState(true);
	
	if (simon->GetPosition().x > 1300 && simon->GetPosition().x < 1400)
	{
		simon->SetVelocity(D3DXVECTOR3(0, 0, 0));
		simon->SetPosition(simon->GetPosition().x + 50 * gameTime,
			simon->GetPosition().y);
		simon->SetAction(MOVE);
		simon->SetCanControlKeyboard(false);
		if (simon->GetPosition().x >= 1408 - 55)
		{
			if (!Sound::GetInstance()->IsPLaying(GO_INTO_CASTLE))
				Sound::GetInstance()->Play(GO_INTO_CASTLE);
		}
	}
	else if (simon->GetPosition().x > 1400)
	{
		SetChangingState(true);
	}
}

void GamePlayStateOne::Render()
{
	ui->Render();
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
	simon->Render(viewPort);

	castleSprite->Render(viewPort);
}

bool GamePlayStateOne::GetChangingState()
{
	return GameState::GetChangingState();
}

void GamePlayStateOne::SetChangingState(bool status)
{
	GameState::SetChangingState(status);
}

void GamePlayStateOne::DestroyAll()
{
	//delete(simon);
	delete(tileMap);
	delete(ground);
	delete(cameraObject);
	delete(listObject);
	delete(viewPort);
}