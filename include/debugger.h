#pragma once

#include <windows.h>
#include <stdio.h>
#include <psapi.h>
#include "pdh.h"

#pragma comment( lib, "Pdh.lib" )

class Debugger 
{
public:
	// Constructor
	Debugger() { /* empty */ };

	// No move or copy constructors allowed
	Debugger(const Debugger& obj) = delete;
	Debugger(const Debugger&& obj) = delete;

	long getPrivateWorkingSet(const wchar_t* process);
	void printMemory();
};