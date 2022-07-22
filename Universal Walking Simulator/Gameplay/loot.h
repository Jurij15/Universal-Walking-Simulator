#pragma once
#include <vector>
#include <random>

#include <Gameplay/helper.h>

namespace Looting
{
	namespace Tables
	{
		std::vector<std::vector<UObject*>> WeaponTable;
		std::vector<UObject*> ConsumableTable;
		std::vector<UObject*> AmmoTable;
		static bool bInitialized = false;

		static int GetWeaponTableIdx()
		{
			// int Random = UKismetMathLibrary::RandomIntegerInRange(0, 100);
			std::random_device rd; // obtain a random number from hardware
			std::mt19937 gen(rd()); // seed the generator
			std::uniform_int_distribution<> distr(0, 100); // define the range

			auto Random = distr(gen);

			if (Random <= 35)
				return 0;

			if (Random <= 60)
				return 1;

			if (Random <= 80)
				return 2;

			if (Random <= 95)
				return 3;

			if (Random <= 100)
				return 4;

			return 0;
		}

		static UObject* GetWeaponDef()
		{
			auto& Table = WeaponTable[GetWeaponTableIdx()];

			while (true)
			{
				auto Weapon = Table[rand() % (Table.size())];

				if (Weapon)
					return Weapon;
			}
		}

		static UObject* GetAmmoDef()
		{
			while (true)
			{
				auto Ammo = AmmoTable[rand() % (AmmoTable.size())];

				if (Ammo)
					return Ammo;
			}
		}

		static UObject* GetConsumableDef()
		{
			while (true)
			{
				auto Consumable = ConsumableTable[rand() % (ConsumableTable.size())];

				if (Consumable)
					return Consumable;
			}
		}

		static DWORD WINAPI Init(LPVOID)
		{
			if (bInitialized)
				return 0;

			bInitialized = true;

			for (int i = 0; i < 5; i++) // 0 = gray, 1 = green, 2 = blue, 3 = purple, 4 = gold
			{
				WeaponTable.push_back(std::vector<UObject*>());
			}

			WeaponTable[0].push_back(FindObject("/Game/Athena/Items/Weapons/WID_Assault_Auto_Athena_C_Ore_T02.WID_Assault_Auto_Athena_C_Ore_T02"));
			WeaponTable[0].push_back(FindObject("/Game/Athena/Items/Weapons/WID_Assault_SemiAuto_Athena_C_Ore_T02.WID_Assault_SemiAuto_Athena_C_Ore_T02"));
			WeaponTable[0].push_back(FindObject("/Game/Athena/Items/Weapons/WID_Shotgun_SemiAuto_Athena_UC_Ore_T03.WID_Shotgun_SemiAuto_Athena_UC_Ore_T03"));
			WeaponTable[0].push_back(FindObject("/Game/Athena/Items/Weapons/WID_Pistol_AutoHeavyPDW_Athena_C_Ore_T03.WID_Pistol_AutoHeavyPDW_Athena_C_Ore_T03"));
			WeaponTable[0].push_back(FindObject("/Game/Athena/Items/Weapons/WID_Pistol_AutoHeavySuppressed_Athena_C_Ore_T02.WID_Pistol_AutoHeavySuppressed_Athena_C_Ore_T02"));
			WeaponTable[0].push_back(FindObject("/Game/Athena/Items/Weapons/WID_Pistol_SemiAuto_Athena_C_Ore_T02.WID_Pistol_SemiAuto_Athena_C_Ore_T02"));
			WeaponTable[0].push_back(FindObject("/Game/Athena/Items/Weapons/WID_Pistol_SixShooter_Athena_C_Ore_T02.WID_Pistol_SixShooter_Athena_C_Ore_T02"));
			WeaponTable[0].push_back(FindObject("/Game/Athena/Items/Weapons/WID_Pistol_AutoHeavy_Athena_C_Ore_T02.WID_Pistol_AutoHeavy_Athena_C_Ore_T02"));
			WeaponTable[0].push_back(FindObject("/Game/Athena/Items/Weapons/WID_Pistol_Flintlock_Athena_C.WID_Pistol_Flintlock_Athena_C"));

			WeaponTable[1].push_back(FindObject("/Game/Athena/Items/Weapons/WID_Assault_Auto_Athena_UC_Ore_T03.WID_Assault_Auto_Athena_UC_Ore_T03"));
			WeaponTable[1].push_back(FindObject("/Game/Athena/Items/Weapons/WID_Assault_SemiAuto_Athena_UC_Ore_T03.WID_Assault_SemiAuto_Athena_UC_Ore_T03"));
			WeaponTable[1].push_back(FindObject("/Game/Athena/Items/Weapons/WID_Shotgun_Standard_Athena_C_Ore_T03.WID_Shotgun_Standard_Athena_C_Ore_T03"));
			WeaponTable[1].push_back(FindObject("/Game/Athena/Items/Weapons/WID_Shotgun_SemiAuto_Athena_R_Ore_T03.WID_Shotgun_SemiAuto_Athena_R_Ore_T03"));
			WeaponTable[1].push_back(FindObject("/Game/Athena/Items/Weapons/WID_Sniper_Auto_Suppressed_Scope_Athena_UC.WID_Sniper_Auto_Suppressed_Scope_Athena_UC"));
			WeaponTable[1].push_back(FindObject("/Game/Athena/Items/Weapons/WID_Sniper_Standard_Scope_Athena_VR_Ore_T03.WID_Sniper_Standard_Scope_Athena_VR_Ore_T03"));
			WeaponTable[1].push_back(FindObject("/Game/Athena/Items/Weapons/WID_Pistol_AutoHeavyPDW_Athena_UC_Ore_T03.WID_Pistol_AutoHeavyPDW_Athena_UC_Ore_T03"));
			WeaponTable[1].push_back(FindObject("/Game/Athena/Items/Weapons/WID_Pistol_AutoHeavySuppressed_Athena_UC_Ore_T03.WID_Pistol_AutoHeavySuppressed_Athena_UC_Ore_T03"));
			WeaponTable[1].push_back(FindObject("/Game/Athena/Items/Weapons/WID_Pistol_Scavenger_Athena_UC_Ore_T03.WID_Pistol_Scavenger_Athena_UC_Ore_T03"));
			WeaponTable[1].push_back(FindObject("/Game/Athena/Items/Weapons/WID_Assault_AutoDrum_Athena_UC_Ore_T03.WID_Assault_AutoDrum_Athena_UC_Ore_T03"));
			WeaponTable[1].push_back(FindObject("/Game/Athena/Items/Weapons/WID_Pistol_SemiAuto_Athena_UC_Ore_T03.WID_Pistol_SemiAuto_Athena_UC_Ore_T03"));
			WeaponTable[1].push_back(FindObject("/Game/Athena/Items/Weapons/WID_DualPistol_Suppresseed_Athena_UC_T01.WID_DualPistol_Suppresseed_Athena_UC_T01"));
			WeaponTable[1].push_back(FindObject("/Game/Athena/Items/Weapons/WID_Pistol_SixShooter_Athena_UC_Ore_T03.WID_Pistol_SixShooter_Athena_UC_Ore_T03"));
			WeaponTable[1].push_back(FindObject("/Game/Athena/Items/Weapons/WID_Pistol_Flintlock_Athena_UC.WID_Pistol_Flintlock_Athena_UC"));
			WeaponTable[1].push_back(FindObject("/Game/Athena/Items/Weapons/WID_Pistol_Revolver_SingleAction_Athena_UC.WID_Pistol_Revolver_SingleAction_Athena_UC"));

			WeaponTable[2].push_back(FindObject("/Game/Athena/Items/Weapons/WID_Assault_Auto_Athena_R_Ore_T03.WID_Assault_Auto_Athena_R_Ore_T03"));
			WeaponTable[2].push_back(FindObject("/Game/Athena/Items/Weapons/WID_Assault_SemiAuto_Athena_R_Ore_T03.WID_Assault_SemiAuto_Athena_R_Ore_T03"));
			WeaponTable[2].push_back(FindObject("/Game/Athena/Items/Weapons/WID_Assault_Heavy_Athena_R_Ore_T03.WID_Assault_Heavy_Athena_R_Ore_T03"));
			WeaponTable[2].push_back(FindObject("/Game/Athena/Items/Weapons/WID_Assault_PistolCaliber_AR_Athena_R_Ore_T03.WID_Assault_PistolCaliber_AR_Athena_R_Ore_T03"));
			WeaponTable[2].push_back(FindObject("/Game/Athena/Items/Weapons/WID_Shotgun_Standard_Athena_UC_Ore_T03.WID_Shotgun_Standard_Athena_UC_Ore_T03"));
			WeaponTable[2].push_back(FindObject("/Game/Athena/Items/Weapons/WID_Shotgun_SemiAuto_Athena_VR_Ore_T03.WID_Shotgun_SemiAuto_Athena_VR_Ore_T03"));
			WeaponTable[2].push_back(FindObject("/Game/Athena/Items/Weapons/WID_Sniper_BoltAction_Scope_Athena_R_Ore_T03.WID_Sniper_BoltAction_Scope_Athena_R_Ore_T03"));
			WeaponTable[2].push_back(FindObject("/Game/Athena/Items/Weapons/WID_Sniper_Auto_Suppressed_Scope_Athena_R.WID_Sniper_Auto_Suppressed_Scope_Athena_R"));
			WeaponTable[2].push_back(FindObject("/Game/Athena/Items/Weapons/WID_Sniper_Standard_Scope_Athena_SR_Ore_T03.WID_Sniper_Standard_Scope_Athena_SR_Ore_T03"));
			WeaponTable[2].push_back(FindObject("/Game/Athena/Items/Weapons/WID_Pistol_AutoHeavyPDW_Athena_R_Ore_T03.WID_Pistol_AutoHeavyPDW_Athena_R_Ore_T03"));
			WeaponTable[2].push_back(FindObject("/Game/Athena/Items/Weapons/WID_Pistol_AutoHeavySuppressed_Athena_R_Ore_T03.WID_Pistol_AutoHeavySuppressed_Athena_R_Ore_T03"));
			WeaponTable[2].push_back(FindObject("/Game/Athena/Items/Weapons/WID_Pistol_Scavenger_Athena_R_Ore_T03.WID_Pistol_Scavenger_Athena_R_Ore_T03"));
			WeaponTable[2].push_back(FindObject("/Game/Athena/Items/Weapons/WID_Pistol_BurstFireSMG_Athena_R_Ore_T03.WID_Pistol_BurstFireSMG_Athena_R_Ore_T03"));
			WeaponTable[2].push_back(FindObject("/Game/Athena/Items/Weapons/WID_Assault_AutoDrum_Athena_R_Ore_T03.WID_Assault_AutoDrum_Athena_R_Ore_T03"));
			WeaponTable[2].push_back(FindObject("/Game/Athena/Items/Weapons/WID_Pistol_SemiAuto_Athena_R_Ore_T03.WID_Pistol_SemiAuto_Athena_R_Ore_T03"));
			WeaponTable[2].push_back(FindObject("/Game/Athena/Items/Weapons/WID_DualPistol_SemiAuto_Athena_VR_Ore_T03.WID_DualPistol_SemiAuto_Athena_VR_Ore_T03"));
			WeaponTable[2].push_back(FindObject("/Game/Athena/Items/Weapons/WID_Pistol_SixShooter_Athena_R_Ore_T03.WID_Pistol_SixShooter_Athena_R_Ore_T03"));
			WeaponTable[2].push_back(FindObject("/Game/Athena/Items/Weapons/WID_Pistol_Revolver_SingleAction_Athena_R.WID_Pistol_Revolver_SingleAction_Athena_R"));
			WeaponTable[2].push_back(FindObject("/Game/Athena/Items/Weapons/WID_Launcher_Grenade_Athena_R_Ore_T03.WID_Launcher_Grenade_Athena_R_Ore_T03"));

			WeaponTable[3].push_back(FindObject("/Game/Athena/Items/Weapons/WID_Assault_AutoHigh_Athena_VR_Ore_T03.WID_Assault_AutoHigh_Athena_VR_Ore_T03"));
			WeaponTable[3].push_back(FindObject("/Game/Athena/Items/Weapons/WID_Assault_SemiAuto_Athena_VR_Ore_T03.WID_Assault_SemiAuto_Athena_VR_Ore_T03"));
			WeaponTable[3].push_back(FindObject("/Game/Athena/Items/Weapons/WID_Assault_Suppressed_Athena_VR_Ore_T03.WID_Assault_Suppressed_Athena_VR_Ore_T03"));
			WeaponTable[3].push_back(FindObject("/Game/Athena/Items/Weapons/WID_Assault_Surgical_Thermal_Athena_VR_Ore_T03.WID_Assault_Surgical_Thermal_Athena_VR_Ore_T03"));
			WeaponTable[3].push_back(FindObject("/Game/Athena/Items/Weapons/WID_Assault_Heavy_Athena_VR_Ore_T03.WID_Assault_Heavy_Athena_VR_Ore_T03"));
			WeaponTable[3].push_back(FindObject("/Game/Athena/Items/Weapons/WID_Assault_PistolCaliber_AR_Athena_VR_Ore_T03.WID_Assault_PistolCaliber_AR_Athena_VR_Ore_T03"));
			WeaponTable[3].push_back(FindObject("/Game/Athena/Items/Weapons/WID_Shotgun_Standard_Athena_VR_Ore_T03.WID_Shotgun_Standard_Athena_VR_Ore_T03"));
			WeaponTable[3].push_back(FindObject("/Game/Athena/Items/Weapons/WID_Shotgun_HighSemiAuto_Athena_VR_Ore_T03.WID_Shotgun_HighSemiAuto_Athena_VR_Ore_T03"));
			WeaponTable[3].push_back(FindObject("/Game/Athena/Items/Weapons/WID_Shotgun_SlugFire_Athena_VR.WID_Shotgun_SlugFire_Athena_VR"));
			WeaponTable[3].push_back(FindObject("/Game/Athena/Items/Weapons/WID_Shotgun_Combat_Athena_VR_Ore_T03.WID_Shotgun_Combat_Athena_VR_Ore_T03"));
			WeaponTable[3].push_back(FindObject("/Game/Athena/Items/Weapons/WID_Shotgun_BreakBarrel_Athena_VR_Ore_T03.WID_Shotgun_BreakBarrel_Athena_VR_Ore_T03"));
			WeaponTable[3].push_back(FindObject("/Game/Athena/Items/Weapons/WID_Sniper_BoltAction_Scope_Athena_VR_Ore_T03.WID_Sniper_BoltAction_Scope_Athena_VR_Ore_T03"));
			WeaponTable[3].push_back(FindObject("/Game/Athena/Items/Weapons/WID_Sniper_Suppressed_Scope_Athena_VR_Ore_T03.WID_Sniper_Suppressed_Scope_Athena_VR_Ore_T03"));
			WeaponTable[3].push_back(FindObject("/Game/Athena/Items/Weapons/WID_Sniper_Auto_Suppressed_Scope_Athena_VR.WID_Sniper_Auto_Suppressed_Scope_Athena_VR"));
			WeaponTable[3].push_back(FindObject("/Game/Athena/Items/Weapons/WID_Pistol_AutoHeavyPDW_Athena_VR_Ore_T03.WID_Pistol_AutoHeavyPDW_Athena_VR_Ore_T03"));
			WeaponTable[3].push_back(FindObject("/Game/Athena/Items/Weapons/WID_Pistol_Scavenger_Athena_VR_Ore_T03.WID_Pistol_Scavenger_Athena_VR_Ore_T03"));
			WeaponTable[3].push_back(FindObject("/Game/Athena/Items/Weapons/WID_Pistol_AutoHeavyPDW_Athena_VR_Ore_T03.WID_Pistol_AutoHeavyPDW_Athena_VR_Ore_T03"));
			WeaponTable[3].push_back(FindObject("/Game/Athena/Items/Weapons/WID_Pistol_Standard_Athena_VR.WID_Pistol_Standard_Athena_VR"));
			WeaponTable[3].push_back(FindObject("/Game/Athena/Items/Weapons/WID_Pistol_SemiAuto_Athena_SR_Ore_T03.WID_Pistol_SemiAuto_Athena_SR_Ore_T03"));
			WeaponTable[3].push_back(FindObject("/Game/Athena/Items/Weapons/WID_DualPistol_SemiAuto_Athena_SR_Ore_T03.WID_DualPistol_SemiAuto_Athena_SR_Ore_T03"));
			WeaponTable[3].push_back(FindObject("/Game/Athena/Items/Weapons/WID_DualPistol_Suppresseed_Athena_VR_T01.WID_DualPistol_Suppresseed_Athena_VR_T01"));
			WeaponTable[3].push_back(FindObject("/Game/Athena/Items/Weapons/WID_Pistol_Scoped_Athena_VR_Ore_T03.WID_Pistol_Scoped_Athena_VR_Ore_T03"));
			WeaponTable[3].push_back(FindObject("/Game/Athena/Items/Weapons/WID_Pistol_HandCannon_Athena_VR_Ore_T03.WID_Pistol_HandCannon_Athena_VR_Ore_T03"));
			WeaponTable[3].push_back(FindObject("/Game/Athena/Items/Weapons/WID_Pistol_Revolver_SingleAction_Athena_VR.WID_Pistol_Revolver_SingleAction_Athena_VR"));
			WeaponTable[3].push_back(FindObject("/Game/Athena/Items/Weapons/WID_Launcher_Rocket_Athena_VR_Ore_T03.WID_Launcher_Rocket_Athena_VR_Ore_T03"));
			WeaponTable[3].push_back(FindObject("/Game/Athena/Items/Weapons/WID_Launcher_Grenade_Athena_VR_Ore_T03.WID_Launcher_Grenade_Athena_VR_Ore_T03"));
			WeaponTable[3].push_back(FindObject("/Game/Athena/Items/Weapons/WID_Launcher_Military_Athena_VR_Ore_T03.WID_Launcher_Military_Athena_VR_Ore_T03"));

			WeaponTable[4].push_back(FindObject("/Game/Athena/Items/Weapons/WID_Assault_AutoHigh_Athena_SR_Ore_T03.WID_Assault_AutoHigh_Athena_SR_Ore_T03"));
			WeaponTable[4].push_back(FindObject("/Game/Athena/Items/Weapons/WID_Assault_SemiAuto_Athena_SR_Ore_T03.WID_Assault_SemiAuto_Athena_SR_Ore_T03"));
			WeaponTable[4].push_back(FindObject("/Game/Athena/Items/Weapons/WID_Assault_Suppressed_Athena_SR_Ore_T03.WID_Assault_Suppressed_Athena_SR_Ore_T03"));
			WeaponTable[4].push_back(FindObject("/Game/Athena/Items/Weapons/WID_Assault_Surgical_Thermal_Athena_SR_Ore_T03.WID_Assault_Surgical_Thermal_Athena_SR_Ore_T03"));
			WeaponTable[4].push_back(FindObject("/Game/Athena/Items/Weapons/WID_Assault_Heavy_Athena_SR_Ore_T03.WID_Assault_Heavy_Athena_SR_Ore_T03"));
			WeaponTable[4].push_back(FindObject("/Game/Athena/Items/Weapons/WID_Assault_PistolCaliber_AR_Athena_SR_Ore_T03.WID_Assault_PistolCaliber_AR_Athena_SR_Ore_T03"));
			WeaponTable[4].push_back(FindObject("/Game/Athena/Items/Weapons/WID_Shotgun_Standard_Athena_SR_Ore_T03.WID_Shotgun_Standard_Athena_SR_Ore_T03"));
			WeaponTable[4].push_back(FindObject("/Game/Athena/Items/Weapons/WID_Shotgun_HighSemiAuto_Athena_SR_Ore_T03.WID_Shotgun_HighSemiAuto_Athena_SR_Ore_T03"));
			WeaponTable[4].push_back(FindObject("/Game/Athena/Items/Weapons/WID_Shotgun_SlugFire_Athena_SR.WID_Shotgun_SlugFire_Athena_SR"));
			WeaponTable[4].push_back(FindObject("/Game/Athena/Items/Weapons/WID_Shotgun_BreakBarrel_Athena_SR_Ore_T03.WID_Shotgun_BreakBarrel_Athena_SR_Ore_T03"));
			WeaponTable[4].push_back(FindObject("/Game/Athena/Items/Weapons/WID_Sniper_BoltAction_Scope_Athena_SR_Ore_T03.WID_Sniper_BoltAction_Scope_Athena_SR_Ore_T03"));
			WeaponTable[4].push_back(FindObject("/Game/Athena/Items/Weapons/WID_Sniper_Heavy_Athena_SR_Ore_T03.WID_Sniper_Heavy_Athena_SR_Ore_T03"));
			WeaponTable[4].push_back(FindObject("/Game/Athena/Items/Weapons/WID_Sniper_Suppressed_Scope_Athena_SR_Ore_T03.WID_Sniper_Suppressed_Scope_Athena_SR_Ore_T03"));
			WeaponTable[4].push_back(FindObject("/Game/Athena/Items/Weapons/WID_Sniper_Auto_Suppressed_Scope_Athena_SR.WID_Sniper_Auto_Suppressed_Scope_Athena_SR"));
			WeaponTable[4].push_back(FindObject("/Game/Athena/Items/Weapons/WID_Pistol_AutoHeavyPDW_Athena_SR_Ore_T03.WID_Pistol_AutoHeavyPDW_Athena_SR_Ore_T03"));
			WeaponTable[4].push_back(FindObject("/Game/Athena/Items/Weapons/WID_Assault_LMG_Athena_SR_Ore_T03.WID_Assault_LMG_Athena_SR_Ore_T03"));
			WeaponTable[4].push_back(FindObject("/Game/Athena/Items/Weapons/WID_Pistol_Standard_Athena_SR.WID_Pistol_Standard_Athena_SR"));
			WeaponTable[4].push_back(FindObject("/Game/Athena/Items/Weapons/WID_Pistol_HandCannon_Athena_SR_Ore_T03.WID_Pistol_HandCannon_Athena_SR_Ore_T03"));
			WeaponTable[4].push_back(FindObject("/Game/Athena/Items/Weapons/WID_Launcher_Rocket_Athena_SR_Ore_T03.WID_Launcher_Rocket_Athena_SR_Ore_T03"));
			WeaponTable[4].push_back(FindObject("/Game/Athena/Items/Weapons/WID_Launcher_Grenade_Athena_SR_Ore_T03.WID_Launcher_Grenade_Athena_SR_Ore_T03"));
			WeaponTable[4].push_back(FindObject("/Game/Athena/Items/Weapons/WID_Launcher_Military_Athena_SR_Ore_T03.WID_Launcher_Military_Athena_SR_Ore_T03"));

			ConsumableTable.push_back(FindObject("/Game/Athena/Items/Consumables/Bandage/Athena_Bandage.Athena_Bandage"));
			ConsumableTable.push_back(FindObject("/Game/Athena/Items/Consumables/Medkit/Athena_Medkit.Athena_Medkit"));
			ConsumableTable.push_back(FindObject("/Game/Athena/Items/Consumables/Shields/Athena_Shields.Athena_Shields"));
			ConsumableTable.push_back(FindObject("/Game/Athena/Items/Consumables/ShieldSmall/Athena_ShieldSmall.Athena_ShieldSmall"));
			ConsumableTable.push_back(FindObject("/Game/Athena/Items/Consumables/PurpleStuff/Athena_PurpleStuff.Athena_PurpleStuff"));
			ConsumableTable.push_back(FindObject("/Game/Athena/Items/Consumables/SuperMedKit/Athena_SuperMedkit.Athena_SuperMedkit"));
			ConsumableTable.push_back(FindObject("/Game/Athena/Items/Consumables/Grenade/Athena_Grenade.Athena_Grenade"));
			ConsumableTable.push_back(FindObject("/Game/Athena/Items/Consumables/KnockGrenade/Athena_KnockGrenade.Athena_KnockGrenade"));
			ConsumableTable.push_back(FindObject("/Game/Athena/Items/Consumables/ShockwaveGrenade/Athena_ShockGrenade.Athena_ShockGrenade"));
			ConsumableTable.push_back(FindObject("/Game/Athena/Items/Consumables/StickyGrenade/Athena_StickyGrenade.Athena_StickyGrenade"));
			ConsumableTable.push_back(FindObject("/Game/Athena/Items/Consumables/ThrownConsumables/Athena_IceGrenade.Athena_IceGrenade"));
			ConsumableTable.push_back(FindObject("/Game/Athena/Items/Consumables/Bush/Athena_Bush.Athena_Bush"));

			AmmoTable.push_back(FindObject("/Game/Athena/Items/Ammo/AthenaAmmoDataRockets.AthenaAmmoDataRockets"));
			AmmoTable.push_back(FindObject("/Game/Athena/Items/Ammo/AthenaAmmoDataShells.AthenaAmmoDataShells"));
			AmmoTable.push_back(FindObject("/Game/Athena/Items/Ammo/AthenaAmmoDataBulletsMedium.AthenaAmmoDataBulletsMedium"));
			AmmoTable.push_back(FindObject("/Game/Athena/Items/Ammo/AthenaAmmoDataBulletsLight.AthenaAmmoDataBulletsLight"));
			AmmoTable.push_back(FindObject("/Game/Athena/Items/Ammo/AthenaAmmoDataBulletsHeavy.AthenaAmmoDataBulletsHeavy"));

			return 0;
		}

		static DWORD WINAPI SpawnFloorLoot(LPVOID)
		{
			Init(nullptr);
			// static UObject* BuildingContainerClass = FindObject(_("Class /Script/FortniteGame.BuildingContainer"));
			static auto FloorLootClass = FindObject(_("BlueprintGeneratedClass /Game/Athena/Environments/Blueprints/Tiered_Athena_FloorLoot_01.Tiered_Athena_FloorLoot_01_C"));
			auto Actors = Helper::GetAllActorsOfClass(FloorLootClass); // BuildingContainerClass);

			std::cout << _("Spawning ") << Actors.Num() << _(" floor loot!\n");

			for (int i = 0; i < Actors.Num(); i++)
			{
				auto Actor = Actors[i];

				// if (Actor->GetName().contains("Tiered_Athena_FloorLoot"))
				if (Actor)
				{
					auto Random = rand() % 10;
					auto bIsConsumable = Random == 1 || Random == 2 || Random == 3 || Random == 0;

					if (bIsConsumable)
					{
						auto Def = Tables::GetConsumableDef();
						if (Def)
							Helper::SummonPickup(nullptr, Def, Helper::GetActorLocation(Actor), EFortPickupSourceTypeFlag::FloorLoot, EFortPickupSpawnSource::Unset, *Def->Member<int>(_("DropCount")));
					}
					else
					{
						auto WeaponDef = Tables::GetWeaponDef();
						if (WeaponDef)
							Helper::SummonPickup(nullptr, WeaponDef, Helper::GetActorLocation(Actor), EFortPickupSourceTypeFlag::FloorLoot, EFortPickupSpawnSource::Unset, *WeaponDef->Member<int>(_("DropCount")));
					}
				}
			}

			std::cout << _("Spawned ") << Actors.Num() << _(" floor loot!\n");

			return 0;
		}

		static void HandleSearch(UObject* BuildingContainer)
		{
			if (BuildingContainer && BuildingContainer->GetName().contains("Tiered_Chest"))
			{
				auto WeaponDef = Tables::GetWeaponDef();
				if (WeaponDef)
				{
					static auto GetAmmoWorldItemDefinition_BP = WeaponDef->Function(_("GetAmmoWorldItemDefinition_BP"));
					if (GetAmmoWorldItemDefinition_BP)
					{
						// UObject* AmmoDef = nullptr;
						struct { UObject* ammo; }ahh{};
						WeaponDef->ProcessEvent(GetAmmoWorldItemDefinition_BP, &ahh);
						auto AmmoDef = ahh.ammo;
						// if (AmmoDef)
						{
							auto Location = Helper::GetActorLocation(BuildingContainer);
							Location.Z += 50;

							if (WeaponDef)
								Helper::SummonPickup(nullptr, WeaponDef, Location, EFortPickupSourceTypeFlag::Container, EFortPickupSpawnSource::Chest, 1);
							if (AmmoDef)
							{
								auto DropCount = *AmmoDef->Member<int>(_("DropCount"));
								try {
									Helper::SummonPickup(nullptr, AmmoDef, Location, EFortPickupSourceTypeFlag::Container, EFortPickupSpawnSource::Chest, DropCount);
								}
								catch (...)
								{
									std::cout << _("Failed to spawn consumable!\n");
								}
							}

							if (auto Consumable = Tables::GetConsumableDef())
							{
								try {
									Helper::SummonPickup(nullptr, Consumable, Location, EFortPickupSourceTypeFlag::Container, EFortPickupSpawnSource::Chest, *Consumable->Member<int>(_("DropCount")));
								}
								catch (...)
								{
									std::cout << _("Failed to spawn consumable!\n");
								}
							}
						}
					}
				}
			}
		}
	}
}