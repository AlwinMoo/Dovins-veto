#ifndef UI_SKILL_TREE_H
#define UI_SKILL_TREE_H
#include "GameObject.h"

namespace UI
{
	struct UI_SKILL_TREE 
	{
		// Player pointer
		GameObject* player;

		// TIER 1: every callback ever
		void UpgradeAOE1();
		void UpgradeRange1();
	};
}

#endif
