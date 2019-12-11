#ifndef ROUND_STATS_SAVER_H
#define ROUND_STATS_SAVER_H

/// <summary>
/// @Author Michael Rainsford Ryan
/// @Date 11/11/19
/// </summary>

#include <yaml-cpp/yaml.h>
#include <iostream>
#include <fstream>

/// <summary>
/// @brief A struct to save all necessary player statistics
/// </summary>
struct Stats
{
	int m_score;
	int m_percentTargetsHit;
	int m_accuracy;
};

/// <summary>
/// @brief A static class to save the player stats to a YAML file
/// </summary>
class RoundStatsSaver
{
public:

	/// <summary>
	/// @brief Save the round stats to the YAML file
	/// </summary>
	/// <param name="t_stats"></param>
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