#include "EditorStyle.h"

/**
* Visual Studio style by MomoDeve from ImThemes
* https://github.com/Patitotective/ImThemes
*/
void EditorStyle::VisualStudioStyle()
{
	// Visual Studio style by MomoDeve from ImThemes
	ImGuiStyle& style = ImGui::GetStyle();

	style.Alpha = 1.0f;
	style.DisabledAlpha = 0.6000000238418579f;
	style.WindowPadding = ImVec2(8.0f, 8.0f);
	style.WindowRounding = 0.0f;
	style.WindowBorderSize = 1.0f;
	style.WindowMinSize = ImVec2(32.0f, 32.0f);
	style.WindowTitleAlign = ImVec2(0.0f, 0.5f);
	style.WindowMenuButtonPosition = ImGuiDir_Left;
	style.ChildRounding = 0.0f;
	style.ChildBorderSize = 1.0f;
	style.PopupRounding = 0.0f;
	style.PopupBorderSize = 1.0f;
	style.FramePadding = ImVec2(4.0f, 3.0f);
	style.FrameRounding = 0.0f;
	style.FrameBorderSize = 0.0f;
	style.ItemSpacing = ImVec2(8.0f, 4.0f);
	style.ItemInnerSpacing = ImVec2(4.0f, 4.0f);
	style.CellPadding = ImVec2(4.0f, 2.0f);
	style.IndentSpacing = 21.0f;
	style.ColumnsMinSpacing = 6.0f;
	style.ScrollbarSize = 14.0f;
	style.ScrollbarRounding = 0.0f;
	style.GrabMinSize = 10.0f;
	style.GrabRounding = 0.0f;
	style.TabRounding = 0.0f;
	style.TabBorderSize = 0.0f;
	style.TabMinWidthForCloseButton = 0.0f;
	style.ColorButtonPosition = ImGuiDir_Right;
	style.ButtonTextAlign = ImVec2(0.5f, 0.5f);
	style.SelectableTextAlign = ImVec2(0.0f, 0.0f);

	style.Colors[ImGuiCol_Text] = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);
	style.Colors[ImGuiCol_TextDisabled] = ImVec4(0.5921568870544434f, 0.5921568870544434f, 0.5921568870544434f, 1.0f);
	style.Colors[ImGuiCol_WindowBg] = ImVec4(0.1450980454683304f, 0.1450980454683304f, 0.1490196138620377f, 1.0f);
	style.Colors[ImGuiCol_ChildBg] = ImVec4(0.1450980454683304f, 0.1450980454683304f, 0.1490196138620377f, 1.0f);
	style.Colors[ImGuiCol_PopupBg] = ImVec4(0.1450980454683304f, 0.1450980454683304f, 0.1490196138620377f, 1.0f);
	style.Colors[ImGuiCol_Border] = ImVec4(0.3058823645114899f, 0.3058823645114899f, 0.3058823645114899f, 1.0f);
	style.Colors[ImGuiCol_BorderShadow] = ImVec4(0.3058823645114899f, 0.3058823645114899f, 0.3058823645114899f, 1.0f);
	style.Colors[ImGuiCol_FrameBg] = ImVec4(0.2000000029802322f, 0.2000000029802322f, 0.2156862765550613f, 1.0f);
	style.Colors[ImGuiCol_FrameBgHovered] = ImVec4(0.1137254908680916f, 0.5921568870544434f, 0.9254902005195618f, 1.0f);
	style.Colors[ImGuiCol_FrameBgActive] = ImVec4(0.0f, 0.4666666686534882f, 0.7843137383460999f, 1.0f);
	style.Colors[ImGuiCol_TitleBg] = ImVec4(0.1450980454683304f, 0.1450980454683304f, 0.1490196138620377f, 1.0f);
	style.Colors[ImGuiCol_TitleBgActive] = ImVec4(0.1450980454683304f, 0.1450980454683304f, 0.1490196138620377f, 1.0f);
	style.Colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.1450980454683304f, 0.1450980454683304f, 0.1490196138620377f, 1.0f);
	style.Colors[ImGuiCol_MenuBarBg] = ImVec4(0.2000000029802322f, 0.2000000029802322f, 0.2156862765550613f, 1.0f);
	style.Colors[ImGuiCol_ScrollbarBg] = ImVec4(0.2000000029802322f, 0.2000000029802322f, 0.2156862765550613f, 1.0f);
	style.Colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.321568638086319f, 0.321568638086319f, 0.3333333432674408f, 1.0f);
	style.Colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.3529411852359772f, 0.3529411852359772f, 0.3725490272045135f, 1.0f);
	style.Colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.3529411852359772f, 0.3529411852359772f, 0.3725490272045135f, 1.0f);
	style.Colors[ImGuiCol_CheckMark] = ImVec4(0.0f, 0.4666666686534882f, 0.7843137383460999f, 1.0f);
	style.Colors[ImGuiCol_SliderGrab] = ImVec4(0.1137254908680916f, 0.5921568870544434f, 0.9254902005195618f, 1.0f);
	style.Colors[ImGuiCol_SliderGrabActive] = ImVec4(0.0f, 0.4666666686534882f, 0.7843137383460999f, 1.0f);
	style.Colors[ImGuiCol_Button] = ImVec4(0.2000000029802322f, 0.2000000029802322f, 0.2156862765550613f, 1.0f);
	style.Colors[ImGuiCol_ButtonHovered] = ImVec4(0.1137254908680916f, 0.5921568870544434f, 0.9254902005195618f, 1.0f);
	style.Colors[ImGuiCol_ButtonActive] = ImVec4(0.1137254908680916f, 0.5921568870544434f, 0.9254902005195618f, 1.0f);
	style.Colors[ImGuiCol_Header] = ImVec4(0.2000000029802322f, 0.2000000029802322f, 0.2156862765550613f, 1.0f);
	style.Colors[ImGuiCol_HeaderHovered] = ImVec4(0.1137254908680916f, 0.5921568870544434f, 0.9254902005195618f, 1.0f);
	style.Colors[ImGuiCol_HeaderActive] = ImVec4(0.0f, 0.4666666686534882f, 0.7843137383460999f, 1.0f);
	style.Colors[ImGuiCol_Separator] = ImVec4(0.3058823645114899f, 0.3058823645114899f, 0.3058823645114899f, 1.0f);
	style.Colors[ImGuiCol_SeparatorHovered] = ImVec4(0.3058823645114899f, 0.3058823645114899f, 0.3058823645114899f, 1.0f);
	style.Colors[ImGuiCol_SeparatorActive] = ImVec4(0.3058823645114899f, 0.3058823645114899f, 0.3058823645114899f, 1.0f);
	style.Colors[ImGuiCol_ResizeGrip] = ImVec4(0.1450980454683304f, 0.1450980454683304f, 0.1490196138620377f, 1.0f);
	style.Colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.2000000029802322f, 0.2000000029802322f, 0.2156862765550613f, 1.0f);
	style.Colors[ImGuiCol_ResizeGripActive] = ImVec4(0.321568638086319f, 0.321568638086319f, 0.3333333432674408f, 1.0f);
	style.Colors[ImGuiCol_Tab] = ImVec4(0.1450980454683304f, 0.1450980454683304f, 0.1490196138620377f, 1.0f);
	style.Colors[ImGuiCol_TabHovered] = ImVec4(0.1137254908680916f, 0.5921568870544434f, 0.9254902005195618f, 1.0f);
	style.Colors[ImGuiCol_TabActive] = ImVec4(0.0f, 0.4666666686534882f, 0.7843137383460999f, 1.0f);
	style.Colors[ImGuiCol_TabUnfocused] = ImVec4(0.1450980454683304f, 0.1450980454683304f, 0.1490196138620377f, 1.0f);
	style.Colors[ImGuiCol_TabUnfocusedActive] = ImVec4(0.0f, 0.4666666686534882f, 0.7843137383460999f, 1.0f);
	style.Colors[ImGuiCol_PlotLines] = ImVec4(0.0f, 0.4666666686534882f, 0.7843137383460999f, 1.0f);
	style.Colors[ImGuiCol_PlotLinesHovered] = ImVec4(0.1137254908680916f, 0.5921568870544434f, 0.9254902005195618f, 1.0f);
	style.Colors[ImGuiCol_PlotHistogram] = ImVec4(0.0f, 0.4666666686534882f, 0.7843137383460999f, 1.0f);
	style.Colors[ImGuiCol_PlotHistogramHovered] = ImVec4(0.1137254908680916f, 0.5921568870544434f, 0.9254902005195618f, 1.0f);
	style.Colors[ImGuiCol_TableHeaderBg] = ImVec4(0.1882352977991104f, 0.1882352977991104f, 0.2000000029802322f, 1.0f);
	style.Colors[ImGuiCol_TableBorderStrong] = ImVec4(0.3098039329051971f, 0.3098039329051971f, 0.3490196168422699f, 1.0f);
	style.Colors[ImGuiCol_TableBorderLight] = ImVec4(0.2274509817361832f, 0.2274509817361832f, 0.2470588237047195f, 1.0f);
	style.Colors[ImGuiCol_TableRowBg] = ImVec4(0.0f, 0.0f, 0.0f, 0.0f);
	style.Colors[ImGuiCol_TableRowBgAlt] = ImVec4(1.0f, 1.0f, 1.0f, 0.05999999865889549f);
	style.Colors[ImGuiCol_TextSelectedBg] = ImVec4(0.0f, 0.4666666686534882f, 0.7843137383460999f, 1.0f);
	style.Colors[ImGuiCol_DragDropTarget] = ImVec4(0.1450980454683304f, 0.1450980454683304f, 0.1490196138620377f, 1.0f);
	style.Colors[ImGuiCol_NavHighlight] = ImVec4(0.1450980454683304f, 0.1450980454683304f, 0.1490196138620377f, 1.0f);
	style.Colors[ImGuiCol_NavWindowingHighlight] = ImVec4(1.0f, 1.0f, 1.0f, 0.699999988079071f);
	style.Colors[ImGuiCol_NavWindowingDimBg] = ImVec4(0.800000011920929f, 0.800000011920929f, 0.800000011920929f, 0.2000000029802322f);
	style.Colors[ImGuiCol_ModalWindowDimBg] = ImVec4(0.1450980454683304f, 0.1450980454683304f, 0.1490196138620377f, 1.0f);
}

/**
* Unreal style by dev0-1 from ImThemes
* https://github.com/Patitotective/ImThemes
*/
void EditorStyle::UnrealEngineStyle()
{
	// Unreal style by dev0-1 from ImThemes
	ImGuiStyle& style = ImGui::GetStyle();

	style.Alpha = 1.0f;
	style.DisabledAlpha = 0.6000000238418579f;
	style.WindowPadding = ImVec2(8.0f, 8.0f);
	style.WindowRounding = 0.0f;
	style.WindowBorderSize = 1.0f;
	style.WindowMinSize = ImVec2(32.0f, 32.0f);
	style.WindowTitleAlign = ImVec2(0.0f, 0.5f);
	style.WindowMenuButtonPosition = ImGuiDir_Left;
	style.ChildRounding = 0.0f;
	style.ChildBorderSize = 1.0f;
	style.PopupRounding = 0.0f;
	style.PopupBorderSize = 1.0f;
	style.FramePadding = ImVec2(4.0f, 3.0f);
	style.FrameRounding = 0.0f;
	style.FrameBorderSize = 0.0f;
	style.ItemSpacing = ImVec2(8.0f, 4.0f);
	style.ItemInnerSpacing = ImVec2(4.0f, 4.0f);
	style.CellPadding = ImVec2(4.0f, 2.0f);
	style.IndentSpacing = 21.0f;
	style.ColumnsMinSpacing = 6.0f;
	style.ScrollbarSize = 14.0f;
	style.ScrollbarRounding = 9.0f;
	style.GrabMinSize = 10.0f;
	style.GrabRounding = 0.0f;
	style.TabRounding = 4.0f;
	style.TabBorderSize = 0.0f;
	style.TabMinWidthForCloseButton = 0.0f;
	style.ColorButtonPosition = ImGuiDir_Right;
	style.ButtonTextAlign = ImVec2(0.5f, 0.5f);
	style.SelectableTextAlign = ImVec2(0.0f, 0.0f);

	style.Colors[ImGuiCol_Text] = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);
	style.Colors[ImGuiCol_TextDisabled] = ImVec4(0.4980392158031464f, 0.4980392158031464f, 0.4980392158031464f, 1.0f);
	style.Colors[ImGuiCol_WindowBg] = ImVec4(0.05882352963089943f, 0.05882352963089943f, 0.05882352963089943f, 0.9399999976158142f);
	style.Colors[ImGuiCol_ChildBg] = ImVec4(1.0f, 1.0f, 1.0f, 0.0f);
	style.Colors[ImGuiCol_PopupBg] = ImVec4(0.0784313753247261f, 0.0784313753247261f, 0.0784313753247261f, 0.9399999976158142f);
	style.Colors[ImGuiCol_Border] = ImVec4(0.4274509847164154f, 0.4274509847164154f, 0.4980392158031464f, 0.5f);
	style.Colors[ImGuiCol_BorderShadow] = ImVec4(0.0f, 0.0f, 0.0f, 0.0f);
	style.Colors[ImGuiCol_FrameBg] = ImVec4(0.2000000029802322f, 0.2078431397676468f, 0.2196078449487686f, 0.5400000214576721f);
	style.Colors[ImGuiCol_FrameBgHovered] = ImVec4(0.4000000059604645f, 0.4000000059604645f, 0.4000000059604645f, 0.4000000059604645f);
	style.Colors[ImGuiCol_FrameBgActive] = ImVec4(0.1764705926179886f, 0.1764705926179886f, 0.1764705926179886f, 0.6700000166893005f);
	style.Colors[ImGuiCol_TitleBg] = ImVec4(0.03921568766236305f, 0.03921568766236305f, 0.03921568766236305f, 1.0f);
	style.Colors[ImGuiCol_TitleBgActive] = ImVec4(0.2862745225429535f, 0.2862745225429535f, 0.2862745225429535f, 1.0f);
	style.Colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.0f, 0.0f, 0.0f, 0.5099999904632568f);
	style.Colors[ImGuiCol_MenuBarBg] = ImVec4(0.1372549086809158f, 0.1372549086809158f, 0.1372549086809158f, 1.0f);
	style.Colors[ImGuiCol_ScrollbarBg] = ImVec4(0.01960784383118153f, 0.01960784383118153f, 0.01960784383118153f, 0.5299999713897705f);
	style.Colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.3098039329051971f, 0.3098039329051971f, 0.3098039329051971f, 1.0f);
	style.Colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.407843142747879f, 0.407843142747879f, 0.407843142747879f, 1.0f);
	style.Colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.5098039507865906f, 0.5098039507865906f, 0.5098039507865906f, 1.0f);
	style.Colors[ImGuiCol_CheckMark] = ImVec4(0.9372549057006836f, 0.9372549057006836f, 0.9372549057006836f, 1.0f);
	style.Colors[ImGuiCol_SliderGrab] = ImVec4(0.5098039507865906f, 0.5098039507865906f, 0.5098039507865906f, 1.0f);
	style.Colors[ImGuiCol_SliderGrabActive] = ImVec4(0.8588235378265381f, 0.8588235378265381f, 0.8588235378265381f, 1.0f);
	style.Colors[ImGuiCol_Button] = ImVec4(0.4392156898975372f, 0.4392156898975372f, 0.4392156898975372f, 0.4000000059604645f);
	style.Colors[ImGuiCol_ButtonHovered] = ImVec4(0.4588235318660736f, 0.4666666686534882f, 0.47843137383461f, 1.0f);
	style.Colors[ImGuiCol_ButtonActive] = ImVec4(0.4196078479290009f, 0.4196078479290009f, 0.4196078479290009f, 1.0f);
	style.Colors[ImGuiCol_Header] = ImVec4(0.6980392336845398f, 0.6980392336845398f, 0.6980392336845398f, 0.3100000023841858f);
	style.Colors[ImGuiCol_HeaderHovered] = ImVec4(0.6980392336845398f, 0.6980392336845398f, 0.6980392336845398f, 0.800000011920929f);
	style.Colors[ImGuiCol_HeaderActive] = ImVec4(0.47843137383461f, 0.4980392158031464f, 0.5176470875740051f, 1.0f);
	style.Colors[ImGuiCol_Separator] = ImVec4(0.4274509847164154f, 0.4274509847164154f, 0.4980392158031464f, 0.5f);
	style.Colors[ImGuiCol_SeparatorHovered] = ImVec4(0.7176470756530762f, 0.7176470756530762f, 0.7176470756530762f, 0.7799999713897705f);
	style.Colors[ImGuiCol_SeparatorActive] = ImVec4(0.5098039507865906f, 0.5098039507865906f, 0.5098039507865906f, 1.0f);
	style.Colors[ImGuiCol_ResizeGrip] = ImVec4(0.9098039269447327f, 0.9098039269447327f, 0.9098039269447327f, 0.25f);
	style.Colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.8078431487083435f, 0.8078431487083435f, 0.8078431487083435f, 0.6700000166893005f);
	style.Colors[ImGuiCol_ResizeGripActive] = ImVec4(0.4588235318660736f, 0.4588235318660736f, 0.4588235318660736f, 0.949999988079071f);
	style.Colors[ImGuiCol_Tab] = ImVec4(0.1764705926179886f, 0.3490196168422699f, 0.5764706134796143f, 0.8619999885559082f);
	style.Colors[ImGuiCol_TabHovered] = ImVec4(0.2588235437870026f, 0.5882353186607361f, 0.9764705896377563f, 0.800000011920929f);
	style.Colors[ImGuiCol_TabActive] = ImVec4(0.196078434586525f, 0.407843142747879f, 0.6784313917160034f, 1.0f);
	style.Colors[ImGuiCol_TabUnfocused] = ImVec4(0.06666667014360428f, 0.1019607856869698f, 0.1450980454683304f, 0.9724000096321106f);
	style.Colors[ImGuiCol_TabUnfocusedActive] = ImVec4(0.1333333402872086f, 0.2588235437870026f, 0.4235294163227081f, 1.0f);
	style.Colors[ImGuiCol_PlotLines] = ImVec4(0.6078431606292725f, 0.6078431606292725f, 0.6078431606292725f, 1.0f);
	style.Colors[ImGuiCol_PlotLinesHovered] = ImVec4(1.0f, 0.4274509847164154f, 0.3490196168422699f, 1.0f);
	style.Colors[ImGuiCol_PlotHistogram] = ImVec4(0.729411780834198f, 0.6000000238418579f, 0.1490196138620377f, 1.0f);
	style.Colors[ImGuiCol_PlotHistogramHovered] = ImVec4(1.0f, 0.6000000238418579f, 0.0f, 1.0f);
	style.Colors[ImGuiCol_TableHeaderBg] = ImVec4(0.1882352977991104f, 0.1882352977991104f, 0.2000000029802322f, 1.0f);
	style.Colors[ImGuiCol_TableBorderStrong] = ImVec4(0.3098039329051971f, 0.3098039329051971f, 0.3490196168422699f, 1.0f);
	style.Colors[ImGuiCol_TableBorderLight] = ImVec4(0.2274509817361832f, 0.2274509817361832f, 0.2470588237047195f, 1.0f);
	style.Colors[ImGuiCol_TableRowBg] = ImVec4(0.0f, 0.0f, 0.0f, 0.0f);
	style.Colors[ImGuiCol_TableRowBgAlt] = ImVec4(1.0f, 1.0f, 1.0f, 0.05999999865889549f);
	style.Colors[ImGuiCol_TextSelectedBg] = ImVec4(0.8666666746139526f, 0.8666666746139526f, 0.8666666746139526f, 0.3499999940395355f);
	style.Colors[ImGuiCol_DragDropTarget] = ImVec4(1.0f, 1.0f, 0.0f, 0.8999999761581421f);
	style.Colors[ImGuiCol_NavHighlight] = ImVec4(0.6000000238418579f, 0.6000000238418579f, 0.6000000238418579f, 1.0f);
	style.Colors[ImGuiCol_NavWindowingHighlight] = ImVec4(1.0f, 1.0f, 1.0f, 0.699999988079071f);
	style.Colors[ImGuiCol_NavWindowingDimBg] = ImVec4(0.800000011920929f, 0.800000011920929f, 0.800000011920929f, 0.2000000029802322f);
	style.Colors[ImGuiCol_ModalWindowDimBg] = ImVec4(0.800000011920929f, 0.800000011920929f, 0.800000011920929f, 0.3499999940395355f);
}

/**
* BlackDevil style by Naeemullah1 from ImThemes
* https://github.com/Patitotective/ImThemes
*/
void EditorStyle::BlackDevilStyle()
{
	// BlackDevil style by Naeemullah1 from ImThemes
	ImGuiStyle& style = ImGui::GetStyle();

	style.Alpha = 1.0f;
	style.DisabledAlpha = 0.5f;
	style.WindowPadding = ImVec2(10.0f, 10.0f);
	style.WindowRounding = 5.0f;
	style.WindowBorderSize = 1.0f;
	style.WindowMinSize = ImVec2(20.0f, 20.0f);
	style.WindowTitleAlign = ImVec2(0.0f, 0.5f);
	style.WindowMenuButtonPosition = ImGuiDir_Left;
	style.ChildRounding = 5.0f;
	style.ChildBorderSize = 1.0f;
	style.PopupRounding = 5.0f;
	style.PopupBorderSize = 1.0f;
	style.FramePadding = ImVec2(5.0f, 5.0f);
	style.FrameRounding = 5.0f;
	style.FrameBorderSize = 0.0f;
	style.ItemSpacing = ImVec2(8.0f, 5.0f);
	style.ItemInnerSpacing = ImVec2(5.0f, 5.0f);
	style.CellPadding = ImVec2(5.0f, 5.0f);
	style.IndentSpacing = 10.0f;
	style.ColumnsMinSpacing = 5.0f;
	style.ScrollbarSize = 10.0f;
	style.ScrollbarRounding = 5.0f;
	style.GrabMinSize = 10.0f;
	style.GrabRounding = 0.0f;
	style.TabRounding = 10.0f;
	style.TabBorderSize = 0.0f;
	style.TabMinWidthForCloseButton = 0.0f;
	style.ColorButtonPosition = ImGuiDir_Right;
	style.ButtonTextAlign = ImVec2(0.5f, 0.5f);
	style.SelectableTextAlign = ImVec2(0.0f, 0.0f);

	style.Colors[ImGuiCol_Text] = ImVec4(0.7843137383460999f, 0.7843137383460999f, 0.7843137383460999f, 1.0f);
	style.Colors[ImGuiCol_TextDisabled] = ImVec4(0.43529412150383f, 0.407843142747879f, 0.3137255012989044f, 1.0f);
	style.Colors[ImGuiCol_WindowBg] = ImVec4(0.05882352963089943f, 0.1176470592617989f, 0.05882352963089943f, 0.3921568691730499f);
	style.Colors[ImGuiCol_ChildBg] = ImVec4(0.05882352963089943f, 0.1176470592617989f, 0.1568627506494522f, 0.7843137383460999f);
	style.Colors[ImGuiCol_PopupBg] = ImVec4(0.0784313753247261f, 0.0784313753247261f, 0.0784313753247261f, 0.7843137383460999f);
	style.Colors[ImGuiCol_Border] = ImVec4(0.3921568691730499f, 0.0f, 0.0f, 0.7843137383460999f);
	style.Colors[ImGuiCol_BorderShadow] = ImVec4(0.0f, 0.0f, 0.0f, 0.0f);
	style.Colors[ImGuiCol_FrameBg] = ImVec4(0.05882352963089943f, 0.1176470592617989f, 0.1568627506494522f, 0.7843137383460999f);
	style.Colors[ImGuiCol_FrameBgHovered] = ImVec4(0.1176470592617989f, 0.2352941185235977f, 0.3529411852359772f, 0.7843137383460999f);
	style.Colors[ImGuiCol_FrameBgActive] = ImVec4(0.3529411852359772f, 0.3529411852359772f, 0.1176470592617989f, 0.7843137383460999f);
	style.Colors[ImGuiCol_TitleBg] = ImVec4(0.05882352963089943f, 0.1176470592617989f, 0.1568627506494522f, 0.7843137383460999f);
	style.Colors[ImGuiCol_TitleBgActive] = ImVec4(0.05882352963089943f, 0.1176470592617989f, 0.1568627506494522f, 0.7843137383460999f);
	style.Colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.05882352963089943f, 0.1176470592617989f, 0.1568627506494522f, 0.196078434586525f);
	style.Colors[ImGuiCol_MenuBarBg] = ImVec4(0.0784313753247261f, 0.0784313753247261f, 0.0784313753247261f, 0.7843137383460999f);
	style.Colors[ImGuiCol_ScrollbarBg] = ImVec4(0.05882352963089943f, 0.1176470592617989f, 0.1568627506494522f, 0.7843137383460999f);
	style.Colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.1176470592617989f, 0.3529411852359772f, 0.2352941185235977f, 0.7843137383460999f);
	style.Colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.1176470592617989f, 0.3529411852359772f, 0.3529411852359772f, 0.7843137383460999f);
	style.Colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.1176470592617989f, 0.5882353186607361f, 0.2352941185235977f, 0.7843137383460999f);
	style.Colors[ImGuiCol_CheckMark] = ImVec4(0.1176470592617989f, 0.5882353186607361f, 0.2352941185235977f, 0.7843137383460999f);
	style.Colors[ImGuiCol_SliderGrab] = ImVec4(0.1176470592617989f, 0.3529411852359772f, 0.2352941185235977f, 0.7843137383460999f);
	style.Colors[ImGuiCol_SliderGrabActive] = ImVec4(0.1176470592617989f, 0.5882353186607361f, 0.2352941185235977f, 0.7843137383460999f);
	style.Colors[ImGuiCol_Button] = ImVec4(0.3529411852359772f, 0.3529411852359772f, 0.1176470592617989f, 0.7843137383460999f);
	style.Colors[ImGuiCol_ButtonHovered] = ImVec4(0.3529411852359772f, 0.4705882370471954f, 0.2352941185235977f, 0.7843137383460999f);
	style.Colors[ImGuiCol_ButtonActive] = ImVec4(0.5882353186607361f, 0.3529411852359772f, 0.2352941185235977f, 0.7843137383460999f);
	style.Colors[ImGuiCol_Header] = ImVec4(0.05882352963089943f, 0.1176470592617989f, 0.1568627506494522f, 0.7843137383460999f);
	style.Colors[ImGuiCol_HeaderHovered] = ImVec4(0.1176470592617989f, 0.3529411852359772f, 0.3529411852359772f, 0.7843137383460999f);
	style.Colors[ImGuiCol_HeaderActive] = ImVec4(0.1176470592617989f, 0.5882353186607361f, 0.2352941185235977f, 0.7843137383460999f);
	style.Colors[ImGuiCol_Separator] = ImVec4(0.3529411852359772f, 0.3529411852359772f, 0.2352941185235977f, 0.7843137383460999f);
	style.Colors[ImGuiCol_SeparatorHovered] = ImVec4(0.1176470592617989f, 0.3529411852359772f, 0.3529411852359772f, 0.7843137383460999f);
	style.Colors[ImGuiCol_SeparatorActive] = ImVec4(0.5882353186607361f, 0.3529411852359772f, 0.2352941185235977f, 0.7843137383460999f);
	style.Colors[ImGuiCol_ResizeGrip] = ImVec4(0.05882352963089943f, 0.1176470592617989f, 0.1568627506494522f, 0.7843137383460999f);
	style.Colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.5882353186607361f, 0.3529411852359772f, 0.3529411852359772f, 0.7843137383460999f);
	style.Colors[ImGuiCol_ResizeGripActive] = ImVec4(0.5882353186607361f, 0.2352941185235977f, 0.2352941185235977f, 0.7843137383460999f);
	style.Colors[ImGuiCol_Tab] = ImVec4(0.3529411852359772f, 0.3529411852359772f, 0.1176470592617989f, 0.7843137383460999f);
	style.Colors[ImGuiCol_TabHovered] = ImVec4(0.3529411852359772f, 0.4705882370471954f, 0.2352941185235977f, 0.7843137383460999f);
	style.Colors[ImGuiCol_TabActive] = ImVec4(0.5882353186607361f, 0.3529411852359772f, 0.2352941185235977f, 0.7843137383460999f);
	style.Colors[ImGuiCol_TabUnfocused] = ImVec4(0.05882352963089943f, 0.1176470592617989f, 0.1568627506494522f, 0.7843137383460999f);
	style.Colors[ImGuiCol_TabUnfocusedActive] = ImVec4(0.5882353186607361f, 0.3529411852359772f, 0.3529411852359772f, 0.7843137383460999f);
	style.Colors[ImGuiCol_PlotLines] = ImVec4(0.3921568691730499f, 0.7843137383460999f, 0.3921568691730499f, 0.7843137383460999f);
	style.Colors[ImGuiCol_PlotLinesHovered] = ImVec4(1.0f, 0.4274509847164154f, 0.3490196168422699f, 0.7843137383460999f);
	style.Colors[ImGuiCol_PlotHistogram] = ImVec4(0.0f, 0.3529411852359772f, 0.3921568691730499f, 0.7843137383460999f);
	style.Colors[ImGuiCol_PlotHistogramHovered] = ImVec4(0.196078434586525f, 0.5882353186607361f, 0.5882353186607361f, 0.7843137383460999f);
	style.Colors[ImGuiCol_TableHeaderBg] = ImVec4(0.1882352977991104f, 0.1882352977991104f, 0.2000000029802322f, 0.7843137383460999f);
	style.Colors[ImGuiCol_TableBorderStrong] = ImVec4(0.3098039329051971f, 0.3098039329051971f, 0.3490196168422699f, 0.7843137383460999f);
	style.Colors[ImGuiCol_TableBorderLight] = ImVec4(0.2274509817361832f, 0.2274509817361832f, 0.2470588237047195f, 0.7843137383460999f);
	style.Colors[ImGuiCol_TableRowBg] = ImVec4(0.0f, 0.0f, 0.0f, 0.7843137383460999f);
	style.Colors[ImGuiCol_TableRowBgAlt] = ImVec4(1.0f, 1.0f, 1.0f, 0.05999999865889549f);
	style.Colors[ImGuiCol_TextSelectedBg] = ImVec4(0.3921568691730499f, 0.3529411852359772f, 0.3921568691730499f, 0.3921568691730499f);
	style.Colors[ImGuiCol_DragDropTarget] = ImVec4(1.0f, 1.0f, 0.0f, 0.8999999761581421f);
	style.Colors[ImGuiCol_NavHighlight] = ImVec4(0.2588235437870026f, 0.5882353186607361f, 0.9764705896377563f, 1.0f);
	style.Colors[ImGuiCol_NavWindowingHighlight] = ImVec4(1.0f, 1.0f, 1.0f, 0.699999988079071f);
	style.Colors[ImGuiCol_NavWindowingDimBg] = ImVec4(0.800000011920929f, 0.800000011920929f, 0.800000011920929f, 0.2000000029802322f);
	style.Colors[ImGuiCol_ModalWindowDimBg] = ImVec4(0.800000011920929f, 0.800000011920929f, 0.800000011920929f, 0.3499999940395355f);
}

/**
* Cherry style by r-lyeh from ImThemes
* https://github.com/Patitotective/ImThemes
*/
void EditorStyle::CherryStyle()
{
	// Cherry style by r-lyeh from ImThemes
	ImGuiStyle& style = ImGui::GetStyle();

	style.Alpha = 1.0f;
	style.DisabledAlpha = 0.6000000238418579f;
	style.WindowPadding = ImVec2(6.0f, 3.0f);
	style.WindowRounding = 0.0f;
	style.WindowBorderSize = 1.0f;
	style.WindowMinSize = ImVec2(32.0f, 32.0f);
	style.WindowTitleAlign = ImVec2(0.5f, 0.5f);
	style.WindowMenuButtonPosition = ImGuiDir_Left;
	style.ChildRounding = 0.0f;
	style.ChildBorderSize = 1.0f;
	style.PopupRounding = 0.0f;
	style.PopupBorderSize = 1.0f;
	style.FramePadding = ImVec2(5.0f, 1.0f);
	style.FrameRounding = 3.0f;
	style.FrameBorderSize = 1.0f;
	style.ItemSpacing = ImVec2(7.0f, 1.0f);
	style.ItemInnerSpacing = ImVec2(1.0f, 1.0f);
	style.CellPadding = ImVec2(4.0f, 2.0f);
	style.IndentSpacing = 6.0f;
	style.ColumnsMinSpacing = 6.0f;
	style.ScrollbarSize = 13.0f;
	style.ScrollbarRounding = 16.0f;
	style.GrabMinSize = 20.0f;
	style.GrabRounding = 2.0f;
	style.TabRounding = 4.0f;
	style.TabBorderSize = 1.0f;
	style.TabMinWidthForCloseButton = 0.0f;
	style.ColorButtonPosition = ImGuiDir_Right;
	style.ButtonTextAlign = ImVec2(0.5f, 0.5f);
	style.SelectableTextAlign = ImVec2(0.0f, 0.0f);

	style.Colors[ImGuiCol_Text] = ImVec4(0.8588235378265381f, 0.929411768913269f, 0.886274516582489f, 0.8799999952316284f);
	style.Colors[ImGuiCol_TextDisabled] = ImVec4(0.8588235378265381f, 0.929411768913269f, 0.886274516582489f, 0.2800000011920929f);
	style.Colors[ImGuiCol_WindowBg] = ImVec4(0.1294117718935013f, 0.1372549086809158f, 0.168627455830574f, 1.0f);
	style.Colors[ImGuiCol_ChildBg] = ImVec4(0.0f, 0.0f, 0.0f, 0.0f);
	style.Colors[ImGuiCol_PopupBg] = ImVec4(0.2000000029802322f, 0.2196078449487686f, 0.2666666805744171f, 0.8999999761581421f);
	style.Colors[ImGuiCol_Border] = ImVec4(0.5372549295425415f, 0.47843137383461f, 0.2549019753932953f, 0.1620000004768372f);
	style.Colors[ImGuiCol_BorderShadow] = ImVec4(0.0f, 0.0f, 0.0f, 0.0f);
	style.Colors[ImGuiCol_FrameBg] = ImVec4(0.2000000029802322f, 0.2196078449487686f, 0.2666666805744171f, 1.0f);
	style.Colors[ImGuiCol_FrameBgHovered] = ImVec4(0.4549019634723663f, 0.196078434586525f, 0.2980392277240753f, 0.7799999713897705f);
	style.Colors[ImGuiCol_FrameBgActive] = ImVec4(0.4549019634723663f, 0.196078434586525f, 0.2980392277240753f, 1.0f);
	style.Colors[ImGuiCol_TitleBg] = ImVec4(0.2313725501298904f, 0.2000000029802322f, 0.2705882489681244f, 1.0f);
	style.Colors[ImGuiCol_TitleBgActive] = ImVec4(0.501960813999176f, 0.07450980693101883f, 0.2549019753932953f, 1.0f);
	style.Colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.2000000029802322f, 0.2196078449487686f, 0.2666666805744171f, 0.75f);
	style.Colors[ImGuiCol_MenuBarBg] = ImVec4(0.2000000029802322f, 0.2196078449487686f, 0.2666666805744171f, 0.4699999988079071f);
	style.Colors[ImGuiCol_ScrollbarBg] = ImVec4(0.2000000029802322f, 0.2196078449487686f, 0.2666666805744171f, 1.0f);
	style.Colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.08627451211214066f, 0.1490196138620377f, 0.1568627506494522f, 1.0f);
	style.Colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.4549019634723663f, 0.196078434586525f, 0.2980392277240753f, 0.7799999713897705f);
	style.Colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.4549019634723663f, 0.196078434586525f, 0.2980392277240753f, 1.0f);
	style.Colors[ImGuiCol_CheckMark] = ImVec4(0.7098039388656616f, 0.2196078449487686f, 0.2666666805744171f, 1.0f);
	style.Colors[ImGuiCol_SliderGrab] = ImVec4(0.4666666686534882f, 0.7686274647712708f, 0.8274509906768799f, 0.1400000005960464f);
	style.Colors[ImGuiCol_SliderGrabActive] = ImVec4(0.7098039388656616f, 0.2196078449487686f, 0.2666666805744171f, 1.0f);
	style.Colors[ImGuiCol_Button] = ImVec4(0.4666666686534882f, 0.7686274647712708f, 0.8274509906768799f, 0.1400000005960464f);
	style.Colors[ImGuiCol_ButtonHovered] = ImVec4(0.4549019634723663f, 0.196078434586525f, 0.2980392277240753f, 0.8600000143051147f);
	style.Colors[ImGuiCol_ButtonActive] = ImVec4(0.4549019634723663f, 0.196078434586525f, 0.2980392277240753f, 1.0f);
	style.Colors[ImGuiCol_Header] = ImVec4(0.4549019634723663f, 0.196078434586525f, 0.2980392277240753f, 0.7599999904632568f);
	style.Colors[ImGuiCol_HeaderHovered] = ImVec4(0.4549019634723663f, 0.196078434586525f, 0.2980392277240753f, 0.8600000143051147f);
	style.Colors[ImGuiCol_HeaderActive] = ImVec4(0.501960813999176f, 0.07450980693101883f, 0.2549019753932953f, 1.0f);
	style.Colors[ImGuiCol_Separator] = ImVec4(0.4274509847164154f, 0.4274509847164154f, 0.4980392158031464f, 0.5f);
	style.Colors[ImGuiCol_SeparatorHovered] = ImVec4(0.09803921729326248f, 0.4000000059604645f, 0.7490196228027344f, 0.7799999713897705f);
	style.Colors[ImGuiCol_SeparatorActive] = ImVec4(0.09803921729326248f, 0.4000000059604645f, 0.7490196228027344f, 1.0f);
	style.Colors[ImGuiCol_ResizeGrip] = ImVec4(0.4666666686534882f, 0.7686274647712708f, 0.8274509906768799f, 0.03999999910593033f);
	style.Colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.4549019634723663f, 0.196078434586525f, 0.2980392277240753f, 0.7799999713897705f);
	style.Colors[ImGuiCol_ResizeGripActive] = ImVec4(0.4549019634723663f, 0.196078434586525f, 0.2980392277240753f, 1.0f);
	style.Colors[ImGuiCol_Tab] = ImVec4(0.1764705926179886f, 0.3490196168422699f, 0.5764706134796143f, 0.8619999885559082f);
	style.Colors[ImGuiCol_TabHovered] = ImVec4(0.2588235437870026f, 0.5882353186607361f, 0.9764705896377563f, 0.800000011920929f);
	style.Colors[ImGuiCol_TabActive] = ImVec4(0.196078434586525f, 0.407843142747879f, 0.6784313917160034f, 1.0f);
	style.Colors[ImGuiCol_TabUnfocused] = ImVec4(0.06666667014360428f, 0.1019607856869698f, 0.1450980454683304f, 0.9724000096321106f);
	style.Colors[ImGuiCol_TabUnfocusedActive] = ImVec4(0.1333333402872086f, 0.2588235437870026f, 0.4235294163227081f, 1.0f);
	style.Colors[ImGuiCol_PlotLines] = ImVec4(0.8588235378265381f, 0.929411768913269f, 0.886274516582489f, 0.6299999952316284f);
	style.Colors[ImGuiCol_PlotLinesHovered] = ImVec4(0.4549019634723663f, 0.196078434586525f, 0.2980392277240753f, 1.0f);
	style.Colors[ImGuiCol_PlotHistogram] = ImVec4(0.8588235378265381f, 0.929411768913269f, 0.886274516582489f, 0.6299999952316284f);
	style.Colors[ImGuiCol_PlotHistogramHovered] = ImVec4(0.4549019634723663f, 0.196078434586525f, 0.2980392277240753f, 1.0f);
	style.Colors[ImGuiCol_TableHeaderBg] = ImVec4(0.1882352977991104f, 0.1882352977991104f, 0.2000000029802322f, 1.0f);
	style.Colors[ImGuiCol_TableBorderStrong] = ImVec4(0.3098039329051971f, 0.3098039329051971f, 0.3490196168422699f, 1.0f);
	style.Colors[ImGuiCol_TableBorderLight] = ImVec4(0.2274509817361832f, 0.2274509817361832f, 0.2470588237047195f, 1.0f);
	style.Colors[ImGuiCol_TableRowBg] = ImVec4(0.0f, 0.0f, 0.0f, 0.0f);
	style.Colors[ImGuiCol_TableRowBgAlt] = ImVec4(1.0f, 1.0f, 1.0f, 0.05999999865889549f);
	style.Colors[ImGuiCol_TextSelectedBg] = ImVec4(0.4549019634723663f, 0.196078434586525f, 0.2980392277240753f, 0.4300000071525574f);
	style.Colors[ImGuiCol_DragDropTarget] = ImVec4(1.0f, 1.0f, 0.0f, 0.8999999761581421f);
	style.Colors[ImGuiCol_NavHighlight] = ImVec4(0.2588235437870026f, 0.5882353186607361f, 0.9764705896377563f, 1.0f);
	style.Colors[ImGuiCol_NavWindowingHighlight] = ImVec4(1.0f, 1.0f, 1.0f, 0.699999988079071f);
	style.Colors[ImGuiCol_NavWindowingDimBg] = ImVec4(0.800000011920929f, 0.800000011920929f, 0.800000011920929f, 0.2000000029802322f);
	style.Colors[ImGuiCol_ModalWindowDimBg] = ImVec4(0.800000011920929f, 0.800000011920929f, 0.800000011920929f, 0.3499999940395355f);
}

/**
* Light style by dougbinks from ImThemes
* https://github.com/Patitotective/ImThemes
*/
void EditorStyle::LightStyle()
{
	// Light style by dougbinks from ImThemes
	ImGuiStyle& style = ImGui::GetStyle();

	style.Alpha = 1.0f;
	style.DisabledAlpha = 0.6000000238418579f;
	style.WindowPadding = ImVec2(8.0f, 8.0f);
	style.WindowRounding = 0.0f;
	style.WindowBorderSize = 1.0f;
	style.WindowMinSize = ImVec2(32.0f, 32.0f);
	style.WindowTitleAlign = ImVec2(0.0f, 0.5f);
	style.WindowMenuButtonPosition = ImGuiDir_Left;
	style.ChildRounding = 0.0f;
	style.ChildBorderSize = 1.0f;
	style.PopupRounding = 0.0f;
	style.PopupBorderSize = 1.0f;
	style.FramePadding = ImVec2(4.0f, 3.0f);
	style.FrameRounding = 0.0f;
	style.FrameBorderSize = 0.0f;
	style.ItemSpacing = ImVec2(8.0f, 4.0f);
	style.ItemInnerSpacing = ImVec2(4.0f, 4.0f);
	style.CellPadding = ImVec2(4.0f, 2.0f);
	style.IndentSpacing = 21.0f;
	style.ColumnsMinSpacing = 6.0f;
	style.ScrollbarSize = 14.0f;
	style.ScrollbarRounding = 9.0f;
	style.GrabMinSize = 10.0f;
	style.GrabRounding = 0.0f;
	style.TabRounding = 4.0f;
	style.TabBorderSize = 0.0f;
	style.TabMinWidthForCloseButton = 0.0f;
	style.ColorButtonPosition = ImGuiDir_Right;
	style.ButtonTextAlign = ImVec2(0.5f, 0.5f);
	style.SelectableTextAlign = ImVec2(0.0f, 0.0f);

	style.Colors[ImGuiCol_Text] = ImVec4(0.0f, 0.0f, 0.0f, 1.0f);
	style.Colors[ImGuiCol_TextDisabled] = ImVec4(0.6000000238418579f, 0.6000000238418579f, 0.6000000238418579f, 1.0f);
	style.Colors[ImGuiCol_WindowBg] = ImVec4(0.9372549057006836f, 0.9372549057006836f, 0.9372549057006836f, 1.0f);
	style.Colors[ImGuiCol_ChildBg] = ImVec4(0.0f, 0.0f, 0.0f, 0.0f);
	style.Colors[ImGuiCol_PopupBg] = ImVec4(1.0f, 1.0f, 1.0f, 0.9800000190734863f);
	style.Colors[ImGuiCol_Border] = ImVec4(0.0f, 0.0f, 0.0f, 0.300000011920929f);
	style.Colors[ImGuiCol_BorderShadow] = ImVec4(0.0f, 0.0f, 0.0f, 0.0f);
	style.Colors[ImGuiCol_FrameBg] = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);
	style.Colors[ImGuiCol_FrameBgHovered] = ImVec4(0.2588235437870026f, 0.5882353186607361f, 0.9764705896377563f, 0.4000000059604645f);
	style.Colors[ImGuiCol_FrameBgActive] = ImVec4(0.2588235437870026f, 0.5882353186607361f, 0.9764705896377563f, 0.6700000166893005f);
	style.Colors[ImGuiCol_TitleBg] = ImVec4(0.95686274766922f, 0.95686274766922f, 0.95686274766922f, 1.0f);
	style.Colors[ImGuiCol_TitleBgActive] = ImVec4(0.8196078538894653f, 0.8196078538894653f, 0.8196078538894653f, 1.0f);
	style.Colors[ImGuiCol_TitleBgCollapsed] = ImVec4(1.0f, 1.0f, 1.0f, 0.5099999904632568f);
	style.Colors[ImGuiCol_MenuBarBg] = ImVec4(0.8588235378265381f, 0.8588235378265381f, 0.8588235378265381f, 1.0f);
	style.Colors[ImGuiCol_ScrollbarBg] = ImVec4(0.9764705896377563f, 0.9764705896377563f, 0.9764705896377563f, 0.5299999713897705f);
	style.Colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.686274528503418f, 0.686274528503418f, 0.686274528503418f, 0.800000011920929f);
	style.Colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.4862745106220245f, 0.4862745106220245f, 0.4862745106220245f, 0.800000011920929f);
	style.Colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.4862745106220245f, 0.4862745106220245f, 0.4862745106220245f, 1.0f);
	style.Colors[ImGuiCol_CheckMark] = ImVec4(0.2588235437870026f, 0.5882353186607361f, 0.9764705896377563f, 1.0f);
	style.Colors[ImGuiCol_SliderGrab] = ImVec4(0.2588235437870026f, 0.5882353186607361f, 0.9764705896377563f, 0.7799999713897705f);
	style.Colors[ImGuiCol_SliderGrabActive] = ImVec4(0.4588235318660736f, 0.5372549295425415f, 0.800000011920929f, 0.6000000238418579f);
	style.Colors[ImGuiCol_Button] = ImVec4(0.2588235437870026f, 0.5882353186607361f, 0.9764705896377563f, 0.4000000059604645f);
	style.Colors[ImGuiCol_ButtonHovered] = ImVec4(0.2588235437870026f, 0.5882353186607361f, 0.9764705896377563f, 1.0f);
	style.Colors[ImGuiCol_ButtonActive] = ImVec4(0.05882352963089943f, 0.529411792755127f, 0.9764705896377563f, 1.0f);
	style.Colors[ImGuiCol_Header] = ImVec4(0.2588235437870026f, 0.5882353186607361f, 0.9764705896377563f, 0.3100000023841858f);
	style.Colors[ImGuiCol_HeaderHovered] = ImVec4(0.2588235437870026f, 0.5882353186607361f, 0.9764705896377563f, 0.800000011920929f);
	style.Colors[ImGuiCol_HeaderActive] = ImVec4(0.2588235437870026f, 0.5882353186607361f, 0.9764705896377563f, 1.0f);
	style.Colors[ImGuiCol_Separator] = ImVec4(0.3882353007793427f, 0.3882353007793427f, 0.3882353007793427f, 0.6200000047683716f);
	style.Colors[ImGuiCol_SeparatorHovered] = ImVec4(0.1372549086809158f, 0.4392156898975372f, 0.800000011920929f, 0.7799999713897705f);
	style.Colors[ImGuiCol_SeparatorActive] = ImVec4(0.1372549086809158f, 0.4392156898975372f, 0.800000011920929f, 1.0f);
	style.Colors[ImGuiCol_ResizeGrip] = ImVec4(0.3490196168422699f, 0.3490196168422699f, 0.3490196168422699f, 0.1700000017881393f);
	style.Colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.2588235437870026f, 0.5882353186607361f, 0.9764705896377563f, 0.6700000166893005f);
	style.Colors[ImGuiCol_ResizeGripActive] = ImVec4(0.2588235437870026f, 0.5882353186607361f, 0.9764705896377563f, 0.949999988079071f);
	style.Colors[ImGuiCol_Tab] = ImVec4(0.7607843279838562f, 0.7960784435272217f, 0.8352941274642944f, 0.9309999942779541f);
	style.Colors[ImGuiCol_TabHovered] = ImVec4(0.2588235437870026f, 0.5882353186607361f, 0.9764705896377563f, 0.800000011920929f);
	style.Colors[ImGuiCol_TabActive] = ImVec4(0.5921568870544434f, 0.7254902124404907f, 0.8823529481887817f, 1.0f);
	style.Colors[ImGuiCol_TabUnfocused] = ImVec4(0.9176470637321472f, 0.9254902005195618f, 0.9333333373069763f, 0.9861999750137329f);
	style.Colors[ImGuiCol_TabUnfocusedActive] = ImVec4(0.7411764860153198f, 0.8196078538894653f, 0.9137254953384399f, 1.0f);
	style.Colors[ImGuiCol_PlotLines] = ImVec4(0.3882353007793427f, 0.3882353007793427f, 0.3882353007793427f, 1.0f);
	style.Colors[ImGuiCol_PlotLinesHovered] = ImVec4(1.0f, 0.4274509847164154f, 0.3490196168422699f, 1.0f);
	style.Colors[ImGuiCol_PlotHistogram] = ImVec4(0.8980392217636108f, 0.6980392336845398f, 0.0f, 1.0f);
	style.Colors[ImGuiCol_PlotHistogramHovered] = ImVec4(1.0f, 0.4470588266849518f, 0.0f, 1.0f);
	style.Colors[ImGuiCol_TableHeaderBg] = ImVec4(0.7764706015586853f, 0.8666666746139526f, 0.9764705896377563f, 1.0f);
	style.Colors[ImGuiCol_TableBorderStrong] = ImVec4(0.5686274766921997f, 0.5686274766921997f, 0.6392157077789307f, 1.0f);
	style.Colors[ImGuiCol_TableBorderLight] = ImVec4(0.6784313917160034f, 0.6784313917160034f, 0.7372549176216125f, 1.0f);
	style.Colors[ImGuiCol_TableRowBg] = ImVec4(0.0f, 0.0f, 0.0f, 0.0f);
	style.Colors[ImGuiCol_TableRowBgAlt] = ImVec4(0.2980392277240753f, 0.2980392277240753f, 0.2980392277240753f, 0.09000000357627869f);
	style.Colors[ImGuiCol_TextSelectedBg] = ImVec4(0.2588235437870026f, 0.5882353186607361f, 0.9764705896377563f, 0.3499999940395355f);
	style.Colors[ImGuiCol_DragDropTarget] = ImVec4(0.2588235437870026f, 0.5882353186607361f, 0.9764705896377563f, 0.949999988079071f);
	style.Colors[ImGuiCol_NavHighlight] = ImVec4(0.2588235437870026f, 0.5882353186607361f, 0.9764705896377563f, 0.800000011920929f);
	style.Colors[ImGuiCol_NavWindowingHighlight] = ImVec4(0.6980392336845398f, 0.6980392336845398f, 0.6980392336845398f, 0.699999988079071f);
	style.Colors[ImGuiCol_NavWindowingDimBg] = ImVec4(0.2000000029802322f, 0.2000000029802322f, 0.2000000029802322f, 0.2000000029802322f);
	style.Colors[ImGuiCol_ModalWindowDimBg] = ImVec4(0.2000000029802322f, 0.2000000029802322f, 0.2000000029802322f, 0.3499999940395355f);
}

/**
* Purple Comfy style by RegularLunar from ImThemes
* https://github.com/Patitotective/ImThemes
*/
void EditorStyle::PurpleComfyStyle()
{
	// Purple Comfy style by RegularLunar from ImThemes
	ImGuiStyle& style = ImGui::GetStyle();

	style.Alpha = 1.0f;
	style.DisabledAlpha = 0.1000000014901161f;
	style.WindowPadding = ImVec2(8.0f, 8.0f);
	style.WindowRounding = 10.0f;
	style.WindowBorderSize = 0.0f;
	style.WindowMinSize = ImVec2(30.0f, 30.0f);
	style.WindowTitleAlign = ImVec2(0.5f, 0.5f);
	style.WindowMenuButtonPosition = ImGuiDir_Right;
	style.ChildRounding = 5.0f;
	style.ChildBorderSize = 1.0f;
	style.PopupRounding = 10.0f;
	style.PopupBorderSize = 0.0f;
	style.FramePadding = ImVec2(5.0f, 3.5f);
	style.FrameRounding = 5.0f;
	style.FrameBorderSize = 0.0f;
	style.ItemSpacing = ImVec2(5.0f, 4.0f);
	style.ItemInnerSpacing = ImVec2(5.0f, 5.0f);
	style.CellPadding = ImVec2(4.0f, 2.0f);
	style.IndentSpacing = 5.0f;
	style.ColumnsMinSpacing = 5.0f;
	style.ScrollbarSize = 15.0f;
	style.ScrollbarRounding = 9.0f;
	style.GrabMinSize = 15.0f;
	style.GrabRounding = 5.0f;
	style.TabRounding = 5.0f;
	style.TabBorderSize = 0.0f;
	style.TabMinWidthForCloseButton = 0.0f;
	style.ColorButtonPosition = ImGuiDir_Right;
	style.ButtonTextAlign = ImVec2(0.5f, 0.5f);
	style.SelectableTextAlign = ImVec2(0.0f, 0.0f);

	style.Colors[ImGuiCol_Text] = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);
	style.Colors[ImGuiCol_TextDisabled] = ImVec4(1.0f, 1.0f, 1.0f, 0.3605149984359741f);
	style.Colors[ImGuiCol_WindowBg] = ImVec4(0.09803921729326248f, 0.09803921729326248f, 0.09803921729326248f, 1.0f);
	style.Colors[ImGuiCol_ChildBg] = ImVec4(1.0f, 0.0f, 0.0f, 0.0f);
	style.Colors[ImGuiCol_PopupBg] = ImVec4(0.09803921729326248f, 0.09803921729326248f, 0.09803921729326248f, 1.0f);
	style.Colors[ImGuiCol_Border] = ImVec4(0.501960813999176f, 0.3019607961177826f, 1.0f, 0.5490196347236633f);
	style.Colors[ImGuiCol_BorderShadow] = ImVec4(0.0f, 0.0f, 0.0f, 0.0f);
	style.Colors[ImGuiCol_FrameBg] = ImVec4(0.1568627506494522f, 0.1568627506494522f, 0.1568627506494522f, 1.0f);
	style.Colors[ImGuiCol_FrameBgHovered] = ImVec4(0.3803921639919281f, 0.4235294163227081f, 0.572549045085907f, 0.5490196347236633f);
	style.Colors[ImGuiCol_FrameBgActive] = ImVec4(0.501960813999176f, 0.3019607961177826f, 1.0f, 0.5490196347236633f);
	style.Colors[ImGuiCol_TitleBg] = ImVec4(0.09803921729326248f, 0.09803921729326248f, 0.09803921729326248f, 1.0f);
	style.Colors[ImGuiCol_TitleBgActive] = ImVec4(0.09803921729326248f, 0.09803921729326248f, 0.09803921729326248f, 1.0f);
	style.Colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.2588235437870026f, 0.2588235437870026f, 0.2588235437870026f, 0.0f);
	style.Colors[ImGuiCol_MenuBarBg] = ImVec4(0.0f, 0.0f, 0.0f, 0.0f);
	style.Colors[ImGuiCol_ScrollbarBg] = ImVec4(0.1568627506494522f, 0.1568627506494522f, 0.1568627506494522f, 0.0f);
	style.Colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.1568627506494522f, 0.1568627506494522f, 0.1568627506494522f, 1.0f);
	style.Colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.2352941185235977f, 0.2352941185235977f, 0.2352941185235977f, 1.0f);
	style.Colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.294117659330368f, 0.294117659330368f, 0.294117659330368f, 1.0f);
	style.Colors[ImGuiCol_CheckMark] = ImVec4(0.501960813999176f, 0.3019607961177826f, 1.0f, 0.5490196347236633f);
	style.Colors[ImGuiCol_SliderGrab] = ImVec4(0.501960813999176f, 0.3019607961177826f, 1.0f, 0.5490196347236633f);
	style.Colors[ImGuiCol_SliderGrabActive] = ImVec4(0.501960813999176f, 0.3019607961177826f, 1.0f, 0.5490196347236633f);
	style.Colors[ImGuiCol_Button] = ImVec4(0.501960813999176f, 0.3019607961177826f, 1.0f, 0.5490196347236633f);
	style.Colors[ImGuiCol_ButtonHovered] = ImVec4(0.501960813999176f, 0.3019607961177826f, 1.0f, 0.5490196347236633f);
	style.Colors[ImGuiCol_ButtonActive] = ImVec4(0.501960813999176f, 0.3019607961177826f, 1.0f, 0.5490196347236633f);
	style.Colors[ImGuiCol_Header] = ImVec4(0.501960813999176f, 0.3019607961177826f, 1.0f, 0.5490196347236633f);
	style.Colors[ImGuiCol_HeaderHovered] = ImVec4(0.501960813999176f, 0.3019607961177826f, 1.0f, 0.5490196347236633f);
	style.Colors[ImGuiCol_HeaderActive] = ImVec4(0.501960813999176f, 0.3019607961177826f, 1.0f, 0.5490196347236633f);
	style.Colors[ImGuiCol_Separator] = ImVec4(0.501960813999176f, 0.3019607961177826f, 1.0f, 0.5490196347236633f);
	style.Colors[ImGuiCol_SeparatorHovered] = ImVec4(0.501960813999176f, 0.3019607961177826f, 1.0f, 0.5490196347236633f);
	style.Colors[ImGuiCol_SeparatorActive] = ImVec4(0.501960813999176f, 0.3019607961177826f, 1.0f, 0.5490196347236633f);
	style.Colors[ImGuiCol_ResizeGrip] = ImVec4(0.501960813999176f, 0.3019607961177826f, 1.0f, 0.5490196347236633f);
	style.Colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.501960813999176f, 0.3019607961177826f, 1.0f, 0.5490196347236633f);
	style.Colors[ImGuiCol_ResizeGripActive] = ImVec4(0.501960813999176f, 0.3019607961177826f, 1.0f, 0.5490196347236633f);
	style.Colors[ImGuiCol_Tab] = ImVec4(0.501960813999176f, 0.3019607961177826f, 1.0f, 0.5490196347236633f);
	style.Colors[ImGuiCol_TabHovered] = ImVec4(0.501960813999176f, 0.3019607961177826f, 1.0f, 0.5490196347236633f);
	style.Colors[ImGuiCol_TabActive] = ImVec4(0.501960813999176f, 0.3019607961177826f, 1.0f, 0.5490196347236633f);
	style.Colors[ImGuiCol_TabUnfocused] = ImVec4(0.0f, 0.4509803950786591f, 1.0f, 0.0f);
	style.Colors[ImGuiCol_TabUnfocusedActive] = ImVec4(0.1333333402872086f, 0.2588235437870026f, 0.4235294163227081f, 0.0f);
	style.Colors[ImGuiCol_PlotLines] = ImVec4(0.294117659330368f, 0.294117659330368f, 0.294117659330368f, 1.0f);
	style.Colors[ImGuiCol_PlotLinesHovered] = ImVec4(0.501960813999176f, 0.3019607961177826f, 1.0f, 0.5490196347236633f);
	style.Colors[ImGuiCol_PlotHistogram] = ImVec4(0.501960813999176f, 0.3019607961177826f, 1.0f, 0.5490196347236633f);
	style.Colors[ImGuiCol_PlotHistogramHovered] = ImVec4(0.7372549176216125f, 0.6941176652908325f, 0.886274516582489f, 0.5490196347236633f);
	style.Colors[ImGuiCol_TableHeaderBg] = ImVec4(0.1882352977991104f, 0.1882352977991104f, 0.2000000029802322f, 1.0f);
	style.Colors[ImGuiCol_TableBorderStrong] = ImVec4(0.501960813999176f, 0.3019607961177826f, 1.0f, 0.5490196347236633f);
	style.Colors[ImGuiCol_TableBorderLight] = ImVec4(0.501960813999176f, 0.3019607961177826f, 1.0f, 0.2901960909366608f);
	style.Colors[ImGuiCol_TableRowBg] = ImVec4(0.0f, 0.0f, 0.0f, 0.0f);
	style.Colors[ImGuiCol_TableRowBgAlt] = ImVec4(1.0f, 1.0f, 1.0f, 0.03433477878570557f);
	style.Colors[ImGuiCol_TextSelectedBg] = ImVec4(0.501960813999176f, 0.3019607961177826f, 1.0f, 0.5490196347236633f);
	style.Colors[ImGuiCol_DragDropTarget] = ImVec4(1.0f, 1.0f, 0.0f, 0.8999999761581421f);
	style.Colors[ImGuiCol_NavHighlight] = ImVec4(0.0f, 0.0f, 0.0f, 1.0f);
	style.Colors[ImGuiCol_NavWindowingHighlight] = ImVec4(1.0f, 1.0f, 1.0f, 0.699999988079071f);
	style.Colors[ImGuiCol_NavWindowingDimBg] = ImVec4(0.800000011920929f, 0.800000011920929f, 0.800000011920929f, 0.2000000029802322f);
	style.Colors[ImGuiCol_ModalWindowDimBg] = ImVec4(0.800000011920929f, 0.800000011920929f, 0.800000011920929f, 0.3499999940395355f);
}

/**
* Soft Cherry style by Patitotective from ImThemes
* https://github.com/Patitotective/ImThemes
*/
void EditorStyle::SoftCherryStyle()
{
	// Soft Cherry style by Patitotective from ImThemes
	ImGuiStyle& style = ImGui::GetStyle();

	style.Alpha = 1.0f;
	style.DisabledAlpha = 0.4000000059604645f;
	style.WindowPadding = ImVec2(10.0f, 10.0f);
	style.WindowRounding = 4.0f;
	style.WindowBorderSize = 0.0f;
	style.WindowMinSize = ImVec2(50.0f, 50.0f);
	style.WindowTitleAlign = ImVec2(0.5f, 0.5f);
	style.WindowMenuButtonPosition = ImGuiDir_Left;
	style.ChildRounding = 0.0f;
	style.ChildBorderSize = 1.0f;
	style.PopupRounding = 1.0f;
	style.PopupBorderSize = 1.0f;
	style.FramePadding = ImVec2(5.0f, 3.0f);
	style.FrameRounding = 3.0f;
	style.FrameBorderSize = 0.0f;
	style.ItemSpacing = ImVec2(6.0f, 6.0f);
	style.ItemInnerSpacing = ImVec2(3.0f, 2.0f);
	style.CellPadding = ImVec2(3.0f, 3.0f);
	style.IndentSpacing = 6.0f;
	style.ColumnsMinSpacing = 6.0f;
	style.ScrollbarSize = 13.0f;
	style.ScrollbarRounding = 16.0f;
	style.GrabMinSize = 20.0f;
	style.GrabRounding = 4.0f;
	style.TabRounding = 4.0f;
	style.TabBorderSize = 1.0f;
	style.TabMinWidthForCloseButton = 0.0f;
	style.ColorButtonPosition = ImGuiDir_Right;
	style.ButtonTextAlign = ImVec2(0.5f, 0.5f);
	style.SelectableTextAlign = ImVec2(0.0f, 0.0f);

	style.Colors[ImGuiCol_Text] = ImVec4(0.8588235378265381f, 0.929411768913269f, 0.886274516582489f, 1.0f);
	style.Colors[ImGuiCol_TextDisabled] = ImVec4(0.5215686559677124f, 0.5490196347236633f, 0.5333333611488342f, 1.0f);
	style.Colors[ImGuiCol_WindowBg] = ImVec4(0.1294117718935013f, 0.1372549086809158f, 0.168627455830574f, 1.0f);
	style.Colors[ImGuiCol_ChildBg] = ImVec4(0.1490196138620377f, 0.1568627506494522f, 0.1882352977991104f, 1.0f);
	style.Colors[ImGuiCol_PopupBg] = ImVec4(0.2000000029802322f, 0.2196078449487686f, 0.2666666805744171f, 1.0f);
	style.Colors[ImGuiCol_Border] = ImVec4(0.1372549086809158f, 0.1137254908680916f, 0.1333333402872086f, 1.0f);
	style.Colors[ImGuiCol_BorderShadow] = ImVec4(0.0f, 0.0f, 0.0f, 1.0f);
	style.Colors[ImGuiCol_FrameBg] = ImVec4(0.168627455830574f, 0.1843137294054031f, 0.2313725501298904f, 1.0f);
	style.Colors[ImGuiCol_FrameBgHovered] = ImVec4(0.4549019634723663f, 0.196078434586525f, 0.2980392277240753f, 1.0f);
	style.Colors[ImGuiCol_FrameBgActive] = ImVec4(0.4549019634723663f, 0.196078434586525f, 0.2980392277240753f, 1.0f);
	style.Colors[ImGuiCol_TitleBg] = ImVec4(0.2313725501298904f, 0.2000000029802322f, 0.2705882489681244f, 1.0f);
	style.Colors[ImGuiCol_TitleBgActive] = ImVec4(0.501960813999176f, 0.07450980693101883f, 0.2549019753932953f, 1.0f);
	style.Colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.2000000029802322f, 0.2196078449487686f, 0.2666666805744171f, 1.0f);
	style.Colors[ImGuiCol_MenuBarBg] = ImVec4(0.2000000029802322f, 0.2196078449487686f, 0.2666666805744171f, 1.0f);
	style.Colors[ImGuiCol_ScrollbarBg] = ImVec4(0.239215686917305f, 0.239215686917305f, 0.2196078449487686f, 1.0f);
	style.Colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.3882353007793427f, 0.3882353007793427f, 0.3725490272045135f, 1.0f);
	style.Colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.6941176652908325f, 0.6941176652908325f, 0.686274528503418f, 1.0f);
	style.Colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.6941176652908325f, 0.6941176652908325f, 0.686274528503418f, 1.0f);
	style.Colors[ImGuiCol_CheckMark] = ImVec4(0.658823549747467f, 0.1372549086809158f, 0.1764705926179886f, 1.0f);
	style.Colors[ImGuiCol_SliderGrab] = ImVec4(0.6509804129600525f, 0.1490196138620377f, 0.3450980484485626f, 1.0f);
	style.Colors[ImGuiCol_SliderGrabActive] = ImVec4(0.7098039388656616f, 0.2196078449487686f, 0.2666666805744171f, 1.0f);
	style.Colors[ImGuiCol_Button] = ImVec4(0.6509804129600525f, 0.1490196138620377f, 0.3450980484485626f, 1.0f);
	style.Colors[ImGuiCol_ButtonHovered] = ImVec4(0.4549019634723663f, 0.196078434586525f, 0.2980392277240753f, 1.0f);
	style.Colors[ImGuiCol_ButtonActive] = ImVec4(0.4549019634723663f, 0.196078434586525f, 0.2980392277240753f, 1.0f);
	style.Colors[ImGuiCol_Header] = ImVec4(0.4549019634723663f, 0.196078434586525f, 0.2980392277240753f, 1.0f);
	style.Colors[ImGuiCol_HeaderHovered] = ImVec4(0.6509804129600525f, 0.1490196138620377f, 0.3450980484485626f, 1.0f);
	style.Colors[ImGuiCol_HeaderActive] = ImVec4(0.501960813999176f, 0.07450980693101883f, 0.2549019753932953f, 1.0f);
	style.Colors[ImGuiCol_Separator] = ImVec4(0.4274509847164154f, 0.4274509847164154f, 0.4980392158031464f, 1.0f);
	style.Colors[ImGuiCol_SeparatorHovered] = ImVec4(0.09803921729326248f, 0.4000000059604645f, 0.7490196228027344f, 1.0f);
	style.Colors[ImGuiCol_SeparatorActive] = ImVec4(0.09803921729326248f, 0.4000000059604645f, 0.7490196228027344f, 1.0f);
	style.Colors[ImGuiCol_ResizeGrip] = ImVec4(0.6509804129600525f, 0.1490196138620377f, 0.3450980484485626f, 1.0f);
	style.Colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.4549019634723663f, 0.196078434586525f, 0.2980392277240753f, 1.0f);
	style.Colors[ImGuiCol_ResizeGripActive] = ImVec4(0.4549019634723663f, 0.196078434586525f, 0.2980392277240753f, 1.0f);
	style.Colors[ImGuiCol_Tab] = ImVec4(0.1764705926179886f, 0.3490196168422699f, 0.5764706134796143f, 1.0f);
	style.Colors[ImGuiCol_TabHovered] = ImVec4(0.2588235437870026f, 0.5882353186607361f, 0.9764705896377563f, 1.0f);
	style.Colors[ImGuiCol_TabActive] = ImVec4(0.196078434586525f, 0.407843142747879f, 0.6784313917160034f, 1.0f);
	style.Colors[ImGuiCol_TabUnfocused] = ImVec4(0.06666667014360428f, 0.1019607856869698f, 0.1450980454683304f, 1.0f);
	style.Colors[ImGuiCol_TabUnfocusedActive] = ImVec4(0.1333333402872086f, 0.2588235437870026f, 0.4235294163227081f, 1.0f);
	style.Colors[ImGuiCol_PlotLines] = ImVec4(0.8588235378265381f, 0.929411768913269f, 0.886274516582489f, 1.0f);
	style.Colors[ImGuiCol_PlotLinesHovered] = ImVec4(0.4549019634723663f, 0.196078434586525f, 0.2980392277240753f, 1.0f);
	style.Colors[ImGuiCol_PlotHistogram] = ImVec4(0.3098039329051971f, 0.7764706015586853f, 0.196078434586525f, 1.0f);
	style.Colors[ImGuiCol_PlotHistogramHovered] = ImVec4(0.4549019634723663f, 0.196078434586525f, 0.2980392277240753f, 1.0f);
	style.Colors[ImGuiCol_TableHeaderBg] = ImVec4(0.1882352977991104f, 0.1882352977991104f, 0.2000000029802322f, 1.0f);
	style.Colors[ImGuiCol_TableBorderStrong] = ImVec4(0.3098039329051971f, 0.3098039329051971f, 0.3490196168422699f, 1.0f);
	style.Colors[ImGuiCol_TableBorderLight] = ImVec4(0.2274509817361832f, 0.2274509817361832f, 0.2470588237047195f, 1.0f);
	style.Colors[ImGuiCol_TableRowBg] = ImVec4(0.0f, 0.0f, 0.0f, 1.0f);
	style.Colors[ImGuiCol_TableRowBgAlt] = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);
	style.Colors[ImGuiCol_TextSelectedBg] = ImVec4(0.3843137323856354f, 0.6274510025978088f, 0.9176470637321472f, 1.0f);
	style.Colors[ImGuiCol_DragDropTarget] = ImVec4(1.0f, 1.0f, 0.0f, 1.0f);
	style.Colors[ImGuiCol_NavHighlight] = ImVec4(0.2588235437870026f, 0.5882353186607361f, 0.9764705896377563f, 1.0f);
	style.Colors[ImGuiCol_NavWindowingHighlight] = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);
	style.Colors[ImGuiCol_NavWindowingDimBg] = ImVec4(0.800000011920929f, 0.800000011920929f, 0.800000011920929f, 1.0f);
	style.Colors[ImGuiCol_ModalWindowDimBg] = ImVec4(0.800000011920929f, 0.800000011920929f, 0.800000011920929f, 0.300000011920929f);
}

/**
* Green Leaf style by Fizub from ImThemes
* https://github.com/Patitotective/ImThemes
*/
void EditorStyle::GreenLeafStyle()
{
	// Green Leaf style by Fizub from ImThemes
	ImGuiStyle& style = ImGui::GetStyle();

	style.Alpha = 1.0f;
	style.DisabledAlpha = 0.6000000238418579f;
	style.WindowPadding = ImVec2(8.0f, 8.0f);
	style.WindowRounding = 0.0f;
	style.WindowBorderSize = 0.0f;
	style.WindowMinSize = ImVec2(32.0f, 32.0f);
	style.WindowTitleAlign = ImVec2(0.0f, 0.5f);
	style.WindowMenuButtonPosition = ImGuiDir_Left;
	style.ChildRounding = 4.199999809265137f;
	style.ChildBorderSize = 1.0f;
	style.PopupRounding = 7.300000190734863f;
	style.PopupBorderSize = 1.0f;
	style.FramePadding = ImVec2(4.0f, 3.0f);
	style.FrameRounding = 0.0f;
	style.FrameBorderSize = 0.0f;
	style.ItemSpacing = ImVec2(8.0f, 4.0f);
	style.ItemInnerSpacing = ImVec2(4.0f, 4.0f);
	style.CellPadding = ImVec2(4.0f, 2.0f);
	style.IndentSpacing = 21.0f;
	style.ColumnsMinSpacing = 6.0f;
	style.ScrollbarSize = 10.19999980926514f;
	style.ScrollbarRounding = 9.0f;
	style.GrabMinSize = 10.0f;
	style.GrabRounding = 0.0f;
	style.TabRounding = 4.0f;
	style.TabBorderSize = 0.0f;
	style.TabMinWidthForCloseButton = 0.0f;
	style.ColorButtonPosition = ImGuiDir_Right;
	style.ButtonTextAlign = ImVec2(0.5f, 0.5f);
	style.SelectableTextAlign = ImVec2(0.0f, 0.0f);

	style.Colors[ImGuiCol_Text] = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);
	style.Colors[ImGuiCol_TextDisabled] = ImVec4(0.4431372582912445f, 0.4431372582912445f, 0.4431372582912445f, 1.0f);
	style.Colors[ImGuiCol_WindowBg] = ImVec4(0.196078434586525f, 0.2274509817361832f, 0.1411764770746231f, 0.8500000238418579f);
	style.Colors[ImGuiCol_ChildBg] = ImVec4(0.0f, 0.0f, 0.0f, 0.0f);
	style.Colors[ImGuiCol_PopupBg] = ImVec4(0.1098039224743843f, 0.1098039224743843f, 0.1372549086809158f, 0.9200000166893005f);
	style.Colors[ImGuiCol_Border] = ImVec4(0.4980392158031464f, 0.4980392158031464f, 0.4980392158031464f, 0.5f);
	style.Colors[ImGuiCol_BorderShadow] = ImVec4(0.0f, 0.0f, 0.0f, 0.0f);
	style.Colors[ImGuiCol_FrameBg] = ImVec4(0.4274509847164154f, 0.4274509847164154f, 0.4274509847164154f, 0.3899999856948853f);
	style.Colors[ImGuiCol_FrameBgHovered] = ImVec4(0.07450980693101883f, 0.7568627595901489f, 0.572549045085907f, 0.4000000059604645f);
	style.Colors[ImGuiCol_FrameBgActive] = ImVec4(0.2784313857555389f, 1.0f, 0.7490196228027344f, 0.6899999976158142f);
	style.Colors[ImGuiCol_TitleBg] = ImVec4(0.3882353007793427f, 0.5333333611488342f, 0.3411764800548553f, 0.8299999833106995f);
	style.Colors[ImGuiCol_TitleBgActive] = ImVec4(0.3019607961177826f, 0.572549045085907f, 0.2980392277240753f, 0.8700000047683716f);
	style.Colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.4392156898975372f, 0.800000011920929f, 0.4000000059604645f, 0.2000000029802322f);
	style.Colors[ImGuiCol_MenuBarBg] = ImVec4(0.2901960909366608f, 0.3803921639919281f, 0.2745098173618317f, 0.800000011920929f);
	style.Colors[ImGuiCol_ScrollbarBg] = ImVec4(0.2000000029802322f, 0.2470588237047195f, 0.2980392277240753f, 0.6000000238418579f);
	style.Colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.3921568691730499f, 0.9137254953384399f, 0.5843137502670288f, 0.300000011920929f);
	style.Colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.4000000059604645f, 0.800000011920929f, 0.5803921818733215f, 0.4000000059604645f);
	style.Colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.3882353007793427f, 0.800000011920929f, 0.4117647111415863f, 0.6000000238418579f);
	style.Colors[ImGuiCol_CheckMark] = ImVec4(0.5137255191802979f, 0.5882353186607361f, 0.3372549116611481f, 1.0f);
	style.Colors[ImGuiCol_SliderGrab] = ImVec4(0.239215686917305f, 0.8784313797950745f, 0.4941176474094391f, 1.0f);
	style.Colors[ImGuiCol_SliderGrabActive] = ImVec4(0.196078434586525f, 0.4901960790157318f, 0.2588235437870026f, 1.0f);
	style.Colors[ImGuiCol_Button] = ImVec4(0.5254902243614197f, 0.8039215803146362f, 0.6470588445663452f, 0.6200000047683716f);
	style.Colors[ImGuiCol_ButtonHovered] = ImVec4(0.3960784375667572f, 0.6352941393852234f, 0.47843137383461f, 0.7900000214576721f);
	style.Colors[ImGuiCol_ButtonActive] = ImVec4(0.1764705926179886f, 0.2862745225429535f, 0.168627455830574f, 1.0f);
	style.Colors[ImGuiCol_Header] = ImVec4(0.3882353007793427f, 0.501960813999176f, 0.3098039329051971f, 0.449999988079071f);
	style.Colors[ImGuiCol_HeaderHovered] = ImVec4(0.4705882370471954f, 0.6392157077789307f, 0.5333333611488342f, 0.800000011920929f);
	style.Colors[ImGuiCol_HeaderActive] = ImVec4(0.5686274766921997f, 0.7843137383460999f, 0.5607843399047852f, 0.800000011920929f);
	style.Colors[ImGuiCol_Separator] = ImVec4(0.4980392158031464f, 0.4980392158031464f, 0.4980392158031464f, 0.6000000238418579f);
	style.Colors[ImGuiCol_SeparatorHovered] = ImVec4(0.2078431397676468f, 0.3294117748737335f, 0.2705882489681244f, 1.0f);
	style.Colors[ImGuiCol_SeparatorActive] = ImVec4(0.294117659330368f, 0.4509803950786591f, 0.3764705955982208f, 1.0f);
	style.Colors[ImGuiCol_ResizeGrip] = ImVec4(1.0f, 1.0f, 1.0f, 0.1000000014901161f);
	style.Colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.4156862795352936f, 0.9921568632125854f, 0.5843137502670288f, 0.6000000238418579f);
	style.Colors[ImGuiCol_ResizeGripActive] = ImVec4(0.3568627536296844f, 0.5372549295425415f, 0.43529412150383f, 0.8999999761581421f);
	style.Colors[ImGuiCol_Tab] = ImVec4(0.2352941185235977f, 0.3450980484485626f, 0.2666666805744171f, 0.7860000133514404f);
	style.Colors[ImGuiCol_TabHovered] = ImVec4(0.1098039224743843f, 0.2313725501298904f, 0.1294117718935013f, 0.800000011920929f);
	style.Colors[ImGuiCol_TabActive] = ImVec4(0.5764706134796143f, 1.0f, 0.5803921818733215f, 0.3819742202758789f);
	style.Colors[ImGuiCol_TabUnfocused] = ImVec4(0.2274509817361832f, 0.4039215743541718f, 0.2235294133424759f, 0.8212000131607056f);
	style.Colors[ImGuiCol_TabUnfocusedActive] = ImVec4(0.3960784375667572f, 0.772549033164978f, 0.5921568870544434f, 0.8371999859809875f);
	style.Colors[ImGuiCol_PlotLines] = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);
	style.Colors[ImGuiCol_PlotLinesHovered] = ImVec4(0.8980392217636108f, 0.6980392336845398f, 0.0f, 1.0f);
	style.Colors[ImGuiCol_PlotHistogram] = ImVec4(0.8980392217636108f, 0.6980392336845398f, 0.0f, 1.0f);
	style.Colors[ImGuiCol_PlotHistogramHovered] = ImVec4(1.0f, 0.6000000238418579f, 0.0f, 1.0f);
	style.Colors[ImGuiCol_TableHeaderBg] = ImVec4(0.2823529541492462f, 0.3764705955982208f, 0.2666666805744171f, 1.0f);
	style.Colors[ImGuiCol_TableBorderStrong] = ImVec4(0.3098039329051971f, 0.4470588266849518f, 0.3254902064800262f, 1.0f);
	style.Colors[ImGuiCol_TableBorderLight] = ImVec4(0.2588235437870026f, 0.2588235437870026f, 0.2784313857555389f, 1.0f);
	style.Colors[ImGuiCol_TableRowBg] = ImVec4(0.0f, 0.0f, 0.0f, 0.0f);
	style.Colors[ImGuiCol_TableRowBgAlt] = ImVec4(1.0f, 1.0f, 1.0f, 0.07000000029802322f);
	style.Colors[ImGuiCol_TextSelectedBg] = ImVec4(0.2627451121807098f, 0.6352941393852234f, 0.364705890417099f, 0.3499999940395355f);
	style.Colors[ImGuiCol_DragDropTarget] = ImVec4(1.0f, 1.0f, 0.0f, 0.8999999761581421f);
	style.Colors[ImGuiCol_NavHighlight] = ImVec4(0.3686274588108063f, 0.501960813999176f, 0.2431372553110123f, 0.800000011920929f);
	style.Colors[ImGuiCol_NavWindowingHighlight] = ImVec4(1.0f, 1.0f, 1.0f, 0.699999988079071f);
	style.Colors[ImGuiCol_NavWindowingDimBg] = ImVec4(0.800000011920929f, 0.800000011920929f, 0.800000011920929f, 0.2000000029802322f);
	style.Colors[ImGuiCol_ModalWindowDimBg] = ImVec4(0.2000000029802322f, 0.2000000029802322f, 0.2000000029802322f, 0.3499999940395355f);
}

/**
* Gold style by CookiePLMonster from ImThemes
* https://github.com/Patitotective/ImThemes
*/
void EditorStyle::GoldStyle()
{
	// Gold style by CookiePLMonster from ImThemes
	ImGuiStyle& style = ImGui::GetStyle();

	style.Alpha = 1.0f;
	style.DisabledAlpha = 0.6000000238418579f;
	style.WindowPadding = ImVec2(8.0f, 8.0f);
	style.WindowRounding = 4.0f;
	style.WindowBorderSize = 1.0f;
	style.WindowMinSize = ImVec2(32.0f, 32.0f);
	style.WindowTitleAlign = ImVec2(1.0f, 0.5f);
	style.WindowMenuButtonPosition = ImGuiDir_Right;
	style.ChildRounding = 0.0f;
	style.ChildBorderSize = 1.0f;
	style.PopupRounding = 4.0f;
	style.PopupBorderSize = 1.0f;
	style.FramePadding = ImVec2(4.0f, 2.0f);
	style.FrameRounding = 4.0f;
	style.FrameBorderSize = 0.0f;
	style.ItemSpacing = ImVec2(10.0f, 2.0f);
	style.ItemInnerSpacing = ImVec2(4.0f, 4.0f);
	style.CellPadding = ImVec2(4.0f, 2.0f);
	style.IndentSpacing = 12.0f;
	style.ColumnsMinSpacing = 6.0f;
	style.ScrollbarSize = 10.0f;
	style.ScrollbarRounding = 6.0f;
	style.GrabMinSize = 10.0f;
	style.GrabRounding = 4.0f;
	style.TabRounding = 4.0f;
	style.TabBorderSize = 0.0f;
	style.TabMinWidthForCloseButton = 0.0f;
	style.ColorButtonPosition = ImGuiDir_Right;
	style.ButtonTextAlign = ImVec2(0.5f, 0.5f);
	style.SelectableTextAlign = ImVec2(0.0f, 0.0f);

	style.Colors[ImGuiCol_Text] = ImVec4(0.9176470637321472f, 0.9176470637321472f, 0.9176470637321472f, 1.0f);
	style.Colors[ImGuiCol_TextDisabled] = ImVec4(0.4392156898975372f, 0.4392156898975372f, 0.4392156898975372f, 1.0f);
	style.Colors[ImGuiCol_WindowBg] = ImVec4(0.05882352963089943f, 0.05882352963089943f, 0.05882352963089943f, 1.0f);
	style.Colors[ImGuiCol_ChildBg] = ImVec4(0.0f, 0.0f, 0.0f, 0.0f);
	style.Colors[ImGuiCol_PopupBg] = ImVec4(0.0784313753247261f, 0.0784313753247261f, 0.0784313753247261f, 0.9399999976158142f);
	style.Colors[ImGuiCol_Border] = ImVec4(0.5098039507865906f, 0.3568627536296844f, 0.1490196138620377f, 1.0f);
	style.Colors[ImGuiCol_BorderShadow] = ImVec4(0.0f, 0.0f, 0.0f, 0.0f);
	style.Colors[ImGuiCol_FrameBg] = ImVec4(0.1098039224743843f, 0.1098039224743843f, 0.1098039224743843f, 1.0f);
	style.Colors[ImGuiCol_FrameBgHovered] = ImVec4(0.5098039507865906f, 0.3568627536296844f, 0.1490196138620377f, 1.0f);
	style.Colors[ImGuiCol_FrameBgActive] = ImVec4(0.7764706015586853f, 0.5490196347236633f, 0.2078431397676468f, 1.0f);
	style.Colors[ImGuiCol_TitleBg] = ImVec4(0.5098039507865906f, 0.3568627536296844f, 0.1490196138620377f, 1.0f);
	style.Colors[ImGuiCol_TitleBgActive] = ImVec4(0.9098039269447327f, 0.6392157077789307f, 0.1294117718935013f, 1.0f);
	style.Colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.0f, 0.0f, 0.0f, 0.5099999904632568f);
	style.Colors[ImGuiCol_MenuBarBg] = ImVec4(0.1098039224743843f, 0.1098039224743843f, 0.1098039224743843f, 1.0f);
	style.Colors[ImGuiCol_ScrollbarBg] = ImVec4(0.05882352963089943f, 0.05882352963089943f, 0.05882352963089943f, 0.5299999713897705f);
	style.Colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.2078431397676468f, 0.2078431397676468f, 0.2078431397676468f, 1.0f);
	style.Colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.4666666686534882f, 0.4666666686534882f, 0.4666666686534882f, 1.0f);
	style.Colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.8078431487083435f, 0.8274509906768799f, 0.8078431487083435f, 1.0f);
	style.Colors[ImGuiCol_CheckMark] = ImVec4(0.7764706015586853f, 0.5490196347236633f, 0.2078431397676468f, 1.0f);
	style.Colors[ImGuiCol_SliderGrab] = ImVec4(0.9098039269447327f, 0.6392157077789307f, 0.1294117718935013f, 1.0f);
	style.Colors[ImGuiCol_SliderGrabActive] = ImVec4(0.9098039269447327f, 0.6392157077789307f, 0.1294117718935013f, 1.0f);
	style.Colors[ImGuiCol_Button] = ImVec4(0.5098039507865906f, 0.3568627536296844f, 0.1490196138620377f, 1.0f);
	style.Colors[ImGuiCol_ButtonHovered] = ImVec4(0.9098039269447327f, 0.6392157077789307f, 0.1294117718935013f, 1.0f);
	style.Colors[ImGuiCol_ButtonActive] = ImVec4(0.7764706015586853f, 0.5490196347236633f, 0.2078431397676468f, 1.0f);
	style.Colors[ImGuiCol_Header] = ImVec4(0.5098039507865906f, 0.3568627536296844f, 0.1490196138620377f, 1.0f);
	style.Colors[ImGuiCol_HeaderHovered] = ImVec4(0.9098039269447327f, 0.6392157077789307f, 0.1294117718935013f, 1.0f);
	style.Colors[ImGuiCol_HeaderActive] = ImVec4(0.929411768913269f, 0.6470588445663452f, 0.1372549086809158f, 1.0f);
	style.Colors[ImGuiCol_Separator] = ImVec4(0.2078431397676468f, 0.2078431397676468f, 0.2078431397676468f, 1.0f);
	style.Colors[ImGuiCol_SeparatorHovered] = ImVec4(0.9098039269447327f, 0.6392157077789307f, 0.1294117718935013f, 1.0f);
	style.Colors[ImGuiCol_SeparatorActive] = ImVec4(0.7764706015586853f, 0.5490196347236633f, 0.2078431397676468f, 1.0f);
	style.Colors[ImGuiCol_ResizeGrip] = ImVec4(0.2078431397676468f, 0.2078431397676468f, 0.2078431397676468f, 1.0f);
	style.Colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.9098039269447327f, 0.6392157077789307f, 0.1294117718935013f, 1.0f);
	style.Colors[ImGuiCol_ResizeGripActive] = ImVec4(0.7764706015586853f, 0.5490196347236633f, 0.2078431397676468f, 1.0f);
	style.Colors[ImGuiCol_Tab] = ImVec4(0.5098039507865906f, 0.3568627536296844f, 0.1490196138620377f, 1.0f);
	style.Colors[ImGuiCol_TabHovered] = ImVec4(0.9098039269447327f, 0.6392157077789307f, 0.1294117718935013f, 1.0f);
	style.Colors[ImGuiCol_TabActive] = ImVec4(0.7764706015586853f, 0.5490196347236633f, 0.2078431397676468f, 1.0f);
	style.Colors[ImGuiCol_TabUnfocused] = ImVec4(0.06666667014360428f, 0.09803921729326248f, 0.1490196138620377f, 0.9700000286102295f);
	style.Colors[ImGuiCol_TabUnfocusedActive] = ImVec4(0.1372549086809158f, 0.2588235437870026f, 0.4196078479290009f, 1.0f);
	style.Colors[ImGuiCol_PlotLines] = ImVec4(0.6078431606292725f, 0.6078431606292725f, 0.6078431606292725f, 1.0f);
	style.Colors[ImGuiCol_PlotLinesHovered] = ImVec4(1.0f, 0.4274509847164154f, 0.3490196168422699f, 1.0f);
	style.Colors[ImGuiCol_PlotHistogram] = ImVec4(0.8980392217636108f, 0.6980392336845398f, 0.0f, 1.0f);
	style.Colors[ImGuiCol_PlotHistogramHovered] = ImVec4(1.0f, 0.6000000238418579f, 0.0f, 1.0f);
	style.Colors[ImGuiCol_TableHeaderBg] = ImVec4(0.1882352977991104f, 0.1882352977991104f, 0.2000000029802322f, 1.0f);
	style.Colors[ImGuiCol_TableBorderStrong] = ImVec4(0.3098039329051971f, 0.3098039329051971f, 0.3490196168422699f, 1.0f);
	style.Colors[ImGuiCol_TableBorderLight] = ImVec4(0.2274509817361832f, 0.2274509817361832f, 0.2470588237047195f, 1.0f);
	style.Colors[ImGuiCol_TableRowBg] = ImVec4(0.0f, 0.0f, 0.0f, 0.0f);
	style.Colors[ImGuiCol_TableRowBgAlt] = ImVec4(1.0f, 1.0f, 1.0f, 0.05999999865889549f);
	style.Colors[ImGuiCol_TextSelectedBg] = ImVec4(0.2588235437870026f, 0.5882353186607361f, 0.9764705896377563f, 0.3499999940395355f);
	style.Colors[ImGuiCol_DragDropTarget] = ImVec4(1.0f, 1.0f, 0.0f, 0.8999999761581421f);
	style.Colors[ImGuiCol_NavHighlight] = ImVec4(0.2588235437870026f, 0.5882353186607361f, 0.9764705896377563f, 1.0f);
	style.Colors[ImGuiCol_NavWindowingHighlight] = ImVec4(1.0f, 1.0f, 1.0f, 0.699999988079071f);
	style.Colors[ImGuiCol_NavWindowingDimBg] = ImVec4(0.800000011920929f, 0.800000011920929f, 0.800000011920929f, 0.2000000029802322f);
	style.Colors[ImGuiCol_ModalWindowDimBg] = ImVec4(0.800000011920929f, 0.800000011920929f, 0.800000011920929f, 0.3499999940395355f);
}
