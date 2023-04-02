/******************************************************************************/
/*!
\file		UI_TextAreaTable.h
\author 	Alonzo Nalpon
\author		Alvin Yeo
\author		Bevan
\par    	email: a.nalpon@digipen.edu
\par		email: a.yeo@digipen.edu
\par		email: bevanzhiwei.lim@digipen.edu
\date   	March 16, 2023
\brief		Alonzo (Primary 33%), Alvin (Secondary 33%), Bevan (Secondary 33%)
			This file contains game text assets to be used with UI


Copyright (C) 2023 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
 */
 /******************************************************************************/
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
		UI::UI_TextArea resumeButton				= { .2f, 0.f, "RESUME" };
		UI::UI_TextArea pausedText					= { .2f, 0.f, "PAUSED" };

		// This is for UI quit prompt
		UI::UI_TextArea menuConfirmText				= { .5f, 0.f, "ARE YOU SURE YOU WANT TO QUIT?" };
		UI::UI_TextArea gameConfirmText				= { .5f, 0.f, "ARE YOU SURE YOU WANT TO QUIT? PROGRESS WILL BE LOST!" };
		UI::UI_TextArea yesButton					= { .2f, 0.f, "YES" };
		UI::UI_TextArea noButton					= { .2f, 0.f, "NO" };
		/*
			start
			how to play
			credits
			quit
		*/

		//For how to play
		UI::UI_TextArea nextButton					= { .2f, 0.f, "NEXT" };

		// For skill tree
		UI::UI_TextArea Skill_Tree					= { .3f, 1.f, "Skill Tree" };
		UI::UI_TextArea RangeBase					= { .3f, 1.f, "Enables shooting with the left mouse button. COSTS: 200" };
		UI::UI_TextArea RangeTier1					= { .3f, 1.f, "Increases the damage of your bullets. COSTS: 250" };
		UI::UI_TextArea RangeTier2					= { .3f, 1.f, "Increased fire rate. COSTS: 250" };
		UI::UI_TextArea RangeTier3					= { .3f, 1.f, "To deal some real damage. COSTS: 250" };
		UI::UI_TextArea RangeTier4					= { .3f, 1.f, "Increased bullet size. COSTS: 350" };
		UI::UI_TextArea RangeTier5					= { .3f, 1.f, "Now your bullets really hurt. COSTS: 350" }; 
		UI::UI_TextArea RangeTier6					= { .3f, 1.f, "Incresed fire rate, like a beam of death. COSTS: 450" };
		UI::UI_TextArea RangeTier7					= { .3f, 1.f, "You really hate the tanks huh? Massive increase to bullet damage. COSTS: 450" };
		UI::UI_TextArea RangeTier8					= { .3f, 1.f, "Your bullets explode on contact with enemies. COSTS: 450" };
		UI::UI_TextArea RangeTier9					= { .3f, 1.f, "Go crazy with the C key, shooting bullets in every direction. COSTS: 550" };

		UI::UI_TextArea MeleeBase					= { .3f, 1.f, "Enables for a big AOE move with the X key. COSTS: 200" };
		UI::UI_TextArea MeleeTier1					= { .3f, 1.f, "Increased damage for the AOE. COSTS: 250" };
		UI::UI_TextArea MeleeTier2					= { .3f, 1.f, "Increased AOE size. COSTS: 300" };
		UI::UI_TextArea MeleeTier3					= { .3f, 1.f, "Lower cooldown for the AOE. COST: 350" };
		UI::UI_TextArea MeleeTier4					= { .3f, 1.f, "Increased AOE damage. COSTS: 350" };
		UI::UI_TextArea MeleeTier5					= { .3f, 1.f, "Control the area around you! Max out your AOE size. COSTS: 400" };
		UI::UI_TextArea MeleeTier6					= { .3f, 1.f, "It's getting out of hand, do you really need that much damage?!? COSTS: 400" };
		UI::UI_TextArea MeleeTier7					= { .3f, 1.f, "The ultimate move, throw your AOE at the enemy! COSTS: 500" };

		UI::UI_TextArea UtiliyBase					= { .3f, 1.f, "Blink with the left shift key, but mind the range! COSTS: 200" };
		UI::UI_TextArea UtilityTier1				= { .3f, 1.f, "Heal your turrets with the V key. COSTS: 200" };
		UI::UI_TextArea UtilityTier2				= { .3f, 1.f, "Reduced cooldown for your blink. COSTS: 200" };
		UI::UI_TextArea UtilityTier3				= { .3f, 1.f, "Reduced cooldwon for your heal. COSTS: 200" };
		UI::UI_TextArea UtilityTier4				= { .3f, 1.f, "Increased blink radius. COSTS: 200" };

		UI_TextAreaTable();
		~UI_TextAreaTable() = default;
	};
}

#endif
