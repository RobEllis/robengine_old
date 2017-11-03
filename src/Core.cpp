// Game Engine Programming Assignment
// By Rob Ellis
// Bournemouth University

// System Includes
#include <conio.h>
#include <iomanip>
#include <iostream>
#include <time.h>
#include <string>
#include <Windows.h>

// Class Header
#include "Core.h"

// SDL & glew
#include "SDL.h"
#include "GL/glew.h"

// Main Constructor
Core::Core(void)
{
	m_finished = false;
	m_console = GetStdHandle(STD_OUTPUT_HANDLE);
}

// Main Destructor
// For the most part, cleanup of variables is done in Exit()
Core::~Core(void) { }

// The Application itself runs around the Launch() function
// If any stage in the application fails, the program returns as an error
int Core::Launch(void)
{
	if (this->Init() == RET_SUCC)
	{
		if (this->Update() == RET_SUCC)
		{
			if (this->Exit() == RET_SUCC)
			{
				// Program ends gracefully
				return RET_SUCC;
			}
		}
	}

	// This code only runs if the program is on a debug build
	// _getch pauses the program until a key is pressed
	#if _DEBUG
		SetConsoleTextAttribute(m_console, 8);
		std::cout << "Press any key to continue...\n";
		_getch();
	#endif
	// Program does not end gracefully
	return RET_ERROR;
}

// Where the program initialises SDL among other libraries
// This function leads onto the Update() function when completed
int Core::Init(void)
{
	SetConsoleTitle(TEXT("Debug Console"));	// Debug Console Window Title
	SetConsoleTextAttribute(getConsole(), 15); // Sets the colour of the console text to white, operating systems outside of Windows should ignore this

	logMsg("Initialising SDL");
	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		logMsg(SEVERE, "Failed to initialise SDL: " + std::string(SDL_GetError()));
		return RET_ERROR;
	}
	logMsg("Creating Window");
	m_window = SDL_CreateWindow(
		"RobEngine v0.01",
		SDL_WINDOWPOS_UNDEFINED,
		SDL_WINDOWPOS_UNDEFINED,
		s_screenWidth,
		s_screenHeight,
		SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL
	);
	if (m_window == NULL)
	{
		logMsg(SEVERE, "Failed to initialise window: " + std::string(SDL_GetError()));
		return RET_ERROR;
	}

	logMsg("Creating Renderer");
	m_renderer = SDL_CreateRenderer(m_window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	SDL_SetRenderDrawColor(m_renderer, 0, 0, 0, 255);

	logMsg("Creating OpenGL Context");
	m_context = SDL_GL_CreateContext(m_window);

	logMsg("Setting OpenGL attributes");
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

#ifndef __APPLE__
	glewExperimental = GL_TRUE;
	glewInit();
#endif

	logMsg("Finished Initialising");
	return RET_SUCC;
}

// Update() is where the main application loop is located.
// This function leads onto the Exit() function when completed
int Core::Update(void)
{
	logMsg("Entering Loop");
	while (!m_finished)
	{
		while (SDL_PollEvent(&m_events))
		{
			if (m_events.type == SDL_QUIT)
				m_finished = true;
		}

		glClearColor(0.0, 0.0, 0.0, 1.0);
		glClear(GL_COLOR_BUFFER_BIT);
		SDL_GL_SwapWindow(m_window);
	}
	logMsg("Exited Loop");

	return RET_SUCC;
}

// Exit() is where the program ensures that all variables are cleaned up before closing
// After this function completes, the program should exit gracefully
int Core::Exit(void)
{
	logMsg("Destroying OpenGL Context");
	SDL_GL_DeleteContext(m_context);

	logMsg("Destroying Renderer");
	SDL_DestroyRenderer(m_renderer);
	m_renderer = nullptr;

	logMsg("Destroying Window");
	SDL_DestroyWindow(m_window);
	m_window = nullptr;

	logMsg("Shutting down SDL");
	SDL_Quit();

	// This code only runs if the program is on a debug build
	// _getch pauses the program until a key is pressed
	#if _DEBUG
		SetConsoleTextAttribute(m_console, 8);
		std::cout << "Press any key to exit debug window...\n";
		_getch();
	#endif

	return RET_SUCC;
}

/* The following functions serve as a method of logging commands to a debug console
 * The user has two options 
 * - Display the message using std::string _msgBody, with the message being a default 'DEBUG' message type
 * - Display the message with a type, using TYPE _msgType, which is one of 'UNDEFINED', 'STANDARD', 'WARN' or 'SEVERE'
 * While this may work on other operating systems, it is primarily designed for debugging on Windows
*/
void Core::logMsg(TYPE _msgType, std::string _msgBody)
{
	time(&m_rawtime);
	m_timer = localtime(&m_rawtime);
	std::cout << "[" << std::setfill('0') << std::setw(2) << m_timer->tm_hour << ":" << std::setw(2) << m_timer->tm_min << ":" << std::setw(2) << m_timer->tm_sec << " ";
	switch (_msgType)
	{
	case STANDARD:
	{
		std::cout << "DEBUG";
		break;
	}
	case WARN:
	{
		SetConsoleTextAttribute(m_console, 14);
		std::cout << "WARN";
		SetConsoleTextAttribute(m_console, 15);
		break;
	}
	case SEVERE:
	{
		SetConsoleTextAttribute(m_console, 12);
		std::cout << "SEVERE";
		SetConsoleTextAttribute(m_console, 15);
		break;
	}
	case UNDEFINED:
	{
		std::cout << "UNDEFINED";
		break;
	}
	}
	std::cout << "]: " << _msgBody << std::endl;
}

void Core::logMsg(std::string _msgBody)
{
	time(&m_rawtime);
	m_timer = localtime(&m_rawtime);
	std::cout << "[" << std::setfill('0') << std::setw(2) << m_timer->tm_hour << ":" << std::setw(2) << m_timer->tm_min << ":" << std::setw(2) << m_timer->tm_sec << " ";
	std::cout << "DEBUG";
	std::cout << "]: " << _msgBody << std::endl;
}