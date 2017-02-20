#include "QuadTree.h"

QuadTree::QuadTree(LPDIRECT3DDEVICE9 gDevice, ViewPort *viewPort)
{
	this->node = NULL;
	this->gDevice = gDevice;
	this->viewPort = viewPort;
	this->region = new RECT();
}

QuadTree::~QuadTree(void)
{
	for (std::vector<GameObject*>::iterator i = listObject->begin(); i != listObject->end(); i++)
	{
		delete[] (*i);
	}

	delete[] listObject;
}

std::map<int, GameObject*> QuadTree::mapObject;

void QuadTree::Load(std::string file)
{
	std::ifstream input;
	input.open(file, std::ifstream::in);
	
	std::string checkEnd;
	std::string trace;

	int id;
	int type;
	float x;
	float y;
	int width; 
	int height;

	float left;
	float top;
	float right;
	float bottom;
	int n;
	int objectID;

	int count = 0;

	while (input >> checkEnd)
	{ 
		if (checkEnd == "END") 
		{
			count++;
			continue;
		}

		if (count == 0) 
		{
			id = atoi(checkEnd.c_str());
			input >> type >> x >> y >> width >> height;
			LoadObject(id, type, x, y, width, height);
		}
		else if (count == 1)
		{
			id = atoi(checkEnd.c_str());
			input >> left >> top >> right >> bottom >> n;

			std::vector<GameObject*>* list = new std::vector<GameObject*>();
			for (int i = 0; i < n; i++)
			{
				input >> objectID;
				list->push_back(mapObject[objectID]);
			}
			
			trace.clear();

			while (true)
			{
				id--;
				if (id < 0)
					break;
				else
				{
					trace = std::to_string(id % 4) + trace;
					id = id / 4;
				}
			}
			
			trace = trace + "e";

			LoadQuad(trace, 0, left, top, right, bottom, list);
		}
	}
}

void QuadTree::LoadQuad(std::string trace, int pos, float left, float top, float right, float bottom, std::vector<GameObject*> *list)
{
	if (trace[pos] == 'e')
	{
		SetRect(this->region, left, top, right, bottom);
		this->listObject = list;
	}
	else
	{
		if (!this->node)
		{
			node = new QuadTree*[4];
			node[0] = new QuadTree(gDevice, viewPort);
			node[1] = new QuadTree(gDevice, viewPort);
			node[2] = new QuadTree(gDevice, viewPort);
			node[3] = new QuadTree(gDevice, viewPort);
		}

		if (trace[pos] == '0')
			node[0]->LoadQuad(trace, pos + 1, left, top, right, bottom, list);
		else if (trace[pos] == '1')
			node[1]->LoadQuad(trace, pos + 1, left, top, right, bottom, list);
		else if (trace[pos] == '2')
			node[2]->LoadQuad(trace, pos + 1, left, top, right, bottom, list);
		else if (trace[pos] == '3')
			node[3]->LoadQuad(trace, pos + 1, left, top, right, bottom, list);
	}
}

void QuadTree::LoadObject(int id, int type, float x, float y, int width, int height)
{
	if (type == 10)
	{
		GameObject *object;
		object = new LargeCandle();
		object->Initialize(gDevice, "Resource\\sprites\\Ground\\0.png", x, y, type);
		object->SetID(id);
		object->SetSize(width, height);
		object->SetRegion(0, width, 0, -height);
		mapObject[id] = object;
	}
	else if (type == TAG_GROUND)
	{
		GameObject *object;
		object = new Ground(width, height);
		object->Initialize(gDevice, "Resource\\sprites\\Ground\\2.png", x, y, TAG_GROUND);
		object->SetID(id);
		object->SetSize(width, height);
		object->SetRegion(0, width, 0, -height);
		object->SetBox(x, y, width, height, 0.0f, 0.0f);
		mapObject[id] = object;
	}
	else if (type == TAG_GROUND_LEVEL_TWO)
	{
		GameObject *object;
		object = new Ground(width, height);
		object->Initialize(gDevice, "Resource\\sprites\\Ground\\brick.png", x, y, TAG_GROUND);
		object->SetID(id);
		object->SetSize(width, height);
		object->SetRegion(0, width, 0, -height);
		object->SetBox(x, y, width, height, 0.0f, 0.0f);
		mapObject[id] = object;
	}
	else if (type == 120)
	{
		GameObject *object;
		object = new Ground(width, height);
		object->Initialize(gDevice, "Resource\\sprites\\Ground\\ground.png", x, y, TAG_GROUND);
		object->SetID(id);
		object->SetSize(width, height);
		object->SetRegion(0, width, 0, -height);
		object->SetBox(x, y - 5, width, height, 0.0f, 0.0f);
		mapObject[id] = object;
	}
	else if (type == TAG_CHANGE_STATE)
	{
		GameObject *object;
		object = new GameObject();
		object->Initialize(gDevice, "Resource\\sprites\\Ground\\2.png", x, y, type);
		object->SetID(id);
		object->SetBox(x, y, 32, 32, 0.0f, 0.0f);
		mapObject[id] = object;
	}
	else if (type == TAG_STAIR)
	{
		GameObject *object;
		object = new Stair();
		object->Initialize(gDevice, "Resource\\sprites\\Ground\\3.png", x, y, type);
		object->SetID(id);
		object->SetBox(x, y, 32, 32, 0.0f, 0.0f);
		mapObject[id] = object;
	}
	else if (type == 130)
	{
		GameObject *object;
		object = new Stair();
		object->Initialize(gDevice, "Resource\\sprites\\Ground\\stair.png", x, y, TAG_STAIR);
		object->SetID(id);
		object->SetBox(x, y, 32, 32, 0.0f, 0.0f);
		mapObject[id] = object;
	}
	else if (type == 140)
	{
		GameObject *object;
		object = new Stair();
		object->Initialize(gDevice, "Resource\\sprites\\Ground\\top_stair.png", x, y, TAG_STAIR_TOP);
		object->SetID(id);
		object->SetBox(x, y, 32, 32, 0.0f, 0.0f);
		mapObject[id] = object;
	}
	else if (type == TAG_STAIR_)
	{
		GameObject *object;
		object = new Stair();
		object->Initialize(gDevice, "Resource\\sprites\\Ground\\3_.png", x, y, type);
		object->SetID(id);
		object->SetBox(x, y, 32, 32, 0.0f, 0.0f);
		mapObject[id] = object;
	}
	else if (type == TAG_STAIR_TOP)
	{
		GameObject *object;
		object = new Stair();
		object->Initialize(gDevice, "Resource\\sprites\\Ground\\4.png", x, y, TAG_STAIR_TOP);
		object->SetID(id);
		object->SetBox(x, y, 32, 32, 0.0f, 0.0f);
		mapObject[id] = object;
	}
	else if (type == TAG_STAIR_TOP_)
	{
		GameObject *object;
		object = new Stair();
		object->Initialize(gDevice, "Resource\\sprites\\Ground\\4_.png", x, y, TAG_STAIR_TOP);
		object->SetID(id);
		object->SetBox(x, y, 32, 32, 0.0f, 0.0f);
		mapObject[id] = object;
	}
	else if (type == TAG_CANDLE)
	{
		GameObject *object;
		object = new Candle();
		object->Initialize(gDevice, "Resource\\sprites\\Ground\\1.png", x, y, type);
		object->SetID(id);
		object->SetBox(x, y, 32, 32, 0.0f, 0.0f);
		mapObject[id] = object;
	}
	else if (type == 22 || type == -22) // O tren cau thang di xuong
	{
		GameObject *object;
		object = new GameObject();
		object->Initialize(gDevice, "Resource\\sprites\\Ground\\stair_top.png", x, y, type);
		object->SetID(id);
		object->SetBox(x, y, 32, 32, 0.0f, 0.0f);
		mapObject[id] = object;
	}
	else if (type == 25 || type == -25) // O duong dat di len cau thang
	{
		GameObject *object;
		object = new GameObject();
		object->Initialize(gDevice, "Resource\\sprites\\Ground\\stair_bottom.png", x, y, type);
		object->SetID(id);
		object->SetBox(x, y, 32, 32, 0.0f, 0.0f);
		mapObject[id] = object;
	}
	else if (type == TAG_CHECK_STAIR)
	{
		GameObject *object;
		object = new SceneCheck();
		object->Initialize(gDevice, "Resource\\sprites\\Ground\\13.png", x, y, type);
		object->SetID(id);
		object->SetBox(x - 15, y, 96, 32 - 30, 0.0f, 0.0f);
		mapObject[id] = object;
	}
	else if (type == TAG_CRUSHER)
	{
		Crusher *object;
		object = new Crusher();
		object->Initialize(gDevice, x, y);
		object->SetID(id);
		mapObject[id] = object;
	}
	else if (type == 27)
	{
		GameObject *object;
		object = new GameObject();
		object->Initialize(gDevice, "Resource\\sprites\\Ground\\2.png", x, y, type);
		object->SetID(id);
		object->SetBox(x, y, 32, 32, 0.0f, 0.0f);
		mapObject[id] = object;
	}
	else if (type == 0)
	{
		GameObject *object;
		object = new Bat(1, 2, 100);
		object->Initialize(gDevice, "Resource\\sprites\\Ground\\2.png", x, y, type);
		object->SetID(id);
		object->SetBox(x, y, 32, 32, 0.0f, 0.0f);
		mapObject[id] = object;
	}
	else if (type == TAG_DOOR)
	{
		GameObject *object;
		object = new Door();
		object->Initialize(gDevice, "Resource\\sprites\\Ground\\10.png", x, y, type);
		object->SetID(id);
		mapObject[id] = object;
	}
	else if (type == TAG_CHECK_POINT)
	{
		GameObject *object;
		object = new CheckPoint();
		object->Initialize(gDevice, "Resource\\sprites\\Ground\\wall.png", x, y, type);
		object->SetID(id);
		object->SetBox(x, y, 32, 64, 0.0f, 0.0f);
		mapObject[id] = object;
	}
	else if (type == TAG_SPEAR_GUARD)
	{
		GameObject *object;
		object = new Spear_guard(8, 2, 200);
		object->Initialize(gDevice, "Resource\\sprites\\Enemies\\SPEAR_GUARD.png", x, y, type);
		object->SetID(id);
		object->SetSize(width, height);
		object->SetRegion(0, width, 0, -height);
		object->SetBox(x, y, width, height, 0.0f, 0.0f);
		object->SetEnable(false);
		object->SetMoveable(true);
		mapObject[id] = object;
	}
}

void QuadTree::Remove(GameObject* object)
{
	if (this->node)
	{
		if (this->node[0]->IsContain(object))
			this->node[0]->Remove(object);

		if (this->node[1]->IsContain(object))
			this->node[1]->Remove(object);

		if (this->node[2]->IsContain(object))
			this->node[2]->Remove(object);

		if (this->node[3]->IsContain(object))
			this->node[3]->Remove(object);
	}
	else if (this->IsContain(object))
	{
		for (std::vector<GameObject*>::iterator i = this->listObject->begin(); i != this->listObject->end(); i++)
		{
			if ((*i) == object)
			{
				this->listObject->erase(i);
				break;
			}
		}
	}
}

void QuadTree::Update(float gameTime)
{
	for (std::vector<GameObject*>::iterator i = listObject->begin(); i != listObject->end(); i++)
	{
		(*i)->Update(gameTime);
	}
	
}

void QuadTree::Render()
{
	for (std::vector<GameObject*>::iterator i = listObject->begin(); i != listObject->end(); i++)
	{
		if ((*i)->IsEnable())
			(*i)->Render(this->viewPort);
	}
}


bool QuadTree::IsContain(GameObject *object)
{
	return object->IsInRegion(this->region);
}

bool QuadTree::IsContain(RECT *region)
{
	if (this->region->right < region->left || this->region->left > region->right ||
		this->region->bottom > region->top || this->region->top < region->bottom)
		return false;
	return true;
}

void QuadTree::GetObjectList(std::vector<GameObject*> *staticList, std::vector<GameObject*> *moveList, RECT *region)
{
	if (this->node)
	{
		if (this->node[0]->IsContain(region))
			this->node[0]->GetObjectList(staticList, moveList, region);

		if (this->node[1]->IsContain(region))
			this->node[1]->GetObjectList(staticList, moveList, region);

		if (this->node[2]->IsContain(region))
			this->node[2]->GetObjectList(staticList, moveList, region);

		if (this->node[3]->IsContain(region))
			this->node[3]->GetObjectList(staticList, moveList, region);
	}
	else if (this->IsContain(region))
	{
		for (std::vector<GameObject*>::iterator i = this->listObject->begin(); i != this->listObject->end(); i++)
		{
			if ((*i)->IsMoveable() == true)
			{
				if (((Enemy*)(*i))->isIn == false && (*i)->IsEnable() == false)
				{
					(*i)->SetEnable(true);
					moveList->push_back(*i);
					((Enemy*)(*i))->isIn = true;
				}
			}
			else
				staticList->push_back(*i);
		}
	}
}

void QuadTree::GetObjectList(std::vector<GameObject*> *staticList, std::vector<GameObject*> *moveList, RECT *region, GameObject* camera)
{
	if (this->node)
	{
		if (this->node[0]->IsContain(region))
			this->node[0]->GetObjectList(staticList, moveList, region);

		if (this->node[1]->IsContain(region))
			this->node[1]->GetObjectList(staticList, moveList, region);

		if (this->node[2]->IsContain(region))
			this->node[2]->GetObjectList(staticList, moveList, region);

		if (this->node[3]->IsContain(region))
			this->node[3]->GetObjectList(staticList, moveList, region);
	}
	else if (this->IsContain(region))
	{
		for (std::vector<GameObject*>::iterator i = this->listObject->begin(); i != this->listObject->end(); i++)
		{
			if ((*i)->IsMoveable() == true)
			{
				if (((Enemy*)(*i))->isIn == false && (*i)->IsEnable() == false)
				{
					if ((*i)->IsInRegion(camera->GetRegion()))
					{
						(*i)->SetEnable(true);
						moveList->push_back(*i);
						((Enemy*)(*i))->isIn = true;
					}
				}
			}
			else
				staticList->push_back(*i);
		}
	}
}


void QuadTree::GetObjectList(std::vector<GameObject*> *returnList, RECT *region)
{
	if (this->node)
	{
		if (this->node[0]->IsContain(region))
			this->node[0]->GetObjectList(returnList, region);

		if (this->node[1]->IsContain(region))
			this->node[1]->GetObjectList(returnList, region);

		if (this->node[2]->IsContain(region))
			this->node[2]->GetObjectList(returnList, region);

		if (this->node[3]->IsContain(region))
			this->node[3]->GetObjectList(returnList, region);
	}
	else if (this->IsContain(region))
	{
		for (std::vector<GameObject*>::iterator i = this->listObject->begin(); i != this->listObject->end(); i++)
			/*if ((*i)->isAdded == false) {*/
				returnList->push_back(*i);
				/*(*i)->isAdded = true;
			}*/
	}
}