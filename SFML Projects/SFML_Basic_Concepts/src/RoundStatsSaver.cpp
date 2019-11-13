#include "..\include\RoundStatsSaver.h"

void RoundStatsSaver::saveRoundStats(Stats t_stats)
{
	YAML::Node gameData = YAML::LoadFile(".//resources//data//gameData.yaml");

	if (gameData["stats"]) {
		YAML::Node& stats = gameData["stats"].as<YAML::Node>();

		for (int i = 0; i < stats.size(); i++)
		{
			std::cout << "Set: " << i + 1 << std::endl;
			std::cout << "Previous score: " << stats[i]["score"].as<int>() << std::endl;
			std::cout << "Previous percentage of targets hit: " << stats[i]["percTargetsHit"].as<int>() << std::endl;
			std::cout << "Previous accuracy: " << stats[i]["accuracy"].as<int>() << std::endl;
		}

		YAML::Node newNode(YAML::NodeType::Map);
		newNode["score"] = 5;
		newNode["percTargetsHit"] = 10;
		newNode["accuracy"] = 10;
		
		stats.push_back(newNode);
	}

	// Save to file, we're not doing anything with the file yet
	std::ofstream fout(".//resources//data//gameData.yaml");
	fout << gameData;
}
