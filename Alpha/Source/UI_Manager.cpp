#include "UI_Manager.h"
#include "Rendering.hpp"
#include <iostream>
extern s8 g_fontID;
namespace UI
{
	/*________________________________________________*/
	// CONSTANT VARIABLES
	const float BUTTON_HOVER_ALPHA{ 0.3f };
	const float HOVER_RGBA[4]{ 0.541f, 0.376f, 0.f, 1.f };
	//const float BUTTON_NORMAL_ALPHA{ 0.f };
	/*________________________________________________*/
	void UI_Manager::ConvertToWS(UI_Button* newButton)
	{
		newButton->wsPos.x = newButton->pos.x - m_winDim.x * 0.5f;
		newButton->wsPos.y = newButton->pos.y - m_winDim.y * 0.5f;

		newButton->wsMin.x = newButton->min.x - m_winDim.x * 0.5f;
		newButton->wsMax.x = newButton->max.x - m_winDim.x * 0.5f;
		newButton->wsMin.y = newButton->min.y + m_winDim.y * 0.5f;
		newButton->wsMax.y = newButton->max.y + m_winDim.y * 0.5f;
	}
	UI_Button* UI_Manager::CreateButton(AEVec2 pos, AEVec2 size, BUTTON_TYPE type, UI_TextArea* buttonText, void(*callback)(), UI_TextArea* hoverText)
	{
		UI_Button* newButton = new UI_Button();
		newButton->scale = size;
		newButton->callback = callback;
		newButton->hoverText = hoverText;
		newButton->buttonText = buttonText;
		newButton->pos = pos;
		
		newButton->min.x = pos.x - size.x * 0.5f;
		newButton->max.x = pos.x + size.x * 0.5f;
		newButton->min.y = pos.y - size.y * 0.5f;
		newButton->max.y = pos.y + size.y * 0.5f;
		ConvertToWS(newButton);
		switch (type) {
		case BUILD_TOWER_BUTTON:
			newButton->texID = TEX_TOWER;
			newButton->meshID = MESH_BOX;
			break;
		case BUILD_NEXUS_BUTTON:
			newButton->texID = TEX_NEXUS;
			newButton->meshID = MESH_BOX;
			break;
		case BUILD_WALL_BUTTON:
			newButton->texID = TEX_WALL;
			newButton->meshID = MESH_BOX;
			break;
		case WHITE_BUTTON:
			newButton->texID = TEX_BUTTON;
			newButton->meshID = MESH_BOX;
			break;
		case END_PHASE_BUTTON:
			newButton->texID = TEX_END_PHASE;
			newButton->meshID = MESH_BOX;
			break;
		case ERASE_BUTTON:
			newButton->texID = TEX_ERASE;
			newButton->meshID = MESH_BOX;
			break;
		case BUILD_PLAYER_BUTTON:
			newButton->texID = TEX_PLAYER;
			newButton->meshID = MESH_BOX;
			break;
		default:
			break;
		}
		m_buttons.push_back(newButton);
		return newButton;
	}

	void UI_Manager::Load()
	{
		
	}

	void UI_Manager::Unload()
	{
		
	}

	void UI_Manager::SetWinDim(f32 x, f32 y)
	{
		m_winDim.x = x;
		m_winDim.y = y;
	}
	void UI_Manager::Update(AEVec2 mousePos, bool lClick)
	{
		for (UI_Button* curr : m_buttons) {
			curr->bHovering = false;

#ifdef DEBUG_UI
			std::cout << "MOUSEPOS = " << mousePos.x << ", " << mousePos.y << '\n';
			std::cout << "CURRMIN  = " << curr->min.x << ", " << curr->min.y << '\n';
			std::cout << "CURRMAX  = " << curr->max.x << ", " << curr->max.y << '\n';
			std::cout << std::endl;
#endif

			if (mousePos.x > curr->max.x || mousePos.x < curr->min.x || mousePos.y > curr->max.y || mousePos.y < curr->min.y)
				continue;
			// CLICKING LOGIC
			if (lClick)
			{
				// Set any bools?
				if (curr->callback)
					curr->callback();
			}
			// HOVER LOGIC
			else
			{
				curr->bHovering = true;
			}
		}
	}

	void UI_Manager::Draw(s32 mouseX, s32 mouseY)
	{
		f32 winX{ AEGfxGetWinMaxX() - AEGfxGetWinMinX() }, winY{ AEGfxGetWinMaxY() - AEGfxGetWinMinY() };
		f32 mouseXN{ mouseX / winX * 2 - 1.f }, mouseYN{ mouseY / winY * -2 + 1.f }; // CALCULATE NORMALIZED COORDINATES
		// Now safe to recalculate mouse pos to worldspace coordinates
		mouseX -= static_cast<s32>(winX)/ 2;
		mouseY -= static_cast<s32>(winY) / 2;
		for (UI_Button* curr : m_buttons) {
			AEGfxSetBlendColor(1.f, 1.f, 1.f, (curr->bHovering ? BUTTON_HOVER_ALPHA: 0.f));
			RenderMesh(curr->wsPos, curr->scale, curr->texID); // Render the mesh
			
		}
		AEGfxSetBlendColor(0.f, 0.f, 0.f, 0.f);

		// NOW DRAW DESCRIPTIONS (IF ANY)
		for (UI_Button* curr : m_buttons) {
			// UI TEST
			// Render text if hovering
			AEGfxSetBlendMode(AEGfxBlendMode::AE_GFX_BM_BLEND);
			if (curr->bHovering && curr->hoverText)
			{
				AEGfxSetBlendColor(HOVER_RGBA[0], HOVER_RGBA[1], HOVER_RGBA[2], HOVER_RGBA[3]);
				//RenderMesh(curr->wsPos, curr->scale, curr->texID); // DRAW THE QUAD BEHIND CURSOR (text box)
				AEGfxSetRenderMode(AE_GFX_RM_TEXTURE);
				AEGfxSetTintColor(1.f, 1.f, 1.f, 1.0f);
				AEGfxSetBlendMode(AE_GFX_BM_BLEND);
				AEGfxSetTransparency(1.f);
				AEGfxTextureSet(TextureList[TEX_BUTTON], 0, 0);

				AEMtx33 scale = { 0 };
				AEMtx33Scale(&scale, curr->hoverText->GetBoxWidth() * 0.5f, curr->hoverText->GetBoxHeight() * 0.5f);

				AEMtx33 rotate = { 0 };
				AEMtx33Rot(&rotate, 0.f);

				AEMtx33 translate = { 0 };
				AEMtx33Trans(&translate, static_cast<f32>(mouseX + curr->hoverText->GetBoxWidth() * 0.25), 
										static_cast<f32>(-mouseY) - curr->hoverText->GetBoxHeight() * 0.25f);

				AEMtx33 transform = { 0 };
				AEMtx33Concat(&transform, &rotate, &scale);
				AEMtx33Concat(&transform, &translate, &transform);

				AEGfxSetTransform(transform.m);
				AEGfxMeshDraw(MeshList[MESH_BOX], AE_GFX_MDM_TRIANGLES);
				// NOW DRAW THE TEXT
				curr->hoverText->Draw(mouseXN, mouseYN, 0.f, 0.f, 0.f);
				AEGfxSetBlendColor(0.f, 0.f, 0.f, 0.f);
			}
		}
	}
	UI_Manager::UI_Manager()
	{
		Load();
		m_buttons = {};
	}

	UI_Manager::~UI_Manager()
	{
		Unload();
	}
}