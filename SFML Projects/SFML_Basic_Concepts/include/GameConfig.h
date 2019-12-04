#ifndef GAME_CONFIG_H
#define GAME_CONFIG_H

/// <summary>
/// This file is used for game configurations, this includes thing such as:
/// Number of players,
/// Control types,
/// Control schemes,
/// etc.
/// </summary>

const unsigned int MAX_PLAYERS{ 4 };

enum class ControlType
{
	Keyboard,
	Controller
};

enum class ControlScheme
{
	None,
	ArrowKeys,
	YGHJKeys,
};


#endif // !GAME_CONFIG_H

