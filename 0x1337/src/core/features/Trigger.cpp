#include "Trigger.h"

#include <Windows.h>

#include "../../util/Logging.h"
#include "../config.h"

void Trigger::runTrigger(CUserCmd* cmd) noexcept
{
	// triggerbot

	//// b(0x42) key to allow the code below to run
	//if (!GetAsyncKeyState(VK_XBUTTON2))
	//	return;

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
		return;

	//make sure player is alive and on other team
	if (!trace.entity->IsAlive() || trace.entity->GetTeam() == globals::localPlayer->GetTeam())
		return;

	cmd->buttons |= CUserCmd::ECommandButton::IN_ATTACK;
}

// Function for conversion of radians to degrees
float M_RAD2DEG(float radian)
{
	return(radian * (180 / 3.14159));
}

void Trigger::hitChanceTrigger(CUserCmd* cmd) noexcept {

	////// b(0x42) key to allow the code below to run
	//if (!GetAsyncKeyState(0x42))
	//	return;

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

	// Total rays to calculate
	int rayAmount = 200;

	// Counted hits
	int hits = 0;

	// Get view angles and eyepos (duh)
	auto viewAngles = cmd->viewAngles;
	CVector start;
	globals::localPlayer->GetEyePosition(start);

	CVector aimPunch;
	globals::localPlayer->GetAimPunch(aimPunch);

	// Calculate weapon spread angle
	auto spreadAngle = M_RAD2DEG(activeWeapon->GetInaccuracy() + activeWeapon->GetSpread());

	for (int i = 0; i < rayAmount; i++) {

		// Ratio of the loop (0 at beginning, 1 at end)
		float ratio = (i / (float)rayAmount);

		// This is the spread angle distance
		float multiplier = ratio * spreadAngle;

		// Calculate spread direction spiral (30 is a number I chose, you can test other numbers too)
		// Using sqrtf to make our spread circle exponentially distributed
		float spreadDir = sqrtf(ratio) * 3.14159f * 30.f;

		// Now we can calculate our actual spread angle
		CVector spreadAngle = CVector(cos(spreadDir) * multiplier, sin(spreadDir) * multiplier, 0);

		// The tested shot angle (spread added to view angles)
		CVector shotAngle = viewAngles + aimPunch + spreadAngle;

		// Calculate where this simulated spread shot will end
		auto endpos = start + shotAngle.AngleToVector() * activeWeapon->GetWeaponData()->range;

		// An example of doing a trace check on a player
		// If you want good ragebot performance, I wouldn't do this
		CRay ray(start, endpos);
		CTrace tr; //g_EngineTrace->ClipRayToEntity(ray, MASK_SHOT, player, &tr);
		interfaces::trace->TraceRay(ray, 0x46004009, globals::localPlayer, tr);
		if (tr.entity &&
			tr.entity->IsPlayer() &&
			tr.entity->IsAlive() &&
			tr.entity->GetTeam() != globals::localPlayer->GetTeam()
		) {
			// If the trace hits an alive enemy player, then Count the hit
			hits++;
		}
	}

	// Now we have our hits
	// You can calculate hitchance or whatever
	//Log("Hits: " + to_string(hits) + " / " + to_string(rayAmount));
#ifdef _DEBUG
	log_console("Hits: " + std::to_string(hits) + " / " + std::to_string(rayAmount) + "\n", Color::Green);
#endif

	if (hits >= config::legitBot.triggerHitChance * 2)
		cmd->buttons |= CUserCmd::ECommandButton::IN_ATTACK;

}