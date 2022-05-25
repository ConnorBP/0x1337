#include "hooks.h"

// include minhook for epic hookage
#include "../../ext/minhook/minhook.h"

#include <intrin.h>

void hooks::Setup() noexcept
{
	MH_Initialize();

	// AllocKeyValuesMemory hook
	MH_CreateHook(
		memory::Get(interfaces::keyValuesSystem, 1),
		&AllocKeyValuesMemory,
		reinterpret_cast<void**>(&AllocKeyValuesMemoryOriginal)
	);

	// CreateMove hook
	MH_CreateHook(
		memory::Get(interfaces::clientMode, 24),
		&CreateMove,
		reinterpret_cast<void**>(&CreateMoveOriginal)
	);

	//// DrawModel Hook
	//MH_CreateHook(
	//	memory::Get(interfaces::studioRender, 29),
	//	&DrawModel,
	//	reinterpret_cast<void**>(&DrawModelOriginal)
	//);

	MH_EnableHook(MH_ALL_HOOKS);
}

void hooks::Destroy() noexcept
{
	// restore hooks
	MH_DisableHook(MH_ALL_HOOKS);
	MH_RemoveHook(MH_ALL_HOOKS);

	// uninit minhook
	MH_Uninitialize();
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

bool __stdcall hooks::CreateMove(float frameTime, CUserCmd* cmd) noexcept
{
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
	const bool result = CreateMoveOriginal(interfaces::clientMode, frameTime, cmd);
	// make sure this function is being called from CInput::CreateMove
	if (!cmd->commandNumber)
		return result;

	// set view angles if return value is true to avoid stuttering.
	// Since we are not gonna return true (which would cause the game to do this for us automatically).
	if (result)
		interfaces::engine->SetViewAngles(cmd->viewAngles);

	// get local player
	globals::localPlayer = interfaces::entityList->GetEntityFromIndex(interfaces::engine->GetLocalPlayerIndex());

	// b key to allow the code below to run
	if (!GetAsyncKeyState(0x42))
		return false;

	if (!globals::localPlayer || !globals::localPlayer->IsAlive())
		return false;

	CVector eyePosition;
	globals::localPlayer->GetEyePosition(eyePosition);
	
	CVector aimPunch;
	globals::localPlayer->GetAimPunch(aimPunch);

	// calculate the destination of the ray
	const CVector dest = eyePosition + CVector{ cmd->viewAngles + aimPunch }.AngleToVector() * 8192.f;
	
	// trace the ray from eyes to dest
	CTrace trace;
	interfaces::trace->TraceRay({ eyePosition, dest }, 0x46004009, globals::localPlayer, trace);

	//make sure we actually hit a player
	if (!trace.entity || !trace.entity->IsPlayer())
		return false;

	//make sure player is alive and on other team
	if (!trace.entity->IsAlive() || trace.entity->GetTeam() == globals::localPlayer->GetTeam())
		return false;

	cmd->buttons |= CUserCmd::ECommandButton::IN_ATTACK;

	return false;

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
			static IMaterial* material = interfaces::materialSystem->FindMaterial("models/inventory_items/music_kit/darude_01/mp3_detail");

			constexpr float visible[3] = { 0.012, 0.165, 0.529 };
			constexpr float hidden[3] = {0.039, 0.514, 0.388};

			interfaces::studioRender->SetAlphaModulation(0.8f);

			// show through walls
			material->SetMaterialVarFlag(IMaterial::IGNOREZ, true);
			interfaces::studioRender->SetColorModulation(hidden);
			interfaces::studioRender->ForcedMaterialOverride(material);
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