#pragma once
#ifndef __DXAUDIO_H__
#define __DXAUDIO_H__

#include "dsutil.h"

int InitDirectSound(HWND);
CSound* LoadSound(char*);
void PlaySound(CSound*);
void LoopSound(CSound*);
void StopSound(CSound*);

#endif // !__DXAUDIO_H__