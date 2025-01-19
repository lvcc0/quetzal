#include "static/input.h"

void Input::press(int key)
{
	keys[key] = true;
}

void Input::release(int key)
{
	keys[key] = false;
}

bool Input::isKeyPressed(int key)
{
	return keys[key];
}

bool Input::isActionPressed(const std::string& action)
{
	return keys[actions.at(action)];
}

void Input::mapAction(const std::string& action, int key)
{
	if (actions.find(action) != actions.end())
	{
		std::cerr << "action \"" << action << "\" already exists, consider using Input::remapAction" << std::endl;
		return;
	}

	actions[action] = key;
}

void Input::remapAction(const std::string& action, int key)
{
	if (actions.find(action) == actions.end())
	{
		std::cerr << "action \"" << action << "\" does not exist, nothing to remap" << std::endl;
		return;
	}

	actions[action] = key;
}

void Input::unmapAction(const std::string& action)
{
	if (actions.find(action) == actions.end())
	{
		std::cerr << "action \"" << action << "\" does not exist, nothing to unmap" << std::endl;
		return;
	}

	actions.erase(action);
}