#pragma once

#include "../content/classes.hpp"
#include "../settings/settings.hpp"

#include "../content/tahoma.h" // <-- Required font!








namespace self {

    void options(void) {

        if (settings::self::enabled) {

            hk_World* World = (hk_World*)*(uint64_t*)(FiveM::World);
            if (!World)
                return;

            hk_Ped* LocalPlayer = World->LocalPlayer();
            if (!LocalPlayer)
                return;

            hk_ObjectNavigationPed* Nav = LocalPlayer->ObjectNavigation();
            if (!LocalPlayer)
                return;

            if (settings::self::godmode) {

                *(bool*)(LocalPlayer + 0x189) = true;
            }
            else {
                *(bool*)(LocalPlayer + 0x189) = false;
            }

            if (settings::self::semigodmode) {

                if (LocalPlayer->GetHealth() < 200)
                    LocalPlayer->SetMaxHealth();
            }
            if (settings::self::Infinite_Armor)
            {
                LocalPlayer->SetArmor(100);

            }
            if (settings::self::nocol)
            {
                DWORD64 addr1 = (DWORD64)World->LocalPlayer()->ObjectNavigation();
                if (addr1) {
                    DWORD64 addr2 = *(DWORD64*)(addr1 + 0x10);
                    if (addr2) {
                        DWORD64 addr3 = *(DWORD64*)(addr2 + 0x20);
                        if (addr3) {
                            DWORD64 addr4 = *(DWORD64*)(addr3 + 0x70);
                            if (addr4) {
                                DWORD64 addr5 = *(DWORD64*)(addr4 + 0x0);
                                if (addr5) {
                                    *(float*)(addr5 + 0x2C) = -1.0f;
                                }
                            }
                        }
                    }
                }
            }
            else
            {
                DWORD64 addr1 = (DWORD64)World->LocalPlayer()->ObjectNavigation();
                if (addr1) {
                    DWORD64 addr2 = *(DWORD64*)(addr1 + 0x10);
                    if (addr2) {
                        DWORD64 addr3 = *(DWORD64*)(addr2 + 0x20);
                        if (addr3) {
                            DWORD64 addr4 = *(DWORD64*)(addr3 + 0x70);
                            if (addr4) {
                                DWORD64 addr5 = *(DWORD64*)(addr4 + 0x0);
                                if (addr5) {
                                    *(float*)(addr5 + 0x2C) = 0.25f;
                                }
                            }
                        }
                    }
                }
            }




            if (SAFE_CALL(GetAsyncKeyState)(settings::self::HotkeyArmor) & 0x8000) {

                LocalPlayer->SetArmor(settings::self::armor);
                settings::self::set_armor = false;
            }

            if (SAFE_CALL(GetAsyncKeyState)(settings::self::HotkeyHealth) & 0x8000) {

                LocalPlayer->SetHealth(settings::self::health * 2);
                settings::self::set_health = false;
            }

            if (settings::self::set_health) {

                LocalPlayer->SetHealth(settings::self::health * 2);
                settings::self::set_health = false;
            }

            //if (settings::self::blockinput)
            //{
            //    ImGui::GetIO().MouseDrawCursor = Menu::;
            //    ImGui::GetIO().WantCaptureKeyboard = Menu::Open;
            //    auto& io = ImGui::GetIO();
            //    if (io.WantCaptureMouse || io.WantCaptureKeyboard) {
            //        return true;
            //    }


            //}

            if (settings::self::set_revive) {

                if (LocalPlayer->GetHealth() < 200)
                    LocalPlayer->SetMaxHealth();
                settings::self::set_revive = false;
            }

            if (settings::self::set_armor) {

                LocalPlayer->SetArmor(settings::self::armor);
                settings::self::set_armor = false;
            }
            if (settings::self::fovcganger)
            {
                DWORD64 addr = FiveM::GetCamera();
                uintptr_t CameraData = *(uintptr_t*)(addr + 0x10);
                *(float*)(CameraData + 0x30) = settings::self::FovValue;
                settings::self::fovcganger = false;
            }

            if (settings::self::invisible)
            {
                LocalPlayer->SetInvisible(0X2c);


            }
            else {

                LocalPlayer->SetInvisible(0X37);

            }



        }
    }

    void noclip(void) {

        if (settings::self::noclip) {

            hk_World* World = (hk_World*)*(uint64_t*)(FiveM::World);
            if (!World)
                return;

            hk_Ped* LocalPlayer = World->LocalPlayer();
            if (!LocalPlayer)
                return;

            hk_ObjectNavigationPed* Nav = LocalPlayer->ObjectNavigation();
            if (!Nav)
                return;

            Vector3 ActualPos = LocalPlayer->GetCoordinate();

            if (LocalPlayer->IsInAVehicule() == true) {
                return;
            }
            if (LocalPlayer->GetHealth() < 100)
                return;

            if (SAFE_CALL(GetAsyncKeyState)(VK_LSHIFT))
                settings::self::noclipspeedlol = true;
            else
                settings::self::noclipspeedlol = false;

            if (settings::self::noclipspeedlol)
            {
                settings::self::noclipspeedlol = 1.0f;
            }
            else
            {
                settings::self::noclipspeedlol = 0.1f;

            }


            Nav->SetRotation(Vector4(0, 0, 0, 0));




            if (SAFE_CALL(GetAsyncKeyState)(settings::self::hotkey_forward) & 0x8000)
            {
                LocalPlayer->SetVelocity();

                DWORD64 addr = FiveM::GetCamera();
                Vector3 TPSangles = *(Vector3*)(addr + 0x40);
                if (TPSangles == Vector3(0, 0, 0))
                {
                    TPSangles = *(Vector3*)(addr + 0x40);
                }
                Vector3 newpos = ActualPos;
                newpos.x += (TPSangles.x * settings::self::noclipspeedlol);
                newpos.y += (TPSangles.y * settings::self::noclipspeedlol);
                newpos.z += (TPSangles.z * settings::self::noclipspeedlol);
                LocalPlayer->SetCoordinate(newpos);
                Nav->SetCoordinate(newpos);
            }

            if (SAFE_CALL(GetAsyncKeyState)(settings::self::hotkey_backward) & 0x8000)
            {
                LocalPlayer->SetVelocity();

                DWORD64 addr = FiveM::GetCamera();
                Vector3 TPSangles = *(Vector3*)(addr + 0x40);
                if (TPSangles == Vector3(0, 0, 0))
                {
                    TPSangles = *(Vector3*)(addr + 0x40);
                }
                Vector3 newpos = ActualPos;
                newpos.x -= (TPSangles.x * settings::self::noclipspeedlol);
                newpos.y -= (TPSangles.y * settings::self::noclipspeedlol);
                newpos.z -= (TPSangles.z * settings::self::noclipspeedlol);
                LocalPlayer->SetCoordinate(newpos);
                Nav->SetCoordinate(newpos);
            }

            if (SAFE_CALL(GetAsyncKeyState)(settings::self::hotkey_left) & 0x8000)
            {
                LocalPlayer->SetVelocity();

                DWORD64 addr = FiveM::GetCamera();
                Vector3 TPSangles = *(Vector3*)(addr + 0x40);
                if (TPSangles == Vector3(0, 0, 0))
                {
                    TPSangles = *(Vector3*)(addr + 0x40);
                }
                Vector3 newpos = ActualPos;
                newpos.x -= (TPSangles.y * settings::self::noclipspeedlol);
                newpos.y += (TPSangles.x * settings::self::noclipspeedlol);
                newpos.z += (TPSangles.z * settings::self::noclipspeedlol);
                LocalPlayer->SetCoordinate(newpos);
                Nav->SetCoordinate(newpos);
            }

            if (SAFE_CALL(GetAsyncKeyState)(settings::self::hotkey_right) & 0x8000)
            {
                LocalPlayer->SetVelocity();

                DWORD64 addr = FiveM::GetCamera();
                Vector3 TPSangles = *(Vector3*)(addr + 0x40);
                if (TPSangles == Vector3(0, 0, 0))
                {
                    TPSangles = *(Vector3*)(addr + 0x40);
                }
                Vector3 newpos = ActualPos;
                newpos.x += (TPSangles.y * settings::self::noclipspeedlol);
                newpos.y -= (TPSangles.x * settings::self::noclipspeedlol);
                newpos.z -= (TPSangles.z * settings::self::noclipspeedlol);
                LocalPlayer->SetCoordinate(newpos);
                Nav->SetCoordinate(newpos);
            }

            //	ENTITY::SET_ENTITY_COLLISION(local_player, false, false);

        }

    }
}

void autofarm(void) {
    if (settings::self::autoarm) {

        if (SAFE_CALL(GetAsyncKeyState)(settings::self::hotkey_autofarm)) {
            hk_World* World = (hk_World*)*(uint64_t*)(FiveM::World);
            if (!World)
                return;

            hk_Ped* LocalPlayer = World->LocalPlayer();
            if (!LocalPlayer)
                return;

            hk_ObjectNavigationPed* Nav = LocalPlayer->ObjectNavigation();
            if (!LocalPlayer)
                return;

            hk_ReplayInterface* ReplayInterface = (hk_ReplayInterface*)*(uint64_t*)(FiveM::ReplayInterface);
            hk_PedInterface* PedInterface = ReplayInterface->PedInterface();
            if (PedInterface) {
                for (int index = 0; index < PedInterface->PedMaximum(); index++) {
                    hk_Ped* Peds = PedInterface->PedList()->Ped(index);
                    if (!Peds)
                        continue;
                    if (Peds == LocalPlayer)
                        continue;

                    bool IsPed = false;

                    auto ped_type = Peds->GetPedType();
                    if (!ped_type) {
                        continue;
                    }
                    ped_type = ped_type << 11 >> 25;

                    if (ped_type != 2) {
                        IsPed = true;
                    }
                    if (!IsPed)
                        continue;

                    auto mypos = LocalPlayer->GetCoordinate();
                    *(Vector3*)(Peds + 0x90) = Vector3(mypos.x, mypos.y + 1, mypos.z);
                    *(float*)(Peds + 0x280) = 0.1f;
                }
            }
        }
    }
}


