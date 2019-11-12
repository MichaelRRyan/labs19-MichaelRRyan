#ifndef ROUND_STATS_SAVER_H
#define ROUND_STATS_SAVER_H

#include <yaml-cpp/yaml.h>
#include <iostream>
#include <fstream>

/// <summary>
/// @Author Michael Rainsford Ryan
/// @Date 11/11/19
/// </summary>

struct Stats
{
	int m_score;
	int m_PercentTargetsHit;
	int m_accuracy;
};

class RoundStatsSaver
{
public:

	static void saveRoundStats(Stats t_stats);
};

#endif // !ROUND_STATS_SAVER_H