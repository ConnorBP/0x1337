#pragma once
#include "checksum_crc.h"
#include "cvector.h"

class CUserCmd
{
public:
    CRC32_t getChecksum() const noexcept
    {
        CRC32_t crc;
        CRC32_Init(&crc);

        CRC32_ProcessBuffer(&crc, &commandNumber, sizeof(commandNumber));
        CRC32_ProcessBuffer(&crc, &tickCount, sizeof(tickCount));
        CRC32_ProcessBuffer(&crc, &viewAngles, sizeof(viewAngles));
        CRC32_ProcessBuffer(&crc, &aimDirection, sizeof(aimDirection));
        CRC32_ProcessBuffer(&crc, &forwardMove, sizeof(forwardMove));
        CRC32_ProcessBuffer(&crc, &sideMove, sizeof(sideMove));
        CRC32_ProcessBuffer(&crc, &upMove, sizeof(upMove));
        CRC32_ProcessBuffer(&crc, &buttons, sizeof(buttons));
        CRC32_ProcessBuffer(&crc, &impulse, sizeof(impulse));
        CRC32_ProcessBuffer(&crc, &weaponSelect, sizeof(weaponSelect));
        CRC32_ProcessBuffer(&crc, &weaponSubType, sizeof(weaponSubType));
        CRC32_ProcessBuffer(&crc, &randomSeed, sizeof(randomSeed));
        CRC32_ProcessBuffer(&crc, &mouseDeltaX, sizeof(mouseDeltaX));
        CRC32_ProcessBuffer(&crc, &mouseDeltaY, sizeof(mouseDeltaY));

        CRC32_Final(&crc);
        return crc;
    }
    enum ECommandButton : std::int32_t
    {
        IN_ATTACK = (1 << 0),
        IN_JUMP = (1 << 1),
        IN_DUCK = (1 << 2),
        IN_FORWARD = (1 << 3),
        IN_BACK = (1 << 4),
        IN_USE = (1 << 5),
        IN_CANCEL = (1 << 6),
        IN_LEFT = (1 << 7),
        IN_RIGHT = (1 << 8),
        IN_MOVELEFT = (1 << 9),
        IN_MOVERIGHT = (1 << 10),
        IN_SECOND_ATTACK = (1 << 11),
        IN_RUN = (1 << 12),
        IN_RELOAD = (1 << 13),
        IN_LEFT_ALT = (1 << 14),
        IN_RIGHT_ALT = (1 << 15),
        IN_SCORE = (1 << 16),
        IN_SPEED = (1 << 17),
        IN_WALK = (1 << 18),
        IN_ZOOM = (1 << 19),
        IN_FIRST_WEAPON = (1 << 20),
        IN_SECOND_WEAPON = (1 << 21),
        IN_BULLRUSH = (1 << 22),
        IN_FIRST_GRENADE = (1 << 23),
        IN_SECOND_GRENADE = (1 << 24),
        IN_MIDDLE_ATTACK = (1 << 25)
    };

    void* vmt;
    std::int32_t commandNumber;
    std::int32_t tickCount;
    CVector viewAngles;
    CVector aimDirection;
    float forwardMove;
    float sideMove;
    float upMove;
    std::int32_t buttons;
    char impulse;
    std::int32_t weaponSelect;
    std::int32_t weaponSubType;
    std::int32_t randomSeed;
    short mouseDeltaX;
    short mouseDeltaY;
    bool hasBeenPredicted;
    CVector headAngles;
    CVector headOffset;
};

class VerifiedUserCmd
{
public:
    CUserCmd cmd;
    unsigned long crc;
};