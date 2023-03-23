#include "UI_Manager.h"
#include "Rendering.hpp"
#include <iostream>
extern s8 g_fontID;
namespace UI
{
	/*________________________________________________*/
	// CONSTANT VARIABLES
	const float BUTTON_HOVER_ALPHA{ 0.3f };
	const UI_Color HOVER_RGBA{ 0.541f, 0.376f, 0.f, 1.f };
	//const float BUTTON_NORMAL_ALPHA{ 0.f };
	/*________________________________________________*/
	void UI_Manager::ConvertToWS(UI_Button* newButton)
	{
		newButton->posWS.x = newButton->pos.x - m_winDim.x * 0.5f;
		newButton->posWS.y = newButton->pos.y - m_winDim.y * 0.5f;

		newButton->minWS.x = newButton->min.x - m_winDim.x * 0.5f;
		newButton->maxWS.x = newButton->max.x - m_winDim.x * 0.5f;
		newButton->minWS.y = newButton->min.y + m_winDim.y * 0.5f;
		newButton->maxWS.y = newButton->max.y + m_winDim.y * 0.5f;
	}
	UI_Button* UI_Manager::CreateButton(AEVec2 pos, AEVec2 size, BUTTON_TYPE type, UI_TextArea* buttonText, void(*callback)(UI_Button*), UI_TextArea* hoverText)
	{
		UI_Button* newButton = new UI_Button();
		newButton->scale = size;			// Scale of button important for render/collision
		newButton->callback = callback;		// callback function, if any
		newButton->hoverText = hoverText;	// set text on mouse hover, if any
		newButton->buttonText = buttonText;	// set button text rendered on button, if any
		newButton->pos = pos;
		
		// Now we set the collision to mouse boundaries for click detection
		newButton->min.x = pos.x - size.x * 0.5f;
		newButton->max.x = pos.x + size.x * 0.5f;
		newButton->min.y = pos.y - size.y * 0.5f;
		newButton->max.y = pos.y + size.y * 0.5f;
		// Convert positions to world space where 0,0 is center
		ConvertToWS(newButton);

		// Button text position
		newButton->CalculatePosN(m_winDim);

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
		case SKILL_TREE_BUTTON:
			newButton->texID = TEX_SKILL_MENU;
			newButton->meshID = MESH_BOX;
			break;
		case CLOSE_BUTTON:
			newButton->texID = TEX_CLOSE;
			newButton->meshID = MESH_BOX;
			break;
		case GUN_SKILL_BUTTON:
			newButton->texID = TEX_GUN_SKILL;
			newButton->meshID = MESH_BOX;
			break;
		default:
			break;
		}
		m_buttons.push_back(newButton);
		return newButton;
	}

	UI_StatElement* UI_Manager::CreateUIStat(AEVec2 pos, AEVec2 meshDim, UI_TextArea* text)
	{
		UI_StatElement* newStat{ new UI_StatElement{text, meshDim} };
		newStat->CalculatePositions();
		m_statElements.push_back(newStat);
		return newStat;
	}

	void UI_Manager::Load()
	{
		
	}

	void UI_Manager::Unload()
	{
		// Clear all buttons
		for (UI_Button* curr : m_buttons)
		{
			delete curr;
		}
		// Clear all stat elements
		for (UI_StatElement* curr : m_statElements)
		{
			delete curr;
		}
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
					curr->callback(curr);
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
		f32 const& winX{ m_winDim.x }, &winY{ m_winDim.y };
		f32 const& mouseXN{ mouseX / winX * 2 - 1.f }, &mouseYN{ mouseY / winY * -2 + 1.f }; // CALCULATE NORMALIZED COORDINATES
		// Now safe to recalculate mouse pos to worldspace coordinates
		mouseX -= static_cast<s32>(winX)/ 2;
		mouseY -= static_cast<s32>(winY) / 2;
		for (UI_Button* curr : m_buttons) {
			AEGfxSetBlendColor(1.f, 1.f, 1.f, (curr->bHovering ? BUTTON_HOVER_ALPHA: 0.f));
			DrawMesh(curr->posWS, curr->scale, curr->texID); // Render the mesh
			// Now render the button text, if any
			if (curr->buttonText)
				curr->buttonText->Draw(curr->posN.x, curr->posN.y, 1.f, 0.f, 0.f);
		}

		// NOW DRAW DESCRIPTIONS (IF ANY)
		for (UI_Button* curr : m_buttons) {
			// UI TEST
			// Render text if hovering
			AEGfxSetBlendMode(AEGfxBlendMode::AE_GFX_BM_BLEND);
			if (curr->bHovering && curr->hoverText)
			{
				// RENDER THE BOX CONTAINING TEXT
				AEVec2 pos{ static_cast<f32>(mouseX) + curr->hoverText->GetBoxWidth() * 0.25f,
						static_cast<f32>(-mouseY) - curr->hoverText->GetBoxHeight() * 0.25f };
				AEVec2 scale{ curr->hoverText->GetBoxWidth() * 0.5f, curr->hoverText->GetBoxHeight() * 0.5f };
				UI::DrawMesh(pos, scale, TEX_BUTTON, HOVER_RGBA);

				// NOW DRAW THE TEXT
				curr->hoverText->Draw(mouseXN, mouseYN, 0.f, 0.f, 0.f);
			}
		}

		// NOW DRAW EVERY UI ELEMENT EVER
		for (UI_StatElement* curr : m_statElements)
		{
			curr->Draw();
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