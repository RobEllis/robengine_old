// Game Engine Programming Assignment
// By Rob Ellis
// Bournemouth University

#ifndef _CORE_H_
#define _CORE_H_

#include <iomanip>
#include <iostream>
#include <time.h>
#include <string>
#include <Windows.h>

#include "SDL.h"

class Core
{
public:
	// Constructor & Destructor
	Core();
	~Core();

	// Debug Console Logging
	enum RETURNS { RET_SUCC, RET_ERROR };
	enum TYPE { UNDEFINED, STANDARD, WARN, SEVERE };

	// Debug Console Getters
	HANDLE getConsole() { return m_console; };
	time_t getRawTime() { return m_rawtime; };

	// Functions
	void logMsg(TYPE _msgType, std::string _msgBody);
	void logMsg(std::string _msgBody);
	int Launch(void);
	int Init(void);
	int Update(void);
	int Exit(void);

private:
	// Window
	const int s_screenWidth = 1280;
	const int s_screenHeight = 900;

	// SDL
	SDL_Window *m_window;
	SDL_Renderer *m_renderer;
	SDL_GLContext m_context;
	SDL_Event m_events;

	// Debug Console Logging
	HANDLE m_console;
	time_t m_rawtime;

	// Application Variables
	bool m_finished;
	bool m_allowRendering;


	// Time Handling
	struct tm *m_timer;
};

#endif 