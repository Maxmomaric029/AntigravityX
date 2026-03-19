#ifndef ANTIGRAVITY_OFFSETS_H
#define ANTIGRAVITY_OFFSETS_H

/**
 * Game Offsets for Free Fire (com.dts.freefireth)
 * Version: PLAY
 */

namespace Offsets {
    // Base address for initialization
    constexpr uintptr_t InitBase            = 0xA3F438C;

    // Core Classes & Match Data
    constexpr uintptr_t StaticClass         = 0x5C;
    constexpr uintptr_t CurrentMatch        = 0x50;
    constexpr uintptr_t MatchStatus         = 0x74;
    constexpr uintptr_t LocalPlayer         = 0x7C;
    constexpr uintptr_t DictionaryEntities  = 0x68;

    // Player Data
    constexpr uintptr_t Player_IsDead       = 0x4C;
    constexpr uintptr_t Player_Name         = 0x28C;
    constexpr uintptr_t Player_Data         = 0x44;
    constexpr uintptr_t IsBot               = 0x294;

    // Avatar Management
    constexpr uintptr_t AvatarManager       = 0x460;
    constexpr uintptr_t AvatarPropManager   = 0x468;
    constexpr uintptr_t Avatar              = 0x94;
    constexpr uintptr_t AvatarData          = 0x10;
    constexpr uintptr_t Avatar_IsVisible    = 0x7C;
    constexpr uintptr_t Avatar_IsTeam       = 0x51;

    // Weapon & Aim
    constexpr uintptr_t Weapon              = 0x39C;
    constexpr uintptr_t WeaponData          = 0x58;
    constexpr uintptr_t WeaponRecoil        = 0x0C;
    constexpr uintptr_t AimRotation         = 0x3A8;
    constexpr uintptr_t WeaponName          = 0x38;

    // Camera
    constexpr uintptr_t MainCameraTransform = 0x1FC;
    constexpr uintptr_t FollowCamera        = 0x3F0;
    constexpr uintptr_t Camera              = 0x14;
    constexpr uintptr_t ViewMatrix          = 0xBC;

    // Visuals & Position
    constexpr uintptr_t Player_ShadowBase   = 0x15E8;
    constexpr uintptr_t XPose               = 0x78;

    // Attributes & Tweaks
    constexpr uintptr_t PlayerAttributes    = 0x45C;
    constexpr uintptr_t NoReload            = 0x91;

    // Silent Aim Specific
    constexpr uintptr_t Silent_1            = 0x4E0;
    constexpr uintptr_t Silent_2            = 0x8F0;
    constexpr uintptr_t Silent_3            = 0x38;
    constexpr uintptr_t Silent_4            = 0x2C;

    // Bone Offsets
    constexpr uintptr_t Bone_Head                = 0x3F8;
    constexpr uintptr_t Bone_Spinex              = 0x400;
    constexpr uintptr_t Bone_Hip                 = 0x3FC;
    constexpr uintptr_t Bone_Root                = 0x40C;
    constexpr uintptr_t Bone_LeftWrist           = 0x3F4;
    constexpr uintptr_t Bone_RightCalf           = 0x410;
    constexpr uintptr_t Bone_LeftCalf            = 0x414;
    constexpr uintptr_t Bone_RightFoot           = 0x418;
    constexpr uintptr_t Bone_LeftFoot            = 0x41C;
    constexpr uintptr_t Bone_RightWrist          = 0x420;
    constexpr uintptr_t Bone_LeftHand            = 0x424;
    constexpr uintptr_t Bone_LeftShoulder        = 0x42C;
    constexpr uintptr_t Bone_RightShoulder       = 0x430;
    constexpr uintptr_t Bone_RightWristJoint     = 0x434;
    constexpr uintptr_t Bone_LeftWristJoint      = 0x438;
    constexpr uintptr_t Bone_LeftElbow           = 0x43C;
    constexpr uintptr_t Bone_RightElbow          = 0x440;
    constexpr uintptr_t Bone_HeadCollider        = 0x444;
}

#endif // ANTIGRAVITY_OFFSETS_H
