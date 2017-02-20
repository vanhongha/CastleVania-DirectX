#include "Animation.h"


Animation::Animation()
{
	timeDelay = 0.0f;
	isDoAllFrame = false;
}


Animation::~Animation()
{
}

bool Animation::Initialize(Sprite *sprite, float _frameWidth, float _frameHeight, float _frameTime)
{
	frameTime = _frameTime;
	frameWidth = _frameWidth;
	frameHeight = _frameHeight;

	//Chinh mau lai cho nhan vat, sau nay an item se doi mau
	sprite->setColor(D3DCOLOR_ARGB(255, 255, 255, 255));

	// Set current frame as first frame from animation
	currentFrame = 0;

	//// Dat animation hien tai bang 0
	currentAnimation = 0;

	isChangeAnim = false;
	isChanged = false;

	return TRUE;
}

void Animation::AddAnimation(int animId, std::vector<D3DXVECTOR2> animation)
{
	lAnimation[animId] = animation;
}

void Animation::Update(float gameTime)
{
	isDoAllFrame = false;

	// Dua rect toi vi tri can ve
	D3DXVECTOR2 frame = lAnimation[currentAnimation][currentFrame];
	rect.top = frameHeight * frame.y;
	rect.left = frameWidth * frame.x;
	rect.bottom = frameHeight * (frame.y + 1);
	rect.right = frameWidth  * (frame.x + 1);

	// Update to get elapsed time
	timeDelay += gameTime;
	if (isChangeAnim)
	{
		timeDelay = gameTime;
		D3DXVECTOR2 frame = lAnimation[currentAnimation][currentFrame];
		rect.top = frameHeight * frame.y;
		rect.left = frameWidth * frame.x;
		rect.bottom = frameHeight * (frame.y + 1);
		rect.right = frameWidth  * (frame.x + 1);
	}
	// If total time bigger than the time between two frame then change frame
	else if (timeDelay >= frameTime)
	{
		// Change to next frame
		// If there's no next frame then return to the first frame
		currentFrame++;
		if (currentFrame >= lAnimation[currentAnimation].size())
		{
			currentFrame = 0;
			isDoAllFrame = true;
		}
		
		// Reset total time from Time set
		timeDelay = 0.0f;
	}
}

void Animation::Render(Sprite *sprite, bool isLeft, ViewPort *viewPort)
{
	sprite->Render(rect, isLeft, frameWidth, frameHeight, viewPort);
}

void Animation::DoAnimation(int animId)
{
	// Dua frame tro ve frame dau tien cua animation moi
	if (currentAnimation != animId)
	{
		currentFrame = 0;
		currentAnimation = animId;
		isChangeAnim = TRUE;
	}
	else
		isChangeAnim = FALSE;
}
