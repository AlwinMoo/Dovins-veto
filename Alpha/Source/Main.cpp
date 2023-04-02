// ---------------------------------------------------------------------------
// includes

#include "AEEngine.h"
#include "../Source/GameStateManager.h"
#include "../Source/GameStateList.h"
#include "../Source/TestScene.h"
#include "UI_Def.h"
// ---------------------------------------------------------------------------
// main
s8 g_fontID;

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPWSTR    lpCmdLine,
	_In_ int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	 
	GSM_Initialize(GS_SPLASH);
	// Using custom window procedure
	//AESysInit(hInstance, nCmdShow, 800, 600, 1, 60, true, NULL);
	AESysInit(hInstance, nCmdShow, 1600, 900, 1, 60, true, NULL);
#if defined(DEBUG) | defined(_DEBUG)
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif

	UI::InitUI();

	// Changing the window title
	AESysSetWindowTitle("Badea 2");
	SetProcessDpiAwarenessContext(DPI_AWARENESS_CONTEXT_PER_MONITOR_AWARE_V2);
	g_fontID = AEGfxCreateFont("Assets/SignikaNegative-Regular.ttf", 16);
	// reset the system modules
	AESysReset();
	// Enable run-time memory check for debug builds.
	// 
	//test_scene::TestScene* mainScene = new test_scene::TestScene();
	//mainScene->Init();
	//s8 defaultFont = AEGfxCreateFont("./Assets/Roboto-Regular.ttf", 0);
	//mainScene->m_fontId = defaultFont;
	// Game Loop
	while (current != GS_QUIT)
	{
		if (current != GS_RESTART)
		{
			GSM_Update();
			fpLoad();
		}
		else
		{
			next = previous;
			current = previous;
		}

		fpInitialize();
		// Informing the system about the loop's start

		// Handling Input

		while (next == current)
		{
			//more steps
			AESysFrameStart();
			AEInputUpdate();
			AEAudioUpdate();
			fpUpdate();
			fpDraw();
			AESysFrameEnd();
			// check if forcing the application to quit
			if (0 == AESysDoesWindowExist())
				next = GS_QUIT;
		}

		fpFree();

		if (next != GS_RESTART)
		{
			fpUnload();
		}

		previous = current;
		current = next;
		

		// Your own rendering logic goes here
		//mainScene->Update();
		//mainScene->Render();
		// Informing the system about the loop's end

	}
	
	//AEGfxDestroyFont(defaultFont);
	UI::UnloadUI();
	// free the system
	AESysExit();
}