/******************************************************************************/
/*!
\file		UI_Manager.cpp
\author 	Alonzo Nalpon (a.nalpon)
\par    	email: a.nalpon@digipen.edu
\date   	January 26, 2023
\brief		This file contains declarations for the UI Manager

Copyright (C) 2023 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
 */
 /******************************************************************************/

#include "UI_Manager.h"
#include "Rendering.hpp"
#include "Menu.h"
#include <iostream>
namespace UI
{
	/*________________________________________________*/
	// CONSTANT VARIABLES
	const float BUTTON_HOVER_ALPHA{ 0.3f };
	const UI_Color HOVER_RGBA	{ 0.541f, 0.376f, 0.f, 1.f };
	const UI_Color DISABLED_RGBA{ 0.f, 0.f, 0.f, .8f };
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

	void UI_Manager::CreatePanel(AEVec2 pos, AEVec2 scale, UI_TEX tex)
	{
		m_panels.push_back(new UI_Panel{ {pos.x - m_winDim.x * 0.5f, pos.y - m_winDim.y * 0.5f}, scale, tex });
	}

	UI_Button* UI_Manager::CreateButton(AEVec2 pos, AEVec2 size, BUTTON_TYPE type, UI_TextArea* buttonText, void(*callback)(UI_Button*), UI_TextArea* hoverText, bool enabled)
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

		// Assign textures AND/OR custom attributes based on button type
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
			newButton->scale.x = 100.f;
			newButton->scale.y = 60.f;
			newButton->texID = TEX_BLUE_NODE;
			newButton->meshID = MESH_BOX;
			break;
		case AOE_SKILL_BUTTON:
			newButton->scale.x = 100.f;
			newButton->scale.y = 60.f;
			newButton->texID = TEX_YELLOW_NODE;
			newButton->meshID = MESH_BOX;
			break;
		case UTILITY_SKILL_BUTTON:
			newButton->scale.x = 100.f;
			newButton->scale.y = 60.f;
			newButton-> texID = TEX_GREEN_NODE;
			newButton->meshID = MESH_BOX;
			break;
		case CREDIT_BUTTON:
			newButton->texID = TEX_CREDIT;
			newButton->meshID = MESH_BOX;
			break;
		case RESTART_BUTTON:
			newButton->texID = TEX_RESTART;
			newButton->meshID = MESH_BOX;
			break;
		case MENU_BUTTON:
			newButton->texID = TEX_MENU;
			newButton->meshID = MESH_BOX;
			break;
		case UI_MENU_BUTTON:
			newButton->texID = TEX_UI_BUTTON;
			newButton->meshID = MESH_BOX;
			break;
		default:
			break;
		}
		newButton->bEnable = enabled; // set flag
		newButton->bBought = false;		// The other flag too
		m_buttons.push_back(newButton); // push back to container
		return newButton;
	}

	UI_StatElement* UI_Manager::CreateUIStat(AEVec2 pos, AEVec2 meshDim, UI_TextArea* text)
	{
		UI_StatElement* element = GenerateUIStat(pos, meshDim, text);
		m_statElements.push_back(element);
		return element;
	}

	UI_StatElement* UI_Manager::GenerateUIStat(AEVec2 pos, AEVec2 meshDim, UI_TextArea* text)
	{
		UI_StatElement* newStat{ new UI_StatElement{pos, text, meshDim} };
		newStat->CalculatePositions();
		return newStat;
	}

	void UI_Manager::RenderUIElement(UI_StatElement& element)
	{
		element.Draw();
	}

	void UI_Manager::Unload()
	{
		// Clear all stat elements
		for (UI_StatElement* curr : m_statElements)
		{
			delete curr;
		}
		// Clear all buttons
		for (UI_Button* curr : m_buttons)
		{
			delete curr;
		}
		// Clear all panels
		for (UI_Panel* curr : m_panels)
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
			// If button is disabled or mouse not on button, skip
			if (!curr->bEnable || 
				mousePos.x > curr->max.x || mousePos.x < curr->min.x || 
				mousePos.y > curr->max.y || mousePos.y < curr->min.y)
				continue;
			// CLICKING LOGIC
			if (lClick)
			{
				if (curr->callback) // Callback if any
					curr->callback(curr);
			}
			// HOVER LOGIC
			else
			{
				curr->bHovering = true; // If not clicking, it's hovered on
			}
		}
	}

	void UI_Manager::Draw(s32 mouseX, s32 mouseY)
	{
		AEGfxSetRenderMode(AE_GFX_RM_TEXTURE);
		AEGfxSetBlendMode(AE_GFX_BM_BLEND);
		f32 const& winX{ m_winDim.x }, &winY{ m_winDim.y };
		f32 const& mouseXN{ mouseX / winX * 2 - 1.f }, &mouseYN{ mouseY / winY * -2 + 1.f }; // CALCULATE NORMALIZED COORDINATES
		// Now safe to recalculate mouse pos to worldspace coordinates
		mouseX -= static_cast<s32>(winX)/ 2;
		mouseY -= static_cast<s32>(winY) / 2;

		// DRAW PANELS
		for (UI_Panel* curr : m_panels)
		{
			curr->Draw();
		}

		// DRAW UI BUTTONS
		for (UI_Button* curr : m_buttons) {
			if (curr->bBought)
				AEGfxSetBlendColor(1.f, 1.f, 1.f, 1.f);
			else if (curr->bEnable)
				AEGfxSetBlendColor(1.f, 1.f, 1.f, (curr->bHovering ? BUTTON_HOVER_ALPHA : 0.f));
			else
				AEGfxSetBlendColor(DISABLED_RGBA.r, DISABLED_RGBA.g, DISABLED_RGBA.b, DISABLED_RGBA.a);

			DrawMesh(curr->posWS, curr->scale, curr->texID); // Render the mesh
			// Now render the button text, if any
			if (curr->buttonText)
				curr->buttonText->Draw({curr->posN.x, curr->posN.y});
			AEGfxSetBlendColor(0.f, 0.f, 0.f, 0.f);
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
				float const xBounds{ m_winDim.x * 0.5f - curr->hoverText->GetBoxWidth() * 0.25f };
				pos.x = (pos.x > xBounds) ? xBounds : pos.x ;
				AEVec2 scale{ curr->hoverText->GetBoxWidth() * 0.5f, curr->hoverText->GetBoxHeight() * 0.5f };
				UI::DrawMesh(pos, scale, TEX_BUTTON, HOVER_RGBA);

				// NOW DRAW THE TEXT
				curr->hoverText->Draw({ mouseXN, mouseYN });
			}
		}

		// NOW DRAW EVERY UI ELEMENT EVER
		for (UI_StatElement* curr : m_statElements)
		{
			curr->Draw();
		}

	}

	UI_Manager::~UI_Manager()
	{
		Unload(); // Automatically clears all the buttons/elements/panels
	}
}