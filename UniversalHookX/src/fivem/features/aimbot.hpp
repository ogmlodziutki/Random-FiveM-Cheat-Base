#pragma once

#include "../content/classes.hpp"
#include "../settings/settings.hpp"

bool NeedToDisable = false;
bool IsDisabled = false;
bool IsOnFOV = false;

bool shooting = false;
bool isonscreen(ImVec2 coords) {
    if (coords.x < 0.1f || coords.y < 0.1 || coords.x > ImGui::GetIO( ).DisplaySize.x || coords.y > ImGui::GetIO( ).DisplaySize.y) {
        return false;
    } else {
        return true;
    }
}

namespace aimbot {

    uintptr_t GetDistancePlayer( ) {
        uintptr_t ClosestPlayer = 0;
        float closestdist = FLT_MAX;
        float dist;
        hk_World* World = (hk_World*)*(uint64_t*)(FiveM::World);
        if (!World)
            return NULL;

        hk_Ped* LocalPlayer = World->LocalPlayer( );
        if (!LocalPlayer)
            return NULL;

        hk_ReplayInterface* ReplayInterface = (hk_ReplayInterface*)*(uint64_t*)(FiveM::ReplayInterface);
        if (!ReplayInterface)
            return NULL;

        hk_PedInterface* PedInterface = ReplayInterface->PedInterface( );
        if (!PedInterface)
            return NULL;

        for (int index = 0; index < PedInterface->PedMaximum( ); index++) {
            hk_Ped* Peds = PedInterface->PedList( )->Ped(index);
            if (!Peds)
                continue;
            if (!settings::visuals::aim_dead) {
                if (Peds->GetHealth( ) <= 0)
                    continue;
            }
            bool IsPed = false;

            auto ped_type = Peds->GetPedType( );

            if (!ped_type)
                continue;
            ped_type = ped_type << 11 >> 25;

            if (ped_type != 2)
                IsPed = true;

            if (!settings::aimbot::ignore_peds)
                if (IsPed)
                    continue;
            if (Peds == LocalPlayer)
                continue;

            Vector3 DistanceCalculation = (LocalPlayer->GetCoordinate( ) - Peds->GetCoordinate( ));
            double Distance = sqrtf(DistanceCalculation.x * DistanceCalculation.x + DistanceCalculation.y * DistanceCalculation.y + DistanceCalculation.z * DistanceCalculation.z);
            void* m_net_player_data = (void*)(Peds->PlayerInfo( ) + 0x20);

          
			auto it = std::find(FiveM::Friend.begin( ), FiveM::Friend.end( ), FiveM::getpname(m_net_player_data));
            if (it != FiveM::Friend.end( ))
                continue;

            if (Distance <= settings::aimbot::aimbot_distance) {

                ImVec2 PedPos = FiveM::WorldToScreen(Peds->GetCoordinate( ));

                ImVec2 Head = FiveM::GetBonePosMatrixW2S(reinterpret_cast<uint64_t>(Peds), 0);
                if (!IsOnScreen(Head))
                    continue;

                ImVec2 middle = ImVec2(FiveM::WindowSize.x / 2, FiveM::WindowSize.y / 2);
                dist = FiveM::pythag(middle, Head);
                if (dist < closestdist) {
                    {
                        closestdist = dist;
                        ClosestPlayer = reinterpret_cast<uint64_t>(Peds);
                    }
                }
            }
        }
        return ClosestPlayer;
    }

    void AimVec(Vector3 point) {
        DWORD64 addr = FiveM::GetCamera( );
        if (addr) {

            Vector3 CrosshairPos = *(Vector3*)(addr + 0x60);
            Vector3 caca(CrosshairPos.x - point.x, CrosshairPos.y - point.y, CrosshairPos.z - point.z);
            float distance = caca.Length( );

            uint64_t CamData = *(DWORD64*)(addr + 0x10);
            if (CamData != NULL) {
                if (*(float*)(CamData + 0x130) == 8.0f) {
                    *(float*)(CamData + 0x130) = 111.0f;
                    *(float*)(CamData + 0x134) = 111.0f;
                    *(float*)(CamData + 0x4CC) = 0.0f;

                    if (*(float*)(CamData + 0x49C) == 1.0f)
                        *(float*)(CamData + 0x49C) = 0.0f;

                    *(float*)(CamData + 0x2AC) = 0.0f;
                    *(float*)(CamData + 0x2B0) = 0.0f;
                }
            }

            float threshold = 0.5f / (settings::aimbot::aimbot_smooth * 20);
            Vector3 Out = Vector3((point.x - CrosshairPos.x) / distance, (point.y - CrosshairPos.y) / distance, (point.z - CrosshairPos.z) / distance);

            if (settings::aimbot::aimbot_smooth <= 1) {
                *(Vector3*)(addr + 0x40) = Out;  // FPS
                *(Vector3*)(addr + 0x3D0) = Out; // TPS
            } else {
                // First Person
                {
                    Vector3 angles = *(Vector3*)(addr + 0x40);
                    if (((Out.x - angles.x) > threshold) || ((angles.x - Out.x) > threshold)) {
                        if (angles.x > Out.x)
                            *(float*)(addr + 0x40 + 0x0) -= threshold;
                        else if (angles.x < Out.x)
                            *(float*)(addr + 0x40 + 0x0) += threshold;
                    }
                    if (((Out.y - angles.y) > threshold) || ((angles.y - Out.y) > threshold)) {
                        if (angles.y > Out.y)
                            *(float*)(addr + 0x40 + 0x4) -= threshold;
                        else if (angles.y < Out.y)
                            *(float*)(addr + 0x40 + 0x4) += threshold;
                    }
                    if (((Out.z - angles.z) > threshold) || ((angles.z - Out.z) > threshold)) {
                        if (angles.z > Out.z)
                            *(float*)(addr + 0x40 + 0x8) -= threshold;
                        else if (angles.z < Out.z)
                            *(float*)(addr + 0x40 + 0x8) += threshold;
                    }
                }
                // Third Person
                {
                    Vector3 angles = *(Vector3*)(addr + 0x3D0);
                    if (((Out.x - angles.x) > threshold) || ((angles.x - Out.x) > threshold)) {
                        if (angles.x > Out.x)
                            *(float*)(addr + 0x3D0 + 0x0) -= threshold;
                        else if (angles.x < Out.x)
                            *(float*)(addr + 0x3D0 + 0x0) += threshold;
                    }
                    if (((Out.y - angles.y) > threshold) || ((angles.y - Out.y) > threshold)) {
                        if (angles.y > Out.y)
                            *(float*)(addr + 0x3D0 + 0x4) -= threshold;
                        else if (angles.y < Out.y)
                            *(float*)(addr + 0x3D0 + 0x4) += threshold;
                    }
                    if (((Out.z - angles.z) > threshold) || ((angles.z - Out.z) > threshold)) {
                        if (angles.z > Out.z)
                            *(float*)(addr + 0x3D0 + 0x8) -= threshold;
                        else if (angles.z < Out.z)
                            *(float*)(addr + 0x3D0 + 0x8) += threshold;
                    }
                    /*
                     if (settings::visuals::preview_target) {

                            if (settings::visuals::preview_style == 0) {

                                    ImGui::GetBackgroundDrawList( )->AddCircle(ImVec2(bone_pos.x, bone_pos.y), 2, settings::visuals::preview_target_colors, 100);
                            }

                            if (settings::visuals::preview_style == 1) {

                                    ImGui::GetBackgroundDrawList( )->AddLine(ImVec2(FiveM::WindowSize.x / 2, FiveM::WindowSize.y / 2), ImVec2(bone_pos.x, bone_pos.y), settings::visuals::preview_target_colors);
                            }

                            if (settings::visuals::preview_style == 2) {

                                    ImGui::GetBackgroundDrawList( )->AddCircle(ImVec2(bone_pos.x, bone_pos.y), 2, settings::visuals::preview_target_colors, 100);

                                    ImGui::GetBackgroundDrawList( )->AddLine(ImVec2(FiveM::WindowSize.x / 2, FiveM::WindowSize.y / 2), ImVec2(bone_pos.x, bone_pos.y), settings::visuals::preview_target_colors);
                            }
                    }*/
                }
            }
        }
    }

    void hook(void) {

        if (settings::aimbot::enabled) {

            if (SAFE_CALL(GetAsyncKeyState)(settings::aimbot::hotkey) & 0x8000) {
                uintptr_t entity = GetDistancePlayer( );
                if (!entity)
                    return;
                if (settings::aimbot::aimbot_type == 1) {
                    auto get_distance = [](double x1, double y1, double x2, double y2) {
                        return sqrtf(pow(x2 - x1, 2.0) + pow(y2 - y1, 2.0));
                    };

                    auto center_x = FiveM::WindowSize.x / 2;
                    auto center_y = FiveM::WindowSize.y / 2;

                    auto bone_pos = FiveM::GetBonePosMatrix(entity, 0);

                    switch (settings::aimbot::aimbot_bone) {
                        case 1:
                            bone_pos = FiveM::GetBonePosMatrix(entity, 7);
                            break;
                        case 2:
                            bone_pos = FiveM::GetBonePosMatrix(entity, 8);
                            break;
                        case 3:
                            bone_pos = FiveM::GetBonePosMatrix(entity, 1);
                            break;
                    }

                    ImVec2 screen = FiveM::WorldToScreen(bone_pos);

                    auto fov = get_distance(center_x, center_y, screen.x, screen.y);

                    if (fov < settings::aimbot::aimbot_fov * 10) {

                        AimVec(bone_pos);
                    }
                }

                // else if (settings::aimbot::aimbot_type == 3) {

                //    uintptr_t entity = GetDistancePlayer( );
                //    if (!entity)
                //        return;
                //    auto bone_pos = FiveM::GetBonePosMatrix(entity, 0);

                //    switch (settings::aimbot::aimbot_bone) {
                //        case 0:
                //            // bone_pos = FiveM::GetBonePosW2S(entity, SKEL_Head);
                //            break;
                //        case 1:
                //            bone_pos = FiveM::GetBonePosMatrix(entity, 7);
                //            break;
                //        case 2:
                //            bone_pos = FiveM::GetBonePosMatrix(entity, 8);
                //            break;
                //        case 3:
                //            bone_pos = FiveM::GetBonePosMatrix(entity, 1);
                //            break;
                //        case 4:
                //            bone_pos = FiveM::GetBonePosMatrix(entity, 2);
                //            break;
                //        case 5:
                //            bone_pos = FiveM::GetBonePosMatrix(entity, 5);
                //            break;
                //        case 6:
                //            bone_pos = FiveM::GetBonePosMatrix(entity, 6);
                //            break;
                //    }

                //    hk_World* World = (hk_World*)*(uint64_t*)(FiveM::World);

                //    hk_Ped* LocalPlayer = World->LocalPlayer( );
                //    if (LocalPlayer) {
                //        hk_WeaponManager* Weapon = LocalPlayer->WeaponManager( );
                //        Weapon->SetWeaponCoordinate(bone_pos);
                //    }
                //}
            }
        }
    }
} // namespace aimbot
