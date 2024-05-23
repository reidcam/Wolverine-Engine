//
//  InputManager.cpp
//  wolverine_engine
//
//  Created by Nicholas Way on 5/21/24.
//

#include "InputManager.h"
#include "Engine.h"

void Input::Init(){
    // initialize keyboard_states
    for(int code = SDL_SCANCODE_UNKNOWN; code < SDL_NUM_SCANCODES; code++){
        keyboard_states[(SDL_Scancode)code] = INPUT_STATE_UP;
    }
}

void Input::ProcessEvent(const SDL_Event & e){

    if(e.type == SDL_KEYDOWN){
        keyboard_states[e.key.keysym.scancode] = INPUT_STATE_JUST_BECAME_DOWN;
        just_became_down_scancodes.push_back(e.key.keysym.scancode);
    }
    else if(e.type == SDL_KEYUP){
        keyboard_states[e.key.keysym.scancode] = INPUT_STATE_JUST_BECAME_UP;
        just_became_up_scancodes.push_back(e.key.keysym.scancode);
    }

    // check for mouse events
    if(e.type == SDL_MOUSEBUTTONDOWN){
        int button;
        if(e.button.button == SDL_BUTTON_LEFT){
            button = 1;
        }
        else if(e.button.button == SDL_BUTTON_MIDDLE){
            button = 2;
        }
        else if(e.button.button == SDL_BUTTON_RIGHT){
            button = 3;
        }
        else{
            button = 0;
        }
        mouse_states[button] = INPUT_STATE_JUST_BECAME_DOWN;
        just_became_down_mouse_buttons.push_back(button);
    }
    else if(e.type == SDL_MOUSEBUTTONUP){
        int button;
        if(e.button.button == SDL_BUTTON_LEFT){
            button = 1;
        }
        else if(e.button.button == SDL_BUTTON_MIDDLE){
            button = 2;
        }
        else if(e.button.button == SDL_BUTTON_RIGHT){
            button = 3;
        }
        else{
            button = 0;
        }
        mouse_states[button] = INPUT_STATE_JUST_BECAME_UP;
        just_became_up_mouse_buttons.push_back(button);
    }

    // check for mouse scroll
    if(e.type == SDL_MOUSEWHEEL){
        mouse_scroll_this_frame = e.wheel.preciseY;
    }

    // check for mouse movement
    if(e.type == SDL_MOUSEMOTION){
        mouse_position = glm::vec2(e.motion.x, e.motion.y);
    }
}

void Input::LateUpdate(){
    for(const SDL_Scancode& scancode : just_became_down_scancodes){
        keyboard_states[scancode] = INPUT_STATE_DOWN;
    }
    just_became_down_scancodes.clear();

    for(const SDL_Scancode& scancode : just_became_up_scancodes){
        keyboard_states[scancode] = INPUT_STATE_UP;
    }
    just_became_up_scancodes.clear();

    for(const int& button : just_became_down_mouse_buttons){
        mouse_states[button] = INPUT_STATE_DOWN;
    }
    just_became_down_mouse_buttons.clear();

    for(const int& button : just_became_up_mouse_buttons){
        mouse_states[button] = INPUT_STATE_UP;
    }
    just_became_up_mouse_buttons.clear();
    mouse_scroll_this_frame = 0.0f;
}

bool Input::GetKey(SDL_Scancode keycode){
    return keyboard_states[keycode] == INPUT_STATE_DOWN || keyboard_states[keycode] == INPUT_STATE_JUST_BECAME_DOWN;
}

bool Input::GetKey_S(std::string key){
    if(__keycode_to_scancode.find(key) == __keycode_to_scancode.end()){
        return false;
    }
    return GetKey(__keycode_to_scancode.at(key));
}

bool Input::GetKeyDown(SDL_Scancode keycode){
    return keyboard_states[keycode] == INPUT_STATE_JUST_BECAME_DOWN;
}

bool Input::GetKeyDown_S(std::string key){
    if(__keycode_to_scancode.find(key) == __keycode_to_scancode.end()){
        return false;
    }
    return GetKeyDown(__keycode_to_scancode.at(key));
}

bool Input::GetKeyUp(SDL_Scancode keycode){
    return keyboard_states[keycode] == INPUT_STATE_JUST_BECAME_UP;
}

bool Input::GetKeyUp_S(std::string key){
    if(__keycode_to_scancode.find(key) == __keycode_to_scancode.end()){
        return false;
    }
    return GetKeyUp(__keycode_to_scancode.at(key));
}

glm::vec2 Input::GetMousePosition(){
    return mouse_position;
}

bool Input::GetMouseButton(int button){
    if(button < 1 || button > 3){
        return false;
    }
    return mouse_states[button] == INPUT_STATE_DOWN || mouse_states[button] == INPUT_STATE_JUST_BECAME_DOWN;
}

bool Input::GetMouseButtonDown(int button){
    if(button < 1 || button > 3){
        return false;
    }
    return mouse_states[button] == INPUT_STATE_JUST_BECAME_DOWN;
}

bool Input::GetMouseButtonUp(int button){
    if(button < 1 || button > 3){
        return false;
    }
    return mouse_states[button] == INPUT_STATE_JUST_BECAME_UP;
}

float Input::GetMouseWheelDelta(){
    return mouse_scroll_this_frame;
}
