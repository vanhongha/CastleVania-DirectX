#include "WaterAnimation.h"



WaterAnimation::WaterAnimation()
{
}


WaterAnimation::~WaterAnimation()
{
}

void WaterAnimation::Initialzie(LPDIRECT3DDEVICE9 gDevice, int x, int y)
{
	position.x = x;
	position.y = y;
		
	for (int i = 0;i < 3;i++)
	{
		wasOnTop[i] = false;
		waterSprite[i] = new Sprite(100, 300);
		waterSprite[i]->Initialize(gDevice, WATER_SPRITE);
	}
	isEnable = false;
}

void WaterAnimation::Render(ViewPort *viewPort)
{
	for (int i = 0;i < 3;i++)
		waterSprite[i]->Render(viewPort);
}

void WaterAnimation::Update(float gameTime)
{
	for (int i = 0;i < 3;i++)
	{
		waterSprite[i]->SetPosition(waterSprite[i]->GetPosition().x + velocity[i].x * gameTime,
			waterSprite[i]->GetPosition().y + velocity[i].y  * gameTime);
		velocity[i].y += -800 * gameTime;
	}
	position = waterSprite[1]->GetPosition();
}

void WaterAnimation::SetPosition(float x, float y)
{
	for (int i = 0; i < 3; i++)
	{
		waterSprite[i]->SetPosition(x, y);
	}
}

void WaterAnimation::SetDefaultForce()
{
	velocity[0].x = -50;
	velocity[0].y = 280;

	velocity[1].x = 0;
	velocity[1].y = 360;

	velocity[2].x = 50;
	velocity[2].y = 280;
}