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
public:
	CharacterStats();
	~CharacterStats() = default;

	void	CalculateStats(); //!< recalculate stats
	void	AddStat(AddStatInfo const* info); //!< adds to a raw stat and dirty = true
	int		GetRawStat(STAT_TYPE stat); //!< get raw stat
	float	GetStat(STAT_TYPE stat); //!< get stat. Automatically calculates if fields are dirty
	
	// Setters
	void	SetRawStat(STAT_TYPE stat, int val);
	void	SetStat(STAT_TYPE stat, float val); //!< should only be called if you don't plan on using raw. Overwritten when raw stat set
};
#endif
