#include "UI_Manager.h"

namespace UI
{
	void UI_Manager::CreateButton(AEVec2 pos, AEVec2 size, BUTTON_TYPE type, s8 fontID, void(*callback)(void*))
	{
		Button* newButton = new Button();
		newButton->callback = callback;
		newButton->pos = pos;
		newButton->size = size;
		switch (type) {
		case WHITE_BUTTON:
			
			break;
		default:
			break;
		}
		m_buttons.push_back(newButton);
	}
	void UI_Manager::LoadFont(char const* path)
	{
		
	}
	void UI_Manager::Unload()
	{
	}
	UI_Manager::UI_Manager()
	{
	}

	UI_Manager::~UI_Manager()
	{
	}
}