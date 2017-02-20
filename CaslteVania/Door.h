#pragma once
#include "GameObject.h"

#define OPENING_GATE	0
#define CLOSING_GATE	1

class Door : public GameObject
{
private:
	bool isOccurred;

	void InitAnim();

	Sprite* spriteDoorAnim;
	Animation *anim;

	bool isDrawable;
	bool isSetPositionAnimation;
public:
	bool isLeft;
	bool settedDoAllFrame;
	int doorAction;

	Door();
	~Door();

	bool Initialize(LPDIRECT3DDEVICE9 gDevice, const char* file, float x, float y, int tag);
	void Render(ViewPort *viewPort);
	void Update(float gameTime);

	void DoAnim();

	void UpdateAnim(float gameTime);

	int GetAction() { return doorAction; }

	void SetAction(int action)
	{
		if (!settedDoAllFrame) 
			anim->SetDoAllFrame(false); 
		settedDoAllFrame = true;
		doorAction = action;
	}

	bool IsDoAllFrame() { return anim->CheckDoAllFrame(); }

	void SetDrawable(bool status) { isDrawable = status; }

	void SetSpriteXPosition(int X);

	bool IsOccurred() { return isOccurred; }
	void SetIsOccurred(bool status) { isOccurred = status; }
};

