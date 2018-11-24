#ifndef __UI_Performance_h__
#define __UI_Performance_h__

#include "UI.h"
#include "SDL.h"

class UI_Performance : public UI
{
public:
	UI_Performance(char* name);
	~UI_Performance();

	void Draw();

	void UpdateFramerates();

public:
	int frames;
	double lastFrameTime;
	double lastSecondTime;
	int logMSIterator;
	int logFPSIterator;
	float* fps_log = NULL;
	float* ms_log = NULL;

};

#endif // !__UI_Performance_h__