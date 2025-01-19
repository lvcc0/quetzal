#include "static/input.h"

void Input::press(int key)
{
    m_Keys[key] = true;
}

void Input::release(int key)
{
    m_Keys[key] = false;
}

bool Input::isKeyPressed(int key)
{
    return m_Keys[key];
}

bool Input::isActionPressed(const std::string& action)
{
    return m_Keys[m_Actions.at(action)];
}

void Input::mapAction(const std::string& action, int key)
{
    if (m_Actions.find(action) != m_Actions.end())
    {
        std::cerr << "action \"" << action << "\" already exists, consider using Input::remapAction" << std::endl;
        return;
    }

    m_Actions[action] = key;
}

void Input::remapAction(const std::string& action, int key)
{
    if (m_Actions.find(action) == m_Actions.end())
    {
        std::cerr << "action \"" << action << "\" does not exist, nothing to remap" << std::endl;
        return;
    }

    m_Actions[action] = key;
}

void Input::unmapAction(const std::string& action)
{
    if (m_Actions.find(action) == m_Actions.end())
    {
        std::cerr << "action \"" << action << "\" does not exist, nothing to unmap" << std::endl;
        return;
    }

    m_Actions.erase(action);
}