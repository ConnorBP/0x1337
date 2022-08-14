#include "Misc.h"

#include "../config.h"

#include <iostream>
#include <ctime>
#include <cstdlib>

#include "../../valve/centity.h"


void Misc::bunnyHop(CUserCmd* cmd) noexcept
{
    //if (config->misc.jumpBug && config->misc.jumpBugKey.isActive())
    //    return;

    static bool bShouldFake = false;
    static int bActualHop = 0;

    //std::srand(std::time(0));  // Initialize random number generator.
    //int num = std::rand() % 100;



    static auto wasLastTimeOnGround{ globals::localPlayer->GetFlags() & CEntity::FL_ONGROUND };

    if (config::misc.enableBhop && !(globals::localPlayer->GetFlags() & CEntity::FL_ONGROUND) && globals::localPlayer->GetMoveType() != CEntity::MoveType::LADDER  && !wasLastTimeOnGround)
        cmd->buttons &= ~CUserCmd::IN_JUMP;

    wasLastTimeOnGround = globals::localPlayer->GetFlags() & CEntity::FL_ONGROUND;


    // simplistic version:
    //if (!(globals::localPlayer->GetFlags() & CEntity::FL_ONGROUND))
    //    cmd->buttons &= ~CUserCmd::IN_JUMP;
}

//void BHopChanceMode(CUserCmd* cmd)
//{
//	if (!Settings::BHop::enabled)
//		return;
//
//	static bool bLastJumped = false;
//	static bool bShouldFake = false;
//	static int bActualHop = 0;
//
//	C_BasePlayer* localplayer = (C_BasePlayer*)entityList->GetClientEntity(engine->GetLocalPlayer());
//
//	if (!localplayer || !localplayer->GetAlive())
//		return;
//
//	if (localplayer->GetMoveType() == MOVETYPE_LADDER || localplayer->GetMoveType() == MOVETYPE_NOCLIP)
//		return;
//
//
//	if (!bLastJumped && bShouldFake)
//	{
//		bShouldFake = false;
//		cmd->buttons |= IN_JUMP;
//	}
//	else if (cmd->buttons & IN_JUMP)
//	{
//		if (localplayer->GetFlags() & FL_ONGROUND)
//		{
//			bActualHop++;
//			bLastJumped = true;
//			bShouldFake = true;
//		}
//		else
//		{
//			if (Settings::BHop::Chance::enabled &&
//				Settings::BHop::Hops::enabledMin &&
//				(bActualHop > Settings::BHop::Hops::Min) &&
//				(rand() % 100 > Settings::BHop::Chance::value))
//				return;
//
//			if (Settings::BHop::Chance::enabled &&
//				!Settings::BHop::Hops::enabledMin &&
//				(rand() % 100 > Settings::BHop::Chance::value))
//				return;
//
//			if (Settings::BHop::Hops::enabledMin &&
//				!Settings::BHop::Chance::enabled &&
//				(bActualHop > Settings::BHop::Hops::Min))
//				return;
//
//			if (Settings::BHop::Hops::enabledMax &&
//				(bActualHop > Settings::BHop::Hops::Max))
//				return;
//
//			cmd->buttons &= ~IN_JUMP;
//			bLastJumped = false;
//		}
//	}
//	else
//	{
//		bActualHop = 0;
//		bLastJumped = false;
//		bShouldFake = false;
//	}
//}

//{
//    static int hops_restricted = 0;
//    static int hops_hit = 0;
//    if (!(cmd->buttons & in_jump)
//        || (local->move_type() & movetype_ladder))
//        return;
//    if (!(local->flags() & fl_onground))
//    {
//        cmd->buttons &= ~in_jump;
//        hops_restricted = 0;
//    }
//    else if ((rand() % 100 > options["bhop_hit_chance"].i_slider			//chance of hitting first hop is always the same, the 2nd part is that so it always doesn't rape your speed
//        && hops_restricted < options["hops_restricted_limit"].i_slider)	//the same amount, it can be made a constant if you want to or can be removed, up to you
//        || (options["max_hops_hit"].i_slider > 0							//force fuck up after certain amount of hops to look more legit, you could add variance to this and
//            && hops_hit > options["max_hops_hit"].i_slider))				//everything but fuck off that's too much customisation in my opinion, i only added this one because prof told me to
//    {
//        cmd->buttons &= ~in_jump;
//        hops_restricted++;
//        hops_hit = 0;
//    }
//    else
//        hops_hit++;
//}

void Misc::prepareRevolver(CUserCmd* cmd) noexcept
{
    constexpr auto timeToTicks = [](float time) {  return static_cast<int>(0.5f + time / interfaces::globals->intervalPerTick); };
    constexpr float revolverPrepareTime{ 0.234375f };

    static float readyTime;
    if (!config::misc.enableAutoRevolver)
        return;

    //if (!config->misc.prepareRevolverKey.isActive())
    //    return;

    if (!globals::localPlayer)
        return;

    const auto activeWeapon = globals::localPlayer->GetActiveWeapon();
    if (activeWeapon && activeWeapon->itemDefinitionIndex2() == WeaponId::Revolver) {
        if (!readyTime) readyTime = interfaces::globals->serverTime() + revolverPrepareTime;
        auto ticksToReady = timeToTicks(readyTime - interfaces::globals->serverTime() - interfaces::engine->getNetworkChannel()->getLatency(0));
        if (ticksToReady > 0 && ticksToReady <= timeToTicks(revolverPrepareTime))
            cmd->buttons |= CUserCmd::IN_ATTACK;
        else
            readyTime = 0.0f;
    }
}

void Misc::gayMessages() noexcept {
    if (!config::misc.enableGaySpam)
        return;
    if (!timerStarted) {
        lastTime = timer::now();
        timerStarted = true;
        return;
    }
    if (std::chrono::duration_cast<std::chrono::seconds>(timer::now() - lastTime).count() > config::misc.gaySpamDelay) {
        lastTime = timer::now();

        std::srand(std::time(0));  // Initialize random number generator.

        int num = std::rand() % (messages.size() -1);
        
        std::string cmd = "say \"";
        cmd += messages[num];
        cmd += '"';
        interfaces::engine->clientCmdUnrestricted(cmd.c_str());
    }
}