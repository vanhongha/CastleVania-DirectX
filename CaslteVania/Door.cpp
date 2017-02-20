#include "Door.h"



Door::Door()
{
}


Door::~Door()
{
}


void Door::InitAnim()
{
	anim = new Animation();
	anim->Initialize(spriteDoorAnim, 50, 96, 0.2);

	std::vector<D3DXVECTOR2> openningGate;
	openningGate.push_back(D3DXVECTOR2(0, 0));
	openningGate.push_back(D3DXVECTOR2(1, 0));

	std::vector<D3DXVECTOR2> closingGate;
	closingGate.push_back(D3DXVECTOR2(1, 0));
	closingGate.push_back(D3DXVECTOR2(1, 0));
	closingGate.push_back(D3DXVECTOR2(0, 0));

	anim->AddAnimation(OPENING_GATE, openningGate);
	anim->AddAnimation(CLOSING_GATE, closingGate);

	doorAction = OPENING_GATE;
}

bool Door::Initialize(LPDIRECT3DDEVICE9 gDevice, const char * file, float x, float y, int tag)
{
	if (!GameObject::Initialize(gDevice, file, x, y, tag))
		return false;

	isOccurred = false;
	isDrawable = false;
	settedDoAllFrame = false;
	isSetPositionAnimation = false;
	spriteDoorAnim = new Sprite(x - 16, y);
	if (!spriteDoorAnim->Initialize(gDevice, DOOR_SPRITE))
		return false;

	InitAnim();

	return true;
}

void Door::Render(ViewPort *viewPort)
{
	if (isDrawable)
	{
		anim->DoAnimation(doorAction);
		anim->Render(spriteDoorAnim, isLeft, viewPort);
	}
	else sprite->Render(viewPort);
}

void Door::Update(float gameTime)
{
	SetBox(sprite->GetPosition().x - 15,
		sprite->GetPosition().y,
		16 + 30,
		96,
		0,
		0);

	if (isOccurred)
		collider->SetTag(TAG_GROUND);
}

void Door::UpdateAnim(float gameTime)
{
	anim->Update(gameTime);
}

void Door::DoAnim()
{
	anim->SetDoAllFrame(false);
	anim->DoAnimation(doorAction);
}

void Door::SetSpriteXPosition(int X)
{
	if (!isSetPositionAnimation)
		spriteDoorAnim->SetPosition(D3DXVECTOR3(spriteDoorAnim->GetPosition().x + X, spriteDoorAnim->GetPosition().y, 0));
	isSetPositionAnimation = true;
}