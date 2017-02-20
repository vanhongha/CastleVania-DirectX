#include "GameObject.h"

int GameObject::GetTag()
{
	return tag;
}

Collider* GameObject::GetCollider()
{
	return collider;
}

void GameObject::SetBox(float x, float y, float w, float h, float vx, float vy)
{
	// Xay dung box collider cho game object
	Box boxCollider;
	boxCollider.top = y;
	boxCollider.bottom = y - h;
	boxCollider.left = x;
	boxCollider.right = x + w;
	boxCollider.vx = vx;
	boxCollider.vy = vy;

	collider->SetBox(boxCollider);
	////////////////////////////////////////
}

D3DXVECTOR3 GameObject::GetVelocity()
{
	return D3DXVECTOR3(0.0f, 0.0f, 0.0f);
}


void GameObject::UpdateEffect(float gameTime)
{
	deadEffect->SetPosition(sprite->GetPosition().x, sprite->GetPosition().y);
	deadEffect->Update(gameTime);
	isDead = !deadEffect->CheckDoAllFrame();
	if (!isDead)
		isEnable = false;
}

GameObject::GameObject()
{
	sprite = NULL;
	region = NULL;
	if (!region)
		region = new RECT();
}

GameObject::~GameObject()
{
	if (sprite)
	{
		delete(sprite);
	}
	if (collider)
		delete(collider);
	if (region)
		delete(region);
}

void GameObject::SetRegion(int left, int right, int top, int bottom)
{
	region->left = position.x + left;
	region->right = position.x + right;
	region->top = position.y + top;
	region->bottom = position.y + bottom;
}

// Is this object's region in another region
bool GameObject::IsInRegion(RECT *region)
{
	if (this->region->right < region->left || this->region->left > region->right ||
		this->region->bottom > region->top || this->region->top < region->bottom)
		return false;
	else
		return true;
}

bool GameObject::Initialize(LPDIRECT3DDEVICE9 _gDevice, const char* _file, float _x, float _y, int tag)
{
	id = 0;
	isEnable = true;
	isDropItem = false;
	isMoveable = false;
	trigger = false;
	isAdded = false;

	if (!sprite)
	{
		sprite = new Sprite(_x, _y);
		if (!sprite->Initialize(_gDevice, _file))
			return false;
	}

	collider = new Collider();

	// Gan tag cho doi tuong
	collider->SetTag(tag);
	this->tag = tag;

	SetSize(sprite->GetWidth(), sprite->GetHeight());
	SetPosition(sprite->GetPosition());
	SetRegion(0, width, 0, -height);

	isDead = false;

	deadEffect = new DeadAnimation();
	deadEffect->Initialize(_gDevice, DEAD_SPRITE, 0, 0);

	collideEffect = new CollideEffect();
	collideEffect->Initialize(_gDevice, COLLIDE_EFFECT_SPRITE, _x, _y - 20);

	// Make copy
	positionC = position;
	regionC = new RECT(*region);

	return true;
}

bool GameObject::InitSprite(LPDIRECT3DDEVICE9 gDevice, const char * file, float x, float y)
{
	if (!sprite)
	{
		sprite = new Sprite(x, y);
		if (!sprite->Initialize(gDevice, file))
			return false;
	}
	return true;
}

void GameObject::SetSize(float width, float height)
{
	this->width = width;
	this->height = height;
}

void GameObject::SetPosition(D3DXVECTOR3 position)
{
	this->position = position;
}

void GameObject::SetPosition(float x, float y)
{
	sprite->SetPosition(x, y);
}

void GameObject::Render(ViewPort *viewPort)
{
	if (isEnable)
		sprite->Render(viewPort);
}

void GameObject::Update(float gameTime)
{

}

void GameObject::Reload()
{
	isEnable = false;
	isInCamera = false;
	trigger = false;
	sprite->SetPosition(positionC);
	*region = *regionC;
}

bool GameObject::IsInRegionC(RECT *region)
{
	if (this->regionC->right < region->left || this->regionC->left > region->right ||
		this->regionC->bottom > region->top || this->regionC->top < region->bottom)
		return false;
	else
		return true;
}

void GameObject::SetEnable(bool _isEnable)
{
	if (_isEnable)
		isDead = false;
	this->isEnable = _isEnable;
}