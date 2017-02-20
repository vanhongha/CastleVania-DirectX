#include "GameState.h"

Simon* GameState::simon = NULL;

void GameState::Update(float gameTime)
{
	if (Item::IsKillAll())
	{
		timeKillAll += gameTime;
		if (timeKillAll < 0.7)
		{
			if ((int)(timeKillAll * 100) % 2 == 0)
				graphics->SetColor(D3DCOLOR_XRGB(255, 255, 255));
			else
				graphics->SetColor(D3DCOLOR_XRGB(0, 0, 0));
		}
		else
		{
			graphics->SetColor(D3DCOLOR_XRGB(0, 0, 0));
			Item::SetIsKillAll(false);
			choose = false;
			timeKillAll = 0;
		}
	}
}