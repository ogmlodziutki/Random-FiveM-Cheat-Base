#pragma once

#include "../content/classes.hpp"

#include "../settings/settings.hpp"

namespace weapon {

    void hook(void) {

        if (settings::weapon::enabled) {

            hk_World* World = (hk_World*)*(uint64_t*)(FiveM::World);
            if (!World)
                return;

            hk_Ped* LocalPlayer = World->LocalPlayer( );
            if (!LocalPlayer)
                return;

            hk_WeaponManager* WeaponManager = LocalPlayer->WeaponManager( );
            if (!WeaponManager)
                return;

            hk_WeaponInfo* WeaponInfo = WeaponManager->WeaponInfo( );
            if (!WeaponInfo)
                return;

            if (settings::weapon::no_recoil) {

                WeaponInfo->SetRecoil(settings::weapon::recoil_value);
            }

            if (settings::weapon::no_spread) {

                WeaponInfo->SetSpread(settings::weapon::spread_value);
            }

            if (settings::weapon::no_reload) {

                WeaponInfo->SetReload(settings::weapon::reload_value * 10);
            }

            if (settings::weapon::damage_boost) {

                WeaponInfo->Damage_boost(settings::weapon::int_damage);
            }

            if (settings::weapon::infinite_range) {

                WeaponInfo->SetRange(settings::weapon::weapon_range);
            }
            if (settings::weapon::explosiveammo) {

                WeaponInfo->SetAmmoType(0x5);
                WeaponInfo->SetAmmoExplosiveType(0x1);
            }

            if (settings::weapon::infite_ammo || settings::weapon::set_ammo) {

                hk_AmmoInfo* AmmoInfo = WeaponInfo->AmmoInfo( );
                if (!AmmoInfo)
                    return;

                hk_AmmoCount* AmmoCount = AmmoInfo->AmmoCount( );
                if (!AmmoCount)
                    return;
                hk_FixedAmmoCount* FixedAmmoCount = AmmoCount->FixedAmmoCount( );
                if (!FixedAmmoCount)
                    return;

                if (settings::weapon::infite_ammo) {

                    FixedAmmoCount->SetAmmo(999);
                }

                if (settings::weapon::set_ammo) {

                    FixedAmmoCount->SetAmmo(settings::weapon::ammo_to_set);

                    settings::weapon::set_ammo = false;
                }
            }
        }
    }
}
