/******************************************************************************/
/*!
\file		Main.cpp
\author		Bevan Lim
\author		Alonzo Nalpon
\author		Alvin Yeo
\author		Alwin Moo
\par        email: moo.g@digipen.edu
\par        email: a.yeo@digipen.edu
\par        email: bevanzhiwe.lim@digipen.edu
\par        email: a.nalpon@digipen.edu
\date       January 10, 2023
\brief		Alwin (25%), Alvin (25%), Bevan (25%), Alonzo (25%)
			Main Game Loop

Copyright (C) 2023 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
 */
 /******************************************************************************/
// ---------------------------------------------------------------------------
// includes

#include "AEEngine.h"
#include "../Source/GameStateManager.h"
#include "../Source/GameStateList.h"
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

	AESysInit(hInstance, nCmdShow, 1600, 900, 0, 60, true, NULL);
	HICON hWindowIcon = NULL;
	hWindowIcon = LoadIcon(hInstance, MAKEINTRESOURCE(101));  //101 is your new icon ID that you added into the reources. 
	SendMessage(AESysGetWindowHandle(), WM_SETICON, ICON_SMALL, (LPARAM)hWindowIcon);
	// Mem leak tests
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
	}
	
	UI::UnloadUI();
	// free the system
	AESysExit();
}