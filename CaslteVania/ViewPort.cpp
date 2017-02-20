#include "ViewPort.h"



ViewPort::ViewPort(float x, float y)
{
	this->cameraPosition.x = x;
	this->cameraPosition.y = y;
	this->cameraPosition.z = 0;
}


ViewPort::~ViewPort()
{
}

D3DXVECTOR3 ViewPort::SetPositionInViewPort(D3DXVECTOR3 position)
{
	D3DXMATRIX mt;
	D3DXMatrixIdentity(&mt); //chuyen ma tran mt ve ma tran dong nhat

	//thiet lap ma tran mt thanh ma tran trung gian
	mt._22 = -1.0f;
	mt._41 = -this->cameraPosition.x; 
	mt._42 = this->cameraPosition.y;

	//hamD3DXVec3Transform se thuc hien nhan position voi ma tran trung gian de ra toa do moi
	//Toa do moi se duoc luu trong vp_pos(viewport position)
	//La toa do da duoc chuyen tu toa do the gioi sang toa do trong viewport
	//Dung toa do nay de ve
	D3DXVECTOR4 vp_pos;
	D3DXVec3Transform(&vp_pos, &position, &mt);

	return D3DXVECTOR3(vp_pos.x, vp_pos.y, 0);
}

void ViewPort::SetCameraPosition(float x, float y)
{
	this->cameraPosition.x = x;
	this->cameraPosition.y = y;
}

D3DXVECTOR3 ViewPort::GetCameraPosition()
{
	return this->cameraPosition;
}
