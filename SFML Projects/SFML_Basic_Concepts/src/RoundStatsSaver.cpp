#include "..\include\RoundStatsSaver.h"

void RoundStatsSaver::saveRoundStats(Stats t_stats)
{
	YAML::Node fileNode = YAML::LoadFile(".//resources//data//gameData.yaml");

	if (fileNode["stats"]) {
		YAML::Node& stats = fileNode["stats"].as<YAML::Node>();
		YAML::Node newStatsData;
		bool scorePlaced = false;

		for (int i = 0; i < stats.size(); i++)
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
				newScoreNode["percTargetsHit"] = t_stats.m_PercentTargetsHit;
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


//// Iterate through all the statistics data
		//std::vector<Stats>::iterator end = statsData.end();
		//for (std::vector<Stats>::iterator it = statsData.begin(); it < end; it++)
		//{
		//	if (t_stats.m_score > it->m_score)
		//	{
		//		statsData.insert(it, t_stats);
		//		break;
		//	}
		//}

		//// Iterate through all of the statistics data from the file
		//for (std::vector<Stats>::iterator it = statsData.begin(); it < statsData.end(); it++)
		//{
		//	if (t_stats.m_score > it->m_score)
		//	{
		//		statsData.insert(it, t_stats);
		//		break;
		//	}
		//}



/*std::cout << "Set: " << i + 1 << std::endl;
			std::cout << "Previous score: " << stats[i]["score"].as<int>() << std::endl;
			std::cout << "Previous percentage of targets hit: " << stats[i]["percTargetsHit"].as<int>() << std::endl;
			std::cout << "Previous accuracy: " << stats[i]["accuracy"].as<int>() << std::endl;*/