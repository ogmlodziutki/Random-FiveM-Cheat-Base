#define IMGUI_DEFINE_MATH_OPERATORS
#include "menu.hpp"

#include "../dependencies/imgui/imgui.h"
#include "../dependencies/imgui/imgui_impl_win32.h"

#include "../utils/utils.hpp"
#include "../hooks/hooks.hpp"

#include "../fivem/features/aimbot.hpp"
#include "../fivem/features/visuals.hpp"
#include "../fivem/features/weapon.hpp"
#include "../fivem/features/self.hpp"

#include "../fivem/settings/settings.hpp"
#include "../dependencies/auth.hpp"
#include "faghack\UniversalHookX\src\dependencies\imgui\bytes.hpp"
#include "faghack\UniversalHookX\src\dependencies\imgui\custom.hpp"
#include "../fivem/features/lootbag.hpp"
#include "../fivem/features/vehicles.hpp"
#include "../../trebucbd.h"
#include "../../IconsFontAwesome6.h"
#include "faghack\UniversalHookX\src\Auth.hpp"
#include "faghack\UniversalHookX\src\base64.h"
#include "faghack\UniversalHookX\src\hwid.h"

//
static int selected_index = 1;
static int selectedVeh_index = 1;
using namespace ImGui;
char key_auth[16] = "";





struct Particle {
    float x, y;
    float speedX, speedY;
    ImVec4 color;
};

const int MaxParticles = 500;
Particle particles[MaxParticles];

void InitParticles( ) {
    for (int i = 0; i < MaxParticles; i++) {
        particles[i].x = rand( ) % (int)ImGui::GetIO( ).DisplaySize.x;
        particles[i].y = rand( ) % (int)ImGui::GetIO( ).DisplaySize.y;
        particles[i].speedX = (rand( ) % 200 - 100) / 1000.0f;
        particles[i].speedY = (rand( ) % 200 - 100) / 1000.0f;
        particles[i].color = ImVec4((rand( ) % 256) / 255.0f, (rand( ) % 256) / 255.0f, (rand( ) % 256) / 255.0f, 1.0f);
    }
}

void UpdateParticles(float deltaTime) {
    for (int i = 0; i < MaxParticles; i++) {
        particles[i].x += particles[i].speedX * deltaTime;
        particles[i].y += particles[i].speedY * deltaTime;

        if (particles[i].x < 0 || particles[i].x > ImGui::GetIO( ).DisplaySize.x)
            particles[i].speedX = -particles[i].speedX;
        if (particles[i].y < 0 || particles[i].y > ImGui::GetIO( ).DisplaySize.y)
            particles[i].speedY = -particles[i].speedY;
    }
}

void RenderParticles( ) {
    ImGui::SetNextWindowPos(ImVec2(0, 0));
    ImGui::SetNextWindowSize(ImGui::GetIO( ).DisplaySize);

    if (ImGui::Begin("Particle System", nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize)) {
        ImDrawList* drawList = ImGui::GetWindowDrawList( );

        for (int i = 0; i < MaxParticles; i++) {
            for (int j = i + 1; j < MaxParticles; j++) {
                float distance = sqrt((particles[i].x - particles[j].x) * (particles[i].x - particles[j].x) + (particles[i].y - particles[j].y) * (particles[i].y - particles[j].y));

                if (distance < 50.0f) // Ajustez cette valeur selon vos besoins
                {
                    drawList->AddLine(ImVec2(particles[i].x, particles[i].y), ImVec2(particles[j].x, particles[j].y), IM_COL32(50, 50, 50, 255));
                }
            }
        }
        for (int i = 0; i < MaxParticles; i++) {
            drawList->AddCircleFilled(ImVec2(particles[i].x, particles[i].y), 3.0f, IM_COL32(100, 100, 100, 255));
        }
    }

    ImGui::End( );
}




#define ALPHA (ImGuiColorEditFlags_AlphaPreview | ImGuiColorEditFlags_NoTooltip | ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoLabel | ImGuiColorEditFlags_AlphaBar | ImGuiColorEditFlags_InputRGB | ImGuiColorEditFlags_Float | ImGuiColorEditFlags_NoDragDrop | ImGuiColorEditFlags_PickerHueBar | ImGuiColorEditFlags_NoBorder)
#define NO_ALPHA (ImGuiColorEditFlags_NoTooltip | ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoLabel | ImGuiColorEditFlags_NoAlpha | ImGuiColorEditFlags_InputRGB | ImGuiColorEditFlags_Float | ImGuiColorEditFlags_NoDragDrop | ImGuiColorEditFlags_PickerHueBar | ImGuiColorEditFlags_NoBorder)

bool draw_demo = false;

int selected_cfg = 0;
const char* cfg[]{ "Legit", "Rage", "PvP", "RP", "Other" };
int current_tabs = 1;

char font[200] = "";

bool register_mode = false;






std::string name = E("");                                                             // application name. right above the blurred text aka the secret on the licenses tab among other tabs
std::string ownerid = E("");                                                            // ownerid, found in account settings. click your profile picture on top right of dashboard and then account settings.
std::string secret = E(""); // app secret, the blurred text on licenses tab and other tabs
std::string version = E("");                                                                   // leave alone unless you've changed version on website
std::string url = E("https://keyauth.win/api/1.3/");                                        // change if you're self-hosting
std::string sslPin = E("ssl pin key (optional)");                                      // don't change unless you intend to pin public certificate key. you can get here in the "Pin SHA256" field https://www.ssllabs.com/ssltest/analyze.html?d=keyauth.win&latest. If you do this you need to be aware of when SSL key expires so you can update it

using namespace KeyAuth;
api KeyAuthApp(name, ownerid, secret, version, url, sslPin);


//loader


char username[255] = "";
char key[50] = "";
static char password[25] = "";

ImFont* fonti;
//save caca
FILE* p_stream;

bool loaded = true;

// security
int auth = 2;
int checking = 0;
bool draw_menu = true;

void LoginNow(void) {
    KeyAuthApp.login(username, password);
    if (KeyAuthApp.data.success) {

        fopen_s(&p_stream, E("Login"), "w+");

        fseek(p_stream, 0, SEEK_SET);

        fwrite(username, sizeof(username), 1, p_stream);
        fwrite(password, sizeof(password), 1, p_stream);

        fclose(p_stream);

        auth = 2;
        draw_menu = true;
        KeyAuthApp.log("User Connected Successfully");

    }


}



void RegisterNow(void) {

    KeyAuthApp.regstr(username, password, key);

    if (KeyAuthApp.data.success) {



        ImGui::End( );
        fopen_s(&p_stream, E("Login"), "w+");

        fseek(p_stream, 0, SEEK_SET);

        fwrite(username, sizeof(username), 1, p_stream); ImColor(0, 0, 0, 255);
        fwrite(password, sizeof(password), 1, p_stream); ImColor(0, 0, 0, 255);

        fclose(p_stream);

        auth = 2;
        draw_menu = true;

    }


}

ImFont* AddCustomDefaultFont(const ImFontConfig* font_cfg_template) {
    ImGuiIO& io = ImGui::GetIO( );
    (void)io;

    ImFontConfig font_cfg = font_cfg_template ? *font_cfg_template : ImFontConfig( );
    if (!font_cfg_template) {
        font_cfg.OversampleH = font_cfg.OversampleV = 1;
        font_cfg.PixelSnapH = true;
    }
    if (font_cfg.SizePixels <= 0.0f)
        font_cfg.SizePixels = 13.0f * 1.0f;
    if (font_cfg.Name[0] == '\0')
        ImFormatString(font_cfg.Name, IM_ARRAYSIZE(font_cfg.Name), "ProggyClean.ttf, %dpx", (int)font_cfg.SizePixels);
    font_cfg.EllipsisChar = (ImWchar)0x0085;
    font_cfg.GlyphOffset.y = 1.0f * IM_FLOOR(font_cfg.SizePixels / 13.0f); // Add +1 offset per 13 units

    // const char* ttf_compressed_base85 = GetDefaultCompressedFontDataTTFBase85();
    const ImWchar* glyph_ranges = font_cfg.GlyphRanges != NULL ? font_cfg.GlyphRanges : io.Fonts->GetGlyphRangesDefault( );
    // ImFont* font = io.Fonts->AddFontFromMemoryCompressedTTF(trebucbd_compressed_data, trebucbd_compressed_size, font_cfg.SizePixels, &font_cfg, glyph_ranges);
  
    ImFont* font = io.Fonts->AddFontFromMemoryCompressedTTF(trebucbd_compressed_data, sizeof(trebucbd_compressed_size), 14.f, nullptr, glyph_ranges);
    return font;
}
#include "faghack\UniversalHookX\imgui_notify.h"
namespace Menu {



    void InitializeContext(HWND hwnd) {
        if (ImGui::GetCurrentContext( ))
            return;

        ImGui::CreateContext( );
        ImGui_ImplWin32_Init(hwnd);
        ImGuiStyle* Style = &ImGui::GetStyle( );
        ImVec4* Colors = Style->Colors;
        Style->WindowPadding = {10.f, 10.f};
        Style->PopupRounding = 0.f;
        Style->FramePadding = {8.f, 4.f};
        Style->ItemSpacing = {10.f, 8.f};
        Style->ItemInnerSpacing = {6.f, 6.f};
        Style->TouchExtraPadding = {0.f, 0.f};
        Style->IndentSpacing = 21.f;
        Style->GrabMinSize = 8.f;
        Style->WindowBorderSize = 1.f;
        Style->ChildBorderSize = 4.f;
        Style->PopupBorderSize = 1.f;
        Style->FrameBorderSize = 0.f;
        Style->TabBorderSize = 0.f;
        Style->WindowRounding = 6.f;
        Style->ChildRounding = 6.f;
        Style->FrameRounding = 0.f;
        Style->GrabRounding = 0.f;
        Style->TabRounding = 0.f;
        Style->WindowTitleAlign = {0.5f, 0.5f};
        Style->ButtonTextAlign = {0.5f, 0.5f};
        Style->DisplaySafeAreaPadding = {3.f, 3.f};


        //ImVec4* Colors = Style->Colors;
        //ImGuiStyle* Style = &ImGui::GetStyle();

        //Colors[ImGuiCol_Border] = ImVec4(ImColor(255, 255, 255, 0));
        //Colors[ImGuiCol_WindowBg] = ImVec4(ImColor(10, 10, 10, 255));
        //Colors[ImGuiCol_ChildBg] = ImVec4(ImColor(20, 20, 20, 175));
        //Colors[ImGuiCol_Text] = ImVec4(ImColor(255, 255, 255, 255));
        //Colors[ImGuiCol_Separator] = ImVec4(ImColor(45, 45, 45, 135));
        //Colors[ImGuiCol_CheckMark] = ImVec4(ImColor(2, 106, 191, 175));
        //Colors[ImGuiCol_PopupBg] = ImVec4(ImColor(20, 20, 20, 175));
        //Colors[ImGuiCol_Button] = ImVec4(ImColor(33, 33, 33, 255));
        //Colors[ImGuiCol_ButtonActive] = ImVec4(ImColor(48, 48, 48, 255));
        //Colors[ImGuiCol_ButtonHovered] = ImVec4(ImColor(43, 43, 43, 255));
        ////Colors[ImGuiCol_FrameBg] = ImVec4(0.16f, 0.29f, 0.48f, 0.54f);
        ////Colors[ImGuiCol_FrameBgHovered] = ImVec4(0.26f, 0.59f, 0.98f, 0.40f);
        ////Colors[ImGuiCol_FrameBgActive] = ImVec4(0.26f, 0.59f, 0.98f, 0.67f);

        //Colors[ImGuiCol_FrameBg] = ImVec4(ImColor(20, 20, 20, 255));
        //Colors[ImGuiCol_FrameBgHovered] = ImVec4(ImColor(33, 32, 32, 255));
        //Colors[ImGuiCol_FrameBgActive] = ImVec4(ImColor(28, 28, 28, 255));


        //Colors[ImGuiCol_Header] = ImVec4(0.26f, 0.59f, 0.98f, 0.31f);
        //Colors[ImGuiCol_HeaderHovered] = ImVec4(0.26f, 0.59f, 0.98f, 0.80f);
        //Colors[ImGuiCol_HeaderActive] = ImVec4(0.26f, 0.59f, 0.98f, 1.00f);

        //Colors[ImGuiCol_Separator] = Colors[ImGuiCol_Border];
        //Colors[ImGuiCol_SeparatorHovered] = ImVec4(0.10f, 0.40f, 0.75f, 0.78f);
        //Colors[ImGuiCol_SeparatorActive] = ImVec4(0.10f, 0.40f, 0.75f, 0.00f);
        //Colors[ImGuiCol_ResizeGrip] = ImVec4(0.26f, 0.59f, 0.98f, 0);
        //Colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.26f, 0.59f, 0.98f, 0);
        //Colors[ImGuiCol_ResizeGripActive] = ImVec4(0.26f, 0.59f, 0.98f, 0);
        //Colors[ImGuiCol_Tab] = ImLerp(Colors[ImGuiCol_Header], Colors[ImGuiCol_TitleBgActive], 0.80f);
        //Colors[ImGuiCol_TabHovered] = Colors[ImGuiCol_HeaderHovered];
        //Colors[ImGuiCol_TabActive] = ImLerp(Colors[ImGuiCol_HeaderActive], Colors[ImGuiCol_TitleBgActive], 0.60f);
        //Colors[ImGuiCol_TabUnfocused] = ImLerp(Colors[ImGuiCol_Tab], Colors[ImGuiCol_TitleBg], 0.80f);
        //Colors[ImGuiCol_TabUnfocusedActive] = ImLerp(Colors[ImGuiCol_TabActive], Colors[ImGuiCol_TitleBg], 0.40f);
        //Colors[ImGuiCol_PlotLines] = ImVec4(0.61f, 0.61f, 0.61f, 1.00f);
        //Colors[ImGuiCol_PlotLinesHovered] = ImVec4(1.00f, 0.43f, 0.35f, 1.00f);
        //Colors[ImGuiCol_PlotHistogram] = ImVec4(0.90f, 0.70f, 0.00f, 1.00f);
        //Colors[ImGuiCol_PlotHistogramHovered] = ImVec4(1.00f, 0.60f, 0.00f, 1.00f);
        //Colors[ImGuiCol_TableHeaderBg] = ImVec4(0.19f, 0.19f, 0.20f, 1.00f);
        //Colors[ImGuiCol_TableBorderStrong] = ImVec4(0.31f, 0.31f, 0.35f, 1.00f);   // Prefer using Alpha=1.0 here
        //Colors[ImGuiCol_TableBorderLight] = ImVec4(0.23f, 0.23f, 0.25f, 1.00f);   // Prefer using Alpha=1.0 here
        //Colors[ImGuiCol_TableRowBg] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
        //Colors[ImGuiCol_TableRowBgAlt] = ImVec4(1.00f, 1.00f, 1.00f, 0.06f);
        //Colors[ImGuiCol_TextSelectedBg] = ImVec4(0.26f, 0.59f, 0.98f, 0.35f);
        //Colors[ImGuiCol_DragDropTarget] = ImVec4(1.00f, 1.00f, 0.00f, 0.90f);
        //Colors[ImGuiCol_NavHighlight] = ImVec4(0.26f, 0.59f, 0.98f, 1.00f);
        //Colors[ImGuiCol_NavWindowingHighlight] = ImVec4(1.00f, 1.00f, 1.00f, 0.70f);
        //Colors[ImGuiCol_NavWindowingDimBg] = ImVec4(0.80f, 0.80f, 0.80f, 0.20f);
        //Colors[ImGuiCol_ModalWindowDimBg] = ImVec4(0.80f, 0.80f, 0.80f, 0.35f);

        if (settings::visuals::darkmode) {
            ImGui::StyleColorsDark( );
        } else {
            ImGui::StyleColorsLight( );
        }
            

        ImGuiIO& io = ImGui::GetIO( );
        io.IniFilename = io.LogFilename = nullptr;
        ImFontConfig font_config;
        font_config.PixelSnapH = false;
        font_config.FontDataOwnedByAtlas = false;
        font_config.OversampleH = 5;
        font_config.OversampleV = 5;
        font_config.RasterizerMultiply = 1.2f;

        static const ImWchar ranges[] = {

            0x0020,
            0x00FF, // Basic Latin + Latin Supplement
            0x0400,
            0x052F, // Cyrillic + Cyrillic Supplement
            0x2DE0,
            0x2DFF, // Cyrillic Extended-A
            0xA640,
            0xA69F, // Cyrillic Extended-B
            0xE000,
            0xE226, // icons
            0,
        };

        font_config.GlyphRanges = ranges;
        AddCustomDefaultFont(nullptr);

        // merge in icons from Font Awesome
        io.Fonts->AddFontFromMemoryTTF(icons_binary, sizeof icons_binary, 19, &font_config, ranges);
        static const ImWchar icons_ranges[] = {ICON_MIN_FA, ICON_MAX_16_FA, 0};

        ImFontConfig icons_config;
        icons_config.MergeMode = true;
        icons_config.PixelSnapH = true;
        ImFont* FontAwesome = io.Fonts->AddFontFromMemoryCompressedTTF(fa6_solid_compressed_data, fa6_solid_compressed_size, 14.f, &icons_config, icons_ranges);
        ImFont* FontAwesomeBig = io.Fonts->AddFontFromMemoryCompressedTTF(fa6_solid_compressed_data, fa6_solid_compressed_size, 19.f, nullptr, icons_ranges);

        ImFont* TitleFont = io.Fonts->AddFontFromMemoryCompressedTTF(trebucbd_compressed_data, trebucbd_compressed_size, 30, nullptr, io.Fonts->GetGlyphRangesDefault( ));


    }

    bool ShowMessageBox(const char* title, const char* message, const char* button1, const char* button2)
    {
        bool result = false;
        ImGui::OpenPopup(title);

        if (ImGui::BeginPopupModal(title, NULL, ImGuiWindowFlags_AlwaysAutoResize))
        {
            ImGui::Text(message);

            if (ImGui::Button(button1))
            {
                result = true;
                ImGui::CloseCurrentPopup();
            }

            if (button2 != NULL && ImGui::Button(button2))
            {
                result = false;
                ImGui::CloseCurrentPopup();
            }

            ImGui::EndPopup();
        }

        return result;
    }

    const char* stristr(const char* str1, const char* str2) {
        const char* ptr = str1;
        const char* substr;
        while (*ptr != '\0') {
            substr = str2;
            while (*ptr != '\0' && *substr != '\0' && tolower(*ptr) == tolower(*substr)) {
                ++ptr;
                ++substr;
            }
            if (*substr == '\0') return str1;
            if (*ptr == '\0') return nullptr;
            ++str1;
            ptr = str1;
        }
        return nullptr;
    }

    int N = 160;
    int lineMaxDist = 1800;
    ImColor lineCol = { 255, 255, 255, 8 };
    float lineThickness = 1.0f;

    void setupPoints(std::vector<std::pair<ImVec2, ImVec2>>& n) {
        ImVec2 screenSize(ImGui::GetIO().DisplaySize);
        for (auto& p : n)
            p.second = p.first = ImVec2(rand() % (int)screenSize.x, rand() % (int)screenSize.y);
    }
    float length(ImVec2 x) { return x.x * x.x + x.y * x.y; }
    void FX(ImDrawList* d, ImVec2 b, float speedMultiplier)
    {

        const int N = 100; // Annahme: Setze die gew체nschte Gr철횩e f체r den Vektor points

        static std::vector<std::pair<ImVec2, ImVec2>> size(N);
        static auto once = (setupPoints(size), true);
        float Dist;

        for (auto& p : size) {
            Dist = sqrt(length(p.first - p.second));
            if (Dist > 0) {
                p.first += (p.second - p.first) / Dist * speedMultiplier; // Geschwindigkeit anpassen
            }
            if (Dist < 4 || rand() % 100 == 0) { // 횆ndere die Bedingung f체r Zufallspositionierung
                p.second = ImVec2(rand() % (int)b.x, rand() % (int)b.y);
            }
        }

        for (int i = 0; i < N; i++) {
            for (int j = i + 1; j < N; j++) {
                Dist = length(size[i].first - size[j].first);
                if (Dist < lineMaxDist) {
                    d->AddLine(GetWindowPos() + size[i].first, GetWindowPos() + size[j].first, custom.get_accent_colorparticles(), lineThickness);
                }
            }
        }
    }

    void Render( ) {

        if (loaded == false) {

            fopen_s(&p_stream, E("Login"), "r+");

            fseek(p_stream, 0, SEEK_SET);

            fread_s(username, sizeof(username), sizeof(username), 1, p_stream); ImColor(0, 0, 0, 255);
            fread_s(password, sizeof(password), sizeof(password), 1, p_stream); ImColor(0, 0, 0, 255);

            KeyAuthApp.init();



            if (!KeyAuthApp.data.success) {
                auth = 3;
                loaded = true;
            }
            auth = 1;
           // auth = 2;
        
           // draw_menu = true;
            loaded = true;
           

        }

        if (loaded) {

            if (auth == 2) {
                	
             //bool notify = false;
             //   if (!notify) {
             //       ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 5.f);                                                    // Round borders
             //       ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(43.f / 255.f, 43.f / 255.f, 43.f / 255.f, 100.f / 255.f)); // Background color
             //       ImGui::RenderNotifications( );
             //       ImGui::InsertNotification({ImGuiToastType_Info, 1500, "Succesfly injected"});
             //       ImGui::PopStyleVar(1); // Don't forget to Pop()
             //       ImGui::PopStyleColor(1);
             //       notify = true;
             //   }
                if (draw_menu) {
                   /* setup_circles( );
                    draw_circles_and_lines(ImColor(255,255,255));*/
                    
   
          
                    aimbot::hook( );
                    visuals::player_esp( );
                    hook::vehicle( );
                    //radar::settings( );
                    visuals::lootbag( );
                    weapon::hook( );
                    self::options( );
                    self::noclip( );
                  /*  self::autofarm( );*/

                    if (settings::visuals::draw_fov) {

                        if (settings::visuals::filled_fov) {
                            ImGui::GetBackgroundDrawList( )->AddCircleFilled(ImVec2(FiveM::WindowSize.x / 2, FiveM::WindowSize.y / 2), settings::aimbot::aimbot_fov * 10, ImColor(0, 0, 0, 90), 100.0f);
                            ImGui::GetBackgroundDrawList( )->AddCircle(ImVec2(FiveM::WindowSize.x / 2, FiveM::WindowSize.y / 2), settings::aimbot::aimbot_fov * 10, settings::visuals::fov_color, 100.0f);
                        } else {

                            ImGui::GetBackgroundDrawList( )->AddCircle(ImVec2(FiveM::WindowSize.x / 2, FiveM::WindowSize.y / 2), settings::aimbot::aimbot_fov * 10, settings::visuals::fov_color, 100.0f);
                        }
                    }

                    if (settings::visuals::crosshair) {
                        if (settings::visuals::crossahir_style == 0) {

                            auto draw = ImGui::GetBackgroundDrawList( );
                            draw->AddCircle(ImVec2(FiveM::WindowSize.x / 2, FiveM::WindowSize.y / 2), 2, settings::visuals::crosshair_color, 0, 3.0f);
                            draw->AddCircle(ImVec2(FiveM::WindowSize.x / 2, FiveM::WindowSize.y / 2), 2, settings::visuals::crosshair_color, 0, 3.0f);
                        }

                        if (settings::visuals::crossahir_style == 1) {

                            auto draw = ImGui::GetBackgroundDrawList( );
                            draw->AddCircle(ImVec2(FiveM::WindowSize.x / 2, FiveM::WindowSize.y / 2), 3, settings::visuals::crosshair_color, 100, 0.0f);
                        }

                        if (settings::visuals::crossahir_style == 2) {

                            auto draw = ImGui::GetBackgroundDrawList( );

                            auto drx = FiveM::WindowSize.x / 2;
                            auto dry = FiveM::WindowSize.y / 2;

                            draw->AddLine({drx - 10, dry}, {drx + 10, dry}, settings::visuals::crosshair_color);
                            draw->AddLine({drx, dry - 10}, {drx, dry + 10}, settings::visuals::crosshair_color);
                        }
                    }

                    if (settings::visuals::watermark) {
                        auto draw_list = ImGui::GetBackgroundDrawList();

                        auto watermark = E("faghack");
                        auto watermark_size = ImGui::CalcTextSize(watermark);

                        auto yOffset = 0;
                        auto offset = 4;

                        draw_list->AddRectFilled(ImVec2(6, 4 + yOffset), ImVec2((4 * 2) + watermark_size.x + 6, 6 + yOffset), ImColor(0, 26, 255, 255));
                        draw_list->AddRectFilled(ImVec2(6, 6 + yOffset), ImVec2((4 * 2) + watermark_size.x + 6, 25 + yOffset), ImColor(0.2117647081613541f, 0.2235294133424759f, 0.2470588237047195f, 1.0f));
                        draw_list->AddText(ImVec2(10, 6 + yOffset), ImColor(255, 255, 255, 255), watermark);
                    }
                    if (SAFE_CALL(GetAsyncKeyState)(settings::self::hotkey_noclip) & 1) {

                        settings::self::noclip = !settings::self::noclip;
                    }

                   
                }
            }
            } else {

                exit(3);
            }
        

      

       
        ImGui::GetIO( ).MouseDrawCursor = bShowMenu;

        if (!bShowMenu)
                return;

            if (loaded) {

                if (auth == 1) {

                    ImGuiIO& io = ImGui::GetIO( );

                    auto windows_x = io.DisplaySize.x / 2;
                    auto windows_y = io.DisplaySize.y / 2;

                    auto myWindowX = windows_x - 250 / 2;
                    auto myWindowY = windows_y - 250 / 2;

                    ImGui::SetNextWindowPos(ImVec2(myWindowX, myWindowY));
                    ImGui::SetNextWindowSize(ImVec2(185 , 220));

                    if (ImGui::Begin("##Login", nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize)) {
                        
                        ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(128, 0, 255, 255));
                        ImGui::Text("faghack");
                        ImGui::PopStyleColor();

                        ImGui::Text(E("Username"));
                        ImGui::InputText(E("##name"), username, IM_ARRAYSIZE(username));

                        ImGui::Text(E("Password"));
                        ImGui::InputText(E("##Password"), password, IM_ARRAYSIZE(password), ImGuiInputTextFlags_Password);

                        if (register_mode) {

                            ImGui::Text(E("License"));
                            ImGui::InputText(E("##License"), key, IM_ARRAYSIZE(key));
                        }

                        ImGui::Spacing( );

                        Checkbox("Register ##aaa", &register_mode);

                        ImGui::Spacing( );

                        if (!register_mode) {

                            if (ImGui::Button(E("Login"), ImVec2(165, 27))) {

                                LoginNow( );
                            }

                        } else {

                            if (ImGui::Button(E("Register"), ImVec2(165, 27))) {

                                RegisterNow( );
                            }
                        }
                    }
                    ImGui::End( );
                }
                if (loaded) {

                    if (auth == 3) {

                        ImGuiIO& io = ImGui::GetIO( );

                        auto windows_x = io.DisplaySize.x / 2;
                        auto windows_y = io.DisplaySize.y / 2;

                        auto myWindowX = windows_x - 250 / 2;
                        auto myWindowY = windows_y - 250 / 2;

                        ImGui::SetNextWindowPos(ImVec2(myWindowX, myWindowY));
                        ImGui::SetNextWindowSize(ImVec2(250, 250));

                        if (ImGui::Begin("##Offline", nullptr, ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize)) {

                            ImGui::Text(" Sync is currently offline.");

                            if (ImGui::Button(E("Exit"), ImVec2(165, 27))) {

                                exit(0);
                            }
                        }
                        ImGui::End( );
                    }

                    if (auth == 4) {

                        ImGuiIO& io = ImGui::GetIO( );

                        auto windows_x = io.DisplaySize.x / 2;
                        auto windows_y = io.DisplaySize.y / 2;

                        auto myWindowX = windows_x - 250 / 2;
                        auto myWindowY = windows_y - 250 / 2;

                        ImGui::SetNextWindowPos(ImVec2(myWindowX, myWindowY));
                        ImGui::SetNextWindowSize(ImVec2(250, 250));

                        if (ImGui::Begin("##BlackLISTED", nullptr, ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize)) {

                            ImGui::Text("You are blacklisted from Sync");
                            if (ImGui::Button(E("Exit"), ImVec2(165, 27))) {

                                abort( );
                                exit(0);
                            }
                            
                        }
                        ImGui::End( );
                    }
                }

                if (auth == 2) {
             
                    if (draw_menu) {
                      /*  setup_circles( );
                        draw_circles_and_lines(ImColor(255, 255, 255));*/
               
                        
                 
                        ImGui::SetNextWindowSize(ImVec2(800, 420));

                        static float color[4] = {1.f, 1.f, 1.f, 1.f};

                        PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
                        ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
                        PushStyleVar(ImGuiStyleVar_WindowMinSize, ImVec2(510, 380));

                        ImGui::Begin("#24151", NULL, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoCollapse | ImGuiCond_Once | ImGuiWindowFlags_NoResize);
                        {
                            auto window = GetCurrentWindow( );
                            auto draw = window->DrawList;
                            auto pos = window->Pos;
                            auto size = window->Size;
                            auto style = GetStyle( );
                          

                            custom.m_anim = ImLerp(custom.m_anim, 1.f, 0.03f);
                            ImGuiStyle* Style = &ImGui::GetStyle( );
                            ImVec4* Colors = Style->Colors;
                           // draw->AddLine(pos + ImVec2(65, 40), pos + ImVec2(size.x - 15, 40), ImColor(1.f, 1.f, 1.f, 0.05f));
                            if (settings::visuals::darkmode) {
                                Colors[ImGuiCol_Text] = ImVec4((ImColor(235, 235, 235, 253)));
                                draw->AddRectFilled(pos + ImVec2(0, size.y - 20), pos + size, ImColor(15, 15, 15, 253), style.WindowRounding, ImDrawFlags_RoundCornersBottom);
                                draw->AddRectFilled(pos + ImVec2(65, 3), pos + ImVec2(size.x - 15, 43), ImColor(15, 15, 15, 253), style.WindowRounding, ImDrawFlags_RoundCornersBottom);
                                draw->AddText(pos + ImVec2(5, size.y - 18), GetColorU32(ImGuiCol_Text), "faghack");

                            } else {
                                Colors[ImGuiCol_Text] = ImVec4((ImColor(23, 23, 23, 253)));
                                draw->AddRectFilled(pos + ImVec2(0, size.y - 20), pos + size, ImColor(235, 235, 235, 253), style.WindowRounding, ImDrawFlags_RoundCornersBottom);
                                draw->AddRectFilled(pos + ImVec2(65, 3), pos + ImVec2(size.x - 15, 43), ImColor(235, 235, 235, 253), style.WindowRounding, ImDrawFlags_RoundCornersBottom);
                                draw->AddText(pos + ImVec2(5, size.y - 18), GetColorU32(ImGuiCol_Text), "faghack");
                            }
                            ImGui::Text("%s", KeyAuthApp.data.username);

                         /*   draw->AddText(io.Fonts->Fonts[2], io.Fonts->Fonts[2]->FontSize, pos custom.get_accent_color(), "X48");*/

                             /*draw->AddText(io.Fonts->Fonts[2], io.Fonts->Fonts[2]->FontSize, pos + ImVec2(150 / 2 - io.Fonts->Fonts[2]->CalcTextSizeA(io.Fonts->Fonts[2]->FontSize, FLT_MAX, 0, "X48").x / 2, 55 / 2 - io.Fonts->Fonts[2]->CalcTextSizeA(io.Fonts->Fonts[2]->FontSize, FLT_MAX, 0, "X48").y / 2), custom.get_accent_color(), "X48");*/
                            


                            switch (custom.m_tab) {

                                case 0:

                                     SetCursorPos(ImVec2(75, 8));
                                    BeginGroup( );

                                    for (int i = 0; i < custom.rage_subtabs.size( ); ++i) {

                                        if (custom.sub_tab(custom.rage_subtabs.at(i), custom.m_rage_subtab == i) && custom.m_rage_subtab != i)
                                            custom.m_rage_subtab = i, custom.m_anim = 0.f;

                                        if (i != custom.rage_subtabs.size( ) - 1)
                                            SameLine( );
                                    }

                                    EndGroup( );

                                    SetCursorPos(ImVec2(65, 50));
                                    BeginChild("##rage_childs", ImVec2(GetWindowWidth( ) - 80, GetWindowHeight( ) - 80));
                                    {

                                        switch (custom.m_rage_subtab) {

                                            case 0:
                                                if (settings::visuals::language) {
                                                    custom.begin_child("Aimbot", ImVec2(GetWindowWidth( ) / 2 - GetStyle( ).ItemSpacing.x / 2, GetWindowHeight( )));
                                                    {

                                                        Checkbox(E("Ativado"), &settings::aimbot::enabled);

                                                        ImGui::Hotkey(E("##"), &settings::aimbot::hotkey, ImVec2(160, 20));

                                                        Checkbox(E("Target Peds"), &settings::aimbot::ignore_peds);
                                                        Checkbox(E("Target Dead"), &settings::visuals::aim_dead);
                                                        Checkbox(E("Show Field Of View"), &settings::visuals::draw_fov);
                                                        if (settings::visuals::draw_fov) {

                                                            ImGui::SameLine();

                                                            ImGui::ColorEdit4((E("##colorfov")), settings::tempcolor::fov_color, ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_AlphaBar | ImGuiColorEditFlags_NoBorder);
                                                            settings::visuals::fov_color.Value.x = settings::tempcolor::fov_color[0];
                                                            settings::visuals::fov_color.Value.y = settings::tempcolor::fov_color[1];
                                                            settings::visuals::fov_color.Value.z = settings::tempcolor::fov_color[2];
                                                            settings::visuals::fov_color.Value.w = settings::tempcolor::fov_color[3];

                                                            Checkbox(E("Draw Filled FOV"), &settings::visuals::filled_fov);
                                                        }
                                                        // ImGui::Text(E("Smooth Horizontal : "));
                                                        //   ImGui::SliderInt(E("Smooth Horizontal"), &settings::aimbot::smoothhorizontal, 0, 30);
                                                        // ImGui::Text(E("Smooth Vertical : "));

                                                        /*  Checkbox(E("Preview Target"), &settings::visuals::preview_target);
                                                          if (settings::visuals::preview_target) {

                                                              ImGui::SameLine( );

                                                              ImGui::ColorEdit4((E("##colortarget")), settings::tempcolor::preview_target, ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_AlphaBar | ImGuiColorEditFlags_NoBorder);
                                                              settings::visuals::preview_target_colors.Value.x = settings::tempcolor::preview_target[0];
                                                              settings::visuals::preview_target_colors.Value.y = settings::tempcolor::preview_target[1];
                                                              settings::visuals::preview_target_colors.Value.z = settings::tempcolor::preview_target[2];
                                                              settings::visuals::preview_target_colors.Value.w = settings::tempcolor::preview_target[3];

                                                              ImGui::Combo("Preview Style :", &settings::visuals::preview_style, settings::visuals::ssxs, IM_ARRAYSIZE(settings::visuals::ssxs));
                                                          }*/

                                                        // Checkbox("Autoshoot", &settings::aimbot::auto_shoot);
                                                    }
                                                    custom.end_child( );
                                                    } else {
                                                    custom.begin_child("Aimbot", ImVec2(GetWindowWidth( ) / 2 - GetStyle( ).ItemSpacing.x / 2, GetWindowHeight( )));
                                                    {

                                                        Checkbox(E("Enabled"), &settings::aimbot::enabled);

                                                        ImGui::Hotkey(E("##"), &settings::aimbot::hotkey, ImVec2(160, 20));

                                                        Checkbox(E("Target Peds"), &settings::aimbot::ignore_peds);
                                                        Checkbox(E("Target Dead"), &settings::visuals::aim_dead);

                                                        Checkbox(E("Show Field Of View"), &settings::visuals::draw_fov);
                                                        if (settings::visuals::draw_fov) {

                                                            ImGui::SameLine();

                                                            ImGui::ColorEdit4((E("##colorfov")), settings::tempcolor::fov_color, ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_AlphaBar | ImGuiColorEditFlags_NoBorder);
                                                            settings::visuals::fov_color.Value.x = settings::tempcolor::fov_color[0];
                                                            settings::visuals::fov_color.Value.y = settings::tempcolor::fov_color[1];
                                                            settings::visuals::fov_color.Value.z = settings::tempcolor::fov_color[2];
                                                            settings::visuals::fov_color.Value.w = settings::tempcolor::fov_color[3];

                                                            Checkbox(E("Draw Filled FOV"), &settings::visuals::filled_fov);
                                                        }

                                                        // ImGui::Text(E("Smooth Horizontal : "));
                                                        //   ImGui::SliderInt(E("Smooth Horizontal"), &settings::aimbot::smoothhorizontal, 0, 30);
                                                        // ImGui::Text(E("Smooth Vertical : "));

                                                        /*  Checkbox(E("Preview Target"), &settings::visuals::preview_target);
                                                          if (settings::visuals::preview_target) {

                                                              ImGui::SameLine( );

                                                              ImGui::ColorEdit4((E("##colortarget")), settings::tempcolor::preview_target, ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_AlphaBar | ImGuiColorEditFlags_NoBorder);
                                                              settings::visuals::preview_target_colors.Value.x = settings::tempcolor::preview_target[0];
                                                              settings::visuals::preview_target_colors.Value.y = settings::tempcolor::preview_target[1];
                                                              settings::visuals::preview_target_colors.Value.z = settings::tempcolor::preview_target[2];
                                                              settings::visuals::preview_target_colors.Value.w = settings::tempcolor::preview_target[3];

                                                              ImGui::Combo("Preview Style :", &settings::visuals::preview_style, settings::visuals::ssxs, IM_ARRAYSIZE(settings::visuals::ssxs));
                                                          }*/

                                                        // Checkbox("Autoshoot", &settings::aimbot::auto_shoot);
                                                    }
                                                    custom.end_child( );
                                                }
        

                                                    SameLine( );
                                                    if (settings::visuals::language) {
                                                        custom.begin_child("Settings", ImVec2(GetWindowWidth( ) / 2 - GetStyle( ).ItemSpacing.x / 2, GetWindowHeight( )));
                                                        {

                                                            ImGui::SliderInt(E("Smoothing"), &settings::aimbot::aimbot_smooth, 0, 100);
                                                            ImGui::SliderInt("Field Of View", &settings::aimbot::aimbot_fov, 0, 180);
                                                            ImGui::SliderInt(E("Distance"), &settings::aimbot::aimbot_distance, 0, 1000);
                                                            Combo(E("Aim Bone"), &settings::aimbot::aimbot_bone, settings::aimbot::aimbot_bones_list, IM_ARRAYSIZE(settings::aimbot::aimbot_bones_list));
                                                        }
                                                        custom.end_child( );
                                                    }
                                                    else
                                                    {
                                                        custom.begin_child("Aim Settings", ImVec2(GetWindowWidth( ) / 2 - GetStyle( ).ItemSpacing.x / 2, GetWindowHeight( )));
                                                        {

                                                            ImGui::SliderInt(E("Smoothing"), &settings::aimbot::aimbot_smooth, 0, 100);
                                                            ImGui::SliderInt("Field Of View", &settings::aimbot::aimbot_fov, 0, 180);
                                                            ImGui::SliderInt(E("Distance"), &settings::aimbot::aimbot_distance, 0, 1000);
                                                            Combo(E("Aim Bone"), &settings::aimbot::aimbot_bone, settings::aimbot::aimbot_bones_list, IM_ARRAYSIZE(settings::aimbot::aimbot_bones_list));
                                                        }
                                                        custom.end_child( );
                                                    }
                                        }
                                    }
                                    EndChild( );

                                    break;

                                case 1:

                                    SetCursorPos(ImVec2(75, 8));
                                    BeginGroup( );

                                    for (int i = 0; i < custom.visuals_subtabs.size( ); ++i) {

                                        if (custom.sub_tab(custom.visuals_subtabs.at(i), custom.m_visuals_subtab == i) && custom.m_visuals_subtab != i)
                                            custom.m_visuals_subtab = i, custom.m_anim = 0.f;

                                        if (i != custom.visuals_subtabs.size( ) - 1)
                                            SameLine( );
                                    }

                                    EndGroup( );

                                    SetCursorPos(ImVec2(65, 50));
                                    BeginChild("##visuals_childs", ImVec2(GetWindowWidth( ) - 80, GetWindowHeight( ) - 80));
                                    {

                                        switch (custom.m_visuals_subtab) {

                                            case 0:
                                                if (settings::visuals::language) {
                                                    custom.begin_child("Settings", ImVec2(GetWindowWidth( ) / 2 - GetStyle( ).ItemSpacing.x / 2, GetWindowHeight( )));
                                                    {
                                                        Checkbox(E("Enabled"), &settings::visuals::enabled);

                                                        Checkbox(E("Health Bar"), &settings::visuals::healthbar);

                                                        Checkbox(E("Armor Bar"), &settings::visuals::armorbar);

                                                        Checkbox(E("Box"), &settings::visuals::box);

                                                        Checkbox(E("Skeleton"), &settings::visuals::skeleton);

                                                        Checkbox(E("Player Name"), &settings::visuals::playername);

                                                        Checkbox(E("Weapon Name"), &settings::visuals::weapon_name);

                                                        Checkbox(E("Snapline"), &settings::visuals::snapline);

                                                        Checkbox(E("Distance"), &settings::visuals::dont_draw_distance);

                                                        Checkbox(E("Show Npcs"), &settings::visuals::draw_ped);

                                                        Checkbox(E("draw dead"), &settings::visuals::draw_dead);

                                                        Checkbox(E("Radar"), &settings::radar::enable2DRadar);
                                                        
                                                       
                                                    }
                                                    custom.end_child( );
                                                }
                                                else
                                                {
                                                    custom.begin_child("Settings", ImVec2(GetWindowWidth( ) / 2 - GetStyle( ).ItemSpacing.x / 2, GetWindowHeight( )));
                                                    {
                                                        Checkbox(E("Enabled"), &settings::visuals::enabled);

                                                        Checkbox(E("Health Bar"), &settings::visuals::healthbar);

                                                        Checkbox(E("Armor Bar"), &settings::visuals::armorbar);

                                                        Checkbox(E("Box"), &settings::visuals::box);

                                                        Checkbox(E("Skeleton"), &settings::visuals::skeleton);

                                                        Checkbox(E("Player Name"), &settings::visuals::playername);

                                                        Checkbox(E("Weapon Name"), &settings::visuals::weapon_name);

                                                        Checkbox(E("Snapline"), &settings::visuals::snapline);

                                                        Checkbox(E("Distance"), &settings::visuals::dont_draw_distance);

                                                        Checkbox(E("Show Npcs"), &settings::visuals::draw_ped);

                                                        Checkbox(E("draw dead"), &settings::visuals::draw_dead);

                                                        Checkbox(E("Radar"), &settings::radar::enable2DRadar);
                                                      
                                                    }
                                                    custom.end_child( );
                                                }


                                                SameLine( );

                                                 if (settings::visuals::language) {
                                                    custom.begin_child("Customization", ImVec2(GetWindowWidth( ) / 2 - GetStyle( ).ItemSpacing.x / 2, GetWindowHeight( )));
                                                    {
                                                        ImGui::Text("Box Color");
                                                        ImGui::SameLine();
                                                        ImGui::ColorEdit4((E("##box")), settings::tempcolor::box, ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_AlphaBar | ImGuiColorEditFlags_NoBorder);
                                                        settings::visuals::boxcolor.Value.x = settings::tempcolor::box[0];
                                                        settings::visuals::boxcolor.Value.y = settings::tempcolor::box[1];
                                                        settings::visuals::boxcolor.Value.z = settings::tempcolor::box[2];
                                                        settings::visuals::boxcolor.Value.w = settings::tempcolor::box[3];
                                                        ImGui::Text("Skeleton Color");
                                                        ImGui::SameLine();
                                                        ImGui::ColorEdit4((E("##skssss")), settings::tempcolor::skeleton, ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_AlphaBar | ImGuiColorEditFlags_NoBorder);
                                                        settings::visuals::skeleton_color.Value.x = settings::tempcolor::skeleton[0];
                                                        settings::visuals::skeleton_color.Value.y = settings::tempcolor::skeleton[1];
                                                        settings::visuals::skeleton_color.Value.z = settings::tempcolor::skeleton[2];
                                                        settings::visuals::skeleton_color.Value.w = settings::tempcolor::skeleton[3];
                                                        ImGui::Text("Distance Color");
                                                        ImGui::SameLine();
                                                        ImGui::ColorEdit4((E("##distance")), settings::tempcolor::distance, ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_AlphaBar | ImGuiColorEditFlags_NoBorder);
                                                        settings::visuals::distance_color.Value.x = settings::tempcolor::distance[0];
                                                        settings::visuals::distance_color.Value.y = settings::tempcolor::distance[1];
                                                        settings::visuals::distance_color.Value.z = settings::tempcolor::distance[2];
                                                        settings::visuals::distance_color.Value.w = settings::tempcolor::distance[3];
                                                        ImGui::Text("Weapon Name Color");
                                                        ImGui::SameLine();
                                                        ImGui::ColorEdit4((E("##Weapon")), settings::tempcolor::weapon, ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_AlphaBar | ImGuiColorEditFlags_NoBorder);
                                                        settings::visuals::namecolor.Value.x = settings::tempcolor::weapon[0];
                                                        settings::visuals::namecolor.Value.y = settings::tempcolor::weapon[1];
                                                        settings::visuals::namecolor.Value.z = settings::tempcolor::weapon[2];
                                                        settings::visuals::namecolor.Value.w = settings::tempcolor::weapon[3];
                                                        /*ImGui::Text("Draw dead Color");
                                                        ImGui::SameLine();
                                                        ImGui::ColorEdit4((E("##Dead")), settings::tempcolor::draw, ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_AlphaBar | ImGuiColorEditFlags_NoBorder);
                                                        settings::visuals::namecolor.Value.x = settings::tempcolor::healthbar[0];
                                                        settings::visuals::namecolor.Value.y = settings::tempcolor::healthbar[1];
                                                        settings::visuals::namecolor.Value.z = settings::tempcolor::healthbar[2];
                                                        settings::visuals::namecolor.Value.w = settings::tempcolor::healthbar[3];
                                                        ImGui::Text("Armor Bar Color");
                                                        ImGui::SameLine();
                                                        ImGui::ColorEdit4((E("##Armor")), settings::tempcolor::armorbar, ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_AlphaBar | ImGuiColorEditFlags_NoBorder);
                                                        settings::visuals::namecolor.Value.x = settings::tempcolor::armorbar[0];
                                                        settings::visuals::namecolor.Value.y = settings::tempcolor::armorbar[1];
                                                        settings::visuals::namecolor.Value.z = settings::tempcolor::armorbar[2];
                                                        settings::visuals::namecolor.Value.w = settings::tempcolor::armorbar[3];*/
                                                        ImGui::Text("Player Name Color");
                                                        ImGui::SameLine();
                                                        ImGui::ColorEdit4((E("##name")), settings::tempcolor::name, ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_AlphaBar | ImGuiColorEditFlags_NoBorder);
                                                        settings::visuals::namecolor.Value.x = settings::tempcolor::name[0];
                                                        settings::visuals::namecolor.Value.y = settings::tempcolor::name[1];
                                                        settings::visuals::namecolor.Value.z = settings::tempcolor::name[2];
                                                        settings::visuals::namecolor.Value.w = settings::tempcolor::name[3];
                                                        ImGui::Text("Snap Line Color");
                                                        ImGui::SameLine();
                                                        ImGui::ColorEdit4((E("##snapline")), settings::tempcolor::snapline, ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_AlphaBar | ImGuiColorEditFlags_NoBorder);
                                                        settings::visuals::snapeline_color.Value.x = settings::tempcolor::snapline[0];
                                                        settings::visuals::snapeline_color.Value.y = settings::tempcolor::snapline[1];
                                                        settings::visuals::snapeline_color.Value.z = settings::tempcolor::snapline[2];
                                                        settings::visuals::snapeline_color.Value.w = settings::tempcolor::snapline[3];
                                                        ImGui::Spacing();

                                                        ImGui::SliderInt(E("Render Distance"), &settings::visuals::render_distance, 0, 1000);
                                                        ImGui::Text("Health & Armor Bar positions");
                                                        /*ImGui::SameLine();*/

                                                        static const char* Healthbar[3] = { "Side", "Bottom", "Top" };
                                                        /*ImGui::SetCursorPosX(10.0f);*/
                                                        ImGui::Combo("##Style", &settings::visuals::healthbar_style, Healthbar, 3);
                                                    }
                                                    custom.end_child( );
                                                 }
                                                 else
                                                 {
                                                    custom.begin_child("Customization", ImVec2(GetWindowWidth( ) / 2 - GetStyle( ).ItemSpacing.x / 2, GetWindowHeight( )));
                                                    {
                                                        ImGui::Text("Box Color");
                                                        ImGui::SameLine( );
                                                        ImGui::ColorEdit4((E("##box")), settings::tempcolor::box, ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_AlphaBar | ImGuiColorEditFlags_NoBorder);
                                                        settings::visuals::boxcolor.Value.x = settings::tempcolor::box[0];
                                                        settings::visuals::boxcolor.Value.y = settings::tempcolor::box[1];
                                                        settings::visuals::boxcolor.Value.z = settings::tempcolor::box[2];
                                                        settings::visuals::boxcolor.Value.w = settings::tempcolor::box[3];
                                                        ImGui::Text("Head Circle Color");
                                                        ImGui::SameLine( );
                                                        ImGui::ColorEdit4((E("##headahas")), settings::tempcolor::head, ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_AlphaBar | ImGuiColorEditFlags_NoBorder);
                                                        settings::visuals::head_color.Value.x = settings::tempcolor::head[0];
                                                        settings::visuals::head_color.Value.y = settings::tempcolor::head[1];
                                                        settings::visuals::head_color.Value.z = settings::tempcolor::head[2];
                                                        settings::visuals::head_color.Value.w = settings::tempcolor::head[3];
                                                        ImGui::Text("Skeleton Color");
                                                        ImGui::SameLine( );
                                                        ImGui::ColorEdit4((E("##skssss")), settings::tempcolor::skeleton, ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_AlphaBar | ImGuiColorEditFlags_NoBorder);
                                                        settings::visuals::skeleton_color.Value.x = settings::tempcolor::skeleton[0];
                                                        settings::visuals::skeleton_color.Value.y = settings::tempcolor::skeleton[1];
                                                        settings::visuals::skeleton_color.Value.z = settings::tempcolor::skeleton[2];
                                                        settings::visuals::skeleton_color.Value.w = settings::tempcolor::skeleton[3];
                                                        ImGui::Text("Distance Color");
                                                        ImGui::SameLine( );
                                                        ImGui::ColorEdit4((E("##distance")), settings::tempcolor::distance, ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_AlphaBar | ImGuiColorEditFlags_NoBorder);
                                                        settings::visuals::distance_color.Value.x = settings::tempcolor::distance[0];
                                                        settings::visuals::distance_color.Value.y = settings::tempcolor::distance[1];
                                                        settings::visuals::distance_color.Value.z = settings::tempcolor::distance[2];
                                                        settings::visuals::distance_color.Value.w = settings::tempcolor::distance[3];
                                                        ImGui::Text("Weapon Color");
                                                        ImGui::SameLine( );
                                                        ImGui::ColorEdit4((E("##weapon")), settings::tempcolor::weapon, ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_AlphaBar | ImGuiColorEditFlags_NoBorder);
                                                        settings::visuals::weapon_color.Value.x = settings::tempcolor::weapon[0];
                                                        settings::visuals::weapon_color.Value.y = settings::tempcolor::weapon[1];
                                                        settings::visuals::weapon_color.Value.z = settings::tempcolor::weapon[2];
                                                        settings::visuals::weapon_color.Value.w = settings::tempcolor::weapon[3];
                                                        ImGui::Text("Weapon Color");
                                                        ImGui::SameLine( );
                                                        ImGui::ColorEdit4((E("##player")), settings::tempcolor::name, ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_AlphaBar | ImGuiColorEditFlags_NoBorder);
                                                        settings::visuals::namecolor.Value.x = settings::tempcolor::name[0];
                                                        settings::visuals::namecolor.Value.y = settings::tempcolor::name[1];
                                                        settings::visuals::namecolor.Value.z = settings::tempcolor::name[2];
                                                        settings::visuals::namecolor.Value.w = settings::tempcolor::name[3];
                                                        ImGui::Text("SnapLine Color");
                                                        ImGui::SameLine( );
                                                        ImGui::ColorEdit4((E("##snapline")), settings::tempcolor::snapline, ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_AlphaBar | ImGuiColorEditFlags_NoBorder);
                                                        settings::visuals::snapeline_color.Value.x = settings::tempcolor::snapline[0];
                                                        settings::visuals::snapeline_color.Value.y = settings::tempcolor::snapline[1];
                                                        settings::visuals::snapeline_color.Value.z = settings::tempcolor::snapline[2];
                                                        settings::visuals::snapeline_color.Value.w = settings::tempcolor::snapline[3];
                                                        ImGui::Spacing( );
                                                        

                                                        ImGui::SliderInt(E("Render Distance"), &settings::visuals::render_distance, 0, 1000);
                                                        ImGui::Text("Health & Armor Bar positions");
                                                        /*ImGui::SameLine();*/

                                                        static const char* Healthbar[3] = { "Side", "Bottom", "Top" };
                                                        /*ImGui::SetCursorPosX(10.0f);*/
                                                        ImGui::Combo("##Style", &settings::visuals::healthbar_style, Healthbar, 3);

                                                        
                                                    }
                                                    custom.end_child( );
                                                 }

                                                break;
                                            case 1:
                                                if (settings::visuals::language) {
                                                    custom.begin_child("Vehicles", ImVec2(GetWindowWidth( ) / 2 - GetStyle( ).ItemSpacing.x / 2, GetWindowHeight( )));
                                                    {
                                                        Checkbox(E("Enable"), &settings::vehicle::vehicle_esp);
                                                        ImGui::Hotkey(E("Hotkey : "), &settings::self::hotkey_vehicleesp, ImVec2(160, 20));

                                                        Checkbox(E("Name"), &settings::vehicle::vehicle_name);
                                                        if (settings::vehicle::vehicle_name) {

                                                            ImGui::SameLine();

                                                            ImGui::ColorEdit4((E("##52352gfgfddaspl")), settings::tempcolor::vehicle_esp, ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_AlphaBar | ImGuiColorEditFlags_NoBorder);
                                                            settings::visuals::vehicle_esp_color.Value.x = settings::tempcolor::vehicle_esp[0];
                                                            settings::visuals::vehicle_esp_color.Value.y = settings::tempcolor::vehicle_esp[1];
                                                            settings::visuals::vehicle_esp_color.Value.z = settings::tempcolor::vehicle_esp[2];
                                                            settings::visuals::vehicle_esp_color.Value.w = settings::tempcolor::vehicle_esp[3];
                                                        }
                                                        Checkbox(E("Distance"), &settings::vehicle::vehicle_distance);
                                                        if (settings::vehicle::vehicle_distance) {

                                                            ImGui::SameLine();

                                                            ImGui::ColorEdit4((E("##52352d231312aspl")), settings::tempcolor::vehicledistance, ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_AlphaBar | ImGuiColorEditFlags_NoBorder);
                                                            settings::visuals::vehicle_esp_distance.Value.x = settings::tempcolor::vehicledistance[0];
                                                            settings::visuals::vehicle_esp_distance.Value.y = settings::tempcolor::vehicledistance[1];
                                                            settings::visuals::vehicle_esp_distance.Value.z = settings::tempcolor::vehicledistance[2];
                                                            settings::visuals::vehicle_esp_distance.Value.w = settings::tempcolor::vehicledistance[3];
                                                        }
                                                        Checkbox(E("Health"), &settings::vehicle::vehicle_health);
                                                        if (settings::vehicle::vehicle_health) {

                                                            ImGui::SameLine();

                                                            ImGui::ColorEdit4((E("##52352daspl")), settings::tempcolor::vehicleesphealth, ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_AlphaBar | ImGuiColorEditFlags_NoBorder);
                                                            settings::visuals::vehicle_esp_health.Value.x = settings::tempcolor::vehicleesphealth[0];
                                                            settings::visuals::vehicle_esp_health.Value.y = settings::tempcolor::vehicleesphealth[1];
                                                            settings::visuals::vehicle_esp_health.Value.z = settings::tempcolor::vehicleesphealth[2];
                                                            settings::visuals::vehicle_esp_health.Value.w = settings::tempcolor::vehicleesphealth[3];
                                                        }
                                                        if (settings::vehicle::vehicle_health) {

                                                            ImGui::SameLine();

                                                            ImGui::ColorEdit4((E("##52352daspl52fsdf")), settings::tempcolor::vehiclebrokencolor, ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_AlphaBar | ImGuiColorEditFlags_NoBorder);
                                                            settings::visuals::vehiclebroken_esp_color.Value.x = settings::tempcolor::vehiclebrokencolor[0];
                                                            settings::visuals::vehiclebroken_esp_color.Value.y = settings::tempcolor::vehiclebrokencolor[1];
                                                            settings::visuals::vehiclebroken_esp_color.Value.z = settings::tempcolor::vehiclebrokencolor[2];
                                                            settings::visuals::vehiclebroken_esp_color.Value.w = settings::tempcolor::vehiclebrokencolor[3];
                                                        }
                                                        Checkbox(E("Marker"), &settings::vehicle::vehicle_marker);
                                                        if (settings::vehicle::vehicle_marker) {

                                                            ImGui::SameLine();

                                                            ImGui::ColorEdit4((E("##5235dvaaafva2daspl")), settings::tempcolor::vehiclemarker, ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_AlphaBar | ImGuiColorEditFlags_NoBorder);
                                                            settings::visuals::vehicle_esp_marker.Value.x = settings::tempcolor::vehiclemarker[0];
                                                            settings::visuals::vehicle_esp_marker.Value.y = settings::tempcolor::vehiclemarker[1];
                                                            settings::visuals::vehicle_esp_marker.Value.z = settings::tempcolor::vehiclemarker[2];
                                                            settings::visuals::vehicle_esp_marker.Value.w = settings::tempcolor::vehiclemarker[3];
                                                        }
                                                        Checkbox(E("Snapline"), &settings::vehicle::vehicle_snapline);

                                                        ImGui::SliderInt(E("Render Distance"), &settings::visuals::vehicle_distance, 0, 1000);
                                                    }
                                                    custom.end_child( );
                                                }
                                                else
                                                {
                                                    custom.begin_child("Vehicles", ImVec2(GetWindowWidth( ) / 2 - GetStyle( ).ItemSpacing.x / 2, GetWindowHeight( )));
                                                    {
                                                        Checkbox(E("Enable"), &settings::vehicle::vehicle_esp);
                                                        ImGui::Hotkey(E("Hotkey : "), &settings::self::hotkey_vehicleesp, ImVec2(160, 20));

                                                        Checkbox(E("Name"), &settings::vehicle::vehicle_name);
                                                        if (settings::vehicle::vehicle_name) {

                                                            ImGui::SameLine( );

                                                            ImGui::ColorEdit4((E("##52352gfgfddaspl")), settings::tempcolor::vehicle_esp, ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_AlphaBar | ImGuiColorEditFlags_NoBorder);
                                                            settings::visuals::vehicle_esp_color.Value.x = settings::tempcolor::vehicle_esp[0];
                                                            settings::visuals::vehicle_esp_color.Value.y = settings::tempcolor::vehicle_esp[1];
                                                            settings::visuals::vehicle_esp_color.Value.z = settings::tempcolor::vehicle_esp[2];
                                                            settings::visuals::vehicle_esp_color.Value.w = settings::tempcolor::vehicle_esp[3];
                                                        }
                                                        Checkbox(E("Distance"), &settings::vehicle::vehicle_distance);
                                                        if (settings::vehicle::vehicle_distance) {

                                                            ImGui::SameLine( );

                                                            ImGui::ColorEdit4((E("##52352d231312aspl")), settings::tempcolor::vehicledistance, ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_AlphaBar | ImGuiColorEditFlags_NoBorder);
                                                            settings::visuals::vehicle_esp_distance.Value.x = settings::tempcolor::vehicledistance[0];
                                                            settings::visuals::vehicle_esp_distance.Value.y = settings::tempcolor::vehicledistance[1];
                                                            settings::visuals::vehicle_esp_distance.Value.z = settings::tempcolor::vehicledistance[2];
                                                            settings::visuals::vehicle_esp_distance.Value.w = settings::tempcolor::vehicledistance[3];
                                                        }
                                                        Checkbox(E("Health"), &settings::vehicle::vehicle_health);
                                                        if (settings::vehicle::vehicle_health) {

                                                            ImGui::SameLine( );

                                                            ImGui::ColorEdit4((E("##52352daspl")), settings::tempcolor::vehicleesphealth, ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_AlphaBar | ImGuiColorEditFlags_NoBorder);
                                                            settings::visuals::vehicle_esp_health.Value.x = settings::tempcolor::vehicleesphealth[0];
                                                            settings::visuals::vehicle_esp_health.Value.y = settings::tempcolor::vehicleesphealth[1];
                                                            settings::visuals::vehicle_esp_health.Value.z = settings::tempcolor::vehicleesphealth[2];
                                                            settings::visuals::vehicle_esp_health.Value.w = settings::tempcolor::vehicleesphealth[3];
                                                        }
                                                        if (settings::vehicle::vehicle_health) {

                                                            ImGui::SameLine( );

                                                            ImGui::ColorEdit4((E("##52352daspl52fsdf")), settings::tempcolor::vehiclebrokencolor, ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_AlphaBar | ImGuiColorEditFlags_NoBorder);
                                                            settings::visuals::vehiclebroken_esp_color.Value.x = settings::tempcolor::vehiclebrokencolor[0];
                                                            settings::visuals::vehiclebroken_esp_color.Value.y = settings::tempcolor::vehiclebrokencolor[1];
                                                            settings::visuals::vehiclebroken_esp_color.Value.z = settings::tempcolor::vehiclebrokencolor[2];
                                                            settings::visuals::vehiclebroken_esp_color.Value.w = settings::tempcolor::vehiclebrokencolor[3];
                                                        }
                                                        Checkbox(E("Marker"), &settings::vehicle::vehicle_marker);
                                                        if (settings::vehicle::vehicle_marker) {

                                                            ImGui::SameLine( );

                                                            ImGui::ColorEdit4((E("##5235dvaaafva2daspl")), settings::tempcolor::vehiclemarker, ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_AlphaBar | ImGuiColorEditFlags_NoBorder);
                                                            settings::visuals::vehicle_esp_marker.Value.x = settings::tempcolor::vehiclemarker[0];
                                                            settings::visuals::vehicle_esp_marker.Value.y = settings::tempcolor::vehiclemarker[1];
                                                            settings::visuals::vehicle_esp_marker.Value.z = settings::tempcolor::vehiclemarker[2];
                                                            settings::visuals::vehicle_esp_marker.Value.w = settings::tempcolor::vehiclemarker[3];
                                                        }
                                                        Checkbox(E("Snapline"), &settings::vehicle::vehicle_snapline);

                                                        ImGui::SliderInt(E("Render Distance"), &settings::visuals::vehicle_distance, 0, 1000);
                                                    }
                                                    custom.end_child( );
                                                }

                                                break;
                                        }
                                    }
                                    EndChild( );

                                    break;
                                case 2:

                                    SetCursorPos(ImVec2(75, 8));
                                    BeginGroup( );

                                    for (int i = 0; i < custom.localplayer_subtab.size( ); ++i) {

                                        if (custom.sub_tab(custom.localplayer_subtab.at(i), custom.m_localplayer_subtab == i) && custom.m_localplayer_subtab != i)
                                            custom.m_localplayer_subtab = i, custom.m_anim = 0.f;

                                        if (i != custom.localplayer_subtab.size( ) - 1)
                                            SameLine( );
                                    }

                                    EndGroup( );

                                    SetCursorPos(ImVec2(65, 50));
                                    BeginChild("##localplayer_childs", ImVec2(GetWindowWidth( ) - 80, GetWindowHeight( ) - 80));
                                    {

                                        switch (custom.m_localplayer_subtab) {

                                            case 0:
                                                if (settings::visuals::language) {
                                                    custom.begin_child("", ImVec2(GetWindowWidth( ) / 2 - GetStyle( ).ItemSpacing.x / 2, GetWindowHeight( )));
                                                    {

                                                    }
                                                    custom.end_child( );
                                                }
                                                else
                                                {
                                                    custom.begin_child("Player", ImVec2(GetWindowWidth( ) / 2 - GetStyle( ).ItemSpacing.x / 2, GetWindowHeight( )));
                                                    {
                                                        Checkbox(E("Enable Self"), &settings::self::enabled);

                                                        
                                                        Checkbox(E("GodMode"), &settings::self::godmode);

                                                        Checkbox(E("Semi-GodMode"), &settings::self::semigodmode);

                                                        ImGui::PopStyleColor( );
                                                        ImGui::SliderInt(E("Health"), &settings::self::health, 0, 100);

                                                        ImGui::SliderInt(E("Armor"), &settings::self::armor, 0, 100);

                                                        ImGui::Hotkey(E("Hotkey Health :   "), &settings::self::HotkeyHealth, ImVec2(150, 20));

                                                        ImGui::Hotkey(E("Hotkey Armor :   "), &settings::self::HotkeyArmor, ImVec2(150, 20));

                                                        ImGui::Spacing( );
                                                        
                                                        if (ImGui::Button(E("Set Healh"))) {

                                                            settings::self::set_health = true;
                                                        }

                                                        ImGui::SameLine( );

                                                        if (ImGui::Button(E("Set Armor"))) {

                                                            settings::self::set_armor = true;
                                                        }

                           
                                                        
                                                  
                                                    }
                                                    custom.end_child( );
                                                }

                                                SameLine( );
                                                if (settings::visuals::language) {
                                                    custom.begin_child("", ImVec2(GetWindowWidth( ) / 2 - GetStyle( ).ItemSpacing.x / 2, GetWindowHeight( )));
                                                    {
                                                    }
                                                    custom.end_child( );
                                                }
                                                else
                                                {
                                                    custom.begin_child("Movement", ImVec2(GetWindowWidth( ) / 2 - GetStyle( ).ItemSpacing.x / 2, GetWindowHeight( )));
                                                    {

                                                        Checkbox(E("Noclip"), &settings::self::noclip);

                                                        ImGui::Hotkey(E("Noclip Key :   "), &settings::self::hotkey_noclip, ImVec2(150, 20));

                                                        Checkbox(E("Fast Run"), &settings::self::fastrun);

                                                        ImGui::SliderFloat((E("Run Speed")), &settings::self::RunSpeedToSet, 1.0, 5.0, nullptr, ImGuiSliderFlags_AlwaysClamp || ImGuiSliderFlags_NoInput);
                                                    }
                                                    custom.end_child( );
                                                }
                                                break;
                                            case 1:
                                                if (settings::visuals::language) {
                                                    custom.begin_child("Weapons Modification", ImVec2(GetWindowWidth( ) / 2 - GetStyle( ).ItemSpacing.x / 2, GetWindowHeight( )));
                                                    {
                                                        Checkbox(E("Enable"), &settings::weapon::enabled);

                                                        Checkbox(E("No Recoil"), &settings::weapon::no_recoil);

                                                        if (settings::weapon::no_recoil) {

                                                            ImGui::SliderFloat(E("Recoil"), &settings::weapon::recoil_value, 0.0, 5.0);
                                                        }

                                                        Checkbox(E("No Spread"), &settings::weapon::no_spread);

                                                        ImGui::PopStyleColor( );
                                                        Checkbox(E("Unlimited Range"), &settings::weapon::infinite_range);

                                                        
                                                    }
                                                    custom.end_child( );
                                                }
                                                else
                                                {
                                                    custom.begin_child("Weapons Modification", ImVec2(GetWindowWidth( ) / 2 - GetStyle( ).ItemSpacing.x / 2, GetWindowHeight( )));
                                                    {
                                                        Checkbox(E("Enable"), &settings::weapon::enabled);

                                                        Checkbox(E("No Recoil"), &settings::weapon::no_recoil);

                                                        if (settings::weapon::no_recoil) {

                                                            ImGui::SliderFloat(E("Recoil"), &settings::weapon::recoil_value, 0.0, 5.0);
                                                        }


                                                        Checkbox(E("No Spread"), &settings::weapon::no_spread);

                                                        Checkbox(E("No Reload"), &settings::weapon::no_reload);
                                                        
                                                        Checkbox(E("Infinite Ammo"), &settings::weapon::infite_ammo);
                                                        ImGui::PopStyleColor( );
                                                        Checkbox(E("Unlimited Range"), &settings::weapon::infinite_range);
                                                        
                                                        Checkbox(E("Damage Boost"), &settings::weapon::damage_boost);
                                                        ImGui::PopStyleColor( );
                                                        if (settings::weapon::damage_boost) {

                                                            /*ImGui::SliderInt(E("Damage"), &settings::weapon::int_damage, 0, 1000);*/
                                                        }
                                                        ImGui::Hotkey(E("Damage Hotkey : "), &settings::self::hotkey_damage, ImVec2(160, 20));
                                                        
                                                        Checkbox(E("Explosive Ammo"), &settings::weapon::explosiveammo);
                                                        ImGui::PopStyleColor( );
                                                        ImGui::Hotkey(E("Explosive Hotkey : "), &settings::self::hotkey_explosiveammo, ImVec2(160, 20));
                                                    }
                                                    custom.end_child( );
                                                }
                                                break;

                                        }
                                    }
                                    EndChild( );

                                    break;
                                case 3:

                                    SetCursorPos(ImVec2(75, 8));
                                    BeginGroup( );

                                    for (int i = 0; i < custom.vehiclemod_subtab.size( ); ++i) {

                                        if (custom.sub_tab(custom.vehiclemod_subtab.at(i), custom.m_vehiclemod_subtab == i) && custom.m_vehiclemod_subtab != i)
                                            custom.m_vehiclemod_subtab = i, custom.m_anim = 0.f;

                                        if (i != custom.vehiclemod_subtab.size( ) - 1)
                                            SameLine( );
                                    }

                                    EndGroup( );

                                    SetCursorPos(ImVec2(65, 50));
                                    BeginChild("##Vehicles_childs", ImVec2(GetWindowWidth( ) - 80, GetWindowHeight( ) - 80));
                                    {

                                        switch (custom.m_vehiclemod_subtab) {

                                            case 0:
                                                if (settings::visuals::language) {
                                                    custom.begin_child("Vehicle Modifications", ImVec2(GetWindowWidth( ) / 2 - GetStyle( ).ItemSpacing.x / 2, GetWindowHeight( )));
                                                    {
                                                        if (ImGui::Button(E("Repair Vehicle"))) {
                                                            settings::vehicle::vehicle_repair = true;
                                                        }
                                                        ImGui::Hotkey(E("Hotkey :   "), &settings::self::hotkey_vehicle, ImVec2(150, 20));

                                                        Checkbox(E("Boost"), &settings::vehicle::vehicle_speed_bool);
                                                        if (settings::vehicle::vehicle_speed_bool) {
                                                            ImGui::SliderInt((E("Boost Speed")), &settings::vehicle::vehicle_speed_value, 0, 100, nullptr, ImGuiSliderFlags_AlwaysClamp || ImGuiSliderFlags_NoInput);
                                                        }
                                                    }
                                                    custom.end_child( );
                                                }
                                                else
                                                {
                                                    custom.begin_child("Vehicle Modifications", ImVec2(GetWindowWidth( ) / 2 - GetStyle( ).ItemSpacing.x / 2, GetWindowHeight( )));
                                                    {
                                                        if (ImGui::Button(E("Repair Vehicle"))) {
                                                            settings::vehicle::vehicle_repair = true;
                                                        }
                                                        ImGui::Hotkey(E("Hotkey :   "), &settings::self::hotkey_vehicle, ImVec2(150, 20));

                                                        Checkbox(E("Boost"), &settings::vehicle::vehicle_speed_bool);
                                                        if (settings::vehicle::vehicle_speed_bool) {
                                                            ImGui::SliderInt((E("Boost Speed")), &settings::vehicle::vehicle_speed_value, 0, 100, nullptr, ImGuiSliderFlags_AlwaysClamp || ImGuiSliderFlags_NoInput);
                                                        }
                                                    }
                                                    custom.end_child( );
                                                }

                                        }
                                    }
                                    EndChild( );

                                    break;
                                case 4:

                                    SetCursorPos(ImVec2(75, 8));
                                    BeginGroup( );

                                    for (int i = 0; i < custom.extinction_subtab.size( ); ++i) {

                                        if (custom.sub_tab(custom.extinction_subtab.at(i), custom.m_extinction_subtab == i) && custom.m_extinction_subtab != i)
                                            custom.m_extinction_subtab = i, custom.m_anim = 0.f;

                                        if (i != custom.extinction_subtab.size( ) - 1)
                                            SameLine( );
                                    }

                                    EndGroup( );

                                    SetCursorPos(ImVec2(65, 50));
                                    BeginChild("##extinction_childs", ImVec2(GetWindowWidth( ) - 80, GetWindowHeight( ) - 80));
                                    {

                                        switch (custom.m_extinction_subtab) {

                                            case 0:
                                                if (settings::visuals::language) {
                                                    custom.begin_child("", ImVec2(GetWindowWidth( ) / 2 - GetStyle( ).ItemSpacing.x / 2, GetWindowHeight( )));
                                                    {
                                                    }
                                                    custom.end_child( );
                                                }
                                                else
                                                {
                                                    custom.begin_child("Extinction", ImVec2(GetWindowWidth( ) / 2 - GetStyle( ).ItemSpacing.x / 2, GetWindowHeight( )));
                                                    {

                                                        Checkbox(E("Auto Farm"), &settings::self::autoarm);
                                                        ImGui::Hotkey(E("Hotkey :   "), &settings::self::hotkey_autofarm, ImVec2(150, 20));
                                                    }
                                                    custom.end_child( );
                                                }
                                          
                                                SameLine( );
                                                if (settings::visuals::language) {
                                                    custom.begin_child("Visuais", ImVec2(GetWindowWidth( ) / 2 - GetStyle( ).ItemSpacing.x / 2, GetWindowHeight( )));
                                                    {

                                                        Checkbox(E("Ativar"), &settings::object::object_esp);
                                                        Checkbox(E("Loot Bag Nome"), &settings::object::object_name);
                                                        if (settings::object::object_name) {

                                                            ImGui::SameLine( );

                                                            ImGui::ColorEdit4((E("##na124141me")), settings::tempcolor::bag_espg, ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_AlphaBar | ImGuiColorEditFlags_NoBorder);
                                                            settings::visuals::bag_esp_color.Value.x = settings::tempcolor::bag_espg[0];
                                                            settings::visuals::bag_esp_color.Value.y = settings::tempcolor::bag_espg[1];
                                                            settings::visuals::bag_esp_color.Value.z = settings::tempcolor::bag_espg[2];
                                                            settings::visuals::bag_esp_color.Value.w = settings::tempcolor::bag_espg[3];
                                                        }

                                                        Checkbox(E("Loot Bag Distancia"), &settings::object::object_distance);
                                                        if (settings::object::object_distance) {

                                                            ImGui::SameLine( );

                                                            ImGui::ColorEdit4((E("##name32532")), settings::tempcolor::bagdistanccolor, ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_AlphaBar | ImGuiColorEditFlags_NoBorder);
                                                            settings::visuals::bagdistance_esp_color.Value.x = settings::tempcolor::bagdistanccolor[0];
                                                            settings::visuals::bagdistance_esp_color.Value.y = settings::tempcolor::bagdistanccolor[1];
                                                            settings::visuals::bagdistance_esp_color.Value.z = settings::tempcolor::bagdistanccolor[2];
                                                            settings::visuals::bagdistance_esp_color.Value.w = settings::tempcolor::bagdistanccolor[3];
                                                        }
                                                        Checkbox(E("Loot Bag Linha"), &settings::object::object_snapline);
                                                        Checkbox(E("Loot Bag Marcador"), &settings::object::object_marker);
                                                        if (settings::object::object_marker) {

                                                            ImGui::SameLine( );

                                                            ImGui::ColorEdit4((E("##57574523141name")), settings::tempcolor::bagmarker, ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_AlphaBar | ImGuiColorEditFlags_NoBorder);
                                                            settings::visuals::bagmarker_esp_color.Value.x = settings::tempcolor::bagmarker[0];
                                                            settings::visuals::bagmarker_esp_color.Value.y = settings::tempcolor::bagmarker[1];
                                                            settings::visuals::bagmarker_esp_color.Value.z = settings::tempcolor::bagmarker[2];
                                                            settings::visuals::bagmarker_esp_color.Value.w = settings::tempcolor::bagmarker[3];
                                                        }
                                                        ImGui::SliderInt(E("Render Distance"), &settings::visuals::lootbag_distance, 0, 1000);
                                                    }
                                                    custom.end_child( );
                                                }
                                                else
                                                {
                                                    custom.begin_child("Visuals", ImVec2(GetWindowWidth( ) / 2 - GetStyle( ).ItemSpacing.x / 2, GetWindowHeight( )));
                                                    {

                                                        Checkbox(E("Enabled"), &settings::object::object_esp);
                                                        Checkbox(E("Loot Bag Name"), &settings::object::object_name);
                                                        if (settings::object::object_name) {

                                                            ImGui::SameLine( );

                                                            ImGui::ColorEdit4((E("##na124141me")), settings::tempcolor::bag_espg, ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_AlphaBar | ImGuiColorEditFlags_NoBorder);
                                                            settings::visuals::bag_esp_color.Value.x = settings::tempcolor::bag_espg[0];
                                                            settings::visuals::bag_esp_color.Value.y = settings::tempcolor::bag_espg[1];
                                                            settings::visuals::bag_esp_color.Value.z = settings::tempcolor::bag_espg[2];
                                                            settings::visuals::bag_esp_color.Value.w = settings::tempcolor::bag_espg[3];
                                                        }

                                                        Checkbox(E("Loot Bag Distance"), &settings::object::object_distance);
                                                        if (settings::object::object_distance) {

                                                            ImGui::SameLine( );

                                                            ImGui::ColorEdit4((E("##name32532")), settings::tempcolor::bagdistanccolor, ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_AlphaBar | ImGuiColorEditFlags_NoBorder);
                                                            settings::visuals::bagdistance_esp_color.Value.x = settings::tempcolor::bagdistanccolor[0];
                                                            settings::visuals::bagdistance_esp_color.Value.y = settings::tempcolor::bagdistanccolor[1];
                                                            settings::visuals::bagdistance_esp_color.Value.z = settings::tempcolor::bagdistanccolor[2];
                                                            settings::visuals::bagdistance_esp_color.Value.w = settings::tempcolor::bagdistanccolor[3];
                                                        }
                                                        Checkbox(E("Loot Bag Snapline"), &settings::object::object_snapline);
                                                        Checkbox(E("Loot Bag Marker"), &settings::object::object_marker);
                                                        if (settings::object::object_marker) {

                                                            ImGui::SameLine( );

                                                            ImGui::ColorEdit4((E("##57574523141name")), settings::tempcolor::bagmarker, ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_AlphaBar | ImGuiColorEditFlags_NoBorder);
                                                            settings::visuals::bagmarker_esp_color.Value.x = settings::tempcolor::bagmarker[0];
                                                            settings::visuals::bagmarker_esp_color.Value.y = settings::tempcolor::bagmarker[1];
                                                            settings::visuals::bagmarker_esp_color.Value.z = settings::tempcolor::bagmarker[2];
                                                            settings::visuals::bagmarker_esp_color.Value.w = settings::tempcolor::bagmarker[3];
                                                        }
                                                        ImGui::SliderInt(E("Render Distance"), &settings::visuals::lootbag_distance, 0, 1000);
                                                    }
                                                    custom.end_child( );
                                                }

                                                break;
                                        }
                                    }
                                    EndChild( );

                                    break;
                                case 5:

                                    SetCursorPos(ImVec2(75, 8));
                                    BeginGroup( );

                                    for (int i = 0; i < custom.playerlist_subtab.size( ); ++i) {

                                        if (custom.sub_tab(custom.playerlist_subtab.at(i), custom.m_playerlist_subtab == i) && custom.m_playerlist_subtab != i)
                                            custom.m_playerlist_subtab = i, custom.m_anim = 0.f;

                                        if (i != custom.playerlist_subtab.size( ) - 1)
                                            SameLine( );
                                    }

                                    EndGroup( );


                                     SetCursorPos(ImVec2(65, 50));
                                    BeginChild("##playerlist", ImVec2(GetWindowWidth( ) - 80, GetWindowHeight( ) - 80));
                                    {
                                        switch (custom.m_playerlist_subtab) {

                                            case 1:
                                                if (settings::visuals::language) {
                                                    custom.begin_child("Player List", ImVec2(GetWindowWidth( ) / 2 - GetStyle( ).ItemSpacing.x / 2, GetWindowHeight( )));
                                                    {
                                                        hk_World* World = (hk_World*)*(uint64_t*)(FiveM::World);
                                                        if (!World)
                                                            return;
                                                        hk_Ped* LocalPlayer = World->LocalPlayer( );

                                                        if (LocalPlayer) {

                                                            hk_ObjectNavigationPed* Nav = LocalPlayer->ObjectNavigation( );
                                                            if (!Nav)
                                                                return;

                                                            hk_ReplayInterface* ReplayInterface = (hk_ReplayInterface*)*(uint64_t*)(FiveM::ReplayInterface);
                                                            if (!ReplayInterface)
                                                                return;

                                                            hk_PedInterface* PedInterface = ReplayInterface->PedInterface( );
                                                            if (!PedInterface)
                                                                return;

                                                            if (ImGui::ListBoxHeader("    ", ImVec2(220, 300))) {
                                                                for (int i = 0; i < PedInterface->PedMaximum( ); i++) {

                                                                    hk_Ped* Peds = PedInterface->PedList( )->Ped(i);
                                                                    if (!Peds)
                                                                        continue;
                                           
                                                                    void* m_net_player_data = (void*)(Peds->PlayerInfo( ) + 0x20);
                                         
                                                                    auto playerName = FiveM::getpname(m_net_player_data);
                                                                    const char* c = playerName;
                                                                    const char* items[] = {c};
                                                                    auto ped_type = Peds->GetPedType( );
                                                                    if (!ped_type) {
                                                                        continue;
                                                                    }
                                                                    bool IsPed = false;
                                                                    ped_type = ped_type << 11 >> 25;

                                                                    if (ped_type != 2) {
                                                                        IsPed = true;

                                                                    }

                                                                    if (IsPed)
                                                                    {
                                                                        continue;
                                                                    }


                                                                    bool is_selected = (selected_index == i);
                                                                    if (ImGui::Selectable(playerName, is_selected)) {

                                                                        selected_index = i;
                                                                    }
                                                                }

                                                                ImGui::ListBoxFooter( );
                                                            }

                                                            hk_Ped* SelectedPed = PedInterface->PedList( )->Ped(selected_index);
                                                        }
                                                    }
                                                    custom.end_child( );
                                                }
                                                else {
                                                    custom.begin_child("Player List", ImVec2(GetWindowWidth() / 2 - GetStyle().ItemSpacing.x / 2, GetWindowHeight()));
                                                    {
                                                        hk_World* World = (hk_World*)*(uint64_t*)(FiveM::World);
                                                        if (!World)
                                                            return;
                                                        hk_Ped* LocalPlayer = World->LocalPlayer();
                                                        if (!World)
                                                            return;
                                                        if (LocalPlayer) {

                                                            hk_ObjectNavigationPed* Nav = LocalPlayer->ObjectNavigation();
                                                            if (!Nav)
                                                                return;

                                                            hk_ReplayInterface* ReplayInterface = (hk_ReplayInterface*)*(uint64_t*)(FiveM::ReplayInterface);
                                                            if (!ReplayInterface)
                                                                return;

                                                            hk_PedInterface* PedInterface = ReplayInterface->PedInterface();
                                                            if (!PedInterface)
                                                                return;

                                                            if (ImGui::ListBoxHeader("    ", ImVec2(220, 300))) {
                                                                for (int i = 0; i < PedInterface->PedMaximum(); i++) {

                                                                    hk_Ped* Peds = PedInterface->PedList()->Ped(i);
                                                                    if (!Peds)
                                                                        continue;
                                                                    //   int PlayerID = FiveM::pointer_to_handle((DWORD64)Peds);
                                                                    void* m_net_player_data = (void*)(Peds->PlayerInfo() + 0x20);
                                                                    bool IsPed = false;


                                                                    auto playerName = FiveM::getpname(m_net_player_data);
                                                                    const char* c = playerName;
                                                                    const char* items[] = { c };

                                                                    auto ped_type = Peds->GetPedType();
                                                                    if (!ped_type) {
                                                                        continue;
                                                                    }
                                                                    ped_type = ped_type << 11 >> 25;

                                                                    if (ped_type != 2) {
                                                                        IsPed = true;

                                                                    }

                                                                    if (IsPed)
                                                                    {
                                                                        continue;
                                                                    }
                                                                    bool is_selected = (selected_index == i);
                                                                    if (ImGui::Selectable(playerName, is_selected)) {

                                                                        selected_index = i;
                                                                    }
                                                                }

                                                                ImGui::ListBoxFooter();
                                                            }

                                                            hk_Ped* SelectedPed = PedInterface->PedList()->Ped(selected_index);
                                                        }
                                                    }
                                                    custom.end_child();
                                                }

                                                SameLine( );

                                                custom.begin_child("Player Options", ImVec2(GetWindowWidth() / 2 - GetStyle().ItemSpacing.x / 2, GetWindowHeight()));
                                                {
                                                    hk_World* World = (hk_World*)*(uint64_t*)(FiveM::World);
                                                    if (!World)
                                                        return;
                                                    hk_Ped* LocalPlayer = World->LocalPlayer();

                                                    if (LocalPlayer) {

                                                        hk_ObjectNavigationPed* Nav = LocalPlayer->ObjectNavigation();
                                                        if (!Nav)
                                                            return;

                                                        hk_ReplayInterface* ReplayInterface = (hk_ReplayInterface*)*(uint64_t*)(FiveM::ReplayInterface);
                                                        if (!ReplayInterface)
                                                            return;

                                                        hk_PedInterface* PedInterface = ReplayInterface->PedInterface();
                                                        if (!PedInterface)
                                                            return;

                                                        hk_Ped* SelectedPed = PedInterface->PedList()->Ped(selected_index);

                                                        if (SelectedPed->GetCoordinate().x != 0) {
                                                            std::string healthtoshow = "Health Of Player : " + std::to_string(SelectedPed->GetHealth());
                                                            ImGui::Text(healthtoshow.c_str());

                                                            if (ImGui::Button("Teleport To Player")) {
                                                                if (SelectedPed != LocalPlayer) {
                                                                    Nav->SetRotation({ 0, 0, 0, 0 });
                                                                    LocalPlayer->SetVelocity();
                                                                    LocalPlayer->SetCoordinate(SelectedPed->GetCoordinate());
                                                                    Nav->SetCoordinate(SelectedPed->GetCoordinate());
                                                                }
                                                            }



                                                            void* m_net_player_data = (void*)(SelectedPed->PlayerInfo() + 0x20);
                                                            auto it = std::find(FiveM::Friend.begin(), FiveM::Friend.end(), FiveM::getpname(m_net_player_data));
                                                            if (it != FiveM::Friend.end()) {
                                                                if (ImGui::Button("Remove Friend")) {
                                                                    // auto it = std::find(FiveM::Friend.begin( ), FiveM::Friend.end( ), (DWORD64)SelectedPed);
                                                                    int index = it - FiveM::Friend.begin();
                                                                    FiveM::Friend.erase(FiveM::Friend.begin() + index);
                                                                }
                                                            }
                                                            else {
                                                                if (ImGui::Button("Add Friend")) {
                                                                    FiveM::Friend.push_back(FiveM::getpname(m_net_player_data));
                                                                }
                                                            }
                                                        }
                                                    }
                                                }
                                                custom.end_child();
                                        }
                                    }
                                    EndChild();

                                    break;
                                case 6:

                                    SetCursorPos(ImVec2(75, 8));
                                    BeginGroup( );

                                    for (int i = 0; i < custom.vehicle_subtab.size( ); ++i) {

                                        if (custom.sub_tab(custom.vehicle_subtab.at(i), custom.m_vehicle_subtab == i) && custom.m_vehicle_subtab != i)
                                            custom.m_vehicle_subtab = i, custom.m_anim = 0.f;

                                        if (i != custom.vehicle_subtab.size( ) - 1)
                                            SameLine( );
                                    }

                                    EndGroup( );

                                    SetCursorPos(ImVec2(65, 50));
                                    BeginChild("##vehicle", ImVec2(GetWindowWidth( ) - 80, GetWindowHeight( ) - 80));
                                    {

                                        switch (custom.m_vehicle_subtab) {

                                            case 0:
                                                    if (settings::visuals::language) {
                                                        custom.begin_child("Vehicles List", ImVec2(GetWindowWidth() / 2 - GetStyle().ItemSpacing.x / 2, GetWindowHeight()));
                                                        {

                                                            hk_World* World = (hk_World*)*(uint64_t*)(FiveM::World);
                                                            if (!World)
                                                                return;
                                                            hk_Ped* LocalPlayer = World->LocalPlayer();

                                                            if (LocalPlayer) {

                                                                hk_ObjectNavigationPed* Nav = LocalPlayer->ObjectNavigation();
                                                                if (!Nav)
                                                                    return;

                                                                hk_ReplayInterface* ReplayInterface = (hk_ReplayInterface*)*(uint64_t*)(FiveM::ReplayInterface);
                                                                if (!ReplayInterface)
                                                                    return;

                                                                hk_VehicleInterface* VehicleInterface = ReplayInterface->VehicleInterface();
                                                                if (!VehicleInterface)
                                                                    return;

                                                                if (ImGui::ListBoxHeader("    ", ImVec2(220, 300))) {
                                                                    for (int i = 0; i < VehicleInterface->VehicleMaximum(); i++) {

                                                                        hk_Vehicle* Peds = VehicleInterface->VehicleList()->Vehicle(i);
                                                                        if (!Peds)
                                                                            continue;
                                                                        auto playerName = Peds->VehicleModelInfo()->GetCharName();
                                                                        const char* c = playerName;
                                                                        const char* items[] = { c };

                                                                        std::string VehicleName = playerName;
                                                                        VehicleName.append("#");
                                                                        VehicleName.append(std::to_string(i));
                                                                        bool is_selected = (selectedVeh_index == i);
                                                                        if (ImGui::Selectable(VehicleName.c_str(), is_selected)) {

                                                                            selectedVeh_index = i;
                                                                        }
                                                                    }

                                                                    ImGui::ListBoxFooter();
                                                                }

                                                                std::string SelectedString = "Vehicle selected : " + std::to_string(selectedVeh_index);
                                                                ImGui::Text(SelectedString.c_str());
                                                            }
                                                        }
                                                        custom.end_child();
                                                    }
                                                else {
                                                    custom.begin_child("Vehicles List", ImVec2(GetWindowWidth( ) / 2 - GetStyle( ).ItemSpacing.x / 2, GetWindowHeight( )));
                                                    {

                                                        hk_World* World = (hk_World*)*(uint64_t*)(FiveM::World);
                                                        if (!World)
                                                            return;
                                                        hk_Ped* LocalPlayer = World->LocalPlayer( );

                                                        if (LocalPlayer) {

                                                            hk_ObjectNavigationPed* Nav = LocalPlayer->ObjectNavigation( );
                                                            if (!Nav)
                                                                return;

                                                            hk_ReplayInterface* ReplayInterface = (hk_ReplayInterface*)*(uint64_t*)(FiveM::ReplayInterface);
                                                            if (!ReplayInterface)
                                                                return;

                                                            hk_VehicleInterface* VehicleInterface = ReplayInterface->VehicleInterface( );
                                                            if (!VehicleInterface)
                                                                return;

                                                            if (ImGui::ListBoxHeader("    ", ImVec2(220, 300))) {
                                                                for (int i = 0; i < VehicleInterface->VehicleMaximum( ); i++) {

                                                                            hk_Vehicle* Peds = VehicleInterface->VehicleList( )->Vehicle(i);
                                                                            if (!Peds)
                                                                                continue;
                                                                            auto playerName = Peds->VehicleModelInfo( )->GetCharName( );
                                                                            const char* c = playerName;
                                                                            const char* items[] = {c};

                                                                            std::string VehicleName = playerName;
                                                                            VehicleName.append("#");
                                                                            VehicleName.append(std::to_string(i));
                                                                            bool is_selected = (selectedVeh_index == i);
                                                                            if (ImGui::Selectable(VehicleName.c_str( ), is_selected)) {

                                                                                selectedVeh_index = i;
                                                                            }
                                                                }

                                                                ImGui::ListBoxFooter( );
                                                            }

                                                            std::string SelectedString = "Vehicle selected : " + std::to_string(selectedVeh_index);
                                                            ImGui::Text(SelectedString.c_str( ));
                                                        }
                                                    }
                                                    custom.end_child( );
                                                }

                                                SameLine( );
                                                if (settings::visuals::language) {
                                                    custom.begin_child("Vehicles Options", ImVec2(GetWindowWidth( ) / 2 - GetStyle( ).ItemSpacing.x / 2, GetWindowHeight( )));
                                                    {
                                                        hk_World* World = (hk_World*)*(uint64_t*)(FiveM::World);
                                                        if (!World)
                                                            return;
                                                        hk_Ped* LocalPlayer = World->LocalPlayer( );

                                                        if (LocalPlayer) {

                                                            hk_ObjectNavigationPed* Nav = LocalPlayer->ObjectNavigation( );
                                                            if (!Nav)
                                                                return;

                                                            hk_ReplayInterface* ReplayInterface = (hk_ReplayInterface*)*(uint64_t*)(FiveM::ReplayInterface);
                                                            if (!ReplayInterface)
                                                                return;

                                                            hk_VehicleInterface* VehicleInterface = ReplayInterface->VehicleInterface( );
                                                            if (!VehicleInterface)
                                                                return;
                                                            hk_Vehicle* SelectedPed = VehicleInterface->VehicleList( )->Vehicle(selectedVeh_index);

                                                            if (SelectedPed->GetCoordinate( ).x != 0) {
                                                                std::string healthtoshow = "Health Of Vehicle: " + std::to_string(SelectedPed->GetHealth( ));
                                                                ImGui::Text(healthtoshow.c_str( ));
                                                                std::string coordstoshow = "X : " + std::to_string(SelectedPed->GetCoordinate( ).x) + "\n  Y : " + std::to_string(SelectedPed->GetCoordinate( ).y) + "\n  Z : " + std::to_string(SelectedPed->GetCoordinate( ).z);
                                                                ImGui::Text(coordstoshow.c_str( ));
                                                                if (ImGui::Button("Teleport To Vehicle")) {

                                                                            Nav->SetRotation({0, 0, 0, 0});
                                                                            LocalPlayer->SetVelocity( );
                                                                            LocalPlayer->SetCoordinate(SelectedPed->GetCoordinate( ));
                                                                            Nav->SetCoordinate(SelectedPed->GetCoordinate( ));
                                                                }
                                                            } else {
                                                                ImGui::Text("You are not ingame");
                                                            }
                                                        }
                                                    }
                                                    custom.end_child( );
                                                }
                                                    else {
                                                    custom.begin_child("Vehicles Options", ImVec2(GetWindowWidth( ) / 2 - GetStyle( ).ItemSpacing.x / 2, GetWindowHeight( )));
                                                    {
                                                        hk_World* World = (hk_World*)*(uint64_t*)(FiveM::World);
                                                        if (!World)
                                                            return;
                                                        hk_Ped* LocalPlayer = World->LocalPlayer( );

                                                        if (LocalPlayer) {

                                                            hk_ObjectNavigationPed* Nav = LocalPlayer->ObjectNavigation( );
                                                            if (!Nav)
                                                                return;

                                                            hk_ReplayInterface* ReplayInterface = (hk_ReplayInterface*)*(uint64_t*)(FiveM::ReplayInterface);
                                                            if (!ReplayInterface)
                                                                return;

                                                            hk_VehicleInterface* VehicleInterface = ReplayInterface->VehicleInterface( );
                                                            if (!VehicleInterface)
                                                                return;
                                                            hk_Vehicle* SelectedPed = VehicleInterface->VehicleList( )->Vehicle(selectedVeh_index);

                                                            if (SelectedPed->GetCoordinate( ).x != 0) {
                                                                std::string healthtoshow = "Health Of Vehicle: " + std::to_string(SelectedPed->GetHealth( ));
                                                                ImGui::Text(healthtoshow.c_str( ));
                                                                std::string coordstoshow = "X : " + std::to_string(SelectedPed->GetCoordinate( ).x) + "\n  Y : " + std::to_string(SelectedPed->GetCoordinate( ).y) + "\n  Z : " + std::to_string(SelectedPed->GetCoordinate( ).z);
                                                                ImGui::Text(coordstoshow.c_str( ));
                                                                if (ImGui::Button("Teleport To Vehicle")) {

                                                                            Nav->SetRotation({0, 0, 0, 0});
                                                                            LocalPlayer->SetVelocity( );
                                                                            LocalPlayer->SetCoordinate(SelectedPed->GetCoordinate( ));
                                                                            Nav->SetCoordinate(SelectedPed->GetCoordinate( ));
                                                                }
                                                            } else {
                                                                ImGui::Text("You are not ingame");
                                                            }
                                                        }
                                                    }
                                                    custom.end_child( );
                                                    }
                                        }
                                    }
                                    EndChild( );

                                    break;
                                case 7:

                                    SetCursorPos(ImVec2(75, 8));
                                    BeginGroup( );

                                    for (int i = 0; i < custom.misc_subtab.size( ); ++i) {

                                        if (custom.sub_tab(custom.misc_subtab.at(i), custom.m_misc_subtab == i) && custom.m_misc_subtab != i)
                                            custom.m_misc_subtab = i, custom.m_anim = 0.f;

                                        if (i != custom.misc_subtab.size( ) - 1)
                                            SameLine( );
                                    }

                                    EndGroup( );

                                    SetCursorPos(ImVec2(65, 50));
                                    BeginChild("##misc_childs", ImVec2(GetWindowWidth( ) - 80, GetWindowHeight( ) - 80));
                                    {

                                        switch (custom.m_misc_subtab) {

                                            case 0:
                                                if (settings::visuals::language) {
                                                    custom.begin_child("Misc", ImVec2(GetWindowWidth( ) / 2 - GetStyle( ).ItemSpacing.x / 2, GetWindowHeight( )));
                                                    {
                                                        Checkbox(E("Watermark"), &settings::visuals::watermark);

                                                        Checkbox(E("Crosshair"), &settings::visuals::crosshair);
                                                         
                                                        if (settings::visuals::language) {
                                                         
                                                            Checkbox(E("Disable Risk Features"), &settings::visuals::language);
                                                        } else {
                                          
                                                            Checkbox(E("Disable Risk Features"), &settings::visuals::language);
                                                        }

                                                        if (settings::visuals::crosshair) {

                                                            ImGui::Combo(E("Crosshair Style"), &settings::visuals::crossahir_style, settings::visuals::sss, IM_ARRAYSIZE(settings::visuals::sss));
                                                        }

                                                        

                                                        ImGui::Hotkey("Menu Bind :", &Menu::menu_key, ImVec2(150, 20));   

                                                       // ImGui::Hotkey("Menu Atalho :", &Menu::menu_key, ImVec2(150, 20));

                                                        //ImGui::SliderInt(E("Transparencia Do Menu"), &settings::visuals::menu_alpha, 0, 255);

                                                    }
                                                    custom.end_child( );

                                                    SameLine( );

                                                    custom.begin_child("Configs", ImVec2(GetWindowWidth( ) / 2 - GetStyle( ).ItemSpacing.x / 2, GetWindowHeight( )));
                                                    {
                                                            ImGui::Text(E("Configs :"));

                                                            ImGui::ListBox(E("##cfg"), &selected_cfg, cfg, IM_ARRAYSIZE(cfg));

                                                            ImGui::Spacing( );

                                                            if (ImGui::Button(E("Load"))) {

                                                                FILE* p_stream;

                                                                // open file for (extended) reading
                                                                fopen_s(&p_stream, cfg[selected_cfg], "r+");

                                                                // set file pointer to 0
                                                                fseek(p_stream, 0, SEEK_SET);

                                                                // Aimbot
                                                                fread_s(&settings::aimbot::enabled, sizeof(bool), sizeof(bool), 1, p_stream);

                                                                fread_s(&settings::aimbot::aimbot_fov, sizeof(bool), sizeof(bool), 1, p_stream);
                                                                fread_s(&settings::aimbot::smoothhorizontal, sizeof(bool), sizeof(bool), 1, p_stream);
                                                                fread_s(&settings::aimbot::smoothvertical, sizeof(bool), sizeof(bool), 1, p_stream);
                                                                fread_s(&settings::aimbot::aimbot_distance, sizeof(bool), sizeof(bool), 1, p_stream);
                                                                fread_s(&settings::aimbot::aimbot_target, sizeof(bool), sizeof(bool), 1, p_stream);
                                                                fread_s(&settings::aimbot::hotkey, sizeof(bool), sizeof(bool), 1, p_stream);
                                                                fread_s(&settings::aimbot::aimbot_bone, sizeof(bool), sizeof(bool), 1, p_stream);
                                                                fread_s(&settings::aimbot::aimbot_type, sizeof(bool), sizeof(bool), 1, p_stream);
                                                                fread_s(&settings::aimbot::aimbot_selection, sizeof(bool), sizeof(bool), 1, p_stream);

                                                                // Visuals
                                                                fread_s(&settings::visuals::enabled, sizeof(bool), sizeof(bool), 1, p_stream);

                                                                fread_s(&settings::visuals::master_switch, sizeof(bool), sizeof(bool), 1, p_stream);
                                                                fread_s(&settings::visuals::box, sizeof(bool), sizeof(bool), 1, p_stream);
                                                                fread_s(&settings::visuals::box_round, sizeof(bool), sizeof(bool), 1, p_stream);
                                                                fread_s(&settings::visuals::filled, sizeof(bool), sizeof(bool), 1, p_stream);
                                                                fread_s(&settings::visuals::playername, sizeof(bool), sizeof(bool), 1, p_stream);
                                                                fread_s(&settings::visuals::playername, sizeof(bool), sizeof(bool), 1, p_stream);
                                                                fread_s(&settings::visuals::weapon_name, sizeof(bool), sizeof(bool), 1, p_stream);
                                                                fread_s(&settings::visuals::snapline, sizeof(bool), sizeof(bool), 1, p_stream);
                                                                fread_s(&settings::visuals::healthbar, sizeof(bool), sizeof(bool), 1, p_stream);
                                                                fread_s(&settings::visuals::armorbar, sizeof(bool), sizeof(bool), 1, p_stream);
                                                                fread_s(&settings::visuals::dont_draw_distance, sizeof(bool), sizeof(bool), 1, p_stream);
                                                                fread_s(&settings::visuals::circle_at_middle, sizeof(bool), sizeof(bool), 1, p_stream);
                                                                fread_s(&settings::visuals::skeleton, sizeof(bool), sizeof(bool), 1, p_stream);
                                                                fread_s(&settings::visuals::watermark, sizeof(bool), sizeof(bool), 1, p_stream);
                                                                fread_s(&settings::visuals::render_distance, sizeof(bool), sizeof(bool), 1, p_stream);
                                                                fread_s(&settings::visuals::draw_dead, sizeof(bool), sizeof(bool), 1, p_stream);
                                                                fread_s(&settings::visuals::draw_ped, sizeof(bool), sizeof(bool), 1, p_stream);
                                                                fread_s(&settings::visuals::draw_self, sizeof(bool), sizeof(bool), 1, p_stream);
                                                                fread_s(&settings::visuals::preview_target, sizeof(bool), sizeof(bool), 1, p_stream);
                                                                fread_s(&settings::visuals::draw_fov, sizeof(bool), sizeof(bool), 1, p_stream);
                                                                fread_s(&settings::visuals::filled_fov, sizeof(bool), sizeof(bool), 1, p_stream);
                                                                fread_s(&settings::visuals::crosshair, sizeof(bool), sizeof(bool), 1, p_stream);
                                                                fread_s(&settings::visuals::crossahir_style, sizeof(bool), sizeof(bool), 1, p_stream);

                                                                // Vehicle
                                                                fread_s(&settings::vehicle::vehicle_esp, sizeof(bool), sizeof(bool), 1, p_stream);
                                                                fread_s(&settings::vehicle::vehicle_distance, sizeof(bool), sizeof(bool), 1, p_stream);
                                                                fread_s(&settings::vehicle::vehicle_health, sizeof(bool), sizeof(bool), 1, p_stream);
                                                                fread_s(&settings::vehicle::vehicle_snapline, sizeof(bool), sizeof(bool), 1, p_stream);

                                                                // Object
                                                                fread_s(&settings::object::object_distance, sizeof(bool), sizeof(bool), 1, p_stream);
                                                                fread_s(&settings::object::object_esp, sizeof(bool), sizeof(bool), 1, p_stream);
                                                                fread_s(&settings::object::object_snapline, sizeof(bool), sizeof(bool), 1, p_stream);

                                                                // Weapon
                                                                fread_s(&settings::weapon::enabled, sizeof(bool), sizeof(bool), 1, p_stream);

                                                                fread_s(&settings::weapon::no_recoil, sizeof(bool), sizeof(bool), 1, p_stream);
                                                                fread_s(&settings::weapon::no_reload, sizeof(bool), sizeof(bool), 1, p_stream);
                                                                fread_s(&settings::weapon::no_spread, sizeof(bool), sizeof(bool), 1, p_stream);
                                                                fread_s(&settings::weapon::infinite_range, sizeof(bool), sizeof(bool), 1, p_stream);
                                                                fread_s(&settings::weapon::infite_ammo, sizeof(bool), sizeof(bool), 1, p_stream);
                                                                fread_s(&settings::weapon::damage_boost, sizeof(bool), sizeof(bool), 1, p_stream);
                                                                fread_s(&settings::weapon::recoil_value, sizeof(bool), sizeof(bool), 1, p_stream);
                                                                fread_s(&settings::weapon::spread_value, sizeof(bool), sizeof(bool), 1, p_stream);
                                                                fread_s(&settings::weapon::weapon_range, sizeof(bool), sizeof(bool), 1, p_stream);
                                                                fread_s(&settings::weapon::ammo_to_set, sizeof(bool), sizeof(bool), 1, p_stream);
                                                                fread_s(&settings::weapon::int_damage, sizeof(bool), sizeof(bool), 1, p_stream);
                                                                fread_s(&settings::weapon::custom_fov, sizeof(bool), sizeof(bool), 1, p_stream);
                                                                fread_s(&settings::weapon::scope_fov_value, sizeof(bool), sizeof(bool), 1, p_stream);

                                                                fread_s(&font, sizeof(bool), sizeof(bool), 1, p_stream);

                                                                fclose(p_stream);
                                                            }

                                                            ImGui::SameLine( );

                                                            if (ImGui::Button(E("Save"))) {

                                                                FILE* p_stream;

                                                                // open file for (extended) reading
                                                                fopen_s(&p_stream, cfg[selected_cfg], "w+");

                                                                // set file pointer to 0
                                                                fseek(p_stream, 0, SEEK_SET);

                                                                // Aimbot
                                                                fwrite(&settings::aimbot::enabled, sizeof(bool), 1, p_stream);

                                                                fwrite(&settings::aimbot::aimbot_fov, sizeof(bool), 1, p_stream);
                                                                fwrite(&settings::aimbot::smoothhorizontal, sizeof(bool), 1, p_stream);
                                                                fwrite(&settings::aimbot::smoothvertical, sizeof(bool), 1, p_stream);
                                                                fwrite(&settings::aimbot::aimbot_distance, sizeof(bool), 1, p_stream);
                                                                fwrite(&settings::aimbot::aimbot_target, sizeof(bool), 1, p_stream);
                                                                fwrite(&settings::aimbot::hotkey, sizeof(bool), 1, p_stream);
                                                                fwrite(&settings::aimbot::aimbot_bone, sizeof(bool), 1, p_stream);
                                                                fwrite(&settings::aimbot::aimbot_type, sizeof(bool), 1, p_stream);
                                                                fwrite(&settings::aimbot::aimbot_selection, sizeof(bool), 1, p_stream);

                                                                // Visuals
                                                                fwrite(&settings::visuals::enabled, sizeof(bool), 1, p_stream);

                                                                fwrite(&settings::visuals::master_switch, sizeof(bool), 1, p_stream);
                                                                fwrite(&settings::visuals::box, sizeof(bool), 1, p_stream);
                                                                fwrite(&settings::visuals::box_round, sizeof(bool), 1, p_stream);
                                                                fwrite(&settings::visuals::filled, sizeof(bool), 1, p_stream);
                                                                fwrite(&settings::visuals::playername, sizeof(bool), 1, p_stream);
                                                                fwrite(&settings::visuals::playername, sizeof(bool), 1, p_stream);
                                                                fwrite(&settings::visuals::weapon_name, sizeof(bool), 1, p_stream);
                                                                fwrite(&settings::visuals::snapline, sizeof(bool), 1, p_stream);
                                                                fwrite(&settings::visuals::healthbar, sizeof(bool), 1, p_stream);
                                                                fwrite(&settings::visuals::armorbar, sizeof(bool), 1, p_stream);
                                                                fwrite(&settings::visuals::dont_draw_distance, sizeof(bool), 1, p_stream);
                                                                fwrite(&settings::visuals::circle_at_middle, sizeof(bool), 1, p_stream);
                                                                fwrite(&settings::visuals::skeleton, sizeof(bool), 1, p_stream);
                                                                fwrite(&settings::visuals::watermark, sizeof(bool), 1, p_stream);      
                                                                fwrite(&settings::visuals::render_distance, sizeof(bool), 1, p_stream);
                                                                fwrite(&settings::visuals::draw_dead, sizeof(bool), 1, p_stream);
                                                                fwrite(&settings::visuals::draw_ped, sizeof(bool), 1, p_stream);
                                                                fwrite(&settings::visuals::draw_self, sizeof(bool), 1, p_stream);
                                                                fwrite(&settings::visuals::preview_target, sizeof(bool), 1, p_stream);
                                                                fwrite(&settings::visuals::draw_fov, sizeof(bool), 1, p_stream);
                                                                fwrite(&settings::visuals::filled_fov, sizeof(bool), 1, p_stream);
                                                                fwrite(&settings::visuals::crosshair, sizeof(bool), 1, p_stream);
                                                                fwrite(&settings::visuals::crossahir_style, sizeof(bool), 1, p_stream);

                                                                // Vehicle
                                                                fwrite(&settings::vehicle::vehicle_esp, sizeof(bool), 1, p_stream);
                                                                fwrite(&settings::vehicle::vehicle_distance, sizeof(bool), 1, p_stream);
                                                                fwrite(&settings::vehicle::vehicle_health, sizeof(bool), 1, p_stream);
                                                                fwrite(&settings::vehicle::vehicle_snapline, sizeof(bool), 1, p_stream);

                                                                // Object
                                                                fwrite(&settings::object::object_distance, sizeof(bool), 1, p_stream);
                                                                fwrite(&settings::object::object_esp, sizeof(bool), 1, p_stream);
                                                                fwrite(&settings::object::object_snapline, sizeof(bool), 1, p_stream);

                                                                // Weapon
                                                                fwrite(&settings::weapon::enabled, sizeof(bool), 1, p_stream);

                                                                fwrite(&settings::weapon::no_recoil, sizeof(bool), 1, p_stream);
                                                                fwrite(&settings::weapon::no_reload, sizeof(bool), 1, p_stream);
                                                                fwrite(&settings::weapon::no_spread, sizeof(bool), 1, p_stream);
                                                                fwrite(&settings::weapon::infinite_range, sizeof(bool), 1, p_stream);
                                                                fwrite(&settings::weapon::infite_ammo, sizeof(bool), 1, p_stream);
                                                                fwrite(&settings::weapon::damage_boost, sizeof(bool), 1, p_stream);
                                                                fwrite(&settings::weapon::recoil_value, sizeof(bool), 1, p_stream);
                                                                fwrite(&settings::weapon::spread_value, sizeof(bool), 1, p_stream);
                                                                fwrite(&settings::weapon::weapon_range, sizeof(bool), 1, p_stream);
                                                                fwrite(&settings::weapon::ammo_to_set, sizeof(bool), 1, p_stream);
                                                                fwrite(&settings::weapon::int_damage, sizeof(bool), 1, p_stream);
                                                                fwrite(&settings::weapon::custom_fov, sizeof(bool), 1, p_stream);
                                                                fwrite(&settings::weapon::scope_fov_value, sizeof(bool), 1, p_stream);

                                                                fwrite(&font, sizeof(bool), 1, p_stream);

                                                                fclose(p_stream);
                                                            }
                                                    }

                                                    custom.end_child( );
                                                } else {
                                                    custom.begin_child("Misc", ImVec2(GetWindowWidth( ) / 2 - GetStyle( ).ItemSpacing.x / 2, GetWindowHeight( )));
                                                    {

                                                     


                                                        if (ImGui::Button((E("Veax Discord (Join For Updates)")), ImVec2(200, 40)))
                                                        {
                                                            system(E("start https://discord.com/invite/qprCzSqntD"));
                                                        }

                                                        ImGui::Text((E("By Stoned1337 & venzo68 (:")));

                                                        ImGui::Text(E("Have Fun With The best Undetected Free cheat (:"));

                                                        ImGui::Text(E("Last Update:  24.06.2024"));


                                                       /* } custom.end_group_box();*/

                                                        Checkbox(E("Watermark"), &settings::visuals::watermark);

                                                        Checkbox(E("Crosshair"), &settings::visuals::crosshair);
       
                                                        //if (settings::visuals::language) {
                                                        //    Checkbox(E("Disable Risk Features"), &settings::visuals::language);
                                                        //} else {

                                                        //    Checkbox(E("Disable Risk Features"), &settings::visuals::language);
                                                        //}

                                                        if (settings::visuals::crosshair) {

                                                            ImGui::Combo(E("Crosshair Style"), &settings::visuals::crossahir_style, settings::visuals::sss, IM_ARRAYSIZE(settings::visuals::sss));
                                                        }

                                                       

                                                        ImGui::Hotkey("Menu Bind :", &Menu::menu_key, ImVec2(150, 20));

                                                        Text("Menu Color");
                                                        SameLine(GetWindowWidth() - 40);
                                                        if (ColorEdit4("##accent", settings::visuals::accent_color, NO_ALPHA))
                                                            StyleColorsDark();

                                                       // ImGui::SliderInt(E("Menu Alpha"), &settings::visuals::menu_alpha, 0, 255);

                                                    }
                                                    custom.end_child( );

                                                    SameLine( );

                                                    custom.begin_child("Configs", ImVec2(GetWindowWidth( ) / 2 - GetStyle( ).ItemSpacing.x / 2, GetWindowHeight( )));
                                                    {
                                                            ImGui::Text(E("Configs :"));

                                                            ImGui::ListBox(E("##cfg"), &selected_cfg, cfg, IM_ARRAYSIZE(cfg));

                                                            ImGui::Spacing( );

                                                            if (ImGui::Button(E("Load"))) {

                                                                FILE* p_stream;

                                                                // open file for (extended) reading
                                                                fopen_s(&p_stream, cfg[selected_cfg], "r+");

                                                                // set file pointer to 0
                                                                fseek(p_stream, 0, SEEK_SET);

                                                                // Aimbot
                                                                fread_s(&settings::aimbot::enabled, sizeof(bool), sizeof(bool), 1, p_stream);

                                                                fread_s(&settings::aimbot::aimbot_fov, sizeof(bool), sizeof(bool), 1, p_stream);
                                                                fread_s(&settings::aimbot::smoothhorizontal, sizeof(bool), sizeof(bool), 1, p_stream);
                                                                fread_s(&settings::aimbot::smoothvertical, sizeof(bool), sizeof(bool), 1, p_stream);
                                                                fread_s(&settings::aimbot::aimbot_distance, sizeof(bool), sizeof(bool), 1, p_stream);
                                                                fread_s(&settings::aimbot::aimbot_target, sizeof(bool), sizeof(bool), 1, p_stream);
                                                                fread_s(&settings::aimbot::hotkey, sizeof(bool), sizeof(bool), 1, p_stream);
                                                                fread_s(&settings::aimbot::aimbot_bone, sizeof(bool), sizeof(bool), 1, p_stream);
                                                                fread_s(&settings::aimbot::aimbot_type, sizeof(bool), sizeof(bool), 1, p_stream);
                                                                fread_s(&settings::aimbot::aimbot_selection, sizeof(bool), sizeof(bool), 1, p_stream);

                                                                // Visuals
                                                                fread_s(&settings::visuals::enabled, sizeof(bool), sizeof(bool), 1, p_stream);

                                                                fread_s(&settings::visuals::master_switch, sizeof(bool), sizeof(bool), 1, p_stream);
                                                                fread_s(&settings::visuals::box, sizeof(bool), sizeof(bool), 1, p_stream);
                                                                fread_s(&settings::visuals::box_round, sizeof(bool), sizeof(bool), 1, p_stream);
                                                                fread_s(&settings::visuals::filled, sizeof(bool), sizeof(bool), 1, p_stream);
                                                                fread_s(&settings::visuals::playername, sizeof(bool), sizeof(bool), 1, p_stream);
                                                                fread_s(&settings::visuals::playername, sizeof(bool), sizeof(bool), 1, p_stream);
                                                                fread_s(&settings::visuals::weapon_name, sizeof(bool), sizeof(bool), 1, p_stream);
                                                                fread_s(&settings::visuals::snapline, sizeof(bool), sizeof(bool), 1, p_stream);
                                                                fread_s(&settings::visuals::healthbar, sizeof(bool), sizeof(bool), 1, p_stream);
                                                                fread_s(&settings::visuals::armorbar, sizeof(bool), sizeof(bool), 1, p_stream);
                                                                fread_s(&settings::visuals::dont_draw_distance, sizeof(bool), sizeof(bool), 1, p_stream);
                                                                fread_s(&settings::visuals::circle_at_middle, sizeof(bool), sizeof(bool), 1, p_stream);
                                                                fread_s(&settings::visuals::skeleton, sizeof(bool), sizeof(bool), 1, p_stream);
                                                                fread_s(&settings::visuals::watermark, sizeof(bool), sizeof(bool), 1, p_stream);
                                                                fread_s(&settings::visuals::render_distance, sizeof(bool), sizeof(bool), 1, p_stream);
                                                                fread_s(&settings::visuals::draw_dead, sizeof(bool), sizeof(bool), 1, p_stream);
                                                                fread_s(&settings::visuals::draw_ped, sizeof(bool), sizeof(bool), 1, p_stream);
                                                                fread_s(&settings::visuals::draw_self, sizeof(bool), sizeof(bool), 1, p_stream);
                                                                fread_s(&settings::visuals::preview_target, sizeof(bool), sizeof(bool), 1, p_stream);
                                                                fread_s(&settings::visuals::draw_fov, sizeof(bool), sizeof(bool), 1, p_stream);
                                                                fread_s(&settings::visuals::filled_fov, sizeof(bool), sizeof(bool), 1, p_stream);
                                                                fread_s(&settings::visuals::crosshair, sizeof(bool), sizeof(bool), 1, p_stream);
                                                                fread_s(&settings::visuals::crossahir_style, sizeof(bool), sizeof(bool), 1, p_stream);

                                                                // Vehicle
                                                                fread_s(&settings::vehicle::vehicle_esp, sizeof(bool), sizeof(bool), 1, p_stream);
                                                                fread_s(&settings::vehicle::vehicle_distance, sizeof(bool), sizeof(bool), 1, p_stream);
                                                                fread_s(&settings::vehicle::vehicle_health, sizeof(bool), sizeof(bool), 1, p_stream);
                                                                fread_s(&settings::vehicle::vehicle_snapline, sizeof(bool), sizeof(bool), 1, p_stream);

                                                                // Object
                                                                fread_s(&settings::object::object_distance, sizeof(bool), sizeof(bool), 1, p_stream);
                                                                fread_s(&settings::object::object_esp, sizeof(bool), sizeof(bool), 1, p_stream);
                                                                fread_s(&settings::object::object_snapline, sizeof(bool), sizeof(bool), 1, p_stream);

                                                                // Weapon
                                                                fread_s(&settings::weapon::enabled, sizeof(bool), sizeof(bool), 1, p_stream);

                                                                fread_s(&settings::weapon::no_recoil, sizeof(bool), sizeof(bool), 1, p_stream);
                                                                fread_s(&settings::weapon::no_reload, sizeof(bool), sizeof(bool), 1, p_stream);
                                                                fread_s(&settings::weapon::no_spread, sizeof(bool), sizeof(bool), 1, p_stream);
                                                                fread_s(&settings::weapon::infinite_range, sizeof(bool), sizeof(bool), 1, p_stream);
                                                                fread_s(&settings::weapon::infite_ammo, sizeof(bool), sizeof(bool), 1, p_stream);
                                                                fread_s(&settings::weapon::damage_boost, sizeof(bool), sizeof(bool), 1, p_stream);
                                                                fread_s(&settings::weapon::recoil_value, sizeof(bool), sizeof(bool), 1, p_stream);
                                                                fread_s(&settings::weapon::spread_value, sizeof(bool), sizeof(bool), 1, p_stream);
                                                                fread_s(&settings::weapon::weapon_range, sizeof(bool), sizeof(bool), 1, p_stream);
                                                                fread_s(&settings::weapon::ammo_to_set, sizeof(bool), sizeof(bool), 1, p_stream);
                                                                fread_s(&settings::weapon::int_damage, sizeof(bool), sizeof(bool), 1, p_stream);
                                                                fread_s(&settings::weapon::custom_fov, sizeof(bool), sizeof(bool), 1, p_stream);
                                                                fread_s(&settings::weapon::scope_fov_value, sizeof(bool), sizeof(bool), 1, p_stream);

                                                                fread_s(&font, sizeof(bool), sizeof(bool), 1, p_stream);

                                                                fclose(p_stream);
                                                            }

                                                            ImGui::SameLine( );

                                                            if (ImGui::Button(E("Save"))) {

                                                                FILE* p_stream;

                                                                // open file for (extended) reading
                                                                fopen_s(&p_stream, cfg[selected_cfg], "w+");

                                                                // set file pointer to 0
                                                                fseek(p_stream, 0, SEEK_SET);

                                                                // Aimbot
                                                                fwrite(&settings::aimbot::enabled, sizeof(bool), 1, p_stream);

                                                                fwrite(&settings::aimbot::aimbot_fov, sizeof(bool), 1, p_stream);
                                                                fwrite(&settings::aimbot::smoothhorizontal, sizeof(bool), 1, p_stream);
                                                                fwrite(&settings::aimbot::smoothvertical, sizeof(bool), 1, p_stream);
                                                                fwrite(&settings::aimbot::aimbot_distance, sizeof(bool), 1, p_stream);
                                                                fwrite(&settings::aimbot::aimbot_target, sizeof(bool), 1, p_stream);
                                                                fwrite(&settings::aimbot::hotkey, sizeof(bool), 1, p_stream);
                                                                fwrite(&settings::aimbot::aimbot_bone, sizeof(bool), 1, p_stream);
                                                                fwrite(&settings::aimbot::aimbot_type, sizeof(bool), 1, p_stream);
                                                                fwrite(&settings::aimbot::aimbot_selection, sizeof(bool), 1, p_stream);

                                                                // Visuals
                                                                fwrite(&settings::visuals::enabled, sizeof(bool), 1, p_stream);

                                                                fwrite(&settings::visuals::master_switch, sizeof(bool), 1, p_stream);
                                                                fwrite(&settings::visuals::box, sizeof(bool), 1, p_stream);
                                                                fwrite(&settings::visuals::box_round, sizeof(bool), 1, p_stream);
                                                                fwrite(&settings::visuals::filled, sizeof(bool), 1, p_stream);
                                                                fwrite(&settings::visuals::playername, sizeof(bool), 1, p_stream);
                                                                fwrite(&settings::visuals::playername, sizeof(bool), 1, p_stream);
                                                                fwrite(&settings::visuals::weapon_name, sizeof(bool), 1, p_stream);
                                                                fwrite(&settings::visuals::snapline, sizeof(bool), 1, p_stream);
                                                                fwrite(&settings::visuals::healthbar, sizeof(bool), 1, p_stream);
                                                                fwrite(&settings::visuals::armorbar, sizeof(bool), 1, p_stream);
                                                                fwrite(&settings::visuals::dont_draw_distance, sizeof(bool), 1, p_stream);
                                                                fwrite(&settings::visuals::circle_at_middle, sizeof(bool), 1, p_stream);
                                                                fwrite(&settings::visuals::skeleton, sizeof(bool), 1, p_stream);
                                                                fwrite(&settings::visuals::watermark, sizeof(bool), 1, p_stream);
                                                                fwrite(&settings::visuals::render_distance, sizeof(bool), 1, p_stream);
                                                                fwrite(&settings::visuals::draw_dead, sizeof(bool), 1, p_stream);
                                                                fwrite(&settings::visuals::draw_ped, sizeof(bool), 1, p_stream);
                                                                fwrite(&settings::visuals::draw_self, sizeof(bool), 1, p_stream);
                                                                fwrite(&settings::visuals::preview_target, sizeof(bool), 1, p_stream);
                                                                fwrite(&settings::visuals::draw_fov, sizeof(bool), 1, p_stream);
                                                                fwrite(&settings::visuals::filled_fov, sizeof(bool), 1, p_stream);
                                                                fwrite(&settings::visuals::crosshair, sizeof(bool), 1, p_stream);
                                                                fwrite(&settings::visuals::crossahir_style, sizeof(bool), 1, p_stream);

                                                                // Vehicle
                                                                fwrite(&settings::vehicle::vehicle_esp, sizeof(bool), 1, p_stream);
                                                                fwrite(&settings::vehicle::vehicle_distance, sizeof(bool), 1, p_stream);
                                                                fwrite(&settings::vehicle::vehicle_health, sizeof(bool), 1, p_stream);
                                                                fwrite(&settings::vehicle::vehicle_snapline, sizeof(bool), 1, p_stream);

                                                                // Object
                                                                fwrite(&settings::object::object_distance, sizeof(bool), 1, p_stream);
                                                                fwrite(&settings::object::object_esp, sizeof(bool), 1, p_stream);
                                                                fwrite(&settings::object::object_snapline, sizeof(bool), 1, p_stream);

                                                                // Weapon
                                                                fwrite(&settings::weapon::enabled, sizeof(bool), 1, p_stream);

                                                                fwrite(&settings::weapon::no_recoil, sizeof(bool), 1, p_stream);
                                                                fwrite(&settings::weapon::no_reload, sizeof(bool), 1, p_stream);
                                                                fwrite(&settings::weapon::no_spread, sizeof(bool), 1, p_stream);
                                                                fwrite(&settings::weapon::infinite_range, sizeof(bool), 1, p_stream);
                                                                fwrite(&settings::weapon::infite_ammo, sizeof(bool), 1, p_stream);
                                                                fwrite(&settings::weapon::damage_boost, sizeof(bool), 1, p_stream);
                                                                fwrite(&settings::weapon::recoil_value, sizeof(bool), 1, p_stream);
                                                                fwrite(&settings::weapon::spread_value, sizeof(bool), 1, p_stream);
                                                                fwrite(&settings::weapon::weapon_range, sizeof(bool), 1, p_stream);
                                                                fwrite(&settings::weapon::ammo_to_set, sizeof(bool), 1, p_stream);
                                                                fwrite(&settings::weapon::int_damage, sizeof(bool), 1, p_stream);
                                                                fwrite(&settings::weapon::custom_fov, sizeof(bool), 1, p_stream);
                                                                fwrite(&settings::weapon::scope_fov_value, sizeof(bool), 1, p_stream);

                                                                fwrite(&font, sizeof(bool), 1, p_stream);

                                                                fclose(p_stream);
                                                            }
                                                    }

                                                    custom.end_child( );
                                                }


                                                break;
                                        }
                                    }
                                    EndChild( );

                                    break;
                            }

                            SetCursorPosY(0);
                            custom.tab_area("##tab_area", ImVec2(50, size.y - 20), []( ) {
                                for (int i = 0; i < custom.tabs.size( ); ++i)
                                    if (custom.tab(custom.tabs_icons.at(i), custom.tabs.at(i), custom.m_tab == i) && custom.m_tab != i)
                                        custom.m_tab = i, custom.m_anim = 0.f;
                            });
                    

                            // footer                    
                       
                            
                      
                            

                        }
                        ImGui::End( );

                    } else {

                        exit(3);
                    }
                }
            }
        }
     }
