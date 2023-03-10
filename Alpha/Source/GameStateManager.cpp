#include "AEEngine.h"
#include "GameStateManager.h"
#include "GameStateList.h"
#include "Bevantest.h"
#include "TestScene.h"
#include "Alwintest.h"

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
			fpLoad = TestScene_Load;
			fpInitialize = TestScene_Initialize;
			fpUpdate = TestScene_Update;
			fpDraw = TestScene_Draw;
			fpFree = TestScene_Free;
			fpUnload = TestScene_Unload;
			break;
		case GS_LEVEL3:
			fpLoad = Alwintest_Load;
			fpInitialize = Alwintest_Initialize;
			fpUpdate = Alwintest_Update;
			fpDraw = Alwintest_Draw;
			fpFree = Alwintest_Free;
			fpUnload = Alwintest_Unload;
			break;
		case GS_RESTART:
			break;
		case GS_QUIT:
			break;
		default:
			break;
	}
}