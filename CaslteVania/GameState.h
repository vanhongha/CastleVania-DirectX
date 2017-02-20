#ifndef __GAMESTATE_H__
#define __GAMESTATE_H__
#include "ViewPort.h"
#include <fstream>
#include <vector>

#include "QuadTree.h"
#include "Sound.h"
#include "Item.h"
#include "Simon.h"

static int score;

class GameState
{
	float simonDeadTime = 1.5;
protected:

	ViewPort *viewPort;
	std::vector<GameObject*> *listObject;
	bool changeState;
	float time;
	D3DXVECTOR3 simonCheckpoint;
	D3DXVECTOR3 cameraCheckpoint;
	bool canUseKeyboard;
	static Simon* simon;

	float timeKillAll;
	bool choose;
	Graphics *graphics;
public:	
	int state;
	GameState() {}
	~GameState() {}
	//virtual bool Initialize(LPDIRECT3DDEVICE9 gDevice) = 0;
	virtual bool Initialize(Graphics *graphics) = 0;
	virtual void Render() {};
	virtual void Update(float gameTime);
	virtual void DestroyAll() = 0;
	
	/*
	* 2 cai nay dung de chuyen scene
	*/
	virtual bool GetChangingState() { return changeState; };
	virtual void SetChangingState(bool status) { changeState = status; }


	void CheckPointHandle(float gameTime, Simon* simon, std::vector<GameObject*> *list)
	{
		if (simon->IsColliderWithCheckPoint(gameTime, list))
		{
			simonCheckpoint = simon->GetPosition();
			cameraCheckpoint = viewPort->GetCameraPosition();
		}
			
		if (simon->GetPosition().y < viewPort->GetCameraPosition().y - GAME_HEIGHT) {
			simon->Reset(simonCheckpoint);
			viewPort->SetCameraPosition(cameraCheckpoint.x, cameraCheckpoint.y);
		}

		if (!simon->IsEnable())
		{
			//simonDeadTime -= gameTime;
			//if (simonDeadTime <= 0)
			{
			//	simonDeadTime = 1.5;
			//	simon->SetAction(DEAD);
				if (state == GAME_PLAY_STATE_TWO)
					Sound::GetInstance()->PlayLoop(STAGE_01_VAMPIRE_KILLER);
				else
					Sound::GetInstance()->PlayLoop(STAGE_04_STALKER);
				simon->Reset(simonCheckpoint);
				viewPort->SetCameraPosition(cameraCheckpoint.x, cameraCheckpoint.y);
			}
		}
	}
	virtual bool CameraFollowHandle(float gameTime) = 0;
};

#endif __GAMESTATE_H__
