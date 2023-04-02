/******************************************************************************/
/*!
\file		CharacterStats.h
\author		Alwin Moo
\author		Alonzo Nalpon
\par        email: moo.g\@digipen.edu
\par        email: a.nalpon\@digipen.edu
\date       March 01, 2023
\brief		Alonzo (Primary 50%), Alwin (Secondary 50%)
			Charcater stats and functions to provide raw and calculated stats.

Copyright (C) 2023 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
 */
 /******************************************************************************/
#ifndef CHARACTER_STATS_H
#define CHARACTER_STATS_H
#include "AEEngine.h"
#include <array>
#include <initializer_list>

enum STAT_TYPE : size_t
{
	STAT_HEALTH = 0,
	STAT_MOVE_SPEED,
	STAT_ATTACK_SPEED,
	STAT_DAMAGE,
	STAT_TOTAL
};

enum class STATE
{
	STATE_NONE,

	STATE_ENEMY_IDLE,
	STATE_ENEMY_MOVE,
	STATE_ENEMY_ATTACK,
	
	STATE_TURRET_IDLE,
	STATE_TURRET_SHOOT,
	STATE_TURRET_SENTRY
};

enum class INNER_STATE
{
	ISTATE_NONE,
	ISTATE_ENTER,
	ISTATE_UPDATE,
	ISTATE_EXIT,
};

//!< info to add stat
struct AddStatInfo
{
	STAT_TYPE stat;
	int value;
};

class CharacterStats
{
private:
	std::array<int, STAT_TOTAL>	m_rawStats;
	std::array<int, STAT_TOTAL> m_subRawStat; //!< stats to add to raw stats
	std::array<float, STAT_TOTAL>m_stats;
	std::array<float, STAT_TOTAL>m_default;
	bool m_dirty;

	STATE currState;
	STATE nextState;

	INNER_STATE currInnerState;
	//INNER_STATE nextInnerState;

public:
	CharacterStats();
	~CharacterStats() = default;

	void	CalculateStats(); //!< recalculate stats
	void	AddStat(AddStatInfo const* info); //!< adds to a raw stat and dirty = true
	float	GetStat(STAT_TYPE stat); //!< get stat. Automatically calculates if fields are dirty
	void SetStat(STAT_TYPE stat, float val);

	// NEVER USE THIS UNLESS IT'S FOR NPC
	int		GetRawStat(STAT_TYPE stat); //!< get raw stat
	void	SetRawStat(STAT_TYPE stat, int val);

	void SetDefault(float, float, float, float);
	float GetNormalisedRemaining(STAT_TYPE stat);

	void SetCurrInnerState(INNER_STATE);
	INNER_STATE GetCurrInnerState();

	void SetNextState(STATE);
	// NEVER USE THIS UNLESS ITS IN EXIT
	void SetCurrStateFromNext();
	STATE GetCurrState();

	bool at_goal;
	f32 path_timer;

	enum class TARGET_TYPE
	{
		TAR_PLAYER,
		TAR_NEXUS,
		TAR_WALL,
		TAR_TURRET,

		NUM_TAR
	};
	TARGET_TYPE target_type;
};
#endif
