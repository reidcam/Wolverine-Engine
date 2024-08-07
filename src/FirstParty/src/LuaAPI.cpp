//
//  LuaAPI.cpp
//  wolverine_engine
//
//  Created by Cameron Reid on 7/3/24.
//  Exposes API functions to lua

#include "LuaAPI.h"

/**
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
    (*GetLuaState())["Application"]["RandomNumber"] = &EngineUtils::RandomNumber;

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
	(*GetLuaState())["Text"]["Draw"] = &RendererData::DrawText;
    
    // Scene Namespace
    (*GetLuaState())["Scene"] = GetLuaState()->create_table();
    (*GetLuaState())["Scene"]["FindActorByID"] = &Scene::FindActorByID;
    (*GetLuaState())["Scene"]["FindActorWithName"] = &Scene::FindActorWithName;
    (*GetLuaState())["Scene"]["FindAllActorsWithName"] = &Scene::FindAllActorsWithName;
    (*GetLuaState())["Scene"]["Load"] = &Scene::ChangeScene;

	// Audio Namespace
	(*GetLuaState())["Audio"] = GetLuaState()->create_table();
	(*GetLuaState())["Audio"]["Play"] = &AudioManager::AudioPlay;
	(*GetLuaState())["Audio"]["Halt"] = &AudioManager::AudioHalt;
	(*GetLuaState())["Audio"]["SetVolume"] = &AudioManager::AudioSetVolume;
	(*GetLuaState())["Audio"]["SetMasterVolume"] = &AudioManager::AudioSetMasterVolume;
	(*GetLuaState())["Audio"]["Resume"] = &AudioManager::AudioResume;
	(*GetLuaState())["Audio"]["CloseMixer"] = &AudioManager::AudioCloseMixer;
	(*GetLuaState())["Audio"]["DeinitMixer"] = &AudioManager::AudioDeinitMixer;
	(*GetLuaState())["Audio"]["Pause"] = &AudioManager::AudioPause;

	// Image Namespace
	(*GetLuaState())["Image"] = GetLuaState()->create_table();
	(*GetLuaState())["Image"]["Draw"] = &RendererData::DrawImage;
	(*GetLuaState())["Image"]["DrawEx"] = &RendererData::DrawImageEx;
	(*GetLuaState())["Image"]["DrawPixel"] = &RendererData::DrawPixel;
    (*GetLuaState())["Image"]["DrawLine"] = &RendererData::DrawLine;
	(*GetLuaState())["Image"]["DrawUI"] = &RendererData::DrawUI;
	(*GetLuaState())["Image"]["DrawUIEx"] = &RendererData::DrawUIEx;

	// Camera Namespace
	(*GetLuaState())["Camera"] = GetLuaState()->create_table();
	(*GetLuaState())["Camera"]["SetPosition"] = &RendererData::SetCameraPosition;
	(*GetLuaState())["Camera"]["GetPosition"] = &RendererData::GetCameraPosition;
	(*GetLuaState())["Camera"]["SetZoom"] = &RendererData::SetCameraZoom;
	(*GetLuaState())["Camera"]["GetZoom"] = &RendererData::GetCameraZoom;

	// Collision Class
	LuaAPI::GetLuaState()->new_usertype<Collision>("Collision",
		"other_actor_id", &Collision::other_actor_id,
		"this_actor_id", &Collision::this_actor_id,
		"point", &Collision::point,
		"relative_velocity", &Collision::relative_velocity,
		"normal", &Collision::normal
		);

	// Vec2 Namespace
	(*GetLuaState())["vec2"] = GetLuaState()->create_table();
	(*GetLuaState())["vec2"]["Distance"] = &b2Distance;
	//(*GetLuaState())["vec2"]["Dot"] = static_cast<float (*)(const b2Vec2&, const b2Vec2&)>(&b2Dot);

	// Vec2 Class
	LuaAPI::GetLuaState()->new_usertype<b2Vec2>("vec2",
		sol::constructors<b2Vec2(float, float)>(),

		"x", &b2Vec2::x,
		"y", & b2Vec2::y,
		"Normalize", &b2Vec2::Normalize,
		"Length", &b2Vec2::Length,

		// overload the add operator
		sol::meta_function::addition, sol::overload(
			sol::resolve<b2Vec2(const b2Vec2&)>(&b2Vec2::operator_add)),

		// overload the subtract operator
		sol::meta_function::subtraction, sol::overload(
			sol::resolve<b2Vec2(const b2Vec2&)>(&b2Vec2::operator_sub)),

		// overload the multiply operator
		sol::meta_function::multiplication, sol::overload(
			sol::resolve<b2Vec2(const b2Vec2&, float)>(&b2Vec2::operator_mul2),
			sol::resolve<b2Vec2(float, const b2Vec2&)>(&b2Vec2::operator_mul)
		)
	);
	
	// Rigidbody Class
	LuaAPI::GetLuaState()->new_usertype<Rigidbody>("Rigidbody",
		"enabled", &Rigidbody::enabled,
		"key", &Rigidbody::key,
		"type", &Rigidbody::type,
		"actor", &Rigidbody::actor,
        "REMOVED_FROM_ACTOR", &Rigidbody::REMOVED_FROM_ACTOR,
		"x", &Rigidbody::x,
		"y", &Rigidbody::y,
		"body_type", &Rigidbody::body_type,
		"precise", &Rigidbody::precise,
		"gravity_scale", &Rigidbody::gravity_scale,
		"density", &Rigidbody::density,
		"angular_friction", &Rigidbody::angular_friction,
		"rotation", &Rigidbody::rotation,
		"has_collider", &Rigidbody::has_collider,
		"has_trigger", &Rigidbody::has_trigger,
		"collider_type", &Rigidbody::collider_type,
		"trigger_type", &Rigidbody::trigger_type,
		"collider_width", &Rigidbody::collider_width,
		"collider_height", &Rigidbody::collider_height,
		"collider_radius", &Rigidbody::collider_radius,
		"trigger_width", &Rigidbody::trigger_width,
		"trigger_height", &Rigidbody::trigger_height,
		"trigger_radius", &Rigidbody::trigger_radius,
		"friction", &Rigidbody::friction,
		"bounciness", &Rigidbody::bounciness,
		"GetPosition", &Rigidbody::GetPosition,
		"GetRotation", &Rigidbody::GetRotation,
		"OnStart", &Rigidbody::OnStart,
        "OnDestroy", &Rigidbody::OnDestroy,
		"AddForce", &Rigidbody::AddForce,
		"SetVelocity", &Rigidbody::SetVelocity,
		"SetPosition", &Rigidbody::SetPosition,
		"SetRotation", &Rigidbody::SetRotation,
		"SetAngularVelocity", &Rigidbody::SetAngularVelocity,
		"SetGravityScale", &Rigidbody::SetGravityScale,
		"SetUpDirection", &Rigidbody::SetUpDirection,
		"SetRightDirection", &Rigidbody::SetRightDirection,
		"GetVelocity", &Rigidbody::GetVelocity,
		"GetAngularVelocity", &Rigidbody::GetAngularVelocity,
		"GetGravityScale", &Rigidbody::GetGravityScale,
		"GetUpDirection", &Rigidbody::GetUpDirection,
		"GetRightDirection", &Rigidbody::GetRightDirection
	);
    
    // Particle System class
    LuaAPI::GetLuaState()->new_usertype<ParticleSystem>("ParticleSystem",
        // Member variables
        "MAX_NUM_PARTICLES", &ParticleSystem::MAX_NUM_PARTICLES,
        "type", &ParticleSystem::type,
        "key", &ParticleSystem::key,
        "actor", &ParticleSystem::actor,
        "REMOVED_FROM_ACTOR", &ParticleSystem::REMOVED_FROM_ACTOR,
        "enabled", &ParticleSystem::enabled,
        "emitting", &ParticleSystem::emitting,
        "duration", &ParticleSystem::duration,
        "loop", &ParticleSystem::loop,
        "emission_rate", &ParticleSystem::emission_rate,
        "particle_lifetime", &ParticleSystem::particle_lifetime,
        "emitter_pos_x", &ParticleSystem::emitter_pos_x,
        "emitter_pos_y", &ParticleSystem::emitter_pos_y,
        "emission_range_x", &ParticleSystem::emission_range_x,
        "emission_range_y", &ParticleSystem::emission_range_y,
        "emission_direction", &ParticleSystem::emission_direction,
        "emission_direction_range", &ParticleSystem::emission_direction_range,
        "change_movement", &ParticleSystem::change_movement,
        "movement_pattern", &ParticleSystem::movement_pattern,
        "speed", &ParticleSystem::speed,
        "speed_sine_loop", &ParticleSystem::speed_sine_loop,
        "speed_sine_amplitude", &ParticleSystem::speed_sine_amplitude,
        "change_size", &ParticleSystem::change_size,
        "size_pattern", &ParticleSystem::size_pattern,
        "size_change_per_second", &ParticleSystem::size_change_per_second,
        "size_sine_loop", &ParticleSystem::size_sine_loop,
        "image", &ParticleSystem::image,
        "change_color", &ParticleSystem::change_color,
        "colors", &ParticleSystem::colors,
        "sorting_order", &ParticleSystem::sorting_order,
        "x", &ParticleSystem::x,
        "y", &ParticleSystem::y,
        "body_type", &ParticleSystem::body_type,
        "precise", &ParticleSystem::precise,
        "gravity_scale", &ParticleSystem::gravity_scale,
        "density", &ParticleSystem::density,
        "angular_friction", &ParticleSystem::angular_friction,
        "rotation", &ParticleSystem::rotation,
        "mass", &ParticleSystem::mass,
        "has_collider", &ParticleSystem::has_collider,
        "collider_type", &ParticleSystem::collider_type,
        "starting_size", &ParticleSystem::starting_size,
        "friction", &ParticleSystem::friction,
        "bounciness", &ParticleSystem::bounciness,
        
        // Member functions
        "StartEmitting", &ParticleSystem::StartEmitting,
        "StopEmitting", &ParticleSystem::StopEmitting,
        "OnStart", &ParticleSystem::OnStart,
        "OnUpdate", &ParticleSystem::OnUpdate,
        "OnDestroy", &ParticleSystem::OnDestroy
    );

    // SpriteRenderer Class
    LuaAPI::GetLuaState()->new_usertype<SpriteRenderer>("SpriteRenderer",
        "enabled", &SpriteRenderer::enabled,
        "key", &SpriteRenderer::key,
        "type", &SpriteRenderer::type,
        "actor", &SpriteRenderer::actor,
        "REMOVED_FROM_ACTOR", &SpriteRenderer::REMOVED_FROM_ACTOR,
        "sprite", &SpriteRenderer::sprite,
        "x", &SpriteRenderer::x,
        "y", &SpriteRenderer::y,
        "color", &SpriteRenderer::color,
        "scale_x", &SpriteRenderer::scale_x,
        "scale_y", &SpriteRenderer::scale_y,
        "pivot_x", &SpriteRenderer::pivot_x,
        "pivot_y", &SpriteRenderer::pivot_y,
        "rotation", &SpriteRenderer::rotation,
        "sorting_order", &SpriteRenderer::sorting_order,
        "OnUpdate", &SpriteRenderer::OnUpdate,
        "OnStart", &SpriteRenderer::OnStart,
        "OnDestroy", &SpriteRenderer::OnDestroy
    );

    
	// "Actor" Class
	sol::usertype<Actor> actor_type = LuaAPI::GetLuaState()->new_usertype<Actor>("Actor");
	actor_type["ID"] = &Actor::ID;
    
    // The "Actors" namespace
    (*GetLuaState())["Actors"] = GetLuaState()->create_table();
    (*GetLuaState())["Actors"]["GetName"] = &Actors::GetName;
    (*GetLuaState())["Actors"]["GetActorEnabled"] = &Actors::GetActorEnabled;
    (*GetLuaState())["Actors"]["RemoveComponent"] = &Actors::RemoveComponentFromActor;
    (*GetLuaState())["Actors"]["GetComponentByType"] = &Actors::GetComponentByType;
    (*GetLuaState())["Actors"]["GetComponentsByType"] = &Actors::GetComponentsByType;
    (*GetLuaState())["Actors"]["GetComponentByKey"] = &Actors::GetComponentByKey;
    (*GetLuaState())["Actors"]["Instantiate"] = &Scene::Instantiate;
    (*GetLuaState())["Actors"]["Destroy"] = &Scene::Destroy;
}


void deny_write() { std::cout << "error: attempt to modify a dead lua table" << std::endl; }

/**
 * Deletes a table on the lua stack
 *
 * Note: does not actualy delete a table from the lua stack as only lua's garbage collection can do that.
 * Instead, this function clears all data from the table and locks it in read-only. This is to make any lingering references to it
 * in other scripts useless, and prevent zombie components from causing undefined behavior.
 * DO NOT EXPOSE TO LUA, THIS IS AN ENGINE TOOL ONLY!
*/
void LuaAPI::DeleteLuaTable(std::shared_ptr<sol::table> table)
{
    if (ComponentManager::IsComponentTypeNative((*table)["type"]))
    {
        // TODO: Find a way to delete native components FROM LUA STATE without memory shenanigans
        // sol::objects cannot be cleared and locked the same way that tables can. So native components must be treated differently here
    }
    else
    {
        (*table).clear();
        
        // Create the new read-only table
        sol::table dead_table = LuaAPI::GetLuaState()->create_table();
        dead_table[sol::meta_function::new_index] = deny_write;
        dead_table[sol::meta_function::index] = sol::lua_nil;
        
        // Set it on the actual table
        (*table)[sol::metatable_key] = dead_table;
    }
}

/**
* Outputs a string to the console
*
* @param    message    the string to output to the console
*/
void LuaAPI::Log(const std::string& message)
{
	std::cout << message + "\n";
}

/**
* Outputs an error message to the console
*
* @param    message    the string to output to the console
*/
void LuaAPI::LogError(const std::string& message)
{
	std::cerr << message + "\n";
}

/**
* Quits the application.
* Used in the Lua API Application namespace
*/
void LuaAPI::Quit()
{
	exit(0);
}

/**
* The app will sleep for the specified number of milliseconds.
* Used in the Lua API Application namespace
*
* @param    milliseconds    the number of milliseconds to sleep the application for
*/
void LuaAPI::Sleep(const float milliseconds)
{
	int milliseconds_int = static_cast<int>(milliseconds);
	std::chrono::milliseconds duration(milliseconds_int);
	std::this_thread::sleep_for(std::chrono::milliseconds(duration));
}

/**
* Returns the current frame number
*
* @returns    the current frame number
*/
int LuaAPI::GetFrame()
{
	return current_frame;
}

/**
* Opens the specified url in the browser
* Used in the Lua API Application namespace
*
* @param    url    the url to open
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
