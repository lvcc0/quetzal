#pragma once

#include <iostream>
#include <string>
#include <map>

// TODO: create separate keyboard.h file with Key class so key modulation and some more stuff can be made

class Input
{
public:
    Input() = delete;
    Input(const Input&) = delete;
    Input(Input&&) = delete;

    static void press(int key);
    static void release(int key);

    static bool isKeyPressed(int key);
    static bool isActionPressed(const std::string& action);

    static void mapAction(const std::string& action, int key);
    static void remapAction(const std::string& action, int key);
    static void unmapAction(const std::string& action);

private:
    inline static bool m_Keys[1024]; // all keys (1 - pressed / 0 - released)

    inline static std::map<std::string, int> m_Actions; // map of user defined actions
};