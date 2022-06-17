#pragma once

#include "configstructs.h"

enum { TRIG_HITC, TRIG_FAST };

namespace config {
	inline struct Misc {
		int enableBhop = 1;
		int enableAutoAccept = 1;
	} misc;
	inline struct Chams {
		bool enableEnemy = true;
		bool enableEnemyZ = true;
		bool enableTeam = false;
		bool enableTeamZ = false;
		//constexpr float visible[3] = { 0.529, 0.165, 0.529 };
		//constexpr float hidden[3] = { 0.039, 0.514, 0.388 };
		fcolor4 enemyColor { 0.529f, 0.165f, 0.529f, 1.0};
		fcolor4 enemyColorZ { 0.039f, 0.514f, 0.388f, 1.0};
		fcolor4 teamColor{0.69,0.5,0.69, 1.0};
		fcolor4 teamColorZ{0.69,0.69,0.5, 1.0};
	} chams;
	inline struct LegitBot {
		int triggerType = TRIG_HITC;
		int triggerHitChance = 75;
	} legitBot;
}