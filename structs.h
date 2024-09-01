#pragma once
#include <Windows.h>
#include <cstdint>
#include <gl/GL.h>
#include <array>
#include <string>

namespace Game
{
	class GunInfo;
}

// Vector Structures
struct Vec3 {
    float x;
    float y;
    float z;
};

struct Vec2 {
    float x;
    float y;
};

struct Vec4 {
    float x;
    float y;
    float z;
    float w;
};

// Entity Structure
struct Entity {
    // Pointers to game data
    BYTE* playerBase = nullptr;
    BYTE* bDead = nullptr;
    float* xHeadCoord = nullptr;
    float* yHeadCoord = nullptr;
    float* zHeadCoord = nullptr;
    int* team = nullptr;
    int* playerNumber = nullptr;
    int* health = nullptr;
    char* name = nullptr;
    float* xOrigin = nullptr;
    float* yOrigin = nullptr;
    float* zOrigin = nullptr;

    // Calculated values
    bool bChecked = false;
    float relativeX = 0.0f;
    float relativeY = 0.0f;
    float relativeZ = 0.0f;
    float distanceFromMe = 99999.0f;
    bool bTargetable = false;
    bool bVisible = false;
    float screenX = 0.0f;
    float screenY = 0.0f;

    // Additional pointers
    float* pitch = nullptr;
    float* yaw = nullptr;
    int* Ammo = nullptr;
    int* grenade = nullptr;
    Game::GunInfo* currentWeapon = nullptr;
    int* Armor = nullptr;
};

// Offsets Structure
struct Offsets {
    // Player information offsets
    static constexpr DWORD gplayerNum = 0x58AC0C;
    static constexpr DWORD entListAddr = 0x18AC04;
    static constexpr DWORD offset_GameWidth = 0x191ED8;
    static constexpr DWORD offset_GameHeight = 0x191EDC;
    static constexpr DWORD crosshairOffset = 0x5F573;

    // Entity offsets
    static constexpr DWORD xHeadCoord = 0x4;
    static constexpr DWORD yHeadCoord = 0x8;
    static constexpr DWORD zHeadCoord = 0xC;
    static constexpr DWORD xOrigin = 0x28;
    static constexpr DWORD yOrigin = 0x2C;
    static constexpr DWORD zOrigin = 0x30;
    static constexpr DWORD team = 0x30C;
    static constexpr DWORD playerNumber = 0x1C4;
    static constexpr DWORD bDead = 0x318;
    static constexpr DWORD name = 0x205;
    static constexpr DWORD health = 0xEC;
    static constexpr DWORD currentWeapon = 0x364;
    static constexpr DWORD Armor = 0xF0;
    static constexpr DWORD EntityList = 0x18AC04;
    static constexpr DWORD localPlayer = 0x18AC00;
    static constexpr DWORD recoil1 = 0x005E;
    static constexpr DWORD recoil2 = 0x0060;

    // Additional offsets
    static constexpr DWORD yaw = 0x34;
    static constexpr DWORD pitch = 0x38;
    static constexpr DWORD Ammo = 0x140;
    static constexpr DWORD grenade = 0x144;
};

// Colors Structure
struct Color {
    std::array<GLubyte, 3> red = { 255, 0, 0 };
    std::array<GLubyte, 3> green = { 0, 255, 0 };
    std::array<GLubyte, 3> blue = { 0, 0, 153 };
    std::array<GLubyte, 3> purple = { 76, 0, 153 };
    std::array<GLubyte, 3> rgb = { 255, 255, 255 };
};

// External Declarations
extern Color color;
extern Entity entity[];
extern Entity myself;
extern Offsets offsets;

// Function Declarations
void UpdateEntInfo();

// Game Namespace
namespace Game {
    class Ent2 {
    public:
        virtual void Function0();
        virtual void Function1();
        virtual void Function2();
        virtual void Function3();
        virtual void Function4();
        virtual void Function5();
        virtual void Function6();
        virtual void Function7();
        virtual void Function8();
        virtual void Function9();

        // Data members
        float x; // 0x0004
        float y; // 0x0008
        float z; // 0x000C
        float x_foot; // 0x0028
        float y_foot; // 0x002C
        float z_foot; // 0x0030
        float yaw; // 0x0034
        float pitch; // 0x0038
        int8_t specMode; // 0x0076
        float velocity; // 0x0010 
        float velocity2; // 0x001C 
        float pos; // 0x0028 
        float vViewAngle; // 0x0034 
        char pad_0x0040[0x4]; // 0x0040
        float N00000015; // 0x0044 
        char pad_0x0048[0x4]; // 0x0048
        float N00000017; // 0x004C 
        float N00000018; // 0x0050 
        float N00000019; // 0x0054 
        char pad_0x0058[0x4]; // 0x0058
        int32_t bJumping; // 0x005C 
        char pad_0x0060[0x4]; // 0x0060
        int32_t bScoping; // 0x0064 
        char pad_0x0068[0x2C]; // 0x0068
        float N00000029; // 0x0094 
        char pad_0x0098[0x54]; // 0x0098
        int32_t health; // 0x00EC 
        int32_t armor; // 0x00F0 
        char pad_0x00F4[0x14]; // 0x00F4
        int32_t pistolMag; // 0x0108 
        int32_t dmrMag; // 0x010C 
        int32_t shotgunMag; // 0x0110 
        int32_t smgMag; // 0x0114 
        int32_t sniperMag; // 0x0118 
        int32_t arMag; // 0x011C 
        char pad_0x0120[0xC]; // 0x0120
        int32_t pistolAmmo1; // 0x012C 
        int32_t dmrAmmo1; // 0x0130 
        int32_t shotgunAmmo1; // 0x0134 
        int32_t smgAmmo1; // 0x0138 
        int32_t sniperAmmo1; // 0x013C 
        int32_t arAmmo1; // 0x0140 
        int32_t grenades; // 0x0144 
        char pad_0x0148[0x4]; // 0x0148
        int32_t knifeWait; // 0x014C 
        int32_t pistolWait; // 0x0150 
        int32_t dmrWait; // 0x0154 
        int32_t shotgunWait; // 0x0158 
        int32_t smgWait; // 0x015C 
        int32_t sniperWait; // 0x0160 
        int32_t arWait; // 0x0164 
        int32_t grenadeWait; // 0x0168 
        char pad_0x016C[0x4]; // 0x016C
        int32_t knifed; // 0x0170 
        int32_t pistolFired; // 0x0174 
        int32_t dmrFired; // 0x0178 
        int32_t shotgunFired; // 0x017C 
        int32_t smgFired; // 0x0180 
        int32_t sniperFired; // 0x0184 
        int32_t arFired; // 0x0188 
        int32_t grenadesThrown; // 0x018C 
        char pad_0x0190[0x4C]; // 0x0190
        int32_t scoreboardKills; // 0x01DC 
        char pad_0x01E0[0x4]; // 0x01E0
        float N00000034; // 0x01E4 
        float N00000035; // 0x01E8 
        float N00000036; // 0x01EC 
        float N00000037; // 0x01F0 
        char pad_0x01F4[0xC]; // 0x01F4
        int32_t N00000038; // 0x0200 
        float N00000039; // 0x0204 
        char pad_0x0208[0x4]; // 0x0208
        int32_t N0000003A; // 0x020C 
        int32_t N0000003B; // 0x0210 
        int32_t N0000003C; // 0x0214 
        float N0000003D; // 0x0218 
        float N0000003E; // 0x021C 
        float N0000003F; // 0x0220 
        int32_t N00000040; // 0x0224 
        char pad_0x0228[0x14]; // 0x0228
    };

    class CurrentWeapon {
    public:
        int32_t id; // 0x0000
        float x; // 0x0004
        float y; // 0x0008
        float z; // 0x000C
        float rotation; // 0x0010
        float pitch; // 0x0014
        float roll; // 0x0018
        float viewAngle; // 0x001C
        float velocity; // 0x0020
        int32_t ammo; // 0x0024
    };

    class LastWeapon {
    public:
        int32_t id; // 0x0000
        float recoil; // 0x0004
        float spread; // 0x0008
        float z; // 0x000C
        float rotation; // 0x0010
        float pitch; // 0x0014
        float roll; // 0x0018
        float viewAngle; // 0x001C
        float velocity; // 0x0020
        int32_t ammo; // 0x0024
    };

    class WeaponData {
    public:
        int32_t id; // 0x0000
        float recoil; // 0x0004
        float spread; // 0x0008
        int32_t damage; // 0x000C
        float range; // 0x0010
    };

    class Weapon {
    public:
        int32_t id; // 0x0000
        int32_t ammo; // 0x0004
        int32_t magSize; // 0x0008
        WeaponData data; // 0x000C
    };

    class WeaponClass {
    public:
        int32_t id; // 0x0000
        std::string name; // 0x0004
        WeaponData data; // 0x0008
    };


    class GunInfo
    {
    public:
        char WeaponID[23]; //0x0000
        char WeaponName[43]; //0x0017
        int16_t sound; //0x0042
        int16_t reload; //0x0044
        int16_t reloadtime; //0x0046
        int16_t attackdelay; //0x0048
        int16_t damage; //0x004A
        int16_t piercing; //0x004C
        int16_t projspeed; //0x004E
        int16_t part; //0x0050
        int16_t spread; //0x0052
        int16_t recoil; //0x0054
        int16_t magsize; //0x0056
        int16_t mdl_kick_rot; //0x0058
        int16_t mdl_kick_back; //0x005A
        int16_t recoilincrease; //0x005C
        int16_t recoilbase; //0x005E
        int16_t maxrecoil; //0x0060
        int16_t recoilbackfade; //0x0062
        int16_t pushfactor; //0x0064
        bool bIsAuto; //0x0066
        char pad_0067[215]; //0x0067
    }; //Size: 0x013E
}

// Ensure proper function prototypes and class method implementations
void UpdateEntInfo();


// Ensure that external variables are defined in a source file
extern Color color;
extern Entity entity[32]; // Adjust size as needed
extern Entity myself;
extern Offsets offsets;

