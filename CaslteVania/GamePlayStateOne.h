#pragma once
#include "GameState.h"
#include "TileMap.h"
#include "Simon.h"
#include "QuadTree.h"
#include "Ground.h"
#include "LargeCandle.h"
#include "Zombie.h"
#include "UI.h"
#include "Item.h"
#include "WaterAnimation.h"

class GamePlayStateOne : public GameState
{
private:
	//Simon *simon;
	TileMap *tileMap;

	Sprite *sprite;
	Sprite	*castleSprite;

	GameObject *ground;
	GameObject *cameraObject;
	GameObject *leftCamera;
	GameObject *rightCamera;

	QuadTree *quadTree;
	UI *ui;

	std::vector<GameObject*> *list;
	std::vector<Item*> *listItem;
	LPDIRECT3DDEVICE9 gDevice;

public:
	GamePlayStateOne();
	~GamePlayStateOne();
	bool Initialize(Graphics *graphics);
	void Render();
	void Update(float gameTime);
	void DestroyAll();
	/*
	* Overload tu class cha
	* Set va get gia tri cua $changeState
	*/
	bool GetChangingState();
	void SetChangingState(bool status);
	bool CameraFollowHandle(float gameTime) { return true; }
};

