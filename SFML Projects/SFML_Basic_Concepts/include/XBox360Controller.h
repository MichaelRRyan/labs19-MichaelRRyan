#ifndef XBOX360CONTROLLER
#define XBOX360CONTROLLER

/// <summary>
/// @Author Michael Rainsford Ryan
/// @Date 16/09/2019
/// (All comments and variables courtesy of Peter Lowe)
/// </summary>

#include <SFML/Graphics.hpp>

/// <summary>
/// Data to store the current state of the controller
/// </summary>
struct GamePadState
{
	bool A{ false };
	bool B{ false };
	bool X{ false };
	bool Y{ false };
	bool LB{ false };
	bool RB{ false };
	bool LeftThumbStickClick{ false };
	bool RightThumbStickClick{ false };
	bool DpadUp{ false };
	bool DpadDown{ false };
	bool DpadLeft{ false };
	bool DpadRight{ false };
	bool Start{ false };
	bool Back{ false };
	bool Xbox{ false };
	float RTrigger{ 0.0f };
	float LTrigger{ 0.0f };
	sf::Vector2f RightThumbStick{ 0.0f, 0.0f };
	sf::Vector2f LeftThumbStick{ 0.0f, 0.0f };
};

/// <summary>
/// Xbox controller class to query current state of controller
/// to mimic the controller states like XNA
/// </summary>
class XBox360Controller
{
private:
	// Deadzone for the dpad (really a joystick)
	const int DPAD_THRESHOLD = 50;

public:
	// Newly added for multiple controller support not required in your project
	// Number of connected controllers
	static int s_noOfControllers;
	// Index count for multiple controllers if connected
	int joystickIndex;
	// The current state of all the buttons
	GamePadState currentState;
	// The previous state to use to check for the moment a button is pressed
	GamePadState previousState;

	XBox360Controller();
	~XBox360Controller();
	void update();
	bool isConnect();
	bool connect();
};


#endif // !XBOX360CONTROLLER