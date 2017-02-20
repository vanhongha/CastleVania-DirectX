#ifndef QUADTREE_H
#define QUADTREE_H

#include "Simon.h"
#include "ViewPort.h"
#include <d3d9.h>
#include <fstream>
#include <vector>
#include <string>
#include <iostream>
#include <map>

#include "LargeCandle.h"
#include "Ground.h"
#include "Stair.h"
#include "Candle.h"
#include "CheckPoint.h"
#include "Spear_guard.h"
#include "ViewPort.h"
#include "Bat.h"
#include "Door.h"
#include "SceneCheck.h"
#include "BonePillar.h"
#include "Ghost.h"
#include "MedusaHead.h"
#include "Crusher.h"

#define MAX_OBJECT_IN_REGION 1
#define MAX_LEVEL 2

class QuadTree
{
protected:
	LPDIRECT3DDEVICE9 gDevice;
	ViewPort* viewPort;
	std::vector<GameObject*> *listObject;
	
	//////////////////////////////////
	int id;
	RECT* region;
	QuadTree** node;

public:
	static std::map<int, GameObject*> mapObject;

	QuadTree(LPDIRECT3DDEVICE9 gDevice, ViewPort *viewPort);
	~QuadTree(void);
	
	void Load(std::string file);
	void LoadQuad(std::string trace, int pos, float left, float top, float right, float bottom, std::vector<GameObject*> *list);
	void LoadObject(int id, int type, float x, float y, int width, int height);

	void Update(float gameTime);
	void Render();

	std::vector<GameObject*> *GetListObject() { return listObject; }


	//////////////////////////////////////////////////////////////////////////////////////////
	void Remove(GameObject* object);
	bool IsContain(GameObject *object);
	bool IsContain(RECT *region);
	void GetObjectList(std::vector<GameObject*> *returnList, RECT *region);
	void GetObjectList(std::vector<GameObject*> *staticList, std::vector<GameObject*> *moveList, RECT *region);
	void GetObjectList(std::vector<GameObject*> *staticList, std::vector<GameObject*> *moveList, RECT *region, GameObject* camera);
};

#endif

