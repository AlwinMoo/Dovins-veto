#include "Tutorial.h"

namespace
{
	AEGfxVertexList* pMesh;
	AEGfxTexture* scene1;
	AEGfxTexture* scene2;
	AEGfxTexture* scene3;
	AEGfxTexture* scene4;
	AEGfxTexture* scene5;
	AEGfxTexture* scene6;
	AEGfxTexture* scene7;
	AEGfxTexture* scene8;
	AEGfxTexture* scene9;
	AEGfxTexture* scene10;
	AEGfxTexture* scene11;

	int counter{};

	UI::UI_Manager* gameUIManager;
	UI::UI_TextAreaTable* textTable;

	// Audio
	AEAudioGroup soundEffects;
	AEAudio clickSound;
	AEAudioGroup music;
	AEAudio BGM;
}

void next_button(UI::UI_Button*);

void tutorial_Load()
{
	scene1 = AEGfxTextureLoad("Assets/tutorial_scene1.png");
	scene2 = AEGfxTextureLoad("Assets/tutorial_scene2.png");
	scene3 = AEGfxTextureLoad("Assets/tutorial_scene3.png");
	scene4 = AEGfxTextureLoad("Assets/tutorial_scene4.png");
	scene5 = AEGfxTextureLoad("Assets/tutorial_scene5.png");
	scene6 = AEGfxTextureLoad("Assets/tutorial_scene6.png");
	scene7 = AEGfxTextureLoad("Assets/tutorial_scene7.png");
	scene8 = AEGfxTextureLoad("Assets/tutorial_scene8.png");
	scene9 = AEGfxTextureLoad("Assets/tutorial_scene9.png");
	scene10 = AEGfxTextureLoad("Assets/tutorial_scene10.png");
	scene11 = AEGfxTextureLoad("Assets/tutorial_scene11.png"); 
	clickSound = AEAudioLoadSound("Assets/Click.wav");
	soundEffects = AEAudioCreateGroup();
	BGM = AEAudioLoadMusic("Assets/tutorialBGM.wav");
	music = AEAudioCreateGroup();
}

void tutorial_Initialize()
{
	AEAudioPlay(BGM, music, 1.f, 1.f, -1);
	pMesh = basic_mesh();

	counter = 1;

	gameUIManager = new UI::UI_Manager{};
	textTable = new UI::UI_TextAreaTable;
	gameUIManager->SetWinDim(static_cast<f32> (AEGetWindowWidth()), static_cast<f32> (AEGetWindowHeight()));

	AEVec2 Button_Pos{ static_cast<f32> (AEGetWindowWidth()) * 0.9f, static_cast<f32> (AEGetWindowHeight()) * 0.1f };
	AEVec2 const buildButtonSize{ 200.f, 70.f };

	gameUIManager->CreateButton(Button_Pos, buildButtonSize, UI::SKILL_TREE_BUTTON,
		&textTable->nextButton, next_button, nullptr); // second last param - > callback. No need hover text
}

void tutorial_Update()
{
	s32 mouseX, mouseY;
	AEInputGetCursorPosition(&mouseX, &mouseY);
	AEVec2 invert_mouse = { static_cast<f32>(mouseX), static_cast<f32>(mouseY) }; // Getting inverted mouse pos to match world space
	invert_mouse.y = gameUIManager->m_winDim.y - mouseY;
	gameUIManager->Update(invert_mouse, AEInputCheckTriggered(AEVK_LBUTTON));
}

void tutorial_Draw()
{
	switch (counter)
	{
	case tutorial::SCENE1:
		generic_draw(pMesh, scene1, 1.f, static_cast<f32> (AEGetWindowWidth()), static_cast<f32> (AEGetWindowHeight()), 0.f, 0.f);
		break;
	case tutorial::SCENE2:
		generic_draw(pMesh, scene2, 1.f, static_cast<f32> (AEGetWindowWidth()), static_cast<f32> (AEGetWindowHeight()), 0.f, 0.f);
		break;
	case tutorial::SCENE3:
		generic_draw(pMesh, scene3, 1.f, static_cast<f32> (AEGetWindowWidth()), static_cast<f32> (AEGetWindowHeight()), 0.f, 0.f);
		break;
	case tutorial::SCENE4:
		generic_draw(pMesh, scene4, 1.f, static_cast<f32> (AEGetWindowWidth()), static_cast<f32> (AEGetWindowHeight()), 0.f, 0.f);
		break;
	case tutorial::SCENE5:
		generic_draw(pMesh, scene5, 1.f, static_cast<f32> (AEGetWindowWidth()), static_cast<f32> (AEGetWindowHeight()), 0.f, 0.f);
		break;
	case tutorial::SCENE6:
		generic_draw(pMesh, scene6, 1.f, static_cast<f32> (AEGetWindowWidth()), static_cast<f32> (AEGetWindowHeight()), 0.f, 0.f);
		break;
	case tutorial::SCENE7:
		generic_draw(pMesh, scene7, 1.f, static_cast<f32> (AEGetWindowWidth()), static_cast<f32> (AEGetWindowHeight()), 0.f, 0.f);
		break;
	case tutorial::SCENE8:
		generic_draw(pMesh, scene8, 1.f, static_cast<f32> (AEGetWindowWidth()), static_cast<f32> (AEGetWindowHeight()), 0.f, 0.f);
		break;
	case tutorial::SCENE9:
		generic_draw(pMesh, scene9, 1.f, static_cast<f32> (AEGetWindowWidth()), static_cast<f32> (AEGetWindowHeight()), 0.f, 0.f);
		break;
	case tutorial::SCENE10:
		generic_draw(pMesh, scene10, 1.f, static_cast<f32> (AEGetWindowWidth()), static_cast<f32> (AEGetWindowHeight()), 0.f, 0.f);
		break;
	case tutorial::SCENE11:
		generic_draw(pMesh, scene11, 1.f, static_cast<f32> (AEGetWindowWidth()), static_cast<f32> (AEGetWindowHeight()), 0.f, 0.f);
		break;
	case tutorial::CHANGE:
		next = GS_MENU;
		break;
	default:
		break;
	}

	s32 cursorX, cursorY;
	AEVec2 FirstPos{ 0.f, 0.f };
	AEInputGetCursorPosition(&cursorX, &cursorY);
	gameUIManager->Draw(cursorX, cursorY);
}

void tutorial_Free()
{
	AEGfxMeshFree(pMesh);
	delete gameUIManager;
	delete textTable;
	AEAudioStopGroup(music);
}

void tutorial_Unload()
{
	AEGfxTextureUnload(scene11);
	AEGfxTextureUnload(scene10);
	AEGfxTextureUnload(scene9);
	AEGfxTextureUnload(scene8);
	AEGfxTextureUnload(scene7);
	AEGfxTextureUnload(scene6);
	AEGfxTextureUnload(scene5);
	AEGfxTextureUnload(scene4);
	AEGfxTextureUnload(scene3);
	AEGfxTextureUnload(scene2);
	AEGfxTextureUnload(scene1);
}

void next_button(UI::UI_Button*)
{
	counter += 1;
	AEAudioPlay(clickSound, soundEffects, 0.6f, 1.f, 0);
}