#include "UI_TextAreaTable.h"

UI::UI_TextAreaTable::UI_TextAreaTable()
{
	endTurnHoverText = { .3f, 1.f, "Ends The Build Phase. BE WARNED: YOU CANNOT BUILD DURING DEFENDING PHASE" };
	buildTowerHoverText = { .3f, 1.f, "Builds a tower. Automatically attacks enemies from range." };
	buildWallHoverText = { .3f, 1.f, "Builds a wall. Most enemies walk around them." };
	buildNexusHoverText = { .3f, 1.f, "Builds the nexus. Protect it with your life." };
	buildPlayerHoverText = { .3f, 1.f, "Place the player. Defend your base." };
	buildNexusPlacedHoverText = { .3f, 1.f, "Nexus already built. You only get one." };
	buildPlayerPlacedHoverText = { .3f, 1.f, "Player already placed. You only get one." };
	eraseHoverText = { .3f, 1.f, "Erase your building. No shame in mistakes." };
}

