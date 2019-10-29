#pragma once

#include <SFML/System/Vector2.hpp>
#include <vector>
#include <sstream>
#include <fstream>
#include <iostream>
#include "yaml-cpp\yaml.h"

/// <summary>
/// @brief A struct to represent Obstacle data in the level.
/// 
/// </summary>
struct ObstacleData
{
	std::string m_type;
	sf::Vector2f m_position;
	double m_rotation;
};

/// <summary>
/// @brief A struct to represent Target data in the level.
/// 
/// </summary>
struct TargetData
{
	std::string m_type;
	sf::Vector2f m_position;
	int m_randomOffset;
	float m_durationSeconds;
	double m_rotation;
};

/// <summary>
/// @brief A struct to store Background texture information.
/// 
/// </summary>
struct BackgroundData
{
	std::string m_fileName;
};

/// <summary>
/// @brief A struct to represent tank data in the level.
/// 
/// </summary>
struct TankData
{
	sf::Vector2f m_position;
};

/// <summary>
/// @brief A struct representing all the Level Data.
/// 
/// Obstacles may be repeated and are therefore stored in a container.
/// </summary>
struct LevelData
{
	BackgroundData m_background;
	TankData m_tank;
	std::vector<ObstacleData> m_obstacles;
	std::vector<TargetData> m_targets;
};

/// <summary>
/// @brief A class to manage level loading.
/// 
/// This class will manage level loading using YAML.
/// </summary>
class LevelLoader
{
public:

	/// <summary>
	/// @brief No-op default constructor.
	/// 
	/// </summary>
	LevelLoader() = default;

	/// <summary>
	/// @brief Loads and parses the yaml level file.
	/// The level file is identified by a number and is assumed to have
	/// the following format: "level" followed by number followed by .yaml extension
	/// E.g. "level1.yaml"
	/// The level information is stored in the specified LevelData object.
	/// If the filename is not found or the file data is invalid, an exception
	/// is thrown.
	/// </summary>
	/// <param name="nr">The level number</param>
	/// <param name="level">A reference to the LevelData object</param>
	static void load(int nr, LevelData& level);
};