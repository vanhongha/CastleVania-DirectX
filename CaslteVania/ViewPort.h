#pragma once
#include <d3dx9.h>
#include "define.h"

class ViewPort
{
private:
	D3DXVECTOR3 cameraPosition;     //Toa do cua camera trong world
	RECT *region;
public:
	ViewPort(float x, float y);
	~ViewPort();

	//Chuyen tu toa do trong the gioi thuc sang toa do trong viewport
	D3DXVECTOR3 SetPositionInViewPort(D3DXVECTOR3 position);

	//Set toa do cua camera trong world(di chuyen camera)
	void SetCameraPosition(float x, float y);

	//Lay toa do cua camera trong the gioi thuc
	D3DXVECTOR3 GetCameraPosition();
};

