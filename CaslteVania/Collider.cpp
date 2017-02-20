#include "Collider.h"

std::vector<Collider*> Collider::listCollider;


Collider::Collider()
{
}

Collider::~Collider()
{

}

Box Collider::GetSweptBoardphaseBox(Box box, float gameTime)
{
	Box broadphasebox;
	broadphasebox.top = box.vy > 0 ? box.top + box.vy * gameTime : box.top;
	broadphasebox.bottom = box.vy > 0 ? box.bottom : box.bottom + box.vy * gameTime;
	broadphasebox.left = box.vx > 0 ? box.left : box.left + box.vx * gameTime;
	broadphasebox.right = box.vx > 0 ? box.right + box.vx * gameTime : box.right;
	broadphasebox.vx = box.vx;
	broadphasebox.vy = box.vy;

	return broadphasebox;
}

bool Collider::AABBCheck(Box b1, Box b2)
{
	return !(b1.right < b2.left || b1.left > b2.right || b1.top < b2.bottom || b1.bottom > b2.top);
}

float Collider::SweptAABB(float gameTime, Box b1, Box b2, float &normalX, float &normalY)
{
	//neu object thu 2 di chuyen
	b1.vx = (b1.vx - b2.vx) * gameTime;
	b1.vy = (b1.vy - b2.vy) * gameTime;

	//khoang cach de hai vat cham vao nhau
	float xInvEntry, yInvEntry;
	float xInvExit, yInvExit;

	if (b1.vx > 0) // Vat b1 di chuyen tu trai qua
	{
		xInvEntry = b2.left - b1.right;
		xInvExit = b2.right - b1.left;
	}
	else // Vat b1 di chuyen tu phai qua
	{
		xInvEntry = b2.right - b1.left;
		xInvExit = b2.left - b1.right;
	}

	if (b1.vy > 0.0f) // Vat di tu duoi len
	{
		yInvEntry = b2.bottom - b1.top; /*Vi xet trong he truc world nen nguoc voi cua thay*/
		yInvExit = b2.top - b1.bottom;
	}
	else // Vat di tu tren xuong
	{
		/*yInvEntry = (b1.y - b1.h) - b2.y;
		yInvExit = b1.y - (b2.y - b2.h);*/
		yInvEntry = b2.top - b1.bottom; /*Vi xet trong he truc world nen nguoc voi cua thay*/
		yInvExit = b2.bottom - b1.top;
	}

	//Thoi gian de va cham theo 2 truc
	float xEntry, yEntry;
	float xExit, yExit;

	if (b1.vx == 0.0f)
	{
		xEntry = -std::numeric_limits<float>::infinity();
		xExit = std::numeric_limits<float>::infinity();
	}
	else
	{
		xEntry = xInvEntry / b1.vx;
		xExit = xInvExit / b1.vx;
	}

	if (b1.vy == 0.0f)
	{
		yEntry = -std::numeric_limits<float>::infinity();
		yExit = std::numeric_limits<float>::infinity();
	}
	else
	{
		yEntry = yInvEntry / b1.vy;
		yExit = yInvExit / b1.vy;
	}

	// Tim thoi gian lon nhat va nho nhat khi va cham 
	float entryTime = std::max(xEntry, yEntry);
	float exitTime = std::min(xExit, yExit);

	// Neu khong co va cham
	if (entryTime > exitTime
		|| (xEntry<0.0f && yEntry<0.0f)
		|| xEntry>1.0f
		|| yEntry>1.0f)
	{
		normalX = 0.0f;
		normalY = 0.0f;
		return 1.0f;
	}
	// Co va cham -> uoc tinh vector phap tuyen
	if (xEntry > yEntry)
	{
		if (xInvEntry < 0.0f) // Dung ve ben phai
		{
			normalX = 1.0f;
			normalY = 0.0f;
		}
		else // Dung ve ben trai
		{
			normalX = -1.0f;
			normalY = 0.0f;
		}
	}
	else
	{
		if (yInvEntry < 0.0f) // Dung o tren
		{
			normalX = 0.0f;
			normalY = 1.0f;
		}
		else // Dung o duoi
		{
			normalX = 0.0f;
			normalY = -1.0f;
		}
	}
	return entryTime;
}


Box Collider::GetBox()
{
	return box;
}

void Collider::SetBox(Box _box)
{
	box = _box;
}

int Collider::GetTag()
{
	return tag;
}

void Collider::SetTag(int _tag)
{
	tag = _tag;
}
