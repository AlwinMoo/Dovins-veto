#pragma once
#include "AEEngine.h"
#include "UI_Def.h"
namespace UI
{
	class UI_Panel {
	private:
		AEVec2 m_pos;
		AEVec2 m_scale;
		UI_TEX m_tex;
	public:
		UI_Panel(AEVec2 pos, AEVec2 scale, UI_TEX tex)
			: m_pos(pos), m_scale(scale), m_tex(tex) {}
		~UI_Panel() = default;
		void Draw()
		{
			DrawMesh(m_pos, m_scale, m_tex);
		}
	};
}