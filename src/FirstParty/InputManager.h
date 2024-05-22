//
//  InputManager.hpp
//  wolverine_engine
//
//  Created by Nicholas Way on 5/21/24.
//

#ifndef InputManager_hpp
#define InputManager_hpp

#include <stdio.h>

#include <SDL.h>
#include <unordered_map>
#include <vector>

#include "KeycodeDB.h"

#include "glm.hpp"

enum INPUT_STATE { INPUT_STATE_UP, INPUT_STATE_JUST_BECAME_DOWN, INPUT_STATE_DOWN, INPUT_STATE_JUST_BECAME_UP };

class Input
{
public:
    /**
     * Must be called before the first frame update, initializes all keycodes to INPUT\_STATE\_UP
     */
    static void Init();
    /**
     * Call every frame at the start of the event loop to process incoming inputs.
     */
    static void ProcessEvent(const SDL_Event & e);
    /**
     * Call every frame at the end of the event loop (removes frame-specific conditions)
     */
    static void LateUpdate();

    static bool GetKey(SDL_Scancode keycode);
    static bool GetKey_S(std::string key);
    static bool GetKeyDown(SDL_Scancode keycode);
    static bool GetKeyDown_S(std::string key);
    static bool GetKeyUp(SDL_Scancode keycode);
    static bool GetKeyUp_S(std::string key);
    /**
     * Returns mouse position in world space
     */
    static glm::vec2 GetMousePosition();
    /**
     * Gets mouse buttons (1 = left click, 2 = right click, 3 = middle click)
     */
    static bool GetMouseButton(int button);
    static bool GetMouseButtonDown(int button);
    static bool GetMouseButtonUp(int button);
    static float GetMouseWheelDelta();

    static int ConvertMouseButtonToIndex(const std::string& button_name);

private:
    static inline std::unordered_map<SDL_Scancode, INPUT_STATE> keyboard_states;
    static inline std::vector<SDL_Scancode> just_became_down_scancodes;
    static inline std::vector<SDL_Scancode> just_became_up_scancodes;
    
    static inline glm::vec2 mouse_position;
    static inline std::unordered_map<int, INPUT_STATE> mouse_states;
    static inline std::vector<int> just_became_down_mouse_buttons;
    static inline std::vector<int> just_became_up_mouse_buttons;

    static inline float mouse_scroll_this_frame = 0.0f;
};

#endif /* InputManager_hpp */
