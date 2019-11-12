#include "..\include\RoundStatsSaver.h"

void RoundStatsSaver::saveRoundStats(Stats t_stats)
{
	YAML::Node gameData = YAML::LoadFile(".//resources//data//gameData.yaml");

	if (gameData["stats"]) {
		const YAML::Node& stats = gameData["stats"].as<YAML::Node>();

		for (int i = 0; i < stats.size(); i++)
		{
			std::cout << "Previous score: " << stats["score"].as<int>() << std::endl;
		}
	}

	// Save to file, we're not doing anything with the file yet
	std::ofstream fout("config.yaml");
	fout << gameData;
}
