#pragma once

#include "../content/classes.hpp"
#include "../settings/settings.hpp"



void WindowFillRect(float x, float y, float w, float h, ImColor color, int rounding) {
    ImGui::GetWindowDrawList()->AddRectFilled(ImVec2(x, y), ImVec2(x + w, y + h), color, rounding);
}





void DrawOutlinedTextG(const char* text, const ImVec2& position, const ImVec4& color, float outlineThickness = 1.0f) {
    ImGuiWindow* window = ImGui::GetCurrentWindow();
    ImDrawList* drawList = window->DrawList;

    ImFont* font = ImGui::GetFont();
    float fontSize = ImGui::GetFontSize();

    // Calculate the outline color
    ImU32 outlineColor = ImGui::ColorConvertFloat4ToU32(ImVec4(0, 0, 0, color.w));

    // Draw the outline
    drawList->AddText(font, fontSize, position + ImVec2(outlineThickness, outlineThickness), outlineColor, text);
    drawList->AddText(font, fontSize, position + ImVec2(-outlineThickness, -outlineThickness), outlineColor, text);
    drawList->AddText(font, fontSize, position + ImVec2(outlineThickness, -outlineThickness), outlineColor, text);
    drawList->AddText(font, fontSize, position + ImVec2(-outlineThickness, outlineThickness), outlineColor, text);

    // Draw the text
    ImU32 textColor = ImGui::ColorConvertFloat4ToU32(color);
    drawList->AddText(font, fontSize, position, textColor, text);
}


ImVec2 WorldToRadar(Vector3 Location, FLOAT RadarX, FLOAT RadarY, float RadarSize, float RadarZoom)
{

    hk_World* World = (hk_World*)*(uint64_t*)(FiveM::World);


    hk_Ped* LocalPlayer = World->LocalPlayer();

    Vector3 localpos = LocalPlayer->m_visual_pos();
    ImVec2 Coord(0, 0);
    ImVec2 Return(0, 0);
    DWORD64 addr = FiveM::GetCameraViewAngles();

    float rot = 0.0f;
    //0x03D0
    Vector3 TPS = *(Vector3*)(addr + 0x03D0);
    rot = acosf(TPS.x) * 180.0f / DirectX::XM_PI;
    if (asinf(TPS.y) * 180.0f / DirectX::XM_PI < 0.0f) rot *= -1.0f;

    ImVec2 forwardVec(0, 0);
    float ForwardDirection = DirectX::XMConvertToRadians(rot);
    forwardVec.x = cos(ForwardDirection);
    forwardVec.y = sin(ForwardDirection);

    FLOAT CosYaw = forwardVec.x;
    FLOAT SinYaw = forwardVec.y;

    FLOAT DeltaX = Location.x - localpos.x;
    FLOAT DeltaY = Location.y - localpos.y;

    FLOAT LocationX = (DeltaY * CosYaw - DeltaX * SinYaw) / RadarZoom;
    FLOAT LocationY = (DeltaX * CosYaw + DeltaY * SinYaw) / RadarZoom;

    if (LocationX > RadarSize / 2.0f - 2.5f)
        LocationX = RadarSize / 2.0f - 2.5f;
    else if (LocationX < -(RadarSize / 2.0f - 2.5f))
        LocationX = -(RadarSize / 2.0f - 2.5f);

    if (LocationY > RadarSize / 2.0f - 2.5f)
        LocationY = RadarSize / 2.0f - 2.5f;
    else if (LocationY < -(RadarSize / 2.0f - 2.5f))
        LocationY = -(RadarSize / 2.0f - 2.5f);

    Return.x = -LocationX + RadarX;
    Return.y = -LocationY + RadarY;

    return Return;
}


namespace visuals {

    // :SKULL: JZ TESTE


    void player_esp(void) {


        if (settings::SilentAim::Enabled) {

            if (settings::SilentAim::DrawFoV) {



                ImGui::GetBackgroundDrawList()->AddCircle(ImVec2(FiveM::WindowSize.x / 2, FiveM::WindowSize.y / 2), settings::SilentAim::MaxFoV * 10, settings::SilentAim::SilentFoVColor, 100.0f);

            }
        }



        if (settings::visuals::enabled) {

            hk_World* World = (hk_World*)*(uint64_t*)(FiveM::World);
            if (!World)
                return;

            hk_Ped* LocalPlayer = World->LocalPlayer();
            if (!LocalPlayer)
                return;

            hk_ReplayInterface* ReplayInterface = (hk_ReplayInterface*)*(uint64_t*)(FiveM::ReplayInterface);
            if (!ReplayInterface)
                return;

            hk_PedInterface* PedInterface = ReplayInterface->PedInterface();
            if (!PedInterface)
                return;

            ImVec2 LocalPlayer_Location = FiveM::WorldToScreen(LocalPlayer->GetCoordinate());

            for (int index = 0; index < PedInterface->PedMaximum(); index++) {

                hk_Ped* Peds = PedInterface->PedList()->Ped(index);
                if (!Peds)
                    continue;

                if (LocalPlayer->GetHealth() <= 100)
                    continue;

                if (!settings::visuals::draw_dead) {
                    if (Peds->GetHealth() <= 0)
                        continue;
                }

                bool IsPed = false;

                auto ped_type = Peds->GetPedType();
                if (!ped_type) {
                    continue;
                }
                ped_type = ped_type << 11 >> 25;

                if (ped_type != 2) {
                    IsPed = true;
                }

                if (!settings::visuals::draw_ped) {

                    if (IsPed) {
                        continue;
                    }
                }

                if (!settings::visuals::draw_self) {

                    if (Peds == LocalPlayer)
                        continue;
                }

                ImVec2 Ped_Location = FiveM::WorldToScreen(Peds->GetCoordinate());
                if (!IsOnScreen(Ped_Location))
                    continue;

                Vector3 DistanceCalculation = (LocalPlayer->GetCoordinate() - Peds->GetCoordinate());

                double Distance = sqrtf(DistanceCalculation.x * DistanceCalculation.x + DistanceCalculation.y * DistanceCalculation.y + DistanceCalculation.z * DistanceCalculation.z);

                if (Distance < settings::visuals::render_distance) {

                    Vector3 mins = Peds->PedModelInfo()->GetMin();
                    Vector3 maxs = Peds->PedModelInfo()->GetMax();

                    Vector3 min = (Peds->GetCoordinate() + mins);
                    Vector3 max = (Peds->GetCoordinate() + maxs);
                    Vector3 PedPosVec3 = *(Vector3*)(Peds + 0x90);
                    Vector3 LocalPosVec3 = *(Vector3*)(LocalPlayer + 0x90);
                    ImVec2 PedPos = FiveM::WorldToScreen(PedPosVec3);
                    ImVec2 LocalPos = FiveM::WorldToScreen(LocalPosVec3);
                    Vector3 HeadPos(PedPosVec3.x, PedPosVec3.y, PedPosVec3.z + 0.8f);
                    Vector3 FootPos(PedPosVec3.x, PedPosVec3.y, PedPosVec3.z - 0.95f);
                    Vector3 FootPos1(PedPosVec3.x, PedPosVec3.y, PedPosVec3.z - 0.96f);
                    Vector3 FootPos2(PedPosVec3.x, PedPosVec3.y, PedPosVec3.z - 0.97f);

                    ImVec2 Head = FiveM::WorldToScreen(HeadPos);
                    ImVec2 Foot = FiveM::WorldToScreen(FootPos);
                    ImVec2 Foot1 = FiveM::WorldToScreen(FootPos1);
                    ImVec2 Foot2 = FiveM::WorldToScreen(FootPos2);
                    float h = Head.y - Foot.y;
                    float w = (Foot.y - Head.y) / 4;
                    Vector3 points[] = {
                        Vector3(min.x, min.y, min.z),
                        Vector3(min.x, max.y, min.z),
                        Vector3(max.x, max.y, min.z),
                        Vector3(max.x, min.y, min.z),
                        Vector3(max.x, max.y, max.z),
                        Vector3(min.x, max.y, max.z),
                        Vector3(min.x, min.y, max.z),
                        Vector3(max.x, min.y, max.z) };

                    ImVec2 w2s_points[8];
                    auto index = 0;
                    for (Vector3 point : points) {
                        w2s_points[index] = FiveM::WorldToScreen(point);
                        if (w2s_points[index].x == 0.f && w2s_points[index].y == 0.f)
                            continue;
                        index++;
                    }

                    float x = w2s_points[0].x;
                    float y = w2s_points[0].y;
                    float width = w2s_points[0].x;
                    float height = w2s_points[0].y;
                    for (auto point : w2s_points) {
                        if (x > point.x)
                            x = point.x;
                        if (width < point.x)
                            width = point.x;
                        if (y > point.y)
                            y = point.y;
                        if (height < point.y)
                            height = point.y;
                    }

                    int out_x = x;
                    int out_y = y;
                    int out_w = width - x;
                    int out_h = height - y;
                    float Height = out_h / 1.05;
                    float Width = out_w;
                    float Widthx = 10.f;
                    float HealthDistance = Width / 16;
                    float health = *(float*)(Peds + 0x280) - 100;
                    float Withdas = (Height / 1.90f);
                    // HEALTHDISTANCE//
                    float HealthDistance1 = (Height / 1.90f);
                    float HealthDistance2 = (Height / 1.70f);
                    float HealthDistance3 = (Height / 1.10f);
                    // ARMORDISTANCE//
                    float ArmorDistance1 = (Height / 1.80f);
                    float ArmorDistance2 = (Height / 1.50f);
                    float ArmorDistance3 = (Height / 0.90f);

                    Width = (Height / (1.80f + 2 / 100));
                    if (((Ped_Location.x + Width / 2) - (Ped_Location.x - Width / 2)) > 300)
                        continue;


                    if (settings::visuals::box)
                    {
                        ImGui::GetBackgroundDrawList()->AddRect(ImVec2(Ped_Location.x - Width / 2, Ped_Location.y - Height / 2.5), ImVec2(Ped_Location.x + Width / 2.5, Ped_Location.y + Height / 2.5), settings::visuals::boxcolor);

                    }

                    if (settings::radar::enable2DRadar)
                    {
                        hk_World* World = (hk_World*)*(uint64_t*)(FiveM::World);
                        if (!World)
                            return;
                        hk_Ped* LocalPlayer = World->LocalPlayer();


                        hk_ReplayInterface* ReplayInterface = (hk_ReplayInterface*)*(uint64_t*)(FiveM::ReplayInterface);
                        if (!ReplayInterface)
                            return;

                        hk_PedInterface* PedInterface = ReplayInterface->PedInterface();
                        if (!PedInterface)
                            return;

                        for (int i = 0; i < PedInterface->PedMaximum(); i++)
                        {
                            hk_Ped* cPed = PedInterface->PedList()->Ped(i);
                            if (!cPed) continue;

                            hk_World* g_pedFactory = (hk_World*)*(uintptr_t*)(FiveM::World);
                            float health = *(float*)(cPed + 0x0280) - 100;


                            if (cPed == g_pedFactory->LocalPlayer())
                                continue;

                            ImGui::PushStyleColor(ImGuiCol_WindowBg, (ImVec4)ImColor(30, 32, 48, 140));
                            {
                                ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
                                {
                                    ImGui::Begin("Radar", &settings::radar::enable2DRadar, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse);
                                    {
                                        ImDrawList* draw_list = ImGui::GetWindowDrawList();

                                        ImVec2 winpos = ImGui::GetWindowPos();
                                        ImVec2 winsize = ImGui::GetWindowSize();

                                        draw_list->AddLine(ImVec2(winpos.x + winsize.x * 0.5f, winpos.y), ImVec2(winpos.x + winsize.x * 0.5f, winpos.y + winsize.y), ImColor(30, 32, 48), 1.f);
                                        draw_list->AddLine(ImVec2(winpos.x, winpos.y + winsize.y * 0.5f), ImVec2(winpos.x + winsize.x, winpos.y + winsize.y * 0.5f), ImColor(30, 32, 48), 1.f);

                                        float RadarZoom = (4 * 1.0f) * 0.2f;

                                        ImVec2 Radar_Enemy_Pos = { ImGui::GetWindowPos().x, ImGui::GetWindowPos().y };
                                        Radar_Enemy_Pos = WorldToRadar(cPed->m_visual_pos(), Radar_Enemy_Pos.x, Radar_Enemy_Pos.y, ImGui::GetWindowSize().x, RadarZoom);

                                        WindowFillRect(Radar_Enemy_Pos.x + ImGui::GetWindowSize().x / 2, Radar_Enemy_Pos.y + ImGui::GetWindowSize().y / 2, 4, 4, ImColor(255, 255, 255), 0);
                                        //DrawHealthBar(Radar_Enemy_Pos.x - 10.0f, Radar_Enemy_Pos.y + 5.0f, 4, 4, health, 100, 0, ImColor(0, 255, 0), 1);
                                        //draw_list->AddRectFilled(ImVec2(Radar_Ped_Pos.x + pos.x / 2, t3rm_Ped_Pos.y + pos.y / 2), 3, ImColor(255, 0, 0), 100);

                                    }
                                    ImGui::End();
                                }
                                ImGui::PopStyleVar();
                            }
                            ImGui::PopStyleColor();

                        }
                    }

                    bool IsOnGlife = false;

                    if (settings::visuals::healthbar) {



                        ImColor HealthColor;
                        if (health > 75) {
                            HealthColor = ImColor(111, 203, 48);
                        }
                        else if (health < 75 && health > 50) {
                            HealthColor = ImColor(172, 187, 47);
                        }
                        else if (health < 50 && health > 25) {
                            HealthColor = ImColor(255, 140, 0);
                        }
                        else if (health < 25 && health > 15) {
                            HealthColor = ImColor(255, 69, 0);
                        }
                        else if (health < 15 && health > 0)
                        {
                            HealthColor = ImColor(255, 0, 0);
                        }

                        if (health > 0)
                        {
                            int healthsize1 = 5;
                            float cur_w = w + w;
                            float H_HEALTH = (h / 100) * health;
                            float W_HEALTH = (cur_w / 100) * health;
                            if (settings::visuals::healthbar_style == 0)
                            {
                                if (false) {
                                    DrawBorder(Head.x - w - 6 - 1, Foot.y - 1, 2 + 1, h + 1, 1, ImColor(0, 0, 0));
                                    FillRect(Head.x - w - 6, Foot.y, 2, h, ImColor(0, 0, 0));
                                    /*FillRectGradient(Head.x - w - 6, Foot.y, 2, H_HEALTH, ImColor(255, 0, 0), ImColor(255, 140, 0), ImColor(172, 187, 47), ImColor(111, 203, 48));*/
                                }
                                else {
                                    DrawBorder(Head.x - w - 6 - 1, Foot.y - 1, 2 + 1, h + 1, 1, ImColor(0, 0, 0));
                                    FillRect(Head.x - w - 6, Foot.y, 2, h, ImColor(0, 0, 0));
                                    FillRect(Head.x - w - 6, Foot.y, 2, H_HEALTH, HealthColor);
                                }
                            }
                            else if (settings::visuals::healthbar_style == 1)
                            {
                                DrawBorder(Foot1.x - w - 1, Foot1.y + 3 - 1, cur_w + 1, 2.5f + 1, 1, ImColor(0, 0, 0));
                                FillRect(Foot1.x - w, Foot1.y + 3, cur_w, 2.5f, ImColor(0, 0, 0), 0);
                                FillRect(Foot1.x - w, Foot1.y + 3, W_HEALTH, 2.5f, HealthColor, 0);
                            }
                            else if (settings::visuals::healthbar_style == 2)
                            {
                                DrawBorder(Foot1.x - w - 1, Head.y - healthsize1 - 1, cur_w + 1, 2 + 1, 1, ImColor(0, 0, 0));
                                FillRect(Foot1.x - w, Head.y - healthsize1, cur_w, 2, ImColor(0, 0, 0), 0);
                                FillRect(Foot1.x - w, Head.y - healthsize1, W_HEALTH, 2, HealthColor);
                            }

                        }
                    }

                    if (settings::visuals::armorbar) {
                        float cur_w = w + w;
                        int healthsize = 8;
                        ImColor col = ImColor(10, 145, 255, 255);
                        ImColor coll = ImColor(0, 0, 0, 255);
                        float armorped = *(float*)(Peds + FiveM::Armor); // wait 2sec
                        float armorPercentage = Height / 100 * (armorped);
                        float W_HEALTH = (cur_w / 100) * armorped;
                        if (settings::visuals::healthbar_style == 0)
                        {
                            DrawBorder(Head.x - w - 11 - 1, Foot.y - 1, 2 + 1, h + 1, 1, ImColor(0, 0, 0));
                            FillRect(Head.x - w - 11, Foot.y, 2, h, ImColor(0, 0, 0));
                            FillRect(Head.x - w - 11, Foot.y, 2, h, ImColor(72, 108, 168));
                        }
                        else if (settings::visuals::healthbar_style == 1)
                        {
                            DrawBorder(Foot2.x - w - 1, Foot2.y + healthsize - 1, cur_w + 1, 2.5f + 1, 1, ImColor(0, 0, 0));
                            FillRect(Foot2.x - w, Foot2.y + healthsize, cur_w, 2.5f, ImColor(0, 0, 0), 0);
                            FillRect(Foot2.x - w, Foot2.y + healthsize, W_HEALTH, 2.5f, ImColor(72, 108, 168), 0);
                        }
                        else if (settings::visuals::healthbar_style == 2)
                        {
                            DrawBorder(Foot2.x - w - 1, Head.y - (healthsize + 1) - 1, cur_w + 1, 2 + 1, 1, ImColor(0, 0, 0));
                            FillRect(Foot2.x - w, Head.y - (healthsize + 1), cur_w, 2, ImColor(0, 0, 0), 0);
                            FillRect(Foot2.x - w, Head.y - (healthsize + 1), W_HEALTH, 2, ImColor(72, 108, 168), 0);
                        }

                    }

                    if (settings::visuals::circle_at_middle) {
                        ImVec2 Head;
                        Head = FiveM::GetBoneArrayW2SPos(reinterpret_cast<uint64_t>(Peds), 0);

                        if (Distance < 20) {
                            ImGui::GetBackgroundDrawList()->AddCircle(ImVec2(Head.x, Head.y), 6, settings::visuals::head_color, 100, 1.0f);
                        }
                        else if (Distance < 30) {
                            ImGui::GetBackgroundDrawList()->AddCircle(ImVec2(Head.x, Head.y), 4, settings::visuals::head_color, 100, 1.0f);
                        }
                        else if (Distance < 50) {
                            ImGui::GetBackgroundDrawList()->AddCircle(ImVec2(Head.x, Head.y), 3, settings::visuals::head_color, 100, 1.0f);
                        }
                        else if (Distance < 100) {
                            ImGui::GetBackgroundDrawList()->AddCircle(ImVec2(Head.x, Head.y), 3, settings::visuals::head_color, 100, 1.0f);
                        }
                        else if (Distance < 200) {
                            ImGui::GetBackgroundDrawList()->AddCircle(ImVec2(Head.x, Head.y), 3, settings::visuals::head_color, 100, 1.0f);
                        }
                    }

                    if (settings::visuals::dont_draw_distance) {
                        int Ddistance = sqrtf(DistanceCalculation.x * DistanceCalculation.x + DistanceCalculation.y * DistanceCalculation.y + DistanceCalculation.z * DistanceCalculation.z);
                        std::string str = "" + std::to_string(Ddistance) + "m";
                        if (settings::visuals::weapon_name) {
                            if (settings::visuals::OutlinedText == 0)
                                DrawOutlinedText(ImGui::GetFont(), str, ImVec2(Ped_Location.x, Ped_Location.y + Height / 1.5 + 11), 3.0f, settings::visuals::distance_color, true);

                        }
                        else {


                            DrawOutlinedText(ImGui::GetFont(), str, ImVec2(Ped_Location.x, Ped_Location.y + Height / 1.5 + 0), 3.0f, settings::visuals::distance_color, true);
                        }
                    }

                    if (settings::visuals::weapon_name) {

                        hk_WeaponManager* PedsWeaponManager = Peds->WeaponManager();
                        if (!PedsWeaponManager)
                            continue;
                        hk_WeaponInfo* PedsWeaponInfo = PedsWeaponManager->WeaponInfo();
                        if (!PedsWeaponInfo)
                            continue;

                        uint64_t Hash = PedsWeaponInfo->GetHash();

                        if (settings::visuals::dont_draw_distance) {

                            DrawOutlinedText(ImGui::GetFont(), get_weapon_name(Hash), ImVec2(Ped_Location.x, Ped_Location.y + Height / 2 + 20), 3.f, settings::visuals::weapon_color, true);


                        }
                        else {
                            DrawOutlinedText(ImGui::GetFont(), get_weapon_name(Hash), ImVec2(Ped_Location.x, Ped_Location.y + Height / 2 + 20), 3.f, settings::visuals::weapon_color, true);
                        }
                    }
                    if (settings::visuals::playername) {

                        void* m_net_player_data = (void*)(Peds->PlayerInfo() + 0x20);
                        if (settings::visuals::healthbar == true)
                        {
                            DrawOutlinedText(ImGui::GetFont(), FiveM::getpname(m_net_player_data), ImVec2(Ped_Location.x, Ped_Location.y + Height / 2 + 2), 3.0f, settings::visuals::namecolor, true);
                        }
                        else
                        {
                            DrawOutlinedText(ImGui::GetFont(), FiveM::getpname(m_net_player_data), ImVec2(Ped_Location.x, Ped_Location.y + Height / 2 + 1), 3.0f, settings::visuals::namecolor, true);
                        }
                    }


                    if (settings::visuals::skeleton)
                        if (settings::visuals::skeleton) {
                            ImVec2 Pelvis = FiveM::GetBonePosW2S(reinterpret_cast<uint64_t>(Peds), 0x2e28);
                            if (!IsOnScreen(Pelvis)) continue;

                            ImVec2 Neck = FiveM::GetBonePosW2S(reinterpret_cast<uint64_t>(Peds), 0x9995);
                            if (!IsOnScreen(Neck)) continue;

                            ImVec2 Clavicle_Left = FiveM::GetBonePosW2S(reinterpret_cast<uint64_t>(Peds), 0xFCD9);
                            if (!IsOnScreen(Clavicle_Left)) continue;

                            ImVec2 Clavicle_Right = FiveM::GetBonePosW2S(reinterpret_cast<uint64_t>(Peds), 0x29D2);
                            if (!IsOnScreen(Clavicle_Right)) continue;

                            ImVec2 Uperarm_left = FiveM::GetBonePosW2S(reinterpret_cast<uint64_t>(Peds), 0xB1C5);
                            if (!IsOnScreen(Uperarm_left)) continue;

                            ImVec2 Uperarm_right = FiveM::GetBonePosW2S(reinterpret_cast<uint64_t>(Peds), 0x9D4D);
                            if (!IsOnScreen(Uperarm_right)) continue;

                            ImVec2 Elbow_l = FiveM::GetBonePosW2S(reinterpret_cast<uint64_t>(Peds), 0x58B7);
                            if (!IsOnScreen(Elbow_l)) continue;

                            ImVec2 Elbow_r = FiveM::GetBonePosW2S(reinterpret_cast<uint64_t>(Peds), 0xBB0);
                            if (!IsOnScreen(Elbow_r)) continue;

                            ImVec2 R_FormArm = FiveM::GetBonePosW2S(reinterpret_cast<uint64_t>(Peds), 0x6E5C);
                            if (!IsOnScreen(R_FormArm)) continue;

                            ImVec2 L_FormArm = FiveM::GetBonePosW2S(reinterpret_cast<uint64_t>(Peds), 0xEEEB);
                            if (!IsOnScreen(L_FormArm)) continue;

                            ImVec2 SKEL_R_Hand = FiveM::GetBonePosW2S(reinterpret_cast<uint64_t>(Peds), 0xDEAD);
                            if (!IsOnScreen(SKEL_R_Hand)) continue;

                            ImVec2 SKEL_L_Hand = FiveM::GetBonePosW2S(reinterpret_cast<uint64_t>(Peds), 0x49D9);
                            if (!IsOnScreen(SKEL_L_Hand)) continue;


                            ImVec2 SKEL_L_Thigh = FiveM::GetBonePosW2S(reinterpret_cast<uint64_t>(Peds), 0xE39F);
                            if (!IsOnScreen(SKEL_L_Thigh)) continue;

                            ImVec2 SKEL_L_Calf = FiveM::GetBonePosW2S(reinterpret_cast<uint64_t>(Peds), 0xF9BB);
                            if (!IsOnScreen(SKEL_L_Calf)) continue;

                            ImVec2 SKEL_R_Thigh = FiveM::GetBonePosW2S(reinterpret_cast<uint64_t>(Peds), 0xCA72);
                            if (!IsOnScreen(SKEL_R_Thigh)) continue;

                            ImVec2 SKEL_R_Calf = FiveM::GetBonePosW2S(reinterpret_cast<uint64_t>(Peds), 0x9000);
                            if (!IsOnScreen(SKEL_R_Calf)) continue;

                            ImVec2 SKEL_L_Foot = FiveM::GetBonePosW2S(reinterpret_cast<uint64_t>(Peds), 0x3779);
                            if (!IsOnScreen(SKEL_L_Foot)) continue;

                            ImVec2 SKEL_R_Foot = FiveM::GetBonePosW2S(reinterpret_cast<uint64_t>(Peds), 0xCC4D);
                            if (!IsOnScreen(SKEL_R_Foot)) continue;



                            ImGui::GetBackgroundDrawList()->AddLine(Neck, Clavicle_Right, settings::visuals::skeleton_color, 1.f);
                            ImGui::GetBackgroundDrawList()->AddLine(Neck, Clavicle_Left, settings::visuals::skeleton_color, 1.f);

                            ImGui::GetBackgroundDrawList()->AddLine(Clavicle_Right, Uperarm_right, settings::visuals::skeleton_color, 1.f);
                            ImGui::GetBackgroundDrawList()->AddLine(Clavicle_Left, Uperarm_left, settings::visuals::skeleton_color, 1.f);

                            ImGui::GetBackgroundDrawList()->AddLine(Uperarm_right, Elbow_r, settings::visuals::skeleton_color, 1.f);
                            ImGui::GetBackgroundDrawList()->AddLine(Uperarm_left, Elbow_l, settings::visuals::skeleton_color, 1.f);

                            ImGui::GetBackgroundDrawList()->AddLine(Elbow_r, R_FormArm, settings::visuals::skeleton_color, 1.f);
                            ImGui::GetBackgroundDrawList()->AddLine(Elbow_l, L_FormArm, settings::visuals::skeleton_color, 1.f);

                            ImGui::GetBackgroundDrawList()->AddLine(R_FormArm, SKEL_R_Hand, settings::visuals::skeleton_color, 1.f);
                            ImGui::GetBackgroundDrawList()->AddLine(L_FormArm, SKEL_L_Hand, settings::visuals::skeleton_color, 1.f);

                            ImGui::GetBackgroundDrawList()->AddLine(Neck, Pelvis, settings::visuals::skeleton_color, 1.f);

                            ImGui::GetBackgroundDrawList()->AddLine(Pelvis, SKEL_L_Thigh, settings::visuals::skeleton_color, 1.f);
                            ImGui::GetBackgroundDrawList()->AddLine(Pelvis, SKEL_R_Thigh, settings::visuals::skeleton_color, 1.f);

                            ImGui::GetBackgroundDrawList()->AddLine(SKEL_L_Thigh, SKEL_L_Calf, settings::visuals::skeleton_color, 1.f);
                            ImGui::GetBackgroundDrawList()->AddLine(SKEL_R_Thigh, SKEL_R_Calf, settings::visuals::skeleton_color, 1.f);

                            ImGui::GetBackgroundDrawList()->AddLine(SKEL_L_Calf, SKEL_L_Foot, settings::visuals::skeleton_color, 1.f);
                            ImGui::GetBackgroundDrawList()->AddLine(SKEL_R_Calf, SKEL_R_Foot, settings::visuals::skeleton_color, 1.f);
                        }

                    if (settings::visuals::snapline) {

                        if (Peds == LocalPlayer) {

                            continue;
                        }

                        ImGui::GetBackgroundDrawList()->AddLine(ImVec2(FiveM::WindowSize.x / 2, FiveM::WindowSize.y), ImVec2(Ped_Location.x, Ped_Location.y + Height / 2), settings::visuals::snapeline_color, 1.0f);
                    }
                }
            }
        }
    }
} // namespace visuals
