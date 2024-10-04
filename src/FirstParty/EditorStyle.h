//
//	EditorStyle.h
//	wolverine_engine
//
//	Created by Cameron Reid on 10/1/2024.
//	Contains sytle functions for the editor
// 
//  Many of the styles in this file come from https://github.com/Patitotective/ImThemes and it's conbtributers
// 
//  DEBUG MODE ONLY

#ifndef EditorStyle_h
#define EditorStyle_h

#include "imgui.h"

class EditorStyle
{
private:
public:
	/**
	* Visual Studio style by MomoDeve from ImThemes
	* https://github.com/Patitotective/ImThemes
	*/
	static void VisualStudioStyle();

	/**
	* Unreal style by dev0-1 from ImThemes
	* https://github.com/Patitotective/ImThemes
	*/
	static void UnrealEngineStyle();

	/**
	* BlackDevil style by Naeemullah1 from ImThemes
	* https://github.com/Patitotective/ImThemes
	*/
	static void BlackDevilStyle();

	/**
	* Cherry style by r-lyeh from ImThemes
	* https://github.com/Patitotective/ImThemes
	*/
	static void CherryStyle();

	/**
	* Light style by dougbinks from ImThemes
	* https://github.com/Patitotective/ImThemes
	*/
	static void LightStyle();

	/**
	* Purple Comfy style by RegularLunar from ImThemes
	* https://github.com/Patitotective/ImThemes
	*/
	static void PurpleComfyStyle();

	/**
	* Soft Cherry style by Patitotective from ImThemes
	* https://github.com/Patitotective/ImThemes
	*/
	static void SoftCherryStyle();

	/**
	* Green Leaf style by Fizub from ImThemes
	* https://github.com/Patitotective/ImThemes
	*/
	static void GreenLeafStyle();

	/**
	* Gold style by CookiePLMonster from ImThemes
	* https://github.com/Patitotective/ImThemes
	*/
	static void GoldStyle();
};

#endif