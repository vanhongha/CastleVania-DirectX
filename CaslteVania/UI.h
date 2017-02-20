#pragma once
#include "d3dx9.h"
#include "ViewPort.h"
#include "define.h"
#include <string>
#include "Sprite.h"
#include <vector>
#include "Simon.h"

class UI
{
private:
	ID3DXFont *font;
	RECT rect;
	Simon *simon;

	std::string information;

	std::vector<Sprite*> *simonHPList;
	std::vector<Sprite*> *enemyHPList;
	std::vector<Sprite*> *noHPList;

	Sprite* heart;
	Sprite* weapon;
	Sprite* black;
	Sprite* axe;
	Sprite* holyWater;
	Sprite* cross;
	Sprite* knife;
	Sprite* stopWatch;
	
	Sprite* upgrade1;
	Sprite* upgrade2;

	int simonHP;
	int bossHP;
	int uiHP;

	int score;
	int time;
	int item;
	int energy;
	int life;
	int stage;
	int subWeapon;

public:
	UI();
	~UI();
	bool Initialize(LPDIRECT3DDEVICE9 gDevice, Simon *simon, int bossHP);
	void Update(int bossHP, int time, int life, int stage);
	void Render();
};