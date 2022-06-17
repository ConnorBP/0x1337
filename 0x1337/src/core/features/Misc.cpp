#include "Misc.h"

#include "../config.h"

void Misc::bunnyHop(CUserCmd* cmd) noexcept
{
    //if (config->misc.jumpBug && config->misc.jumpBugKey.isActive())
    //    return;

    static auto wasLastTimeOnGround{ globals::localPlayer->GetFlags() & CEntity::FL_ONGROUND };

    if (config::misc.enableBhop && !(globals::localPlayer->GetFlags() & CEntity::FL_ONGROUND) && globals::localPlayer->GetMoveType() != CEntity::MoveType::LADDER  && !wasLastTimeOnGround)
        cmd->buttons &= ~CUserCmd::IN_JUMP;

    wasLastTimeOnGround = globals::localPlayer->GetFlags() & CEntity::FL_ONGROUND;


    // simplistic version:
    //if (!(globals::localPlayer->GetFlags() & CEntity::FL_ONGROUND))
    //    cmd->buttons &= ~CUserCmd::IN_JUMP;
}