#pragma once
#include <map>
#include "dxsound.h"

enum eSoundID
{
	USING_WHIP,
	SLASH_WATER,
	SELECT,
	PAUSE,
	OPEN_DOOR,
	HOLY_WATER,
	HOLY_CROSS,
	HIT_CANNON,
	HIT_SOUND,
	FALLING_DOWN_WATER_SUFACE,
	FALLING,
	DECREASE_WEAPON_USE_POINT,
	COLLECT_WEAPON,
	COLLECT_ITEM,
	BOSS_BATTLE_POISON_MIND,
	GAME_OVER,
	GAME_START_PROLOGUE,
	LIFE_LOST,
	STAGE_01_VAMPIRE_KILLER,
	STAGE_04_STALKER,
	STAGE_CLEAR,
	TITLE_THEME_PRELUDE,
	GO_INTO_CASTLE,
	SIMON_HURTED,
	EAT_CHANGE_STATE_BALL,
	MONEY
};

class Sound
{
private:
	std::map<eSoundID, CSound*> listSound;
	Sound();
	static Sound* instance;
public:

	~Sound();
	static Sound* GetInstance();
	void loadSound(HWND hwnd);
	void Play(eSoundID);
	void Stop(eSoundID);
	void PlayLoop(eSoundID);
	bool IsPLaying(eSoundID);
};

