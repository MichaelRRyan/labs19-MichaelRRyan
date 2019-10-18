/// @Author Michael Rainsford Ryan
/// @Date 16/09/2019

#include "XBox360Controller.h"
#include <iostream>

int XBox360Controller::s_noOfControllers = 0;

/// <summary>
/// Default constructer for the controller.
/// <para>Sets the joystick index to 0 by default</para>
/// </summary>
XBox360Controller::XBox360Controller() :
	joystickIndex{ 0 } // Set the joystick index to a default of 0
{
}

/// <summary>
/// Default deconstructer for the controller
/// </summary>
XBox360Controller::~XBox360Controller()
{
}

/// <summary>
/// Updates all the controller buttons and analogs states.
/// </summary>
void XBox360Controller::update()
{
	// Set the previous state
	previousState = currentState;

	// Face buttons
	currentState.A = sf::Joystick::isButtonPressed(joystickIndex, 0); // A

	currentState.B = sf::Joystick::isButtonPressed(joystickIndex, 1); // B

	currentState.X = sf::Joystick::isButtonPressed(joystickIndex, 2); // X

	currentState.Y = sf::Joystick::isButtonPressed(joystickIndex, 3); // Y

	// Bumper buttons
	currentState.LB = sf::Joystick::isButtonPressed(joystickIndex, 4); // LB

	currentState.RB = sf::Joystick::isButtonPressed(joystickIndex, 5); // RB

	// Option buttons
	currentState.Back = sf::Joystick::isButtonPressed(joystickIndex, 6); // Back

	currentState.Start = sf::Joystick::isButtonPressed(joystickIndex, 7); // Start

	// Thumb stick click
	currentState.LeftThumbStickClick = sf::Joystick::isButtonPressed(joystickIndex, 8); // Left thumbstick click

	currentState.RightThumbStickClick = sf::Joystick::isButtonPressed(joystickIndex, 9); // Right thumbstick click

	// Trigger buttons
	if (sf::Joystick::getAxisPosition(joystickIndex, sf::Joystick::Axis::Z) > 0.0f)
	{
		currentState.LTrigger = sf::Joystick::getAxisPosition(joystickIndex, sf::Joystick::Axis::Z); // Left trigger
	}
	else
	{
		currentState.RTrigger = sf::Joystick::getAxisPosition(joystickIndex, sf::Joystick::Axis::Z) * -1; // Right trigger
	}

	// Dpad horisontal
	currentState.DpadRight = (sf::Joystick::getAxisPosition(joystickIndex, sf::Joystick::Axis::PovX) > DPAD_THRESHOLD); // Dpad right

	currentState.DpadLeft = (sf::Joystick::getAxisPosition(joystickIndex, sf::Joystick::Axis::PovX) < -DPAD_THRESHOLD); // Dpad left

	// Dpad Vertical
	currentState.DpadUp = (sf::Joystick::getAxisPosition(joystickIndex, sf::Joystick::Axis::PovY) > DPAD_THRESHOLD); // Dpad up

	currentState.DpadDown = (sf::Joystick::getAxisPosition(joystickIndex, sf::Joystick::Axis::PovY) < -DPAD_THRESHOLD); // Dpad down
	
	// Analog sticks
	currentState.LeftThumbStick.x = sf::Joystick::getAxisPosition(joystickIndex, sf::Joystick::Axis::X); // Left thumb stick X-Axis
	currentState.LeftThumbStick.y = sf::Joystick::getAxisPosition(joystickIndex, sf::Joystick::Axis::Y); // Left thumb stick Y-Axis

	currentState.RightThumbStick.x = sf::Joystick::getAxisPosition(joystickIndex, sf::Joystick::Axis::U); // Right thumb stick X-Axis
	currentState.RightThumbStick.y = sf::Joystick::getAxisPosition(joystickIndex, sf::Joystick::Axis::V); // Right thumb stick Y-Axis
}

/// <summary>
/// Returns whether or not the controller is connected
/// </summary>
/// <returns>Whether the controller is connected</returns>
bool XBox360Controller::isConnect()
{
	return sf::Joystick::isConnected(joystickIndex); // Return whether the controller is connected or not
}

/// <summary>
/// Connect the controller
/// -- The majority of this function is inspired by a controller class used in Game Jam --
/// </summary>
/// <returns>Whether the controller connected successfully</returns>
bool XBox360Controller::connect()
{
	// Loops through all available controllers from the last know connected controller
	for (int i = s_noOfControllers; i < sf::Joystick::Count; i++)
	{
		if (sf::Joystick::isConnected(i)) // Stops when it finds a connected controller
		{
			joystickIndex = i; // Sets the index of this object to the connected controller index
			s_noOfControllers++; // Add one to the number of connected controllers
#ifdef _DEBUG // Display the information of this controller if in debug mode
			std::cout << "connected controller id:" << i;
			sf::Joystick::Identification id = sf::Joystick::getIdentification(0);
			std::cout << "\nVendor ID: " << id.vendorId << "\nProduct ID: " << id.productId << std::endl;
#endif // _DEBUG
			return true;
		}
	}
	return false;
}
