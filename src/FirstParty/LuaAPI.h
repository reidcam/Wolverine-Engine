//
//  LuaAPI.h
//  wolverine_engine
//
//  Created by Cameron Reid on 7/3/24.
//  Exposes API functions to lua

#ifndef LUAAPI_H
#define LUAAPI_H

#include <thread>

#include "sol/sol.hpp"
#include "box2d/box2d.h"

#include "ActorManager.h"
#include "InputManager.h"
#include "Renderer.h"
#include "RenderRequests.h"
#include "AudioManager.h"
#include "SceneManager.h"
#include "Rigidbody.h"

class LuaAPI
{
public:
	/*
	* Initializes the Lua state
	*/
	static inline void InitLuaState() {
		l_state = new sol::state();
		l_state->open_libraries(sol::lib::base, sol::lib::package, sol::lib::table);
	}

	/*
	* Increments the current frame counter
	*/
	static inline void IncrementFrameCounter() { current_frame++; }

	/*
	* Exposes all of the API functions to Sol to be used in Lua
	*/
	static void ExposeLuaAPI();

	/*
	 * Get the Lua state
	*/
	static inline sol::state* GetLuaState() { return l_state; }
    
    /*
     * Deletes a table on the lua stack
     *
     * Note: does not actualy delete a table from the lua stack as only lua's garbage collection can do that.
     * Instead, this function clears all data from the table and locks it in read-only. This is to make any lingering references to it
     * in other scripts useless, and prevent zombie components from causing undefined behavior.
     * DO NOT EXPOSE TO LUA, THIS IS AN ENGINE TOOL ONLY!
    */
    static void DeleteLuaTable(std::shared_ptr<sol::table> table);
    
private:
	static inline sol::state* l_state;
	static inline int current_frame = -1;

	/*
	* Outputs a string to the console.
	* Used in the Lua API Debug namespace
	* 
	* @parameters    message    the string to output to the console
	*/
	static void Log(const std::string& message);

	/*
	* Outputs an error message to the console.
	* Used in the Lua API Debug namespace
	* 
	* @parameters    message    the string to output to the console
	*/
	static void LogError(const std::string& message);

	/*
	* Quits the application.
	* Used in the Lua API Application namespace
	*/
	static void Quit();

	/*
	* The app will sleep for the specified number of milliseconds.
	* Used in the Lua API Application namespace
	* 
	* @parameter    milliseconds    the number of milliseconds to sleep the application for
	*/
	static void Sleep(const float milliseconds);

	/*
	* Returns the current frame number
	* Used in the Lua API Application namespace
	* 
	* @returns    the current frame number
	*/
	static int GetFrame();

	/*
	* Opens the specified url in the browser
	* Used in the Lua API Application namespace
	* 
	* @parameters    url    the url to open
	*/
	static void OpenURL(const std::string& url);
};

#endif
