#pragma once

#include "configstructs.h"

//enum { OPT1, OPT2 };

namespace config {
	inline struct Misc {
		int enableSpectatorList = 1;
		int enableGaySpam = 1;
		int gaySpamDelay = 45;
	} misc;
	inline struct DrawBoxes {
		int enabled = true;
		int enableDebug = true;
		int enableEnemy = true;
		int enableTeam = false;
		int drawNames = true;
		//constexpr float visible[3] = { 0.529, 0.165, 0.529 };
		//constexpr float hidden[3] = { 0.039, 0.514, 0.388 };
		fcolor4 enemyColor{ 0.75f, 0.26f, 0.0f, 0.99f };
		fcolor4 nameColor{ 0.92f, 0.71f, 0.22f, 0.6f };
		//fcolor4 enemyColorZ { 0.039f, 0.514f, 0.388f, 1.0};
		fcolor4 teamColor{0.69,0.5,0.69, 1.0};
		//fcolor4 teamColorZ{0.69,0.69,0.5, 1.0};
	} drawboxes;
}