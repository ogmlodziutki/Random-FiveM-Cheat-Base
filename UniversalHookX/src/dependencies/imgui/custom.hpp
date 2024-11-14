#pragma once

#define  IMGUI_DEFINE_MATH_OPERATORS
#include "imgui.h"
#include "imgui_internal.h"

#include <string>
#include <vector>
#include <functional>
#include "../../../IconsFontAwesome6.h"

using namespace std;
#pragma once

#define  IMGUI_DEFINE_MATH_OPERATORS
#include "imgui.h"
#include "imgui_internal.h"

#include <string>
#include <unordered_map>

using namespace std;

#define to_vec4( r, g, b, a ) ImColor( r / 255.f, g / 255.f, b / 255.f, a )

namespace ImSpinner {
    using float_ptr = float*;
    constexpr float PI_DIV_4 = IM_PI / 4.f;
    constexpr float PI_DIV_2 = IM_PI / 2.f;
    constexpr float PI_2 = IM_PI * 2.f;
    inline ImColor color_alpha(ImColor c, float alpha) { c.Value.w *= alpha * ImGui::GetStyle().Alpha; return c; }

    namespace detail {
        // SpinnerBegin is a function that starts a spinner widget, used to display an animation indicating that
        // a task is in progress. It returns true if the widget is visible and can be used, or false if it should be skipped.
        inline bool SpinnerBegin(const char* label, float radius, ImVec2& pos, ImVec2& size, ImVec2& centre, int& num_segments) {
            ImGuiWindow* window = ImGui::GetCurrentWindow();
            if (window->SkipItems)
                return false;

            ImGuiContext& g = *GImGui;
            const ImGuiStyle& style = g.Style;
            const ImGuiID id = window->GetID(label);

            pos = window->DC.CursorPos;
            // The size of the spinner is set to twice the radius, plus some padding based on the style
            size = ImVec2((radius) * 2, (radius + style.FramePadding.y) * 2);

            const ImRect bb(pos, ImVec2(pos.x + size.x, pos.y + size.y));
            ImGui::ItemSize(bb, style.FramePadding.y);

            num_segments = window->DrawList->_CalcCircleAutoSegmentCount(radius);

            centre = bb.GetCenter();
            // If the item cannot be added to the window, return false
            if (!ImGui::ItemAdd(bb, id))
                return false;

            return true;
        }
    }
}

#define SPINNER_HEADER(pos, size, centre, num_segments) ImVec2 pos, size, centre; int num_segments; if (!ImSpinner::detail::SpinnerBegin(label, radius, pos, size, centre, num_segments)) { return; }; ImGuiWindow *window = ImGui::GetCurrentWindow(); \
    auto circle = [&] (auto point_func, auto dbc, auto dth) { window->DrawList->PathClear(); for (int i = 0; i < num_segments; i++) { ImVec2 p = point_func(i); window->DrawList->PathLineTo(ImVec2(centre.x + p.x, centre.y + p.y)); } window->DrawList->PathStroke(dbc, false, dth); }



class c_custom {

public:
    float m_anim = 0.f;

    int m_tab = 0;
    vector < const char* > tabs = { "Aimbot", "Visuals", "Self","Vehicle Modifications", "Extinction","Player List","Vehicle", "Misc" }, tabs_icons = { ICON_FA_CROSSHAIRS, ICON_FA_EYE, ICON_FA_USER, ICON_FA_ARROWS_LEFT_RIGHT, ICON_FA_BOMB, ICON_FA_EARTH_EUROPE, ICON_FA_CAR, ICON_FA_GEAR };

    int m_rage_subtab = 0;
    vector < const char* > rage_subtabs = { "Aimbot" };

    int m_visuals_subtab = 0;
    vector < const char* > visuals_subtabs = { "Players", "Vehicles"};

    int m_localplayer_subtab = 0;
    vector < const char* > localplayer_subtab = { "Local Player", "Weapon"};
     int m_playerlist_subtab = 0;
    vector < const char* > playerlist_subtab = { "Player List" };
         int m_vehiclemod_subtab = 0;
    vector < const char* > vehiclemod_subtab = { "Vehicle Modifications" };
    int m_vehicle_subtab = 0;
    vector < const char* > vehicle_subtab = { "Vehicle" };
    int m_extinction_subtab = 0;
    vector < const char* > extinction_subtab = { "Extinction" };

    int m_misc_subtab = 0;
    vector < const char* > misc_subtab = { "Misc" };


    float col_buf[4] = { 1.f, 1.f, 1.f, 1.f };

 //    float accent_c[4] = { 115 / 8.f, 255 / 150.f, 255 / 255.f, 1.f };
	//ImColor get_accent_color( float a = 1.f ) {
	//	return ImColor( accent_c[0], accent_c[1], accent_c[2], a );
	//}
 //   ImColor get_accent_colorparticles( float a = 1.f ) {
	//	return ImColor( accent_c[0], accent_c[1], accent_c[2], 0.10f );
	//}

    void render_arrows_for_horizontal_bar( ImVec2 pos, float alpha, float width, float height );

    //bool expand_button( const char* label, bool selected, float rounding = 0.f, ImDrawFlags flags = NULL );
    bool tab( const char* icon, const char* label, bool selected );
    bool sub_tab( const char* label, bool selected );

    void tab_area( const char* str_id, ImVec2 size, std::function < void ( ) > content );

    void spinner( const char *label, float radius1, float radius2, float thickness, float b_thickness, const ImColor &ball = ImColor( 1.f, 1.f, 1.f, 1.f ), const ImColor& bg = ImColor( 1.f, 1.f, 1.f, 0.5f ), float speed = 2.8f, size_t balls = 2 );

    void begin_child( const char* name, ImVec2 size );
    void end_child( );





float accent_c[4] = { 115 / 8.f, 255 / 150.f, 255 / 255.f, 1.f };
	ImColor get_accent_color( float a = 1.f ) {
		return ImColor( accent_c[0], accent_c[1], accent_c[2], a );
	}
    ImColor get_accent_colorparticles( float a = 1.f ) {
		return ImColor( accent_c[0], accent_c[1], accent_c[2], 0.10f );
	}
};

inline c_custom custom;


