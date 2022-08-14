#include "iglobalvars.h"

#include "../core/globals.h"
#include "cusercmd.h"
#include "centity.h"


float IGlobalVars::serverTime(CUserCmd* cmd) const noexcept
{
    static int tick;
    static CUserCmd* lastCmd;

    if (cmd) {
        if (globals::localPlayer && (!lastCmd || lastCmd->hasBeenPredicted))
            tick = globals::localPlayer->tickBase();
        else
            tick++;
        lastCmd = cmd;
    }
    return tick * intervalPerTick;
}
