//
//  LuaAPI.cpp
//  wolverine_engine
//
//  Created by Cameron Reid on 7/3/24.
//  Exposes API functions to lua

#include "LuaAPI.h"

/*
* Exposes all of the API functions to Sol to be used in Lua
*/
void LuaAPI::ExposeLuaAPI()
{
	// Debug Namespace
	(*GetLuaState())["Debug"] = GetLuaState()->create_table();
	(*GetLuaState())["Debug"]["Log"] = &LuaAPI::Log;
	(*GetLuaState())["Debug"]["LogError"] = &LuaAPI::LogError;

	// Application Namespace
	(*GetLuaState())["Application"] = GetLuaState()->create_table();
	(*GetLuaState())["Application"]["Quit"] = &LuaAPI::Quit;
	(*GetLuaState())["Application"]["Sleep"] = &LuaAPI::Sleep;
	(*GetLuaState())["Application"]["GetFrame"] = &LuaAPI::GetFrame;
	(*GetLuaState())["Application"]["OpenURL"] = &LuaAPI::OpenURL;

	// Input Namespace
	(*GetLuaState())["Input"] = GetLuaState()->create_table();
	(*GetLuaState())["Input"]["GetKey"] = &Input::GetKey_S;
	(*GetLuaState())["Input"]["GetKeyDown"] = &Input::GetKeyDown_S;
	(*GetLuaState())["Input"]["GetKeyUp"] = &Input::GetKeyUp_S;
	(*GetLuaState())["Input"]["GetMousePosition"] = &Input::GetMousePosition;
	(*GetLuaState())["Input"]["GetMouseButton"] = &Input::GetMouseButton;
	(*GetLuaState())["Input"]["GetMouseButtonDown"] = &Input::GetMouseButtonDown;
	(*GetLuaState())["Input"]["GetMouseButtonUp"] = &Input::GetMouseButtonUp;
	(*GetLuaState())["Input"]["GetMouseScrollDelta"] = &Input::GetMouseWheelDelta;
	(*GetLuaState())["Input"]["GetButton"] = &Input::GetMouseButton;
	(*GetLuaState())["Input"]["GetButtonDown"] = &Input::GetMouseButtonDown;
	(*GetLuaState())["Input"]["GetButtonUp"] = &Input::GetMouseButtonUp;

	// Text Namespace
	(*GetLuaState())["Text"] = GetLuaState()->create_table();
    
    // Scene Namespace
    (*GetLuaState())["Scene"] = GetLuaState()->create_table();
    // TODO: Move thse functions to the actor static class when its created
    (*GetLuaState())["Scene"]["Instantiate"] = &Scene::Instantiate;
    (*GetLuaState())["Scene"]["Destroy"] = &Scene::Destroy;
    
    (*GetLuaState())["Scene"]["FindActorByID"] = &Scene::FindActorByID;
    (*GetLuaState())["Scene"]["FindActorWithName"] = &Scene::FindActorWithName;
    (*GetLuaState())["Scene"]["FindAllActorsWithName"] = &Scene::FindAllActorsWithName;
    (*GetLuaState())["Scene"]["Load"] = &Scene::ChangeScene;

	// Vec2 Class
	sol::usertype<glm::vec2> vec2_type = LuaAPI::GetLuaState()->new_usertype<glm::vec2>("vec2");
	vec2_type["x"] = &glm::vec2::x;
	vec2_type["y"] = &glm::vec2::y;

	// "Actor" Class
	sol::usertype<Actor> actor_type = LuaAPI::GetLuaState()->new_usertype<Actor>("Actor");
	actor_type["ID"] = &Actor::ID;
	actor_type["GetName"] = &Actors::GetName;
    actor_type["RemoveComponent"] = &Actors::RemoveComponentFromActor;
    actor_type["GetComponentByType"] = &Actors::GetComponentByType;
}


void deny_write() { std::cout << "error: attempt to modify a dead lua table" << std::endl; }
/*
 * Deletes a table on the lua stack
 *
 * Note: does not actualy delete a table from the lua stack as only lua's garbage collection can do that.
 * Instead, this function clears all data from the table and locks it in read-only. This is to make any lingering references to it
 * in other scripts useless, and prevent zombie components from causing undefined behavior.
 * DO NOT EXPOSE TO LUA, THIS IS AN ENGINE TOOL ONLY!
*/
void LuaAPI::DeleteLuaTable(std::shared_ptr<sol::table> table)
{
    (*table).clear();
    
    // Create the new read-only table
    sol::table dead_table = LuaAPI::GetLuaState()->create_table();
    dead_table[sol::meta_function::new_index] = deny_write;
    dead_table[sol::meta_function::index] = sol::lua_nil;
    
    // Set it on the actual table
    (*table)[sol::metatable_key] = dead_table;
}

/*
* Outputs a string to the console
*
* @parameters    message    the string to output to the console
*/
void LuaAPI::Log(const std::string& message)
{
	std::cout << message + "\n";
}

/*
* Outputs an error message to the console
*
* @parameters    message    the string to output to the console
*/
void LuaAPI::LogError(const std::string& message)
{
	std::cerr << message + "\n";
}

/*
* Quits the application.
* Used in the Lua API Application namespace
*/
void LuaAPI::Quit()
{
	exit(0);
}

/*
* The app will sleep for the specified number of milliseconds.
* Used in the Lua API Application namespace
*
* @parameter    milliseconds    the number of milliseconds to sleep the application for
*/
void LuaAPI::Sleep(const float milliseconds)
{
	int milliseconds_int = static_cast<int>(milliseconds);
	std::chrono::milliseconds duration(milliseconds_int);
	std::this_thread::sleep_for(std::chrono::milliseconds(duration));
}

/*
* Returns the current frame number
*
* @returns    the current frame number
*/
int LuaAPI::GetFrame()
{
	return current_frame;
}

/*
* Opens the specified url in the browser
* Used in the Lua API Application namespace
*
* @parameters    url    the url to open
*/
void LuaAPI::OpenURL(const std::string& url)
{
	#if defined(_WIN32) || defined(_WIN64)
		// Windows
		std::string command = "start " + url;
		system(command.c_str());
	#elif defined(__APPLE__) && defined(__MACH__)
		// macOS
		std::string command = "open " + url;
		system(command.c_str());
	#elif defined(__linux__) || defined(__linux) || defined(linux) || defined(__gnu_linux__)
		// Linux
		std::string command = "xdg-open " + url;
		system(command.c_str());
	#else
		std::cerr << "Platform not supported!" << std::endl;
	#endif
}
