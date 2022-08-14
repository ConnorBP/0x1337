#include "LegitBot.h"

void LegitBot::RunAimbot(CUserCmd* cmd) noexcept {
	// if we are not currently trying to attack, then return (make this configurable with a lock setting later)
	if (!(cmd->buttons & CUserCmd::IN_ATTACK))
		return;
	if (globals::localPlayer->IsDefusing())
		return;

	// get the currently held weapon from our local player
	CEntity* activeWeapon = globals::localPlayer->GetActiveWeapon();

	// if weapon is null don't continue
	if (!activeWeapon)
		return;

	const int weaponType = activeWeapon->GetWeaponType();

	// if any of the following weapons are held, continue, else exit the function
	switch (weaponType)
	{
	case CEntity::WEAPONTYPE_MACHINEGUN:
	case CEntity::WEAPONTYPE_RIFLE:
	case CEntity::WEAPONTYPE_SHOTGUN:
	case CEntity::WEAPONTYPE_SNIPER:
	case CEntity::WEAPONTYPE_PISTOL:
	{
		// exit if no ammo
		if (!activeWeapon->GetClip())
			return;
		// exit if using sniper and not scoped
		if (weaponType == CEntity::WEAPONTYPE_SNIPER &&
			!globals::localPlayer->IsScoped())
			return;
		break;
	}
	default:
		return;
	}

	bool doRecoil = false;

	// check for what weapons we should do recoil for
	switch (weaponType)
	{
	case CEntity::WEAPONTYPE_MACHINEGUN:
	case CEntity::WEAPONTYPE_RIFLE:
	case CEntity::WEAPONTYPE_PISTOL:
	{
		doRecoil = true;
		break;
	}
	default:
		doRecoil = false;
		break;
	}

	CVector bestAngle{};
	float bestFov{ 30.f };

	// start loop at 1 because index 0 is always the world entity
	for (int i = 1; i <= interfaces::globals->maxClients; ++i) {
		CEntity* player = interfaces::entityList->GetEntityFromIndex(i);
		// check if player is valid at index
		if (!player)
			continue;
		if (player->IsDormant() || !player->IsAlive())
			continue;
		if (player->GetTeam() == globals::localPlayer->GetTeam())
			continue;
		if (player->HasGunGameImmunity())
			continue;

		// get player bone matrix
		CMatrix3x4 boneMatrix[128];
		if (!player->SetupBones(boneMatrix, 128, 256, interfaces::globals->currentTime))
			continue;

		// get our players eye position
		CVector localEyePosition;
		globals::localPlayer->GetEyePosition(localEyePosition);

		// get aim punch
		CVector aimPunch;
		if (doRecoil)
			globals::localPlayer->GetAimPunch(aimPunch);
		CTrace trace;
		interfaces::trace->TraceRay(
			CRay{ localEyePosition, boneMatrix[8].Origin() },
			MASK_SHOT,
			CTraceFilter{ globals::localPlayer },
			trace
		);

		if (!trace.entity || trace.fraction < 0.97f)
			return;

		CVector enemyAngle{
			(boneMatrix[8].Origin() - localEyePosition).AngleToVector() - (cmd->viewAngles + aimPunch)
		};

		if (const float fov = std::hypot(enemyAngle.x, enemyAngle.y); fov < bestFov)
		{
			bestFov = fov;
			bestAngle = enemyAngle;
		}
	}

	cmd->viewAngles = cmd->viewAngles + bestAngle;// .Scale(0.5);
}