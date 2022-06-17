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
}