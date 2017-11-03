// Game Engine Programming Assignment
// By Rob Ellis
// Bournemouth University

#include <iostream>
#include "Core.h"

int main(int argc, char* args[])
{
	// Create instance of the Core class
	Core *m_testCore = new Core();

	// The Core::Launch function will return RET_SUCC (0) or RET_ERROR (1)
	return m_testCore->Launch();
}