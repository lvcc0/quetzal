#pragma once

#include <type_traits>
#include <iostream>
#include <map>
#include <initializer_list>

#include "scene/main/node.h"
#include "static/physics.h"

// Note: without using inline with functions and variables program wouldnt compile 

#define DEBUGGER_ADD_CLASSES(...) debugger::checkClasses<__VA_ARGS__>()
#define DEBUGGER_PRINT_IN_CONSOLE(...) debugger::printMessageInConsole(__VA_ARGS__)

namespace debugger
{
	// If you added new property in class info, rework macro in gui.h and function in gui.cpp
	struct ClassInfo
	{
		bool is_polymorphic;
		bool is_basedOnNode;
		bool is_physical;
	};

	inline std::map<std::string, ClassInfo> m_MapOfClasses;

	template<class... C>
	inline constexpr void checkClasses()
	{
		(void(m_MapOfClasses[typeid(C).name()] = { std::is_polymorphic_v<C>, std::is_base_of_v<qtzl::Node, C>, is_physical_object<C> }), ...);
	}

	template<class... T>
	inline constexpr void printMessageInConsole(T... values)
	{
		(void(std::cout << values << ' '), ...);
		std::cout << '\n';
	}
}