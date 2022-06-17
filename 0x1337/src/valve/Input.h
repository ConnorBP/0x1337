#pragma once

#include "CUserCmd.h"
#include "Pad.h"
#include "cvector.h"
#include "VirtualMethod.h"

class Input {
public:
    PAD(12)
        bool isTrackIRAvailable;
    bool isMouseInitialized;
    bool isMouseActive;
    PAD(154)
        bool isCameraInThirdPerson;
    PAD(2)
        CVector cameraOffset;
    PAD(56)
        CUserCmd* commands;
    VerifiedUserCmd* verifiedCommands;

    VIRTUAL_METHOD(CUserCmd*, getUserCmd, 8, (int slot, int sequenceNumber), (this, slot, sequenceNumber))

        VerifiedUserCmd* getVerifiedUserCmd(int sequenceNumber) noexcept
    {
        return &verifiedCommands[sequenceNumber % 150];
    }
};