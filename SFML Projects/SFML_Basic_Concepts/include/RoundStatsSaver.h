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
	int m_percentTargetsHit;
	int m_accuracy;
};

class RoundStatsSaver
{
public:

	static void saveRoundStats(Stats t_stats);

	/// <summary>
	/// Returns the nth best score, where n is a value passed in, between 0
	/// and 4 where 0 is the best and 4 is the worst
	/// </summary>
	/// <param name="t_n">number between 0 and 4</param>
	/// <returns>nth best score</returns>
	static Stats returnNthBestScore(int t_n);
};

#endif // !ROUND_STATS_SAVER_H