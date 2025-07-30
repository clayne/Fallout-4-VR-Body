#pragma once

#include "f4vr/MiscStructs.h"
#include "f4vr/VRControllersManager.h"

namespace frik
{
    void turnPlayerRadioOn(bool isActive);

    void triggerShortHaptic(f4vr::Hand hand = f4vr::Hand::Primary);

    bool isAnyPipboyOpen();

    bool isCameraLookingAtObject(const RE::NiAVObject* cameraNode, const RE::NiAVObject* objectNode, float detectThresh);
    bool isArmorHasHeadLamp();

    bool isBetterScopesVRModLoaded();

    f4vr::MuzzleFlash* getMuzzleFlashNodes();
}
