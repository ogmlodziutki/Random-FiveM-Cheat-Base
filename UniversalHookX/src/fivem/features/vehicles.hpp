#pragma once
#include "../content/classes.hpp"
#include "../settings/settings.hpp"
namespace hook {

    void vehicle(void) {

        hk_World* World = (hk_World*)*(uint64_t*)(FiveM::World);
        if (!World)
            return;

        hk_Ped* LocalPlayer = World->LocalPlayer( );
        if (!LocalPlayer)
            return;

        hk_ReplayInterface* ReplayInterface = (hk_ReplayInterface*)*(uint64_t*)(FiveM::ReplayInterface);
        if (!ReplayInterface)
            return;

        hk_VehicleInterface* VehicleInterface = ReplayInterface->VehicleInterface( );
        if (!VehicleInterface)
            return;

        ImVec2 LocalPlayer_Location = FiveM::WorldToScreen(LocalPlayer->GetCoordinate( ));

        hk_Vehicle* ActualVehicle = LocalPlayer->VehicleManager( );
        if (!ActualVehicle)
            return;

        if (settings::vehicle::vehicle_speed_bool) {

            ActualVehicle->VehicleHandlingInfo( )->SetAcceleration(settings::vehicle::vehicle_speed_value);
        }
        if (settings::vehicle::vehicle_repair) {

            ActualVehicle->SetEngineHealth(1000);
            settings::vehicle::vehicle_repair = false;
        }

        if (settings::vehicle::vehicle_esp) {
            for (int i = 0; i < VehicleInterface->VehicleMaximum( ); i++) {

                hk_Vehicle* Vehicle = VehicleInterface->VehicleList( )->Vehicle(i);
                if (!Vehicle)
                    return;

                ImVec2 Vehicle_location = FiveM::WorldToScreen(Vehicle->GetCoordinate( ));
                if (!IsOnScreen(Vehicle_location))
                    continue;

                Vector3 DistanceCalculation = (LocalPlayer->GetCoordinate( ) - Vehicle->GetCoordinate( ));

                int Distance = sqrtf(DistanceCalculation.x * DistanceCalculation.x + DistanceCalculation.y * DistanceCalculation.y + DistanceCalculation.z * DistanceCalculation.z);

                if (Distance < settings::visuals::vehicle_distance) {

                    if (LocalPlayer->GetHealth( ) <= 100)
                        continue;

                    const char* caca = Vehicle->VehicleModelInfo( )->GetCharName( );
                    if (settings::vehicle::vehicle_name) {
                        DrawOutlinedText(ImGui::GetFont( ), caca, ImVec2(Vehicle_location.x, Vehicle_location.y), 13.0f, settings::visuals::vehicle_esp_color, true);
                    }

                    if (settings::vehicle::vehicle_health) {

                        int bite = Vehicle->GetHealth( );
                        if (!settings::vehicle::vehicle_name) {
                            if (Vehicle->GetHealth( ) != 0) {

                                DrawOutlinedText(ImGui::GetFont( ), std::to_string(bite), ImVec2(Vehicle_location.x, Vehicle_location.y + 10), 13.0f, settings::visuals::vehicle_esp_health, true);
                            } else {
                                DrawOutlinedText(ImGui::GetFont( ), "[BROKEN]", ImVec2(Vehicle_location.x, Vehicle_location.y + 10), 13.0f, settings::visuals::vehiclebroken_esp_color, true);
                            }
                        }

                        else if (settings::vehicle::vehicle_distance) {

                            if (Vehicle->GetHealth( ) != 0) {

                                DrawOutlinedText(ImGui::GetFont( ), std::to_string(bite), ImVec2(Vehicle_location.x, Vehicle_location.y + 20), 13.0f, settings::visuals::vehicle_esp_health, true);
                            } else {
                                DrawOutlinedText(ImGui::GetFont( ), "[BROKEN]", ImVec2(Vehicle_location.x, Vehicle_location.y + 20), 13.0f, settings::visuals::vehiclebroken_esp_color, true);
                            }

                        } else {

                            if (Vehicle->GetHealth( ) != 0) {

                                DrawOutlinedText(ImGui::GetFont( ), std::to_string(bite), ImVec2(Vehicle_location.x, Vehicle_location.y + 10), 13.0f, settings::visuals::vehicle_esp_health, true);
                            } else {
                                DrawOutlinedText(ImGui::GetFont( ), "[BROKEN]", ImVec2(Vehicle_location.x, Vehicle_location.y + 10), 13.0f, settings::visuals::vehiclebroken_esp_color, true);
                            }
                        }
                    }

                    if (settings::vehicle::vehicle_distance) {

                        std::string str = std::to_string(Distance) + "m";
                        if (!settings::vehicle::vehicle_name) {

                            DrawOutlinedText(ImGui::GetFont( ), str, ImVec2(Vehicle_location.x, Vehicle_location.y + 0), 13.0f, settings::visuals::vehicle_esp_distance, true);

                        }

                        else if (settings::vehicle::vehicle_health) {

                            DrawOutlinedText(ImGui::GetFont( ), str, ImVec2(Vehicle_location.x, Vehicle_location.y + 10), 13.0f, settings::visuals::vehicle_esp_distance, true);

                        } else {

                            DrawOutlinedText(ImGui::GetFont( ), str, ImVec2(Vehicle_location.x, Vehicle_location.y + 10), 13.0f, settings::visuals::vehicle_esp_distance, true);
                        }
                    }

                    if (settings::vehicle::vehicle_marker) {

                        ImGui::GetBackgroundDrawList( )->AddCircle(ImVec2(Vehicle_location.x, Vehicle_location.y + 37), 2, ImColor(0, 0, 0, 255), 100, 3.0f);
                        ImGui::GetBackgroundDrawList( )->AddCircle(ImVec2(Vehicle_location.x, Vehicle_location.y + 37), 2, settings::visuals::vehicle_esp_marker, 100, 3.2f);
                    }
                    if (settings::vehicle::vehicle_snapline) {

                        ImGui::GetBackgroundDrawList( )->AddLine(ImVec2(FiveM::WindowSize.x / 2, FiveM::WindowSize.y), ImVec2(Vehicle_location.x, Vehicle_location.y), ImColor(255, 255, 255, 255), 0.5f);
                    }
                }
            }
        }
    }
} // namespace hook
