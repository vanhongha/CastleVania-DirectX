#pragma once
#include "Enemy.h"
#include <time.h> // use for random
#include "ViewPort.h"
#include "Simon.h"
#include "define.h"

class Item : public Enemy
{
protected:
	bool changedState;
	std::string itemPath;
	int itemType;
	static Simon *simon;
	float remainTime; // Time to disapear after being initialized
	static std::vector<GameObject*> *listEnemy;
	static bool isKillAll;
	float stateClearTime;
public:
	Item();
	~Item();

	void Initialize(LPDIRECT3DDEVICE9 gDevice, float x, float y);
	void Initialize(LPDIRECT3DDEVICE9 gDevice, float x, float y, int itemType, float remainTime);

	static void TakeSimonPointer(Simon *simon);
	static void TakeListEnemyPointer(std::vector<GameObject*> *listEnemy); // Use for KillAll()

	void Update(float gameTime);
	void Render(ViewPort *viewPort);

	void SetupCollider(float gameTime);

	void Random();
	void RandomItem();
	void RandomSubWeapon();

	void AddEnergy(int energy); // SMALL_HEART || BIG_HEART : Add energy to Simon
	void RecoverHP(int hp); // ROAST : Simon's HP after healed
	void UpgradeWhip(); // WHIP : Upgrade whip for Simon
	void ChangeSubWeapon(); // ALL SUB WEAPON : Change sub weapon after take item, USE TAG
	void MakeInvincible(int time); // INVISIBLE : Make simon invincible to enemy
	void AddScore(int point); // MONEY : Add more point to score
	void KillAll(); // ROSARY : Kill all enemy in camera
	void IncreaseSubWeapon(int no); // DOUBLE/TRIPPLE SHOT : Increase number of subweapon use onscreen
	void NextStage(float gameTime); // BALL : Heal Simon and change stage
	void StopTime(float time);

	static bool IsKillAll() { return isKillAll; }
	static void SetIsKillAll(bool _isKillAll) { isKillAll = _isKillAll; }
};

