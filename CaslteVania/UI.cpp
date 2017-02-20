#include "UI.h"

UI::UI()
{
	simonHP = NULL;
	simonHP = NULL;
}


UI::~UI()
{
	delete[] simonHPList;
	delete[] enemyHPList;
}


bool UI::Initialize(LPDIRECT3DDEVICE9 gDevice, Simon *simon, int bossHP)
{
	this->bossHP = bossHP;
	this->simon = simon;

	black = new Sprite(0, 0);
	black->Initialize(gDevice, "Resource\\sprites\\UI\\black.png");

	simonHPList = new std::vector<Sprite*>();
	for (int i = 0; i < 16; i++)
	{
		Sprite* sprite = new Sprite(120 + 12 * (i + 1), 43);
		sprite->Initialize(gDevice, "Resource\\sprites\\UI\\HP.png");
		simonHPList->push_back(sprite);
	}
	enemyHPList = new std::vector<Sprite*>();
	for (int i = 0; i < 16; i++)
	{
		Sprite* sprite = new Sprite(120 + 12 * (i + 1), 67);
		sprite->Initialize(gDevice, "Resource\\sprites\\UI\\EnemyHP.png");
		enemyHPList->push_back(sprite);
	}
	noHPList = new std::vector<Sprite*>();
	for (int i = 0; i < 16; i++)
	{
		Sprite* sprite = new Sprite(120 + 12 * (i + 1), 43);
		sprite->Initialize(gDevice, "Resource\\sprites\\UI\\NoHP.png");
		noHPList->push_back(sprite);
	}

	heart = new Sprite(420, 43);
	heart->Initialize(gDevice, "Resource\\sprites\\UI\\Heart.png");

	axe = new Sprite(350, 55);
	axe->Initialize(gDevice, "Resource\\sprites\\UI\\AXE.png");

	holyWater = new Sprite(350, 55);
	holyWater->Initialize(gDevice, "Resource\\sprites\\UI\\HOLY_WATER.png");

	cross = new Sprite(350, 55);
	cross->Initialize(gDevice, "Resource\\sprites\\UI\\CROSS.png");

	knife = new Sprite(350, 55);
	knife->Initialize(gDevice, "Resource\\sprites\\UI\\KNIFE.png");

	stopWatch = new Sprite(350, 55);
	stopWatch->Initialize(gDevice, "Resource\\sprites\\UI\\STOP_WATCH.png");

	upgrade1 = new Sprite(500, 55);
	upgrade1->Initialize(gDevice, "Resource\\sprites\\UI\\DOUBLE_SHOOT_UI.png");

	upgrade2 = new Sprite(500, 55);
	upgrade2->Initialize(gDevice, "Resource\\sprites\\UI\\TRIPLE_SHOOT_UI.png");

	font = NULL;
	AddFontResourceEx(GAME_FONT, FR_PRIVATE, NULL);
	HRESULT result = D3DXCreateFont(
		gDevice, 20, 0, FW_NORMAL, 1, false,
		DEFAULT_CHARSET, OUT_DEFAULT_PRECIS,
		ANTIALIASED_QUALITY, FF_DONTCARE, "Press Start", &font);

	if (!SUCCEEDED(result))
		return false;

	SetRect(&rect, 0, 20, GAME_WIDTH, GAME_HEIGHT);
	information = "SCORE_000000 TIME 0000 STAGE 00\n";
	information += "PLAYER                =62\n";
	information += "ENEMY                P=3\n";
}


void UI::Update(int bossHP, int time, int life, int stage)
{

#pragma region Calculate boss HP in UI

	if (bossHP <= 0)
		this->uiHP = 0;
	else {
		int x = bossHP / (this->bossHP / 16);
		int y = bossHP % (this->bossHP / 16);

		if (y == 0)
			this->uiHP = x;
		else
			this->uiHP = x + 1;
	}

#pragma endregion

	this->time = time;
	this->stage = stage;


	std::string scoreString = std::to_string(simon->GetScore());
	while (scoreString.length() < 6)
		scoreString = "0" + scoreString;


	std::string timeString = std::to_string(this->time);
	while (timeString.length() < 4)
		timeString = "0" + timeString;


	std::string stageString = std::to_string(this->stage);
	while (stageString.length() < 2)
		stageString = "0" + stageString;


	information = "SCORE_" + scoreString + " TIME " + timeString + " STAGE " + stageString + "\n";
	information += "PLAYER                =" + std::to_string(simon->GetEnergy()) + "\n";
	information += "ENEMY                P=" + std::to_string(simon->GetLife()) + "\n";


	switch (simon->currentWeapon)
	{
	case ITEM_AXE:
		weapon = nullptr;
		weapon = axe;
		break;
	case ITEM_HOLY_WATER:
		weapon = nullptr;
		weapon = holyWater;
		break;
	case ITEM_CROSS:
		weapon = nullptr;
		weapon = cross;
		break;
	case ITEM_KNIFE:
		weapon = nullptr;
		weapon = knife;
		break;
	case ITEM_STOP_WATCH:
		weapon = nullptr;
		weapon = stopWatch;
		break;
	default:
		break;
	}
}

void UI::Render()
{
	RECT newRect;
	SetRect(&newRect, 0, 0, GAME_WIDTH, 116);
	black->Render(black->GetPosition(), newRect);
	if (font)
		font->DrawTextA(NULL, information.c_str(), -1, &rect, DT_LEFT, D3DCOLOR_XRGB(255, 255, 255));


	for (std::vector<Sprite*>::iterator i = noHPList->begin(); i != noHPList->end(); i++)
	{
		SetRect(&newRect, 0, 0, 8, 15);
		(*i)->Render((*i)->GetPosition(), newRect);
		D3DXVECTOR3 newPosition = (*i)->GetPosition();
		newPosition.y = 67;
		(*i)->Render(newPosition, newRect);
	}


	int count = 0;
	for (std::vector<Sprite*>::iterator i = simonHPList->begin(); i != simonHPList->end(); i++)
	{
		if (count < simon->GetHP())
		{
			SetRect(&newRect, 0, 0, 8, 15);
			(*i)->Render((*i)->GetPosition(), newRect);
		}
		count++;
	}


	count = 0;
	for (std::vector<Sprite*>::iterator i = enemyHPList->begin(); i != enemyHPList->end(); i++)
	{
		if (count < uiHP)
		{
			SetRect(&newRect, 0, 0, 8, 15);
			(*i)->Render((*i)->GetPosition(), newRect);
		}
		count++;
	}


	D3DXVECTOR3 newPosition = { 330, 45, 0 };
	SetRect(&newRect, 0, 0, 70, 45);
	(*enemyHPList)[0]->Render(newPosition, newRect);

	SetRect(&newRect, 0, 0, 16, 16);
	heart->Render(heart->GetPosition(), newRect);

	weapon->Render(weapon->GetPosition());

	switch (simon->GetNoSubWeapon()) {
	case 2: upgrade1->Render(upgrade1->GetPosition()); break;
	case 3: upgrade2->Render(upgrade2->GetPosition()); break;
	}
}