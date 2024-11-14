#include <Windows.h>
#include <iostream>
#include <thread>

#include "console/console.hpp"

#include "hooks/hooks.hpp"
#include "utils/utils.hpp"

#include "dependencies/minhook/MinHook.h"

#include "fivem/content/includes.hpp"
#include "fivem/content/classes.hpp"
#include <delayimp.h>

#define DISABLE_LOGGING_CONSOLE

std::string gen_random(const int len) {
    static const char alphanum[] =
        "0123456789"
        "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
        "abcdefghijklmnopqrstuvwxyz";
    std::string tmp_s;
    tmp_s.reserve(len);

    for (int i = 0; i < len; ++i) {
        tmp_s += alphanum[rand() % (sizeof(alphanum) - 1)];
    }

    return tmp_s;
}


DWORD WINAPI OnProcessAttach(LPVOID lpParam);
DWORD WINAPI OnProcessDetach(LPVOID lpParam);

void DetachProcess() {

    MH_DisableHook(MH_ALL_HOOKS);
}

static uint64_t imgbase() {
    return *(uint64_t*)(__readgsqword(0x60) + 0x10);
}

BOOL WINAPI DllMain(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpReserved) {
    auto baseaddr = imgbase();
    if (fdwReason == DLL_PROCESS_ATTACH) {
        DisableThreadLibraryCalls(hinstDLL);
        std::uint64_t Gtabase = (uint64_t)GetModuleHandleA(nullptr);
        U::SetRenderingBackend(DIRECTX11);
        FiveM::getpname = reinterpret_cast<decltype(FiveM::getpname)>((uint64_t)GetModuleHandleA("citizen-playernames-five.dll") + 0x15300);
        FiveM::FiveM_2802 = true;
        FiveM::FiveM_b2944 = true;
        FiveM::BonePos = Memory::PatternScan(E("48 89 5C 24 ?? 48 89 6C 24 ?? 48 89 74 24 ?? 57 48 83 EC 60 48 8B 01 41 8B E8 48 8B F2 48 8B F9 33 DB"), NULL, NULL);
        auto modules = (uint64_t)GetModuleHandleA("rage-scripting-five.dll");

        //origfastPathMap = reinterpret_cast<g_fastPathMap_t*>(modules + 0x313460);
        //g_fastPathMap = *(g_fastPathMap_t*)origfastPathMap;
        //g_mappingTable = *(g_mappingTable_t*)(modules + 0x3132f0);
        //getnativehandlerdo_obf = decltype(getnativehandlerdo_obf)(modules + 0x134410);
        //getnativehandlerdo_old = decltype(getnativehandlerdo_old)(modules + 0x1357c0);

        if (SAFE_CALL(GetModuleHandleA)("FiveM_b3095_GameProcess.exe") || SAFE_CALL(GetModuleHandleA)("FiveM_b3095_GTAProcess.exe"))
        {
            FiveM::EntityType = 0x1098;
            FiveM::Armor = 0x150C;
            FiveM::WeaponManager = 0x10B8;
            FiveM::PlayerInfo = 0x10A8;
            FiveM::Recoil = 0x2F4;
            FiveM::Spread = 0x84;
            FiveM::ReloadMultiplier = 0x134;
            FiveM::AmmoType = 0x20;
            FiveM::AmmoExplosiveType = 0x24;
            FiveM::WeaponName = 0x5F0;
            FiveM::IsInAVehicule = 0x146B;
            FiveM::Range = 0x28C;
            FiveM::World = baseaddr + 0x2593320;
            FiveM::ReplayInterface = baseaddr + 0x1F58B58;
            FiveM::Camera = baseaddr + 0x20025B8;
            FiveM::ViewPort = baseaddr + 0x20019E0;
            FiveM::pointer_to_handle = reinterpret_cast<FiveM::pointerr>(baseaddr + 0x1624F80);
            addr::dwThreadCollectionPtr = Gtabase + 0x2EF5870;
            addr::origscriptthread = reinterpret_cast<addr::RunScriptThreads_t>(Gtabase + 0xA596B0);
            addr::activethreadtls = 0x161D8F9;
        }

        if (SAFE_CALL(GetModuleHandleA)("FiveM_b2802_GameProcess.exe") || SAFE_CALL(GetModuleHandleA)("FiveM_b2802_GTAProcess.exe")) {
            FiveM::EntityType = 0x1098;
            FiveM::Armor = 0x150C;
            FiveM::freeze = 0x250;
            FiveM::pointer_to_handle = reinterpret_cast<FiveM::pointerr>(baseaddr + 0x15EA290);
            FiveM::WeaponManager = 0x10B8;
            FiveM::PlayerInfo = 0x10A8;
            FiveM::Recoil = 0x2F4;
            FiveM::Spread = 0x84;
            FiveM::ReloadMultiplier = 0x134;
            FiveM::AmmoType = 0x20;
            FiveM::AmmoExplosiveType = 0x24;
            // FiveM::m_player_id = 0x88;
            FiveM::BonePos = (Gtabase + 0x794784);
            FiveM::WeaponName = 0x5F0;
            FiveM::IsInAVehicule = 0x146B;
            FiveM::Range = 0x28C;
            FiveM::FiveM_2802 = true;
            //  FiveM::m_frame_flags = 0x0218;

            FiveM::World = baseaddr + 0x254D448;
            FiveM::ReplayInterface = baseaddr + 0x1F5B820;
            FiveM::Camera = baseaddr + 0x1FBCCD8;
            FiveM::ViewPort = baseaddr + 0x1FBC100;

        }

        if (SAFE_CALL(GetModuleHandleA)("FiveM_b2699_GTAProcess.exe") || SAFE_CALL(GetModuleHandleA)("FiveM_b2699_GameProcess.exe")) {
            FiveM::EntityType = 0x10B8;
            FiveM::Armor = 0x1530;
            FiveM::pointer_to_handle = reinterpret_cast<FiveM::pointerr>(baseaddr + 0x15D6CEC);
            FiveM::freeze = 0x238;
            FiveM::BonePos = (Gtabase + 0x78F450);
            FiveM::WeaponManager = 0x10D8;
            FiveM::PlayerInfo = 0x10C8;
            FiveM::Recoil = 0x2F4;
            FiveM::Spread = 0x84;
            FiveM::ReloadMultiplier = 0x134;
            FiveM::AmmoType = 0x20;
            FiveM::AmmoExplosiveType = 0x24;
            FiveM::WeaponName = 0x5F0;
            FiveM::IsInAVehicule = 0x146B;
            FiveM::Range = 0x28C;
            //  FiveM::m_player_id = 0x88;
            //  FiveM::m_frame_flags = 0x0218;
            FiveM::World = baseaddr + 0x26684D8;
            FiveM::ReplayInterface = baseaddr + 0x20304C8;
            FiveM::Camera = baseaddr + 0x20D9868;
            FiveM::ViewPort = baseaddr + 0x20D8C90;
        }
        if (SAFE_CALL(GetModuleHandleA)("FiveM_b2612_GameProcess.exe") || SAFE_CALL(GetModuleHandleA)("FiveM_b2612_GTAProcess.exe")) {
            FiveM::EntityType = 0x10B8;
            FiveM::Armor = 0x1530;
            FiveM::freeze = 0x238;
            FiveM::pointer_to_handle = reinterpret_cast<FiveM::pointerr>(baseaddr + 0x15DFE20);
            FiveM::WeaponManager = 0x10D8;
            FiveM::PlayerInfo = 0x10C8;
            FiveM::Recoil = 0x2F4;
            FiveM::BonePos = (Gtabase + 0x78C758);
            FiveM::Spread = 0x84;
            FiveM::ReloadMultiplier = 0x134;
            FiveM::AmmoType = 0x20;
            FiveM::AmmoExplosiveType = 0x24;
            FiveM::WeaponName = 0x5F0;
            FiveM::IsInAVehicule = 0x146B;
            FiveM::Range = 0x28C;
            //  FiveM::m_player_id = 0x88;
            //  FiveM::m_frame_flags = 0x0218;
            FiveM::World = baseaddr + 0x2567DB0;
            FiveM::ReplayInterface = baseaddr + 0x1F77EF0;
            FiveM::Camera = baseaddr + 0x1FD9148;
            FiveM::ViewPort = baseaddr + 0x1FD8570;
        }
        if (SAFE_CALL(GetModuleHandleA)("FiveM_b2545_GameProcess.exe") || SAFE_CALL(GetModuleHandleA)("FiveM_b2545_GTAProcess.exe")) {
            FiveM::EntityType = 0x10B8;
            FiveM::Armor = 0x14E0 + 0x50;
            FiveM::freeze = 0x238;
            FiveM::pointer_to_handle = reinterpret_cast<FiveM::pointerr>(baseaddr + 0x15DEEB0);
            FiveM::WeaponManager = 0x10D8;
            FiveM::PlayerInfo = 0x10C8;
            FiveM::Recoil = 0x2F4;
            FiveM::Spread = 0x84;
            FiveM::BonePos = (Gtabase + 0x78C5E4);
            FiveM::ReloadMultiplier = 0x134;
            FiveM::AmmoType = 0x20;
            FiveM::AmmoExplosiveType = 0x24;
            FiveM::WeaponName = 0x5F0;
            FiveM::IsInAVehicule = 0x146B;
            FiveM::Range = 0x28C;
            //  FiveM::m_player_id = 0x88;
            //  FiveM::m_frame_flags = 0x0218;
            FiveM::World = baseaddr + 0x25667E8;
            FiveM::ReplayInterface = baseaddr + 0x1F2E7A8;
            FiveM::Camera = baseaddr + 0x1FD7B48;
            FiveM::ViewPort = baseaddr + 0x1FD6F70;

        }
        if (SAFE_CALL(GetModuleHandleA)("FiveM_b2372_GameProcess.exe") || SAFE_CALL(GetModuleHandleA)("FiveM_b2372_GTAProcess.exe")) {
            FiveM::EntityType = 0x10B8;
            FiveM::Armor = 0x14E0;
            FiveM::freeze = 0x218;
            FiveM::pointer_to_handle = reinterpret_cast<FiveM::pointerr>(baseaddr + 0x15CD2C0);
            FiveM::WeaponManager = 0x10D8;
            FiveM::PlayerInfo = 0x10C8;
            FiveM::Recoil = 0x2F4;
            FiveM::Spread = 0x84;
            FiveM::BonePos = (Gtabase + 0x787FE4);
            FiveM::ReloadMultiplier = 0x134;
            FiveM::AmmoType = 0x20;
            FiveM::AmmoExplosiveType = 0x24;
            FiveM::WeaponName = 0x5F0;
            FiveM::IsInAVehicule = 0x146B;
            FiveM::Range = 0x28C;
            FiveM::World = baseaddr + 0x252DCD8;
            FiveM::ReplayInterface = baseaddr + 0x1F05208;

            FiveM::Camera = baseaddr + 0x1F9F5C8;
            FiveM::ViewPort = baseaddr + 0x1F9E9F0;
        }
        if (SAFE_CALL(GetModuleHandleA)("FiveM_b2189_GameProcess.exe") || SAFE_CALL(GetModuleHandleA)("FiveM_b2189_GTAProcess.exe")) {
            FiveM::EntityType = 0x10B8;
            FiveM::Armor = 0x14E0;
            FiveM::pointer_to_handle = reinterpret_cast<FiveM::pointerr>(baseaddr + 0x15B1008);
            FiveM::freeze = 0x218;
            FiveM::WeaponManager = 0x10D8;
            FiveM::PlayerInfo = 0x10C8;
            FiveM::Recoil = 0x2F4;
            FiveM::Spread = 0x84;
            FiveM::ReloadMultiplier = 0x134;
            FiveM::AmmoType = 0x20;
            FiveM::EngineHealth = 0x908;
            FiveM::BonePos = (Gtabase + 0x79B5DC);
            FiveM::AmmoExplosiveType = 0x24;
            FiveM::WeaponName = 0x5F0;
            FiveM::IsInAVehicule = 0x146B;
            FiveM::Range = 0x28C;
            /*   FiveM::m_player_id = 0x68;
               FiveM::m_frame_flags = 0x0218;*/
            FiveM::World = baseaddr + 0x24E6D90;
            FiveM::ReplayInterface = baseaddr + 0x1EE18A8;
            FiveM::Camera = baseaddr + 0x1F89498;
            FiveM::ViewPort = baseaddr + 0x1F888C0;
        }
        if (SAFE_CALL(GetModuleHandleA)("FiveM_b2060_GameProcess.exe") || SAFE_CALL(GetModuleHandleA)("FiveM_b2060_GTAProcess.exe")) {
            FiveM::EntityType = 0x10A8;
            FiveM::Armor = 0x14E0;
            FiveM::freeze = 0x218;
            FiveM::WeaponManager = 0x10D8;
            FiveM::PlayerInfo = 0x10B8;
            FiveM::Recoil = 0x2F4;
            FiveM::pointer_to_handle = reinterpret_cast<FiveM::pointerr>(baseaddr + 0x159B224);
            FiveM::Spread = 0x84;
            FiveM::ReloadMultiplier = 0x134;
            FiveM::AmmoType = 0x20;
            FiveM::AmmoExplosiveType = 0x24;
            FiveM::BonePos = (Gtabase + 0x7935D0);
            FiveM::WeaponName = 0x5F0;
            FiveM::IsInAVehicule = 0x146B;
            FiveM::Range = 0x28C;
            /*  FiveM::m_player_id = 0x88;
              FiveM::m_frame_flags = 0x0218;*/
            FiveM::World = baseaddr + 0x24C8858;
            FiveM::ReplayInterface = baseaddr + 0x1EC3828;
            FiveM::Camera = baseaddr + 0x1F6B3C0;
            FiveM::ViewPort = baseaddr + 0x1F6A7E0;
        }

        if (SAFE_CALL(GetModuleHandleA)("FiveM_b2944_GameProcess.exe") || SAFE_CALL(GetModuleHandleA)("FiveM_b2944_GTAProcess.exe")) {
            FiveM::EntityType = 0x1098;
            FiveM::Armor = 0x150C;
            FiveM::freeze = 0x250;
            FiveM::WeaponManager = 0x10B8;
            FiveM::PlayerInfo = 0x10A8;
            FiveM::pointer_to_handle = reinterpret_cast<FiveM::pointerr>(baseaddr + 0x160FF90);
            FiveM::Recoil = 0x2F4;
            FiveM::Spread = 0x84;
            FiveM::ReloadMultiplier = 0x134;
            FiveM::AmmoType = 0x20;
            FiveM::AmmoExplosiveType = 0x24;
            FiveM::WeaponName = 0x5F0;
            FiveM::IsInAVehicule = 0x146B;
            FiveM::Range = 0x28C;
            FiveM::BonePos = (Gtabase + 0x795F9C);
            FiveM::FiveM_b2944 = true;
            FiveM::World = baseaddr + 0x257BEA0;
            FiveM::ReplayInterface = baseaddr + 0x1F42068;
            FiveM::Camera = baseaddr + 0x1FEB698;
            FiveM::ViewPort = baseaddr + 0x1FEAAC0;
        }

        if (SAFE_CALL(GetModuleHandleA)("FiveM_GameProcess.exe") || SAFE_CALL(GetModuleHandleA)("FiveM_GTAProcess.exe")) {
            FiveM::EntityType = 0x10A8;
            FiveM::Armor = 0x14B8;
            FiveM::freeze = 0x218;
            /*           FiveM::m_player_id = 0x68; */
            FiveM::WeaponManager = 0x10C8;
            FiveM::pointer_to_handle = reinterpret_cast<FiveM::pointerr>(baseaddr + 0x1561E30);
            FiveM::PlayerInfo = 0x10B8;
            FiveM::BonePos = (Gtabase + 0xCAF950);
            FiveM::Recoil = 0x2E8;
            FiveM::Spread = 0x74;
            FiveM::ReloadMultiplier = 0x12C;
            FiveM::AmmoType = 0x20;
            FiveM::AmmoExplosiveType = 0x24;
            FiveM::WeaponName = 0x5E0;
            FiveM::IsInAVehicule = 0x146B;
            FiveM::Range = 0x25C;
            /*    FiveM::m_frame_flags = 0x01F8;*/
            FiveM::World = baseaddr + 0x247F840;
            FiveM::ReplayInterface = baseaddr + 0x1EFD4C8;
            FiveM::Camera = baseaddr + 0x2088360;
            FiveM::ViewPort = baseaddr + 0x2087780;
        }
        /*   initoffset();
           std::thread(scripth::startscripthook).detach();*/
        (HANDLE)_beginthreadex(NULL, 0, (_beginthreadex_proc_type)OnProcessAttach, hinstDLL, 0, NULL);
    }

    return TRUE;
}



DWORD WINAPI OnProcessAttach(LPVOID lpParam) {



    std::this_thread::sleep_for(std::chrono::milliseconds(9));
    if (U::GetRenderingBackend() == NONE) {
        std::cin.get();

        FreeLibraryAndExitThread(reinterpret_cast<HMODULE>(lpParam), 0);
        return 0;
    }

    MH_Initialize();
    H::Init();
    return 0;
}

DWORD WINAPI OnProcessDetach(LPVOID lpParam) {
    H::Free();
    MH_Uninitialize();

    Console::Free();

    return 0;
}
