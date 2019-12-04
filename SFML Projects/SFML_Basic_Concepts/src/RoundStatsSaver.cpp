#include "..\include\RoundStatsSaver.h"

////////////////////////////////////////////////////////////
void RoundStatsSaver::saveRoundStats(Stats t_stats)
{
	YAML::Node fileNode = YAML::LoadFile(".//resources//data//gameData.yaml");

	if (fileNode["stats"]) {
		YAML::Node& stats = fileNode["stats"].as<YAML::Node>();
		YAML::Node newStatsData;
		bool scorePlaced = false;

		for (int i = 0; i < static_cast<int>(stats.size()); i++)
		{
			YAML::Node newNode(YAML::NodeType::Map);

			newNode["score"] = stats[i]["score"].as<int>();
			newNode["percTargetsHit"] = stats[i]["percTargetsHit"].as<int>();
			newNode["accuracy"] = stats[i]["accuracy"].as<int>();

			// Place the new score in front of the current one
			if (!scorePlaced // Check the score hasn't been put into the file already
				&& t_stats.m_score > newNode["score"].as<int>()) // Check if the new score is greater than the current score
			{	
				YAML::Node newScoreNode(YAML::NodeType::Map);

				newScoreNode["score"] = t_stats.m_score;
				newScoreNode["percTargetsHit"] = t_stats.m_percentTargetsHit;
				newScoreNode["accuracy"] = t_stats.m_accuracy;

				newStatsData.push_back(newScoreNode);
				scorePlaced = true;

				if (newStatsData.size() == 5)
				{
					std::cout << "No room in the file" << std::endl;
					break;
				}
			}

			newStatsData.push_back(newNode);

			if (newStatsData.size() == 5)
			{
				std::cout << "No room in the file" << std::endl;
				break;
			}
		}

		stats = newStatsData;

		std::ofstream fout(".//resources//data//gameData.yaml");
		fout << fileNode;
	}
}

////////////////////////////////////////////////////////////
Stats RoundStatsSaver::returnNthBestScore(int t_n)
{
	YAML::Node fileNode = YAML::LoadFile(".//resources//data//gameData.yaml");

	if (t_n >= 0 && t_n < 5)
	{
		try
		{
			YAML::Node& statsNode = fileNode["stats"].as<YAML::Node>();

			if (t_n >= static_cast<int>(statsNode.size()))
			{
				std::cout << "Error while reading game stats: There is no stats stored in position "
					<< t_n << " in the file. Number of stats stored is " << statsNode.size() << "." << std::endl;
				return Stats{ 0 };
			}

			Stats nthStats;

			nthStats.m_score = statsNode[t_n]["score"].as<int>();
			nthStats.m_percentTargetsHit = statsNode[t_n]["percTargetsHit"].as<int>();
			nthStats.m_accuracy = statsNode[t_n]["accuracy"].as<int>();

			return nthStats;
		}
		catch (std::exception e)
		{
			std::cout << "Failed to load previous game stats: " << e.what() << std::endl;
		}
	}
	else
	{
		std::cout << "Error while reading game stats: " << t_n << " is not in the range 0-4." << std::endl;
	}
	
	
	return Stats{0};
}