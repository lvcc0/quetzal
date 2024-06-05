#include <windows.h>
#include <stdio.h>
#include <psapi.h>
#include "pdh.h"
#pragma comment( lib, "Pdh.lib" )

class Debugger 
{
	int memory;

public:
	/*constructor*/
	Debugger();

	long getPrivateWorkingSet(const wchar_t* process);
	void printMemory();

	/*deleting move or copy constructors*/
	Debugger(const Debugger& obj) = delete;
	Debugger(const Debugger&& obj) = delete;
	/*---------------------------------------*/
};