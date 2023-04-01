#ifndef UI_TEXT_AREA_TABLE_H
#define UI_TEXT_AREA_TABLE_H
#include "UI_TextAreaTable.h"
#include "UI_TextArea.h"
namespace UI
{
	struct UI_TextAreaTable 
	{
		UI::UI_TextArea endTurnHoverText			= { .3f, 1.f, "Ends The Build Phase. BE WARNED: YOU CANNOT BUILD DURING DEFENDING PHASE" };
		UI::UI_TextArea buildTowerHoverText			= { .3f, 1.f, "Builds a tower. Automatically attacks enemies from range." };
		UI::UI_TextArea buildWallHoverText			= { .3f, 1.f, "Builds a wall. Most enemies walk around them." };
		UI::UI_TextArea buildNexusHoverText			= { .3f, 1.f, "Builds the nexus. Protect it with your life." };
		UI::UI_TextArea buildPlayerHoverText		= { .3f, 1.f, "Place the player. Defend your base." };
		UI::UI_TextArea buildNexusPlacedHoverText	= { .3f, 1.f, "Nexus already built. You only get one." };
		UI::UI_TextArea buildPlayerPlacedHoverText	= { .3f, 1.f, "Player already placed. You only get one." };
		UI::UI_TextArea eraseHoverText				= { .3f, 1.f, "Erase your building. No shame in mistakes." };
		UI::UI_TextArea elementTestText				= { .2f, 0.f, "HEALTH:" };
		UI::UI_TextArea playButton					= { .2f, 0.f, "PLAY" };
		UI::UI_TextArea howToButton					= { .2f, 0.f, "HOW TO PLAY" };
		UI::UI_TextArea creditsButton				= { .2f, 0.f, "CREDITS" };
		UI::UI_TextArea quitButton					= { .2f, 0.f, "QUIT" };
		UI::UI_TextArea restartButton				= { .2f, 0.f, "TRY AGAIN ? " };
		UI::UI_TextArea MenuButton					= { .2f, 0.f, "MENU" };

		// This is for UI quit prompt
		UI::UI_TextArea confirmText					= { .5f, 0.f, "ARE YOU SURE YOU WANT TO QUIT?" };
		UI::UI_TextArea yesButton					= { .2f, 0.f, "YES" };
		UI::UI_TextArea noButton					= { .2f, 0.f, "NO" };
		/*
			start
			how to play
			credits
			quit
		*/
		// For skill tree
		UI::UI_TextArea Skill_Tree					= { .3f, 1.f, "Skill Tree" };
		UI::UI_TextArea RangeBase					= { .3f, 1.f, "Enables shooting with the left mouse button.\nCOSTS: 200" };
		UI::UI_TextArea RangeTier1					= { .3f, 1.f, "Increases the damage of your bullets.\nCOSTS: 250" };
		UI::UI_TextArea RangeTier2					= { .3f, 1.f, "Increased fire rate.\nCOSTS: 250" };
		UI::UI_TextArea RangeTier3					= { .3f, 1.f, "To deal some real damage.\nCOSTS: 250" };
		UI::UI_TextArea RangeTier4					= { .3f, 1.f, "Increased bullet size.\nCOSTS: 350" };
		UI::UI_TextArea RangeTier5					= { .3f, 1.f, "Now your bullets really hurt.\nCOSTS: 350" }; 
		UI::UI_TextArea RangeTier6					= { .3f, 1.f, "Incresed fire rate, like a beam of death. \nCOSTS: 450" };
		UI::UI_TextArea RangeTier7					= { .3f, 1.f, "You really hate the tanks huh? Massive increase to bullet damage.\nCOSTS: 450" };
		UI::UI_TextArea RangeTier8					= { .3f, 1.f, "Your bullets explode on contact with enemies.\nCOSTS: 450" };
		UI::UI_TextArea RangeTier9					= { .3f, 1.f, "Go crazy with the C key, shooting bullets in every direction.\nCOSTS: 550" };

		UI::UI_TextArea MeleeBase					= { .3f, 1.f, "Enables for a big AOE move with the X key.\nCOSTS: 200" };
		UI::UI_TextArea MeleeTier1					= { .3f, 1.f, "Increased damage for the AOE.\nCOSTS: 250" };
		UI::UI_TextArea MeleeTier2					= { .3f, 1.f, "Increased AOE size\nCOSTS: 300" };
		UI::UI_TextArea MeleeTier3					= { .3f, 1.f, "Lower cooldown for the AOE.\nCOST: 350" };
		UI::UI_TextArea MeleeTier4					= { .3f, 1.f, "Increased AOE damage.\nCOSTS: 350" };
		UI::UI_TextArea MeleeTier5					= { .3f, 1.f, "Control the area around you! Max out your AOE size.\nCOSTS: 400" };
		UI::UI_TextArea MeleeTier6					= { .3f, 1.f, "It's getting out of hand, do you really need that much damage?!?\nCOSTS: 400" };
		UI::UI_TextArea MeleeTier7					= { .3f, 1.f, "The ultimate move, throw your AOE at the enemy!\nCOSTS: 500" };

		UI::UI_TextArea UtiliyBase					= { .3f, 1.f, "Blink with the left shift key, but mind the range! COSTS: 200" };
		UI::UI_TextArea UtilityTier1				= { .3f, 1.f, "Heal your turrets with the V key.\nCOSTS: 200" };
		UI::UI_TextArea UtilityTier2				= { .3f, 1.f, "Reduced cooldown for your blink.\nCOSTS: 200" };
		UI::UI_TextArea UtilityTier3				= { .3f, 1.f, "Reduced cooldwon for your heal.\nCOSTS: 200" };
		UI::UI_TextArea UtilityTier4				= { .3f, 1.f, "Increased blink radius.\nCOSTS: 200" };

		UI_TextAreaTable();
		~UI_TextAreaTable() = default;
	};
}

#endif
