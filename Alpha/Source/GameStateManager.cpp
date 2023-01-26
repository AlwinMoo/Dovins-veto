#include "AEEngine.h"
#include "GameStateManager.h"
#include "GameStateList.h"
#include "Bevantest.h"

int current = 0, previous = 0, next = 0;

FP fpLoad = nullptr, fpInitialize = nullptr, fpUpdate = nullptr, fpDraw = nullptr, fpFree = nullptr, fpUnload = nullptr;

void GSM_Initialize(int startingState)
{
	current = previous = next;
}

void GSM_Update()
{
	switch (current)
	{
	case GS_LEVEL1:
		fpLoad = Bevantest_Load;
		fpInitialize = Bevantest_Initialize;
		fpUpdate = Bevantest_Update;
		fpDraw = Bevantest_Draw;
		fpFree = Bevantest_Free;
		fpUnload = Bevantest_Unload;
		break;
	case GS_LEVEL2:
		fpLoad = Level2_Load;
		fpInitialize = Level2_Initialize;
		fpUpdate = Level2_Update;
		fpDraw = Level2_Draw;
		fpFree = Level2_Free;
		fpUnload = Level2_Unload;
		break;
	case GS_RESTART:
		break;
	case GS_QUIT:
		break;
	default:
		break;
	}
}