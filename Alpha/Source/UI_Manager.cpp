#include "UI_Manager.h"
#include "Rendering.hpp"
#include <iostream>
extern s8 g_fontID;
namespace UI
{
	/*________________________________________________*/
	// CONSTANT VARIABLES
	const float BUTTON_HOVER_ALPHA{ 0.3f };
	//const float BUTTON_NORMAL_ALPHA{ 0.f };
	/*________________________________________________*/
	void UI_Manager::ConvertToWS(Button* newButton)
	{
		newButton->wsPos.x = newButton->pos.x - m_winDim.x * 0.5f;
		newButton->wsPos.y = newButton->pos.y - m_winDim.y * 0.5f;

		newButton->wsMin.x = newButton->min.x - m_winDim.x * 0.5f;
		newButton->wsMax.x = newButton->max.x - m_winDim.x * 0.5f;
		newButton->wsMin.y = newButton->min.y + m_winDim.y * 0.5f;
		newButton->wsMax.y = newButton->max.y + m_winDim.y * 0.5f;
	}
	void UI_Manager::CreateButton(AEVec2 pos, AEVec2 size, BUTTON_TYPE type, TextArea* buttonText, void(*callback)(), TextArea* hoverText)
	{
		Button* newButton = new Button();
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
		case WHITE_BUTTON:
			newButton->texID = TEX_BUTTON;
			newButton->meshID = MESH_BOX;
			break;
		case END_PHASE_BUTTON:
			newButton->texID = TEX_END_PHASE;
			newButton->meshID = MESH_BOX;
			break;
		default:
			break;
		}
		m_buttons.push_back(newButton);
	}
	void UI_Manager::Load()
	{
		m_fontID = g_fontID;
		m_textures.at(TEX_BUTTON)		= AEGfxTextureLoad("Assets/SquareButton.png");
		m_textures.at(TEX_END_PHASE)	= AEGfxTextureLoad("Assets/Hourglass.png");
		m_textures.at(TEX_TOWER)		= AEGfxTextureLoad("Assets/Tower.png");
		m_textures.at(TEX_NEXUS)		= AEGfxTextureLoad("Assets/Nexus.png");
		m_mesh.at(MESH_BOX) = render::GenerateQuad();
	}
	void UI_Manager::Unload()
	{
		for (AEGfxTexture* i : m_textures) {
			if (!i)
				continue;
			AEGfxTextureUnload(i);
		}
		for (AEGfxVertexList* i : m_mesh) {
			if (!i)
				continue;
			AEGfxMeshFree(i);
		}
		for (Button* i : m_buttons) {
			if (!i)
				continue;
			delete i;
		}
	}
	void UI_Manager::SetWinDim(f32 x, f32 y)
	{
		m_winDim.x = x;
		m_winDim.y = y;
	}
	void UI_Manager::Update(AEVec2 mousePos, bool lClick)
	{
		for (Button* curr : m_buttons) {
			curr->bHovering = false;
			//std::cout << "MOUSEPOS = " << mousePos.x << ", " << mousePos.y << '\n';
			//std::cout << "CURRMIN  = " << curr->min.x << ", " << curr->min.y << '\n';
			//std::cout << "CURRMAX  = " << curr->max.x << ", " << curr->max.y << '\n';
			//std::cout << std::endl;
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

	void UI_Manager::Draw(f32 const& mouseXN, f32 const& mouseYN)
	{
		for (Button* curr : m_buttons) {
			// UI TEST
			AEGfxSetRenderMode(AE_GFX_RM_TEXTURE);
			AEGfxSetTintColor(1.f, 1.f, 1.f, 1.0f);
			AEGfxSetBlendMode(AE_GFX_BM_BLEND);
			AEGfxSetTransparency(1.f);
			AEGfxTextureSet(m_textures[curr->texID], 0, 0);

			AEMtx33 scale = { 0 };
			AEMtx33Scale(&scale, curr->scale.x, curr->scale.y);

			AEMtx33 rotate = { 0 };
			AEMtx33Rot(&rotate, 0.f);

			AEMtx33 translate = { 0 };
			//AEMtx33Trans(&translate, AEGetWindowWidth() / 2.f, AEGetWindowHeight() / 2.f);
			AEMtx33Trans(&translate, curr->wsPos.x, curr->wsPos.y);

			AEMtx33 transform = { 0 };
			AEMtx33Concat(&transform, &rotate, &scale);
			AEMtx33Concat(&transform, &translate, &transform);

			AEGfxSetTransform(transform.m);
			AEGfxSetBlendColor(1.f, 1.f, 1.f, (curr->bHovering ? BUTTON_HOVER_ALPHA: 0.f));
			AEGfxMeshDraw(m_mesh[curr->meshID], AE_GFX_MDM_TRIANGLES);
			
		}
		AEGfxSetBlendColor(0.f, 0.f, 0.f, 0.f);

		// NOW DRAW DESCRIPTIONS (IF ANY)
		for (Button* curr : m_buttons) {
			// UI TEST
			// Render text if hovering
			AEGfxSetBlendMode(AEGfxBlendMode::AE_GFX_BM_BLEND);
			if (curr->bHovering && curr->hoverText)
			{
				curr->hoverText->Draw(mouseXN, mouseYN, 0.f, 0.f, 0.f);
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