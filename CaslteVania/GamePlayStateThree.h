#pragma once
#include "GameState.h"
#include "TileMap.h"
#include "Simon.h"
#include "QuadTree.h"
#include "Ground.h"
#include "LargeCandle.h"
#include "Zombie.h"
#include "UI.h"
#include "MovableGround.h"
#include "Spear_guard.h"
#include "BlueBat.h"
#include "BonePillar.h"
#include "MedusaHead.h"
#include "Ghost.h"

class GamePlayStateThree : public GameState
{
private:
	//Simon *simon;
	TileMap *tileMap;
	Sprite *sprite;
	GameObject *ground;
	GameObject *cameraObject;
	GameObject *leftCamera;
	GameObject *rightCamera;

	QuadTree *quadTree;
	UI *ui;
	bool inBoss;
	MovableGround *movableGround[3];
	BlueBat *blueBat[3];
	BonePillar *bonePillar[3];
	MedusaHead *medusaHead[2];
	Ghost *ghost;


	std::vector<GameObject*> *list;
	std::vector<Item*> *listItem;
	std::vector<GameObject*> *moveList;
	std::vector <GameObject*> *listEnemy;

	Medusa *medusa;
	LPDIRECT3DDEVICE9 gDevice;

public:
	GamePlayStateThree();
	~GamePlayStateThree();
	//bool Initialize(LPDIRECT3DDEVICE9 gDevice);
	bool Initialize(Graphics *graphics);
	void Render();
	void Update(float gameTime);
	void DestroyAll();

	bool IsInCamera(float x, float y);

	/*
	* Overload tu class cha
	* Set va get gia tri cua $changeState
	*/
	bool GetChangingState();
	void SetChangingState(bool status);
	void SimonCollideWithDoorHandle(float gameTime);
	bool CameraFollowHandle(float gameTime);

	/*
		Load object to Game
	*/

	void InitMovableGround();
	void InitBlueBat();
	void InitBonePillar();
	void InitMedusaHead();
	void InitGhost();

	/*
		Update game object
	*/
	void UpdateMovableGround(float gameTime);
	void UpdateBlueBat(float gameTime);
	void UpdateBonePillar(float gameTime);
	void UpdateMedusaHead(float gameTime);
	void UpdateGhost(float gameTime);
};

