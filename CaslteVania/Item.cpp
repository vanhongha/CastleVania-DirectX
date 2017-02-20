#include "Item.h"

Item::Item()
{
}

Item::~Item()
{
}

void Item::Initialize(LPDIRECT3DDEVICE9 gDevice, float x, float y)
{
	std::srand(time(0));
	Random();
	remainTime = 5;

	GameObject::Initialize(gDevice, itemPath.c_str(), x, y, itemType);
}

void Item::Initialize(LPDIRECT3DDEVICE9 gDevice, float x, float y, int itemType, float remainTime)
{
	stateClearTime = 0;
	changedState = false;
	this->remainTime = remainTime;

	switch (itemType) {
	case ITEM_BALL:
		itemPath = "Resource\\sprites\\Items\\13.png";
		break;
	case ITEM_UPGRADE_1:
		itemPath = "Resource\\sprites\\Items\\DOUBLE_SHOT.png";
		break;
	case ITEM_UPGRADE_2:
		itemPath = "Resource\\sprites\\Items\\TRIPLE_SHOT.png";
		break;
	case ITEM_ROSARY:
		itemPath = "Resource\\sprites\\Items\\ROSARY.png";
		break;
	case ITEM_MONEY:
		itemPath = "Resource\\sprites\\Items\\Money_bag_white.png";
		break;
	case ITEM_INVISIBLE:
		itemPath = "Resource\\sprites\\Items\\INVICIBILITY_ITEM.png";
		break;
	case ITEM_STOP_WATCH:
		itemPath = "Resource\\sprites\\Items\\STOP_WATCH.png";
		break;
	default:
		MessageBox(NULL, "There no type like that", "Error!", MB_OK);
		break;
	}

	this->itemType = itemType;

	GameObject::Initialize(gDevice, itemPath.c_str(), x, y, itemType);
}

/*STATIC*/
void Item::TakeSimonPointer(Simon *simon)
{
	Item::simon = simon;
}

/*STATIC*/
void Item::TakeListEnemyPointer(std::vector<GameObject*>* listEnemy)
{
	Item::listEnemy = listEnemy;
}

/*STATIC*/
Simon* Item::simon = NULL;

/*STATIC*/
bool Item::isKillAll = false;

/*STATIC*/
std::vector<GameObject*>* Item::listEnemy = NULL;

void Item::Update(float gameTime)
{
	if (isEnable == true)
	{
		if (remainTime > 0)
		{
			remainTime -= gameTime;
			// Check if Simon intersect with item
			if (IsInRegion(simon->GetRegion()) == true)
			{
				switch (itemType)
				{
				case ITEM_SMALL_HEART:
					Sound::GetInstance()->Play(COLLECT_ITEM);
					AddEnergy(1);
					break;
				case ITEM_BIG_HEART:
					Sound::GetInstance()->Play(COLLECT_ITEM);
					AddEnergy(5);
					break;
				case ITEM_ROAST:
					Sound::GetInstance()->Play(COLLECT_ITEM);
					RecoverHP(8);
					break;
				case ITEM_WHIP:
					Sound::GetInstance()->Play(COLLECT_WEAPON);
					UpgradeWhip();
					break;
				case ITEM_BALL:			
					Sound::GetInstance()->Play(EAT_CHANGE_STATE_BALL);
					NextStage(gameTime);			
					break;
				case ITEM_MONEY:	
					Sound::GetInstance()->Play(MONEY);
					AddScore(1000);		
					break;
				case ITEM_INVISIBLE:
					Sound::GetInstance()->Play(COLLECT_ITEM);
					MakeInvincible(5);
					break;
				case ITEM_ROSARY:	
					Sound::GetInstance()->Play(HOLY_CROSS);
					KillAll();				
					break;
				case ITEM_UPGRADE_1:	
					Sound::GetInstance()->Play(COLLECT_ITEM);
					IncreaseSubWeapon(2);
					break;
				case ITEM_UPGRADE_2:	
					Sound::GetInstance()->Play(COLLECT_ITEM);
					IncreaseSubWeapon(3);
					break;
				default:
					Sound::GetInstance()->Play(COLLECT_WEAPON);
					ChangeSubWeapon();
					break;
				}
				isEnable = false;
			}
		}
		else
		{
			isEnable = false;
		}

		GravityHandle(gameTime);
		SetupCollider(gameTime);
	}
}

void Item::SetupCollider(float gameTime)
{

	Enemy::SetBox(sprite->GetPosition().x, sprite->GetPosition().y, sprite->GetWidth(),
		sprite->GetHeight(), velocity.x, velocity.y);

	sprite->SetPosition(sprite->GetPosition().x + velocity.x * gameTime,
		sprite->GetPosition().y + velocity.y * gameTime);

	SetPosition(sprite->GetPosition());
	SetRegion(0, sprite->GetWidth(), 0, -sprite->GetHeight());
}

void Item::Render(ViewPort *viewPort)
{
	if (isEnable == true)
		GameObject::Render(viewPort);
}

void Item::Random()
{
	int percent = rand() % 100; // random theo tỉ lệ

	if (percent < 90)
		RandomItem();
	else
		RandomSubWeapon();
}

void Item::RandomItem()
{
	int percent = rand() % 100; // random theo tỉ lệ

	if (percent < 60)
	{
		// If whip level of Simon is max then auto give small heart
		if (simon->GetTypeOfWhip() < 3)
		{
			itemPath = "Resource\\sprites\\Items\\MORNING_STAR.png";
			itemType = ITEM_WHIP;
		}
		else
		{
			itemPath = "Resource\\sprites\\Items\\SMALL_HEART.png";
			itemType = ITEM_SMALL_HEART;
		}
	}
	else if (percent < 70)
	{
		itemPath = "Resource\\sprites\\Items\\BIG_HEART.png";
		itemType = ITEM_BIG_HEART;
	}
	else if (percent < 72)
	{
		itemPath = "Resource\\sprites\\Items\\ROAST.png";
		itemType = ITEM_ROAST;
	}
	else if (percent < 74)
	{
		itemPath = "Resource\\sprites\\Items\\INVICIBILITY_ITEM.png";
		itemType = ITEM_INVISIBLE;
	}
	else if (percent < 80)
	{
		itemPath = "Resource\\sprites\\Items\\Money_bag_white.png";
		itemType = ITEM_MONEY;
	}
	else if (percent < 95)
	{
		if (simon->GetNoSubWeapon() == 1) {
			itemPath = "Resource\\sprites\\Items\\DOUBLE_SHOT.png";
			itemType = ITEM_UPGRADE_1;
		}
		else if (simon->GetNoSubWeapon() == 2) {
			itemPath = "Resource\\sprites\\Items\\TRIPLE_SHOT.png";
			itemType = ITEM_UPGRADE_2;
		}
		else {
			itemPath = "Resource\\sprites\\Items\\BIG_HEART.png";
			itemType = ITEM_BIG_HEART;
		}
	}
	else
	{
		itemPath = "Resource\\sprites\\Items\\ROSARY.png";
		itemType = ITEM_ROSARY;
	}
}

void Item::RandomSubWeapon()
{
	int percent = rand() % 100; // random theo tỉ lệ

	if (percent < 40)
	{
		itemPath = "Resource\\sprites\\Sub_weapons\\AXE.png";
		itemType = ITEM_AXE;
	}
	else if (percent < 80)
	{
		itemPath = "Resource\\sprites\\Sub_weapons\\CROSS.png";
		itemType = ITEM_CROSS;
	}
	else if (percent < 90)
	{
		itemPath = "Resource\\sprites\\Sub_weapons\\HOLY_WATER.png";
		itemType = ITEM_HOLY_WATER;
	}
	else if (percent < 95)
	{
		itemPath = "Resource\\sprites\\Sub_weapons\\KNIFE.png";
		itemType = ITEM_KNIFE;
	}
	else
	{
		itemPath = "Resource\\sprites\\Sub_weapons\\STOP_WATCH.png";
		itemType = ITEM_STOP_WATCH;
	}
}

void Item::AddEnergy(int energy)
{
	simon->SetEnergy(simon->GetEnergy() + energy);
}

void Item::RecoverHP(int hp)
{
	int hpAfterHealed = simon->GetHP() + hp;

	if (hpAfterHealed > 16) {
		simon->SetHP(16);
		simon->SetPrevHP(16);
	}	
	else {
		simon->SetHP(hpAfterHealed);
		simon->SetPrevHP(hpAfterHealed);
	}
}

void Item::UpgradeWhip()
{
	simon->SetTypeOfWhip(simon->GetTypeOfWhip() + 1);
}

void Item::ChangeSubWeapon()
{
	simon->SetTypeOfSubWeapon(itemType);
}

void Item::MakeInvincible(int time)
{
	simon->SetInvincible(time);
}

void Item::AddScore(int point)
{
	Enemy::score += point;
}

void Item::KillAll()
{
	isKillAll = true;
	if (listEnemy != NULL) {
		for (auto i = listEnemy->begin(); i != listEnemy->end(); i++) {
			if ((*i)->IsEnable() && !(*i)->IsDead()) {
				if ((*i)->GetTag() == TAG_PROJECTILE)
					(*i)->SetEnable(false);
				else
					((Enemy*)(*i))->SetHP(0); // Kill enemy
			}
		}
	}
}

void Item::IncreaseSubWeapon(int no)
{
	// .............................
	simon->SetNoSubWeapon(no);
}

void Item::NextStage(float gameTime)
{
	Sound::GetInstance()->Play(eSoundID::STAGE_CLEAR);
	if (Sound::GetInstance()->IsPLaying(eSoundID::BOSS_BATTLE_POISON_MIND))
		Sound::GetInstance()->Stop(BOSS_BATTLE_POISON_MIND);


	if (Sound::GetInstance()->IsPLaying(eSoundID::STAGE_CLEAR))
	{
		simon->SetCanControlKeyboard(false);
		simon->Stand();
	}
	else
		simon->nextStage = true;
}

void Item::StopTime(float time)
{
	Enemy::stopTime = time;
}
