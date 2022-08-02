#pragma once

#include <UE/structs.h>
#include <functional>

#include "color.hpp"
#include <Net/server.h>
#include <Gameplay/helper.h>
#include <Net/funcs.h>
#include <Net/nethooks.h>
#include <Gameplay/abilities.h>
#include <Gameplay/events.h>

#include <mutex>
#include <Gameplay/player.h>

#define LOGGING

// HEAVILY INSPIRED BY KEMOS UFUNCTION HOOKING

static bool bStarted = false;

inline void initStuff()
{
	if (!bStarted && bTraveled)
	{
		bStarted = true;

		CreateThread(0, 0, Helper::Console::Setup, 0, 0, 0);

		auto world = Helper::GetWorld();
		auto gameState = *world->Member<UObject*>(_("GameState"));

		if (gameState)
		{
			auto AuthGameMode = *world->Member<UObject*>(_("AuthorityGameMode"));

			*(*AuthGameMode->Member<UObject*>(_("GameSession")))->Member<int>(_("MaxPlayers")) = 100;

			if (std::stod(FN_Version) >= 8 && AuthGameMode)
			{
				static auto PlayerControllerClass = FindObject(_("BlueprintGeneratedClass /Game/Athena/Athena_PlayerController.Athena_PlayerController_C"));
				std::cout << _("PlayerControllerClass: ") << PlayerControllerClass << '\n';
				*AuthGameMode->Member<UObject*>(_("PlayerControllerClass")) = PlayerControllerClass;
			}

			*gameState->Member<char>(_("bGameModeWillSkipAircraft")) = false;
			*gameState->Member<float>(_("AircraftStartTime")) = 99999.0f;
			*gameState->Member<float>(_("WarmupCountdownEndTime")) = 99999.0f;

			*gameState->Member<EFriendlyFireType>(_("FriendlyFireType")) = EFriendlyFireType::On;

			if (Engine_Version >= 420)
			{
				*gameState->Member<EAthenaGamePhase>(_("GamePhase")) = EAthenaGamePhase::Warmup;

				struct {
					EAthenaGamePhase OldPhase;
				} params2{ EAthenaGamePhase::None };

				static const auto fnGamephase = gameState->Function(_("OnRep_GamePhase"));

				if (fnGamephase)
					gameState->ProcessEvent(fnGamephase, &params2);
			}

			if (AuthGameMode)
			{
				AuthGameMode->ProcessEvent(AuthGameMode->Function(_("StartPlay")), nullptr);

				auto FNVer = std::stod(FN_Version);

				if (std::floor(FNVer) == 3 || FNVer >= 8.0) {
					//If This is called on Seasons 4, 6, or 7 then Setting The Playlist Crashes.
					AuthGameMode->ProcessEvent(AuthGameMode->Function(_("StartMatch")), nullptr);
				}
				// *AuthGameMode->Member<bool>(_("bAlwaysDBNO")) = true;

				// Is this correct?
				/*class UClass* VehicleClass = (UClass*)(FindObject(_("Class /Script/FortniteGame.FortAthenaFerretVehicle")));//(UClass*(FindObject(_("Class /Script/FortniteGame.FortAthenaFerretVehicle.FortAthenaFerretVehicle"))));
				struct 
				{
					class UClass* VehicleClass;
				} SSVParams{ VehicleClass };
				
				auto SSVFn = *AuthGameMode->Member<bool>(_("ShouldSpawnVehicle")) = true;*/
				
				// static auto Playlist = FindObject(_("FortPlaylistAthena /Game/Athena/Playlists/Playlist_DefaultSolo.Playlist_DefaultSolo"));
				// static auto Playlist = FindObject(_("FortPlaylistAthena /Game/Athena/Playlists/Playlist_DefaultDuo.Playlist_DefaultDuo"));
				// static auto Playlist = FindObject(_("FortPlaylistAthena /Game/Athena/Playlists/Playlist_DefaultSquad.Playlist_DefaultSquad"));
				static auto Playlist = FindObject(_("FortPlaylistAthena /Game/Athena/Playlists/Playground/Playlist_Playground.Playlist_Playground"));
				// static auto Playlist = FindObject(_("/Game/Athena/Playlists/Fill/Playlist_Fill_Solo.Playlist_Fill_Solo"));
				 
				if (std::stod(FN_Version) >= 7.00)
				{
					auto OnRepPlaylist = gameState->Function(_("OnRep_CurrentPlaylistInfo"));

					static auto BasePlaylistOffset = FindOffsetStruct(_("ScriptStruct /Script/FortniteGame.PlaylistPropertyArray"), _("BasePlaylist"));
					static auto PlaylistReplicationKeyOffset = FindOffsetStruct(_("ScriptStruct /Script/FortniteGame.PlaylistPropertyArray"), _("PlaylistReplicationKey"));

					if (BasePlaylistOffset && OnRepPlaylist && Playlist)
					{
						if (Engine_Version <= 422)
						{
							static auto PlaylistInfo = gameState->Member<FFastArraySerializerOL>(_("CurrentPlaylistInfo"));

							auto BasePlaylist = (UObject**)(__int64(PlaylistInfo) + BasePlaylistOffset);// *gameState->Member<UObject>(_("CurrentPlaylistInfo"))->Member<UObject*>(_("BasePlaylist"), true);
							auto PlaylistReplicationKey = (int*)(__int64(PlaylistInfo) + PlaylistReplicationKeyOffset);

							if (BasePlaylist)
							{
								*BasePlaylist = Playlist;
								(*PlaylistReplicationKey)++;
								PlaylistInfo->MarkArrayDirty();
								std::cout << _("Set playlist to: ") << Playlist->GetFullName() << '\n';
							}
							else
								std::cout << _("Base Playlist is null!\n");
						}
						else
						{
							static auto PlaylistInfo = gameState->Member<FFastArraySerializerSE>(_("CurrentPlaylistInfo"));

							auto BasePlaylist = (UObject**)(__int64(PlaylistInfo) + BasePlaylistOffset);// *gameState->Member<UObject>(_("CurrentPlaylistInfo"))->Member<UObject*>(_("BasePlaylist"), true);
							auto PlaylistReplicationKey = (int*)(__int64(PlaylistInfo) + PlaylistReplicationKeyOffset);

							if (BasePlaylist)
							{
								*BasePlaylist = Playlist;
								(*PlaylistReplicationKey)++;
								PlaylistInfo->MarkArrayDirty();
								std::cout << _("Set playlist to: ") << Playlist->GetFullName() << '\n';
							}
							else
								std::cout << _("Base Playlist is null!\n");
						}
					}
					else
					{
						std::cout << _("Missing something related to the Playlist!\n");
						std::cout << _("BasePlaylist Offset: ") << BasePlaylistOffset << '\n';
						std::cout << _("OnRepPlaylist: ") << OnRepPlaylist << '\n';
						std::cout << _("Playlist: ") << Playlist << '\n';
					}

					gameState->ProcessEvent(OnRepPlaylist);
				}
				else
				{
					if (Playlist) {
						static auto OnRepPlaylist = gameState->Function(_("OnRep_CurrentPlaylistData"));
						*gameState->Member<UObject*>(_("CurrentPlaylistData")) = Playlist;

						if (OnRepPlaylist)
							gameState->ProcessEvent(OnRepPlaylist);
					}
					else {
						std::cout << _("Playlist is NULL") << '\n';
					}
				}
			}
			else
			{
				std::cout << dye::yellow(_("[WARNING] ")) << _("Failed to find AuthorityGameMode!\n");
			}

			if (Engine_Version != 421)
			{
				auto PlayersLeft = gameState->Member<int>(_("PlayersLeft"));

				if (PlayersLeft && *PlayersLeft)
					*PlayersLeft = 0;

				static auto OnRep_PlayersLeft = gameState->Function(_("OnRep_PlayersLeft"));

				if (OnRep_PlayersLeft)
					gameState->ProcessEvent(OnRep_PlayersLeft);
			}

			if (Helper::IsRespawnEnabled())
			{
				// Enable glider redeploy (idk if this works)
				FString GliderRedeployCmd;
				GliderRedeployCmd.Set(L"Athena.EnableParachuteEverywhere 1");
				Helper::Console::ExecuteConsoleCommand(GliderRedeployCmd);
			}
		}

		Listen(7777);
		// CreateThread(0, 0, MapLoadThread, 0, 0, 0);

		InitializeNetHooks();

		std::cout << _("Initialized NetHooks!\n");

		if (Engine_Version >= 420) {
			Events::LoadEvents();
			Helper::FixPOIs();
		}
	}
}

bool ServerLoadingScreenDroppedHook(UObject* PlayerController, UFunction* Function, void* Parameters)
{
	auto PlayerState = *PlayerController->Member<UObject*>(_("PlayerState"));
	auto Pawn = *PlayerController->Member<UObject*>(_("Pawn"));

	return false;
}

bool ServerUpdatePhysicsParamsHook(UObject* Vehicle, UFunction* Function, void* Parameters) // FortAthenaVehicle
{
	if (Vehicle && Parameters)
	{
		/* struct parms { __int64 InState; };
		auto Params = (parms*)Parameters;

		auto TranslationOffset = FindOffsetStruct(_("ScriptStruct /Script/FortniteGame.ReplicatedAthenaVehiclePhysicsState"), _("Translation"));
		auto Translation = (FVector*)(__int64(&Params->InState) + TranslationOffset);

		std::cout << _("TranslationOffset: ") << TranslationOffset << '\n';

		auto RotationOffset = FindOffsetStruct(_("ScriptStruct /Script/FortniteGame.ReplicatedAthenaVehiclePhysicsState"), _("Rotation"));
		auto Rotation = (FQuat*)(__int64(&Params->InState) + RotationOffset);

		if (Translation && Rotation)
		{
			std::cout << _("X: ") << Translation->X << '\n';
			std::cout << _("Y: ") << Translation->Y << '\n';
			std::cout << _("Z: ") << Translation->Z << '\n';

			// Helper::SetActorLocation(Vehicle, *Translation);
			auto rot = Rotation->Rotator();
			Helper::SetActorLocationAndRotation(Vehicle, *Translation, rot);
		} */
	}

	return false;
}

bool ServerAttemptAircraftJumpHook(UObject* PlayerController, UFunction* Function, void* Parameters)
{
	struct Param{
		FRotator                                    ClientRotation;
	};

	auto Params = (Param*)Parameters;

	if (PlayerController && Params) // PlayerController->IsInAircraft()
	{
		static auto world = Helper::GetWorld();
		static auto GameState = *world->Member<UObject*>(_("GameState"));

		if (GameState)
		{
			auto Aircraft = GameState->Member<TArray<UObject*>>(_("Aircrafts"))->At(0);

			if (Aircraft)
			{
				auto ExitLocation = Helper::GetActorLocation(Aircraft);

				Helper::InitPawn(PlayerController, false, ExitLocation);

				// ClientSetRotation
			}
		}
	}

	return false;
}

bool ReadyToStartMatchHook(UObject* Object, UFunction* Function, void* Parameters)
{
	initStuff();
	return false;
}

void LoadInMatch()
{
	auto GameInstance = *GetEngine()->Member<UObject*>(_("GameInstance"));
	auto& LocalPlayers = *GameInstance->Member<TArray<UObject*>>(_("LocalPlayers"));
	auto PlayerController = *LocalPlayers.At(0)->Member<UObject*>(_("PlayerController"));

	if (PlayerController)
	{
		static auto SwitchLevelFn = PlayerController->Function(_("SwitchLevel"));
		FString Map;
		Map.Set(GetMapName());
		PlayerController->ProcessEvent(SwitchLevelFn, &Map);
		// Map.Free();
		bTraveled = true;
	}
	else
	{
		std::cout << dye::red(_("[ERROR] ")) << _("Unable to find PlayerController!\n");
	}
}
// HEAVILY INSPIRED BY RAIDER 
inline bool ServerCheatHook(UObject* Controller, UFunction* Function, void* Parameters)
{
	struct SCParams { FString* Msg; };
	auto MsgParams = (SCParams*)Parameters;

	if (Controller && MsgParams && MsgParams->Msg)
	{
		auto Message = MsgParams->Msg->ToString() + ' ';
		std::vector<std::string> Args;
		while (Message.find(" ") != -1)
		{
			Args.push_back(Message.substr(0, Message.find(' ')));
			Message.erase(0, Message.find(' ') + 1);
		}
		auto ArgsNum = Args.size() - 1;

		if (ArgsNum >= 0)
		{
			auto& Command = Args[0];
			std::transform(Command.begin(), Command.end(), Command.begin(), ::tolower);

			if (Command == "giveitem" && ArgsNum >= 1)
			{
				auto& weaponName = Args[1];
				int slot = 1;
				int count = 1;
				try
				{
					if (ArgsNum >= 2)
						slot = std::stoi(Args[2]);

					if (ArgsNum >= 3)
						count = std::stoi(Args[3]);
				}
				catch (...)
				{
				}
				auto WID = FindObject("FortWeaponRangedItemDefinition " + weaponName + '.' + weaponName);
				if (WID)
				{
					Inventory::CreateAndAddItem(Controller, WID, EFortQuickBars::Primary, slot, count);
					std::cout << "Gave weapon!\n";
				}
			}
			// Should work i think
			else if (Command == "launchplayer" && ArgsNum >= 0)
			{
				auto Pawn = *Controller->Member<UObject*>("Pawn");
				FVector LaunchVelocity{ 0, 0, 5000 };
				bool bXYOveride = false;
				bool bZOverride = false;
				bool bIgnoreFallDamage = true;
				bool bPlayFeedbackEvent = false;
				
				Helper::LaunchPlayer(Pawn, LaunchVelocity, bXYOveride = false, bZOverride = false, bIgnoreFallDamage = true, bPlayFeedbackEvent = false);
			}
			else if (Command == "revive")
			{
				Controller->ProcessEvent(_("Suicide"));
				/* auto Pawn = Controller->Member<UObject>("Pawn");
				bool bIsDBNO = Pawn->Member<UObject>("bIsDBNO");
				bool bIsDBNO = false;
				auto OnRep_DBNOFn = Pawn->Function("OnRep_IsDBNO");
				Pawn->ProcessEvent(OnRep_DBNOFn);
				float NewHealth = 100.0f;
				struct 
				{
					float NewHealth;
				} SHParams{ NewHealth };
				auto SHFn = Pawn->Function("SetHealth");
				Pawn->ProcessEvent(SHFn, &SHParams); */
			}
		}
	}
	return true;
}

EDeathCause GetDeathCause(FGameplayTagContainer Tags) // Credits: Pakchunk on github, from raider3.5
{
	static std::map<std::string, EDeathCause> DeathCauses{
		{ "weapon.ranged.shotgun", EDeathCause::Shotgun },
		{ "weapon.ranged.assault", EDeathCause::Rifle },
		{ "Gameplay.Damage.Environment.Falling", EDeathCause::FallDamage },
		{ "weapon.ranged.sniper", EDeathCause::Sniper },
		{ "Weapon.Ranged.SMG", EDeathCause::SMG },
		{ "weapon.ranged.heavy.rocket_launcher", EDeathCause::RocketLauncher },
		{ "weapon.ranged.heavy.grenade_launcher", EDeathCause::GrenadeLauncher },
		{ "Weapon.ranged.heavy.grenade", EDeathCause::Grenade },
		{ "Weapon.Ranged.Heavy.Minigun", EDeathCause::Minigun },
		{ "Weapon.Ranged.Crossbow", EDeathCause::Bow },
		{ "trap.floor", EDeathCause::Trap },
		{ "weapon.ranged.pistol", EDeathCause::Pistol },
		{ "Gameplay.Damage.OutsideSafeZone", EDeathCause::OutsideSafeZone },
		{ "Weapon.Melee.Impact.Pickaxe", EDeathCause::Melee }
	};

	for (int i = 0; i < Tags.GameplayTags.Num(); i++)
	{
		auto TagName = Tags.GameplayTags.At(i).TagName;

		if (TagName.ComparisonIndex == 0 || TagName.Number == 0)
			continue;

		for (auto& Map : DeathCauses)
		{
			if (TagName.ToString() == Map.first) return Map.second;
			else continue;
		}
	}

	std::cout << std::format("Unspecified Death: {}\n", Tags.ToStringSimple(false));

	return EDeathCause::Unspecified;
}

inline bool ClientOnPawnDiedHook(UObject* DeadPC, UFunction* Function, void* Parameters)
{
	if (DeadPC && Parameters)
	{
		auto DeadPawn = *DeadPC->Member<UObject*>(_("Pawn"));
		auto DeadPlayerState = *DeadPC->Member<UObject*>(_("PlayerState"));

		struct parms { __int64 DeathReport; };

		auto Params = (parms*)Parameters;

		if (Helper::IsRespawnEnabled())
			Player::RespawnPlayer(DeadPC);
		else
		{
			// PlayersLeft--;

			if (Engine_Version >= 423) // wrong
			{
				auto Chip = Helper::SpawnChip(DeadPC);

				if (Chip)
				{
					// 0x0018
					struct FFortResurrectionData
					{
						bool                                               bResurrectionChipAvailable;                               // 0x0000(0x0001) (ZeroConstructor, IsPlainOldData)
						unsigned char                                      UnknownData00[0x3];                                       // 0x0001(0x0003) MISSED OFFSET
						float                                              ResurrectionExpirationTime;                               // 0x0004(0x0004) (ZeroConstructor, IsPlainOldData)
						float                                              ResurrectionExpirationLength;                             // 0x0008(0x0004) (ZeroConstructor, IsPlainOldData)
						struct FVector                                     WorldLocation;                                            // 0x000C(0x000C) (ZeroConstructor, IsPlainOldData)
					};

					auto PlayerState = *DeadPawn->Member<UObject*>(_("PlayerState"));
					auto FortResurrectionData = PlayerState->Member<FFortResurrectionData>(_("ResurrectionChipAvailable"));

					if (FortResurrectionData)
					{
						std::cout << _("FortResurrectionData valid!\n");
						std::cout << _("FortResurrectionData Location X: ") << FortResurrectionData->WorldLocation.X << '\n';
					}
					else
						std::cout << _("No FortResurrectionData!\n");

					auto ResurrectionData = FFortResurrectionData{};
					ResurrectionData.bResurrectionChipAvailable = true;
					ResurrectionData.ResurrectionExpirationLength = 99999.f;
					ResurrectionData.ResurrectionExpirationTime = 99999.f;
					ResurrectionData.WorldLocation = Helper::GetActorLocation(Chip);

					if (FortResurrectionData)
						*FortResurrectionData = ResurrectionData;

					std::cout << _("Spawned Chip!\n");
				}
			}
		}

		static auto KillerPawnOffset = FindOffsetStruct(_("ScriptStruct /Script/FortniteGame.FortPlayerDeathReport"), _("KillerPawn"));
		static auto KillerPlayerStateOffset = FindOffsetStruct(_("ScriptStruct /Script/FortniteGame.FortPlayerDeathReport"), _("KillerPlayerState"));

		auto KillerPawn = *(UObject**)(__int64(&Params->DeathReport) + KillerPawnOffset);
		auto KillerPlayerState = *(UObject**)(__int64(&Params->DeathReport) + KillerPlayerStateOffset);

		UObject* KillerController = nullptr;

		if (KillerPawn)
			KillerController = *KillerPawn->Member<UObject*>(_("Controller"));

		if (KillerPlayerState && KillerPlayerState != DeadPlayerState)
		{
			if (KillerController)
			{
				static auto ClientReceiveKillNotification = KillerController->Function(_("ClientReceiveKillNotification"));

				struct {
					// Both playerstates
					UObject* Killer;
					UObject* Killed;
				} ClientReceiveKillNotification_Params{KillerPlayerState, DeadPlayerState};

				if (ClientReceiveKillNotification)
					KillerController->ProcessEvent(ClientReceiveKillNotification, &ClientReceiveKillNotification_Params);
			}

			auto DeathInfo = DeadPlayerState->Member<__int64>(_("DeathInfo"));

			static auto TagsOffset = FindOffsetStruct(_("ScriptStruct /Script/FortniteGame.DeathInfo"), _("Tags"));
			static auto DeathCauseOffset = FindOffsetStruct(_("ScriptStruct /Script/FortniteGame.DeathInfo"), _("DeathCause"));
			static auto FinisherOrDownerOffset = FindOffsetStruct(_("ScriptStruct /Script/FortniteGame.DeathInfo"), _("FinisherOrDowner"));
			static auto bDBNOOffset = FindOffsetStruct(_("ScriptStruct /Script/FortniteGame.DeathInfo"), _("bDBNO"));

			auto Tags = (FGameplayTagContainer*)(__int64(&*DeathInfo) + TagsOffset);

			*(EDeathCause*)(__int64(&*DeathInfo) + DeathCauseOffset) = EDeathCause::Cube; // Tags ? GetDeathCause(*Tags) : EDeathCause::Unspecified;
			*(UObject**)(__int64(&*DeathInfo) + FinisherOrDownerOffset) = KillerPlayerState ? KillerPlayerState : DeadPlayerState;
			*(bool*)(__int64(&*DeathInfo) + bDBNOOffset) = false;

			static auto OnRep_DeathInfo = DeadPlayerState->Function(_("OnRep_DeathInfo"));

			if (OnRep_DeathInfo)
				DeadPlayerState->ProcessEvent(OnRep_DeathInfo);

			// *DeadPlayerState->Member<__int64>(_("DeathInfo")) = *(__int64*)malloc(GetSizeOfStruct(FindObject(_("ScriptStruct /Script/FortniteGame.DeathInfo"))));

			(*KillerPlayerState->Member<int>(_("KillScore")))++;
			(*KillerPlayerState->Member<int>(_("TeamKillScore")))++;

			static auto ClientReportKill = KillerPlayerState->Function(_("ClientReportKill"));
			struct { UObject* PlayerState; }ClientReportKill_Params{DeadPlayerState};
			if (ClientReportKill)
				KillerPlayerState->ProcessEvent(ClientReportKill, &ClientReportKill_Params);

			static auto OnRep_Kills = KillerPlayerState->Function(_("OnRep_Kills"));
			if (OnRep_Kills)
				KillerPlayerState->ProcessEvent(OnRep_Kills);
		}
	}
	
	return true;
}



inline bool ServerAttemptExitVehicleHook(UObject* Controller, UFunction* Function, void* Parameters)
{
	auto Pawn = Controller->Member<UObject*>(_("Pawn"));

	if (Pawn && *Pawn)
	{
		UObject* Vehicle = Helper::GetVehicle(*Pawn);

		if (Vehicle)
		{
			Helper::SetLocalRole(*Pawn, ENetRole::ROLE_Authority);
			// Helper::SetLocalRole(Vehicle, ENetRole::ROLE_Authority);
		}
	}

	return false;
}

inline bool ServerPlayEmoteItemHook(UObject* Controller, UFunction* Function, void* Parameters)
{
	auto Pawn = *Controller->Member<UObject*>(_("Pawn"));

	struct SPEIParams  { UObject* EmoteAsset; }; // UFortMontageItemDefinitionBase
	auto EmoteParams = (SPEIParams*)Parameters;

	auto EmoteAsset = EmoteParams->EmoteAsset;

	if (Controller /* && !Controller->IsInAircraft() */ && Pawn && EmoteAsset)
	{
		struct {
			TEnumAsByte<EFortCustomBodyType> BodyType;
			TEnumAsByte<EFortCustomGender> Gender;
			UObject* AnimMontage; // UAnimMontage
		} GAHRParams{EFortCustomBodyType::All, EFortCustomGender::Both}; // (CurrentPawn->CharacterBodyType, CurrentPawn->CharacterGender)
		static auto fn = EmoteAsset->Function(_("GetAnimationHardReference"));

		if (fn)
		{
			EmoteAsset->ProcessEvent(fn, &GAHRParams);
			auto Montage = GAHRParams.AnimMontage;
			if (Montage)
			{
				std::cout << _("Playing Montage: ") << Montage->GetFullName() << '\n';

				auto AbilitySystemComponent = *Pawn->Member<UObject*>(_("AbilitySystemComponent"));
				static auto EmoteClass = FindObject(_("BlueprintGeneratedClass /Game/Abilities/Emotes/GAB_Emote_Generic.GAB_Emote_Generic_C"));

				TArray<FGameplayAbilitySpec> Specs;

				if (Engine_Version <= 422)
					Specs = (*AbilitySystemComponent->Member<FGameplayAbilitySpecContainerOL>(_("ActivatableAbilities"))).Items;
				else
					Specs = (*AbilitySystemComponent->Member<FGameplayAbilitySpecContainerSE>(_("ActivatableAbilities"))).Items;

				UObject* DefaultObject = EmoteClass->CreateDefaultObject();

				for (int i = 0; i < Specs.Num(); i++)
				{
					auto& CurrentSpec = Specs[i];

					if (CurrentSpec.Ability == DefaultObject)
					{
						auto ActivationInfo = CurrentSpec.Ability->Member<FGameplayAbilityActivationInfo>(_("CurrentActivationInfo"));

						// Helper::SetLocalRole(Pawn, ENetRole::ROLE_SimulatedProxy);
						auto Dura = PlayMontage(AbilitySystemComponent, CurrentSpec.Ability, FGameplayAbilityActivationInfo(), Montage, 1.0f, FName(0));
						// Helper::SetLocalRole(Pawn, ENetRole::ROLE_AutonomousProxy);

						std::cout << _("Played for: ") << Dura << '\n';
					}
				}
			}
		}
	}

	return false;
}

inline bool ServerAttemptInteractHook(UObject* Controllera, UFunction* Function, void* Parameters)
{
	UObject* Controller = Controllera;

	if (Engine_Version >= 423)
	{
		Controller = Helper::GetOwnerOfComponent(Controllera);
	}

	// ProcessEventO(Controller, Function, Parameters);

	struct SAIParams {
		UObject* ReceivingActor;                                           // (Parm, ZeroConstructor, IsPlainOldData)
		UObject* InteractComponent;                                        // (Parm, ZeroConstructor, InstancedReference, IsPlainOldData)
		TEnumAsByte<ETInteractionType>                     InteractType;                                             // (Parm, ZeroConstructor, IsPlainOldData)
		UObject* OptionalObjectData;                                       // (Parm, ZeroConstructor, IsPlainOldData)
	};

	auto Params = (SAIParams*)Parameters;

	if (Params && Controller)
	{
		auto Pawn = *Controller->Member<UObject*>(_("Pawn"));
		auto ReceivingActor = Params->ReceivingActor;

		if (ReceivingActor)
		{
			auto ReceivingActorName = ReceivingActor->GetName(); // There has to be a better way, right?

			std::cout << _("ReceivingActorName: ") << ReceivingActorName << '\n';

			if (ReceivingActorName.contains(_("Tiered")))
			{
				struct BitField_Container
				{
					unsigned char                                      bAlwaysShowContainer : 1;                                 // 0x0D99(0x0001) (Edit, BlueprintVisible)
					unsigned char                                      bAlwaysMaintainLoot : 1;                                  // 0x0D99(0x0001) (Edit, DisableEditOnTemplate)
					unsigned char                                      bDestroyContainerOnSearch : 1;                            // 0x0D99(0x0001) (Edit, BlueprintVisible)
					unsigned char                                      bAlreadySearched : 1;
				};

				// TODO: Implement bitfields better

				auto BitField = ReceivingActor->Member<BitField_Container>(_("bAlreadySearched"));
				BitField->bAlreadySearched = true;

				static auto AlreadySearchedFn = ReceivingActor->Function(_("OnRep_bAlreadySearched"));
				if (AlreadySearchedFn)
				{
					ReceivingActor->ProcessEvent(AlreadySearchedFn);
					Looting::Tables::HandleSearch(ReceivingActor);
				}
			}

			if (ReceivingActorName.contains(_("SupplyDrop")))
			{
				Looting::Tables::HandleSearch(ReceivingActor);
			}

			if (ReceivingActorName.contains(_("B_Athena_VendingMachine")))
			{
				// *ReceivingActor->Member<int>(_("CostAmount"))
				// MaterialType
				auto Super = (UClass_FTT*)ReceivingActor;
				for (auto Super = (UClass_FTT*)ReceivingActor; Super; Super = (UClass_FTT*)Super->SuperStruct)
				{
					std::cout << _("SuperStruct: ") << Super->GetFullName() << '\n';
				}

				static auto GetCurrentActiveItem = ReceivingActor->Function(_("GetCurrentActiveItem"));
				UObject* CurrentMaterial = nullptr;
				ReceivingActor->ProcessEvent(GetCurrentActiveItem, &CurrentMaterial);

				auto SpawnLocation = *ReceivingActor->Member<FVector>(_("LootSpawnLocation"));
			}

			if (ReceivingActorName.contains(_("Vehicle")))
			{
				// Helper::SetRemoteRole(*Controller->Member<UObject*>(_("Pawn")), ENetRole::ROLE_AutonomousProxy);
				// Helper::SetLocalRole(*Controller->Member<UObject*>(_("Pawn")), ENetRole::ROLE_AutonomousProxy);
				// Helper::SetLocalRole(ReceivingActor, ENetRole::ROLE_AutonomousProxy);
			}
		}
	}

	return false;
}

inline bool ServerSendZiplineStateHook(UObject* Pawn, UFunction* Function, void* Parameters)
{
	if (Pawn && Parameters)
	{
		struct parms { __int64 ZiplineState; };
		auto Params = (parms*)Parameters;

		static auto ZiplineOffset = FindOffsetStruct(_("ScriptStruct /Script/FortniteGame.ZiplinePawnState"), _("Zipline"));

		auto Zipline = (UObject**)(__int64(&Params->ZiplineState) + ZiplineOffset);

		// Helper::SetLocalRole(Pawn, ENetRole::ROLE_AutonomousProxy);
		// Helper::SetRemoteRole(Pawn, ENetRole::ROLE_Authority);

		if (Zipline && *Zipline)
		{
			// Helper::SetLocalRole(*Zipline, ENetRole::ROLE_AutonomousProxy);
			// Helper::SetLocalRole(*Zipline, ENetRole::ROLE_Authority); // UNTESTED
			// Helper::SetRemoteRole(*Zipline, ENetRole::ROLE_Authority);
		}
		else
			std::cout << _("No zipline!\n");
	}

	return false;
}

inline bool PlayButtonHook(UObject* Object, UFunction* Function, void* Parameters)
{
	LoadInMatch();
	return false;
}

inline bool ServerSuicideHook(UObject* Controller, UFunction* Function, void* Parameters)
{
	if (Controller)
	{
		struct {
			FVector LaunchVelocity;
			bool bXYOverride;
			bool bZOverride;
			bool bIgnoreFallDamage;
			bool bPlayFeedbackEvent;
		} LCJParams{{ 90000, 90000, 90000 }, false, false, true, true};

		auto Pawn = Controller->Member<UObject*>(_("Pawn"));

		if (Pawn && *Pawn)
		{
			auto ParachuteAttachment = (*Pawn)->Member<UObject*>(_("ParachuteAttachment"));

			if (ParachuteAttachment && *ParachuteAttachment)
			{
				*(*ParachuteAttachment)->Member<bool>(_("bParachuteVisible")) = false;
				*(*ParachuteAttachment)->Member<UObject*>(_("PlayerPawn")) = nullptr;
				(*ParachuteAttachment)->ProcessEvent(_("OnRep_PlayerPawn"));
				(*Pawn)->ProcessEvent(_("OnRep_ParachuteAttachment"));
				Helper::DestroyActor(*ParachuteAttachment);
				std::cout << _("Destroyed ParachuteAttachment!\n");
			}
			else
				std::cout << _("No ParachuteAttachment!\n");

			static auto FortGliderInstance = FindObject(_("BlueprintGeneratedClass /Game/Athena/Cosmetics/Blueprints/Gliders/B_BaseGlider.B_BaseGlider_C"));
			auto Gliders = Helper::GetAllActorsOfClass(FortGliderInstance);
			
			std::cout << _("Glider Num: ") << Gliders.Num() << '\n';

			for (int i = 0; i < Gliders.Num(); i++)
			{
				auto Glider = Gliders[i];

				if (!Glider)
					continue;

				Helper::DestroyActor(Glider);
			}

			Gliders.Free();

			/* UObject* GliderAnimInstance = nullptr;

			if (GliderAnimInstance)
			{
				auto OwnerGlider = GliderAnimInstance->Member<UObject*>(_("OwnerGlider"));

				if (OwnerGlider && *OwnerGlider)
				{

				}
			} */
			// Helper::SpawnChip(Controller);
			*(*Pawn)->Member<bool>(_("bIsDBNO")) = false;
			(*Pawn)->ProcessEvent(_("OnRep_IsDBNO"));

			struct { UObject* EventInstigator; } COPRParams{ Controller };
			static auto COPRFn = FindObject(_("Function /Script/FortniteGame.FortPlayerControllerZone.ClientOnPawnRevived"));

			Controller->ProcessEvent(COPRFn, &COPRParams);

			static auto setHealthFn = (*Pawn)->Function(_("SetHealth"));
			struct { float NewHealthVal; }healthParams{ 30 };

			if (setHealthFn)
				(*Pawn)->ProcessEvent(setHealthFn, &healthParams);
			else
				std::cout << _("Unable to find setHealthFn!\n");

			(*Pawn)->ProcessEvent(_("ForceReviveFromDBNO"));

			// std::cout << _("Spawned Chip!\n");
			/* static auto fn = (*Pawn)->Function(_("LaunchCharacterJump"));

			if (fn)
				(*Pawn)->ProcessEvent(fn, &LCJParams);
			else
				std::cout << _("No LaunchCharacterJump!\n"); */
		}
		else
			std::cout << _("No Pawn!\n");
	}

	return false;
}

inline bool IsResurrectionChipAvailableHook(UObject* PlayerState, UFunction* Function, void* Parameters)
{
	struct IRCAParams { bool ret; };

	auto Params = (IRCAParams*)Parameters;

	if (Params)
	{
		Params->ret = true;
	}

	return true;
}

inline bool ServerClientPawnLoadedHook(UObject* Controller, UFunction* Function, void* Parameters)
{
	struct SCPLParams { bool bIsPawnLoaded; };
	auto Params = (SCPLParams*)Parameters;

	if (Parameters && Controller)
	{
		std::cout << _("bIsPawnLoaded: ") << Params->bIsPawnLoaded << '\n';

		auto Pawn = *Controller->Member<UObject*>(_("Pawn"));

		if (Pawn)
		{
			auto bLoadingScreenDropped = *Controller->Member<bool>(_("bLoadingScreenDropped"));
			if (bLoadingScreenDropped && !Params->bIsPawnLoaded)
			{
			}
			else
				std::cout << _("Loading screen is not dropped!\n");
		}
		else
			std::cout << _("Pawn is not valid!\n");
	}

	return false;
}

inline bool AircraftExitedDropZoneHook(UObject* GameMode, UFunction* Function, void* Parameters)
{
	return true;
}

inline bool ServerChoosePartHook(UObject* Pawn, UFunction* Function, void* Parameters)
{
	struct SCP_Params {
		TEnumAsByte<EFortCustomPartType> Part;
		UObject* ChosenCharacterPart;
	};

	auto Params = (SCP_Params*)Parameters;

	if (Params && (!Params->ChosenCharacterPart && Params->Part.Get() != EFortCustomPartType::Backpack))
		return true;

	return false;
}

void FinishInitializeUHooks()
{
	if (Engine_Version < 422)
		AddHook(_("BndEvt__BP_PlayButton_K2Node_ComponentBoundEvent_1_CommonButtonClicked__DelegateSignature"), PlayButtonHook);

	AddHook(_("Function /Script/Engine.GameMode.ReadyToStartMatch"), ReadyToStartMatchHook);
	AddHook(_("Function /Script/FortniteGame.FortPlayerControllerAthena.ServerAttemptAircraftJump"), ServerAttemptAircraftJumpHook);
	AddHook(_("Function /Script/FortniteGame.FortGameModeAthena.OnAircraftExitedDropZone"), AircraftExitedDropZoneHook); // "fix" (temporary) for aircraft after it ends on newer versions.
	//AddHook(_("Function /Script/FortniteGame.FortPlayerController.ServerSuicide"), ServerSuicideHook);
	// AddHook(_("Function /Script/FortniteGame.FortPlayerController.ServerCheat"), ServerCheatHook); // Commands Hook
	AddHook(_("Function /Script/FortniteGame.FortPlayerController.ServerClientPawnLoaded"), ServerClientPawnLoadedHook);
	AddHook(_("Function /Script/FortniteGame.FortPlayerControllerZone.ClientOnPawnDied"), ClientOnPawnDiedHook);
	AddHook(_("Function /Script/FortniteGame.FortPlayerPawn.ServerSendZiplineState"), ServerSendZiplineStateHook);

	if (Engine_Version >= 420)
		AddHook(_("Function /Script/FortniteGame.FortAthenaVehicle.ServerUpdatePhysicsParams"), ServerUpdatePhysicsParamsHook);

	if (PlayMontage)
		AddHook(_("Function /Script/FortniteGame.FortPlayerController.ServerPlayEmoteItem"), ServerPlayEmoteItemHook);

	if (Engine_Version < 423)
	{ // ??? Idk why we need the brackets
		AddHook(_("Function /Script/FortniteGame.FortPlayerController.ServerAttemptInteract"), ServerAttemptInteractHook);
	}
	else
		AddHook(_("Function /Script/FortniteGame.FortControllerComponent_Interaction.ServerAttemptInteract"), ServerAttemptInteractHook);

	AddHook(_("Function /Script/FortniteGame.FortPlayerControllerZone.ServerAttemptExitVehicle"), ServerAttemptExitVehicleHook);

	AddHook(_("Function /Script/FortniteGame.FortPlayerPawn.ServerChoosePart"), ServerChoosePartHook);

	for (auto& Func : FunctionsToHook)
	{
		if (!Func.first)
			std::cout << _("Detected null UFunction!\n");
	}

	std::cout << std::format("Hooked {} UFunctions!\n", std::to_string(FunctionsToHook.size()));
}

void* ProcessEventDetour(UObject* Object, UFunction* Function, void* Parameters)
{
	if (Object && Function)
	{
#ifdef LOGGING
		if (bStarted && bListening)
		{
			auto FunctionName = Function->GetName();
			// if (Function->FunctionFlags & 0x00200000 || Function->FunctionFlags & 0x01000000) // && FunctionName.find("Ack") == -1 && FunctionName.find("AdjustPos") == -1))
			if (FunctionName.starts_with(_("Server")) || FunctionName.starts_with(_("Client")) || FunctionName.starts_with(_("OnRep_")))
			{
				if (!FunctionName.contains("ServerUpdateCamera") && !FunctionName.contains("ServerMove")
					&& !FunctionName.contains(_("ServerUpdateLevelVisibility"))
					&& !FunctionName.contains(_("AckGoodMove")))
				{
					std::cout << "RPC Called: " << FunctionName << '\n';
				}
			}
		}
#endif

		for (auto& Func : FunctionsToHook)
		{
			if (Function == Func.first)
			{
				if (Func.second(Object, Function, Parameters)) // If the fuynction returned true, then cancel execution.
				{
					return 0;
				}
			}
		}
	}

	return ProcessEventO(Object, Function, Parameters);
}

__int64 __fastcall FixCrashDetour(int32_t* PossiblyNull, __int64 a2, int* a3)
{
	if (!PossiblyNull)
	{
		std::cout << "Prevented Crash!\n";
		return 0;
	}

	return FixCrash(PossiblyNull, a2, a3);
}

void __fastcall GetPlayerViewPointDetour(UObject* pc, FVector* a2, FRotator* a3)
{
	if (pc)
	{
		static auto fn = FindObject(_("Function /Script/Engine.Controller.GetViewTarget"));
		UObject* TheViewTarget = nullptr; // *pc->Member<UObject*>(_("Pawn"));
		pc->ProcessEvent(fn, &TheViewTarget);

		if (TheViewTarget)
		{
			if (a2)
				*a2 = Helper::GetActorLocation(TheViewTarget);
			if (a3)
				*a3 = Helper::GetActorRotation(TheViewTarget);
			// std::cout << _("Did the ViewPoint!\n");

			return;
		}
		// else
			// std::cout << _("unable to get viewpoint!\n"); // This will happen if someone leaves iirc
	}

	return GetPlayerViewPoint(pc, a2, a3);
}

void InitializeHooks()
{
	MH_CreateHook((PVOID)ProcessEventAddr, ProcessEventDetour, (void**)&ProcessEventO);
	MH_EnableHook((PVOID)ProcessEventAddr);

	if (Engine_Version >= 423)
	{
		MH_CreateHook((PVOID)FixCrashAddr, FixCrashDetour, (void**)&FixCrash);
		MH_EnableHook((PVOID)FixCrashAddr);
	}

	if (GetPlayerViewpointAddr) //Engine_Version == 423)
	{
		//Fixes Flashing
		MH_CreateHook((PVOID)GetPlayerViewpointAddr, GetPlayerViewPointDetour, (void**)&GetPlayerViewPoint);
		MH_EnableHook((PVOID)GetPlayerViewpointAddr);
	}
	else
		std::cout << _("[WARNING] Could not fix flashing!\n");
}
