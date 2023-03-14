#include "CharacterStats.h"

/*___________*/
// CONSTANTS //
/*___________*/
std::array<const float, STAT_TOTAL> STAT_MULT
{
	1.f, //STAT_HEALTH,
	1.f, //STAT_MOVE_SPEED,
	1.f //STAT_ATTACK_SPEED
};

CharacterStats::CharacterStats() : m_rawStats{}, m_subRawStat{}, m_stats{}, m_dirty{}, currState(STATE::STATE_NONE), currInnerState(INNER_STATE::ISTATE_NONE)
{

}

void CharacterStats::CalculateStats()
{
	for (size_t i{}; i < STAT_TOTAL; ++i)
	{
		// Add subraw stat and calculates final stat in float
		m_stats.at(i) = static_cast<float>(m_rawStats.at(i) + m_subRawStat.at(i)) * STAT_MULT.at(i);
	}
	m_dirty = false;	// Updated!
}

void CharacterStats::AddStat(AddStatInfo const* info)
{
	m_rawStats.at(info->stat) += info->value;
	m_dirty = true;
}

int CharacterStats::GetRawStat(STAT_TYPE stat)
{
	return m_rawStats.at(stat);
}

float CharacterStats::GetStat(STAT_TYPE stat)
{
	if (m_dirty)
		CalculateStats();
	return m_stats.at(stat);
}

void CharacterStats::SetRawStat(STAT_TYPE stat, int val)
{
	m_dirty = true;
	m_rawStats.at(stat) = val;
}

void CharacterStats::SetStat(STAT_TYPE stat, float val)
{
	m_dirty = false;
	m_stats.at(stat) = val;
}

void CharacterStats::SetNextInnerState(INNER_STATE st)
{
	nextInnerState = st;
}

INNER_STATE CharacterStats::GetCurrInnerState()
{
	return currInnerState;
}

void CharacterStats::SetNextState(STATE st)
{
	nextState = st;
}

STATE CharacterStats::GetCurrState()
{
	return currState;
}
