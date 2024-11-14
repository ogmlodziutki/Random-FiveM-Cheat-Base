#pragma once

#include "../content/classes.hpp"
#include "../settings/settings.hpp"
namespace visuals {
    void lootbag(void) {

        if (settings::object::object_esp) {

            hk_World* World = (hk_World*)*(uint64_t*)(FiveM::World);
            if (!World)
                return;

            hk_Ped* LocalPlayer = World->LocalPlayer( );
            if (!LocalPlayer)
                return;

            hk_ReplayInterface* ReplayInterface = (hk_ReplayInterface*)*(uint64_t*)(FiveM::ReplayInterface);
            if (!ReplayInterface)
                return;

            hk_ObjectInterface* PedInterface = ReplayInterface->ObjectInterface( );
            if (!PedInterface)
                return;

            ImVec2 LocalPlayer_Location = FiveM::WorldToScreen(LocalPlayer->GetCoordinate( ));

            for (int i = 0; i < PedInterface->ObjectMaximum( ); i++) {

                if (LocalPlayer->GetHealth( ) <= 100)
                    continue;

                hk_Object* Peds = PedInterface->ObjectList( )->Object(i);
                if (!Peds)
                    continue;

                ImVec2 Object_Location = FiveM::WorldToScreen(Peds->GetCoordinate( ));

                Vector3 DistanceCalculation = (LocalPlayer->GetCoordinate( ) - Peds->GetCoordinate( ));

                int Distance = sqrtf(DistanceCalculation.x * DistanceCalculation.x + DistanceCalculation.y * DistanceCalculation.y + DistanceCalculation.z * DistanceCalculation.z);

                if (Distance < settings::visuals::lootbag_distance) {

                    ImVec2 Object_location = FiveM::WorldToScreen(Peds->GetCoordinate( ));

                    int32_t WeaponHash = *(int32_t*)(Peds->ObjectModelInfo( ) + 0x18);

                    if (WeaponHash == 1234788901) {
                        if (settings::object::object_name) {
                            DrawOutlinedText(ImGui::GetFont( ), "Loot Bag", ImVec2(Object_location.x, Object_location.y), 13.0f, settings::visuals::bag_esp_color, true);
                        }
                        

                        if (settings::object::object_distance) {
                            std::string str = std::to_string(Distance) + "m";
                            if (settings::object::object_name) {

                                DrawOutlinedText(ImGui::GetFont( ), str, ImVec2(Object_location.x, Object_location.y + 10), 10.0f, settings::visuals::bagdistance_esp_color, true);

                            } else {

                              DrawOutlinedText(ImGui::GetFont( ), str, ImVec2(Object_location.x, Object_location.y), 10.0f, settings::visuals::bagdistance_esp_color, true);
                            }
               

                            
                        }
                        if (settings::object::object_marker) {
                            if (settings::object::object_distance) {

                                ImGui::GetBackgroundDrawList( )->AddCircle(ImVec2(Object_location.x, Object_location.y + 20), 2, ImColor(0, 0, 0, 255), 100, 3.0f);
                                ImGui::GetBackgroundDrawList( )->AddCircle(ImVec2(Object_location.x, Object_location.y + 20), 2, settings::visuals::bagmarker_esp_color, 100, 3.2f);

                            } else {

                               ImGui::GetBackgroundDrawList( )->AddCircle(ImVec2(Object_location.x, Object_location.y + 20), 2, ImColor(0, 0, 0, 255), 100, 3.0f);
                                ImGui::GetBackgroundDrawList( )->AddCircle(ImVec2(Object_location.x, Object_location.y + 20), 2, settings::visuals::bagmarker_esp_color, 100, 3.2f);
                            }
                            
                        }
                        if (settings::object::object_snapline) {

                            ImGui::GetBackgroundDrawList( )->AddLine(LocalPlayer_Location, ImVec2(Object_location.x, Object_location.y), ImColor(255, 255, 255, 255), 0.5f);
                        }
             
                    }
                }
            }
        }
    }
}
