#pragma once

#include <type_traits>
#include <iostream>
#include <map>
#include "scene/main/node.h"

#define DEBUGGER_ADD_CLASSES(...) Debugger::checkClasses<__VA_ARGS__>()


class Debugger
{
	// If you added new property in class info, rework macro in gui.h and function in gui.cpp
	struct ClassInfo
	{
		bool is_polymorphic;
		bool is_basedOnNode;
	};
public:
	static inline std::map<std::string, ClassInfo> m_MapOfClasses;

	template<class... C>
	static constexpr void checkClasses();
};

template<class... C>
constexpr void Debugger::checkClasses()
{
	(void(m_MapOfClasses[typeid(C).name()] = { std::is_polymorphic_v<C>, std::is_base_of_v<qtzl::Node, C> }), ...);
}