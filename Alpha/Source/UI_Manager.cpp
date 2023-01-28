#include "UI_Manager.h"
#include "Rendering.hpp"
namespace UI
{
	void UI_Manager::CreateButton(AEVec2 pos, AEVec2 size, BUTTON_TYPE type, UI_FONT fontID, void(*callback)())
	{
		Button* newButton = new Button();
		newButton->font = fontID;
		newButton->scale = size;
		newButton->callback = callback;
		newButton->pos = pos;
		newButton->min.x = pos.x - size.x / 2.f;
		newButton->max.x = pos.x + size.x / 2.f;
		newButton->min.y = pos.y - size.y / 2.f;
		newButton->max.y = pos.y + size.y / 2.f;
		switch (type) {
		case WHITE_BUTTON:
			
			break;
		default:
			break;
		}
		m_buttons.push_back(newButton);
	}
	void UI_Manager::Load()
	{
		m_fonts.at(FONT_ROBOTO) = AEGfxCreateFont("Assets/Roboto-Regular.ttf", 16);
		m_textures.at(TEX_BUTTON) = AEGfxTextureLoad("Assets/SquareButton.png");
		m_mesh.at(MESH_BOX) = render::GenerateQuad();
	}
	void UI_Manager::Unload()
	{
	}
	void UI_Manager::Update(AEVec2 mousePos, bool lClick)
	{
		// IF IMPLEMENTING HOVER, REMOVE THIS IF
		if (!lClick)
			return;
		for (Button* curr : m_buttons) {
			if (mousePos.x > curr->max.x || mousePos.x < curr->min.x || mousePos.y > curr->max.y || mousePos.y < curr->min.y)
				continue;
			// clicked assumed. edit if hover implemented
			curr->callback();
			// TODO: Hover logic if not clicking?
		}
	}
	void UI_Manager::Draw()
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
			AEMtx33Trans(&translate, curr->pos.x, curr->pos.y);

			AEMtx33 transform = { 0 };
			AEMtx33Concat(&transform, &rotate, &scale);
			AEMtx33Concat(&transform, &translate, &transform);

			AEGfxSetTransform(transform.m);
			AEGfxMeshDraw(m_mesh[curr->meshID], AE_GFX_MDM_TRIANGLES);
		}
	}
	UI_Manager::UI_Manager()
	{
	}

	UI_Manager::~UI_Manager()
	{
	}
}