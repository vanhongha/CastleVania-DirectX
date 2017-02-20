#include "CollideEffect.h"



CollideEffect::CollideEffect()
{
}


CollideEffect::~CollideEffect()
{
}

bool CollideEffect::Initialize(LPDIRECT3DDEVICE9 gDevice, const char* file, float x, float y)
{
	if (!sprite)
	{
		sprite = new Sprite(x, y);
		if (!sprite->Initialize(gDevice, file))
			return false;
	}
	timeDelay = 0.0f;
	isVisible = false;
	return true;
}

void CollideEffect::Render(ViewPort *viewPort)
{
	if (isVisible)
		sprite->Render(viewPort);
}

void CollideEffect::Update(float gameTime)
{
	if (isVisible)
	{
		timeDelay += gameTime;
		if (timeDelay > 0.45f)
		{
			timeDelay = 0.0f;
			isVisible = false;
		}
	}
}

void CollideEffect::SetPosition(float x, float y)
{
	sprite->SetPosition(x, y);
}