#include "hooks.h"

// include minhook for epic hookage
#include "../../ext/minhook/minhook.h"

#include <stdexcept>
#include <intrin.h>

#include "features/Misc.h"
#include "features/Trigger.h"
#include "features//LegitBot.h"

#include "../util/Logging.h"

#include "config.h"

#include "gui.h"

void hooks::Setup()
{
	// minhook returns non zero on error
	if (MH_Initialize())
		throw std::runtime_error("Unable to initialize minhook");

	// AllocKeyValuesMemory hook
	MH_CreateHook(
		memory::Get(interfaces::keyValuesSystem, 1),
		&AllocKeyValuesMemory,
		reinterpret_cast<void**>(&AllocKeyValuesMemoryOriginal)
	);

	MH_CreateHook(
		memory::clientReturnCheck,
		&verify_return_address_hook,
		reinterpret_cast<void**>(&VerifyClientReturnOriginal)
	);

	MH_CreateHook(
		memory::engineReturnCheck,
		&verify_return_address_hook,
		reinterpret_cast<void**>(&VerifyEngineReturnOriginal)
	);

	MH_CreateHook(
		memory::studiorenderReturnCheck,
		&verify_return_address_hook,
		reinterpret_cast<void**>(&VerifyStudioReturnOriginal)
	);

	MH_CreateHook(
		memory::materialsystemReturnCheck,
		&verify_return_address_hook,
		reinterpret_cast<void**>(&VerifyMaterialReturnOriginal)
	);

	// CreateMove hook
	MH_CreateHook(
		memory::Get(interfaces::client, 22),
		&CreateMoveProxy,
		reinterpret_cast<void**>(&CreateMoveOriginal)
	);

	// DrawModel Hook
	MH_CreateHook(
		memory::Get(interfaces::studioRender, 29),
		&DrawModel,
		reinterpret_cast<void**>(&DrawModelOriginal)
	);

	// emit sound hook for auto accept
	MH_CreateHook(
		memory::Get(interfaces::sound, 5),
		&hkEmitSound1,
		reinterpret_cast<void**>(&EmitSoundOriginal)
	);

	//endscene hook
	if (MH_CreateHook(
		memory::Get(gui::device, 16),
		&Reset,
		reinterpret_cast<void**>(&ResetOriginal)
	))
		throw std::runtime_error("failed to create endscene hook");

	//reset hook
	if (MH_CreateHook(
		memory::Get(gui::device, 42),
		&EndScene,
		reinterpret_cast<void**>(&EndSceneOriginal)
	))
		throw std::runtime_error("failed to create endscene hook");

	//reset hook
	if (MH_CreateHook(
		memory::Get(interfaces::surface, 67),
		&hkLockCursor,
		reinterpret_cast<void**>(&LockCursorOriginal)
	))
		throw std::runtime_error("failed to create hkLockCursor hook");

	if (MH_EnableHook(MH_ALL_HOOKS))
		throw std::runtime_error("unable to enable hooks");

	// we don't need this once we use it to create the hooks
	gui::DestroyDirectX();
}

void hooks::Destroy() noexcept
{
	// restore hooks
	MH_DisableHook(MH_ALL_HOOKS);
	MH_RemoveHook(MH_ALL_HOOKS);

	// uninit minhook
	MH_Uninitialize();
}

bool __fastcall hooks::verify_return_address_hook(void* ecx, void* edx, const char* module_name) noexcept
{
	return true;
}

void* __stdcall hooks::AllocKeyValuesMemory(const std::int32_t size) noexcept
{
	// if function is returning to speficied addresses, return nullptr to "bypass"
	if (const std::uint32_t address = reinterpret_cast<std::uint32_t>(_ReturnAddress());
		address == reinterpret_cast<std::uint32_t>(memory::allocKeyValuesEngine) ||
		address == reinterpret_cast<std::uint32_t>(memory::allocKeyValuesClient))
		return nullptr;

	// return original
	return AllocKeyValuesMemoryOriginal(interfaces::keyValuesSystem, size);
}


bool __stdcall hooks::CreateMove(float frameTime, CUserCmd* cmd, bool& sendPacket) noexcept
{
	static auto previousViewAngles{ cmd->viewAngles };
	const auto viewAngles{ cmd->viewAngles };
	auto currentViewAngles{ cmd->viewAngles };
	const auto currentCmd{ *cmd };
	//// make sure this function is being called from CInput::CreateMove
	//if (!cmd->commandNumber)
	//	return CreateMoveOriginal(interfaces::clientMode, frameTime, cmd);

	//// this would be done anyway by returning true
	//if (CreateMoveOriginal(interfaces::clientMode, frameTime, cmd))
	//	interfaces::engine->SetViewAngles(cmd->viewAngles);

	//// get our local player here
	//globals::UpdateLocalPlayer();

	//if (globals::localPlayer && globals::localPlayer->IsAlive())
	//{
	//	// example bhop
	//	if (!(globals::localPlayer->GetFlags() & CEntity::FL_ONGROUND))
	//		cmd->buttons &= ~CUserCmd::IN_JUMP;
	//}

	//return false;

	// store original return value
	//const bool result = CreateMoveOriginal(interfaces::clientMode, frameTime, cmd);
	// make sure this function is being called from CInput::CreateMove
	//if (!cmd->commandNumber)
	//	return result;

	//// set view angles if return value is true to avoid stuttering.
	//// Since we are not gonna return true (which would cause the game to do this for us automatically).
	//if (result)
	//	interfaces::engine->SetViewAngles(cmd->viewAngles);



	


	//// get local player
	globals::localPlayer = interfaces::entityList->GetEntityFromIndex(interfaces::engine->GetLocalPlayerIndex());

	if (globals::localPlayer && globals::localPlayer->IsAlive()) {

		Misc::bunnyHop(cmd);
		if (GetAsyncKeyState(VK_XBUTTON2)) {
			if (config::legitBot.triggerType == TRIG_HITC) {
				Trigger::hitChanceTrigger(cmd);
			}
			else {
				Trigger::runTrigger(cmd);
			}
		}
		LegitBot::RunAimbot(cmd);

	}

	// triggerbot
	/*
	//// b key to allow the code below to run
	if (!GetAsyncKeyState(0x42))
		return false;

	if (!globals::localPlayer || !globals::localPlayer->IsAlive())
		return false;

	CVector eyePosition;
	globals::localPlayer->GetEyePosition(eyePosition);
	
	CVector aimPunch;
	globals::localPlayer->GetAimPunch(aimPunch);

	//// calculate the destination of the ray
	const CVector dest = eyePosition + CVector{ cmd->viewAngles + aimPunch }.AngleToVector() * 8192.f;

	//// trace the ray from eyes to dest
	CTrace trace;
	interfaces::trace->TraceRay({ eyePosition, dest }, 0x46004009, globals::localPlayer, trace);

	////make sure we actually hit a player
	if (!trace.entity || !trace.entity->IsPlayer())
		return false;

	//make sure player is alive and on other team
	if (!trace.entity->IsAlive() || trace.entity->GetTeam() == globals::localPlayer->GetTeam())
		return false;

	cmd->buttons |= CUserCmd::ECommandButton::IN_ATTACK;
	*/
	return false;
}

static void __stdcall hooks::CHLCreateMove(int sequenceNumber, float inputSampleTime, bool active, bool& sendPacket) noexcept
{
	//static auto original = hooks->client.getOriginal<void, 22>(sequenceNumber, inputSampleTime, active);
	//original(interfaces->client, sequenceNumber, inputSampleTime, active);
	CreateMoveOriginal(interfaces::client, sequenceNumber, inputSampleTime, active);

	CUserCmd* cmd = interfaces::input->getUserCmd(0, sequenceNumber);
	if (!cmd || !cmd->commandNumber)
		return;

	VerifiedUserCmd* verified = interfaces::input->getVerifiedUserCmd(sequenceNumber);
	if (!verified)
		return;

	bool cmoveactive = CreateMove(inputSampleTime, cmd, sendPacket);

	verified->cmd = *cmd;
	verified->crc = cmd->getChecksum();
}

#pragma warning(disable : 4409)
__declspec(naked) void __stdcall hooks::CreateMoveProxy(int sequenceNumber, float inputSampleTime, bool active)
{
	__asm
	{
		PUSH	EBP
		MOV		EBP, ESP
		PUSH	EBX
		LEA		ECX, [ESP]
		PUSH	ECX
		PUSH	active
		PUSH	inputSampleTime
		PUSH	sequenceNumber
		CALL	CHLCreateMove
		POP		EBX
		POP		EBP
		RETN	0xC
	}
}


void __stdcall hooks::DrawModel(
	void* results,
	const CDrawModelInfo& info,
	CMatrix3x4* bones,
	float* flexWeights,
	float* flexDelayedWights,
	const CVector& modelOrigin,
	const std::int32_t flags
) noexcept {
	// make sure local player and renderable pointer are not null
	if (globals::localPlayer && info.renderable) {
		CEntity* entity = info.renderable->GetIClientUnknown()->GetBaseEntity();

		if (entity && entity->IsPlayer() && entity->GetTeam() != globals::localPlayer->GetTeam()) {
			/*
			materials:
				"debug/debugambientcube"
				"models/inventory_items/trophy_majors/gold"
				"models/player/ct_fbi/ct_fbi_glass"
				"models/gibs/glass/glass"
				"models/inventory_items/trophy_majors/crystal_clear"
				"models/inventory_items/wildfire_gold/wildfire_gold_detail"
				"models/inventory_items/trophy_majors/crystal_blue"
				"models/inventory_items/trophy_majors/velvet"
				"models/inventory_items/cologne_prediction/cologne_prediction_glass"
				"models/inventory_items/dogtags/dogtags_outline"
				"models/inventory_items/dogtags/dogtags_lightray"
				"models/inventory_items/contributor_map_tokens/contributor_charset_color"
				"models/inventory_items/music_kit/darude_01/mp3_detail"
			*/
			static IMaterial* material = interfaces::materialSystem->FindMaterial("debug/debugambientcube");
			static IMaterial* wallMaterial = interfaces::materialSystem->FindMaterial("models/inventory_items/cologne_prediction/cologne_prediction_glass");

			constexpr float visible[3] = { 0.529, 0.165, 0.529 };
			constexpr float hidden[3] = {0.039, 0.514, 0.388};

			interfaces::studioRender->SetAlphaModulation(0.8f);

			// show through walls
			wallMaterial->SetMaterialVarFlag(IMaterial::IGNOREZ, true);
			interfaces::studioRender->SetColorModulation(hidden);
			interfaces::studioRender->ForcedMaterialOverride(wallMaterial);
			DrawModelOriginal(interfaces::studioRender, results, info, bones, flexWeights, flexDelayedWights, modelOrigin, flags);

			// not through walls
			material->SetMaterialVarFlag(IMaterial::IGNOREZ, false);
			interfaces::studioRender->SetColorModulation(visible);
			interfaces::studioRender->ForcedMaterialOverride(material);
			DrawModelOriginal(interfaces::studioRender, results, info, bones, flexWeights, flexDelayedWights, modelOrigin, flags);

			return interfaces::studioRender->ForcedMaterialOverride(nullptr);
		}
	}

	// call original to make sure we render every other model that does not have chams
	DrawModelOriginal(interfaces::studioRender, results, info, bones, flexWeights, flexDelayedWights, modelOrigin, flags);
}

void __stdcall hooks::hkEmitSound1(IRecipientFilter& filter, int iEntIndex, int iChannel, const char* pSoundEntry, unsigned int nSoundEntryHash, const char* pSample, float flVolume, int nSeed, float flAttenuation, int iFlags, int iPitch, const CVector* pOrigin, const CVector* pDirection, void* pUtlVecOrigins, bool bUpdatePositions, float soundtime, int speakerentity, int unk) {

	if (config::misc.enableAutoAccept && !strcmp(pSoundEntry, "UIPanorama.popup_accept_match_beep")) {
		static auto fnAccept = reinterpret_cast<bool(__stdcall*)(const char*)>(memory::PatternScan("client.dll", "55 8B EC 83 E4 F8 8B 4D 08 BA ? ? ? ? E8 ? ? ? ? 85 C0 75 12"));

		if (fnAccept) {

			fnAccept("");

			// This will flash the CSGO window on the taskbar
			// so we know a game was found (you cant hear the beep sometimes cause it auto-accepts too fast)
			FLASHWINFO fi;
			fi.cbSize = sizeof(FLASHWINFO);
			fi.hwnd = gui::window;
			fi.dwFlags = FLASHW_ALL | FLASHW_TIMERNOFG;
			fi.uCount = 0;
			fi.dwTimeout = 0;
			FlashWindowEx(&fi);
			log_console("Match Accepted", Color::Blue);
		}
	}

	EmitSoundOriginal(interfaces::sound, filter, iEntIndex, iChannel, pSoundEntry, nSoundEntryHash, pSample, flVolume, nSeed, flAttenuation, iFlags, iPitch, pOrigin, pDirection, pUtlVecOrigins, bUpdatePositions, soundtime, speakerentity, unk);
}

void __stdcall hooks::hkLockCursor() noexcept
{
	if (globals::localPlayer && globals::localPlayer->IsAlive() && interfaces::engine->IsInGame())
		interfaces::inputSystem->EnableInput(true);

	gui::open ? interfaces::surface->UnlockCursor() : LockCursorOriginal(interfaces::surface);
}