#ifndef CHARACTER_STATS_H
#define CHARACTER_STATS_H
#include "AEEngine.h"
#include <array>
enum STAT_TYPE : size_t
{
	STAT_HEALTH = 0,
	STAT_MOVE_SPEED,
	STAT_ATTACK_SPEED,
	STAT_TOTAL
};

enum class STATE
{
	STATE_NONE,

	STATE_ENEMY_IDLE,
	STATE_ENEMY_ATTACK,
	
	STATE_TURRET_IDLE,
	STATE_TURRET_SHOOT,
	STATE_TURRET_SENTRY
};

enum class INNER_STATE
{
	ISTATE_NONE,
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
	bool m_dirty;

	STATE currState;
	STATE nextState;

	INNER_STATE currInnerState;
	INNER_STATE nextInnerState;

public:
	CharacterStats();
	~CharacterStats() = default;

	void	CalculateStats(); //!< recalculate stats
	void	AddStat(AddStatInfo const* info); //!< adds to a raw stat and dirty = true
	int		GetRawStat(STAT_TYPE stat); //!< get raw stat
	float	GetStat(STAT_TYPE stat); //!< get stat. Automatically calculates if fields are dirty
	
	// Setters
	void	SetRawStat(STAT_TYPE stat, int val);
	void SetStat(STAT_TYPE stat, float val);

	void SetNextInnerState(INNER_STATE);
	INNER_STATE GetCurrInnerState();

	void SetNextState(STATE);
	STATE GetCurrState();

	AEVec2 target;
	bool at_goal;
	f32 path_timer;

	enum class TARGET_TYPE
	{
		TAR_PLAYER,
		TAR_NEXUS,
		TAR_WALL,

		NUM_TAR
	};
	TARGET_TYPE target_type;
};
#endif
