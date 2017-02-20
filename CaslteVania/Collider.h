#pragma once
#include <limits>
#include <algorithm>
#include <vector>

typedef struct BOX
{
	float left;
	float right;
	float top;
	float bottom;
	float vx;
	float vy;
}Box;

class Collider
{
private:
	int sideX, sideY;
	Box box;
	int tag;
public:
	static std::vector<Collider*> listCollider;

	Collider();
	~Collider();

	Box GetSweptBoardphaseBox(Box box, float gameTime);
	bool AABBCheck(Box b1, Box b2);
	float SweptAABB(float gameTime, Box b1, Box b2, float &normalX, float &normalY);

	Box GetBox();
	void SetBox(Box _box);

	int GetTag();
	void SetTag(int _tag);
	int getSideX() { return sideX; }
	int getSideY() { return sideY; }
};

