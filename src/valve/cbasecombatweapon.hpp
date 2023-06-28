#pragma once

#include "cbaseanimating.hpp"
#include "../utils/memory.hpp"

class CUserCmd;
class Activity;
class CBaseCombatCharacter;
class CBaseViewModel;
struct WeaponProficiencyInfo_t;
struct WeaponProficiency_t;
class C_BaseAnimating;
struct WeaponSound_t;
#define NUM_SHOOT_SOUND_TYPES 16
#define MAX_SHOOT_SOUNDS	16			// Maximum number of shoot sounds per shoot type

#define MAX_WEAPON_STRING	80
#define MAX_WEAPON_PREFIX	16
#define MAX_WEAPON_AMMO_NAME		32

class CHudTexture;
class KeyValues;
struct acttable_t;
struct poseparamtable_t;

struct FileWeaponInfo_t
{
public:
	FileWeaponInfo_t()
	{
	}

	virtual void Parse(KeyValues* pKeyValuesData, const char* szWeaponName) = 0;

public:
	bool bParsedScript;
	bool bLoadedHudElements;
	char szClassName[MAX_WEAPON_STRING];
	char szPrintName[MAX_WEAPON_STRING]; // Name for showing in HUD, etc.
	char szViewModel[MAX_WEAPON_STRING]; // View model of this weapon
	char szWorldModel[MAX_WEAPON_STRING]; // Model of this weapon seen carried by the player
	char szAnimationPrefix[MAX_WEAPON_PREFIX];
	// Prefix of the animations that should be used by the player carrying this weapon
	int iSlot; // inventory slot.
	int iPosition; // position in the inventory slot.
	int iMaxClip1; // max primary clip size (-1 if no clip)
	int iMaxClip2; // max secondary clip size (-1 if no clip)
	int iDefaultClip1; // amount of primary ammo in the gun when it's created
	int iDefaultClip2; // amount of secondary ammo in the gun when it's created
	int iWeight; // this value used to determine this weapon's importance in autoselection.
	int iRumbleEffect; // Which rumble effect to use when fired? (xbox)
	bool bAutoSwitchTo; // whether this weapon should be considered for autoswitching to
	bool bAutoSwitchFrom; // whether this weapon can be autoswitched away from when picking up another weapon or ammo
	int iFlags; // miscellaneous weapon flags
	char szAmmo1[MAX_WEAPON_AMMO_NAME]; // "primary" ammo type
	char szAmmo2[MAX_WEAPON_AMMO_NAME]; // "secondary" ammo type
	char aShootSounds[NUM_SHOOT_SOUND_TYPES][MAX_WEAPON_STRING];
	int iAmmoType;
	int iAmmo2Type;
	bool m_bMeleeWeapon; // Melee weapons can always "fire" regardless of ammo.
	bool m_bBuiltRightHanded;
	bool m_bAllowFlipping;
	int iSpriteCount;
	CHudTexture* iconActive;
	CHudTexture* iconInactive;
	CHudTexture* iconAmmo;
	CHudTexture* iconAmmo2;
	CHudTexture* iconCrosshair;
	CHudTexture* iconAutoaim;
	CHudTexture* iconZoomedCrosshair;
	CHudTexture* iconZoomedAutoaim;
	CHudTexture* iconSmall;
	bool bShowUsageHint; // if true, then when you receive the weapon, show a hint about it
};

enum WeaponDefinition
{
	WEAPON_KNIFE = 28,
	WEAPON_P228 = 1,
	WEAPON_GLOCK = 2,
	WEAPON_SCOUT = 3,
	WEAPON_HE_GRENADE = 4,
	WEAPON_XM1014 = 5,
	WEAPON_C4 = 6,
	WEAPON_MAC10 = 7,
	WEAPON_AUG = 8,
	WEAPON_SMOKE_GRENADE = 9,
	WEAPON_ELITE = 10,
	WEAPON_FIVESEVEN = 11,
	WEAPON_UMP = 12,
	WEAPON_SG550 = 13,
	WEAPON_GALIL = 14,
	WEAPON_FAMAS = 15,
	WEAPON_USP = 16,
	WEAPON_AWP = 17,
	WEAPON_MP5 = 18,
	WEAPON_M249 = 19,
	WEAPON_M3 = 20,
	WEAPON_M4A1 = 21,
	WEAPON_TMP = 22,
	WEAPON_G3SG1 = 23,
	WEAPON_FLASHBANG = 24,
	WEAPON_DEAGLE = 25,
	WEAPON_SG552 = 26,
	WEAPON_AK47 = 27,
	WEAPON_P90 = 29,
};

enum EWeaponType
{
	WEAPONTYPE_UNKNOWN = -1,
	WEAPONTYPE_KNIFE,
	WEAPONTYPE_PISTOL,
	WEAPONTYPE_SUBMACHINEGUN,
	WEAPONTYPE_RIFLE,
	WEAPONTYPE_SHOTGUN,
	WEAPONTYPE_SNIPER,
	WEAPONTYPE_MACHINEGUN,
	WEAPONTYPE_C4,
	WEAPONTYPE_GRENADE,
};

class C_BaseCombatWeapon : public C_BaseAnimating 
{
public:
	NETVAR(GetClip, "CBaseCombatWeapon->m_iClip1", int);
	NETVAR(WeaponMode, "CWeaponCSBase->m_weaponMode", int);

	FileWeaponInfo_t& GetWeaponData()
	{
		using OriginalFn = FileWeaponInfo_t & (__thiscall*)(C_BaseCombatWeapon*);
		static auto ReturnFn = (OriginalFn)((unsigned long)memory::PatternScan(
			"client.dll", "0F B7 81 ? ? ? ? 50 E8 ? ? ? ? 83 C4 04 C3"));

		return ReturnFn(this);
	}

	constexpr const int GetWeaponID() noexcept
	{
		return memory::Call<const int>(this, 365);
	}

	constexpr const bool IsSilenced() noexcept
	{
		return memory::Call<const bool>(this, 366);
	}

	constexpr std::string GetWeaponName() noexcept
	{
		auto weaponName = std::string(GetWeaponData().szClassName);
		weaponName.replace(0, 7, "");
		return weaponName;
	}

	constexpr EWeaponType GetWeaponType() noexcept
	{
		auto Weapon = static_cast<WeaponDefinition>(GetWeaponID());

		switch (Weapon)
		{
		case WEAPON_GLOCK:
		case WEAPON_USP:
		case WEAPON_P228:
		case WEAPON_DEAGLE:
		case WEAPON_FIVESEVEN:
		case WEAPON_ELITE:
			return WEAPONTYPE_PISTOL;
		case WEAPON_M3:
		case WEAPON_XM1014:
			return WEAPONTYPE_SHOTGUN;
		case WEAPON_TMP:
		case WEAPON_MAC10:
		case WEAPON_MP5:
		case WEAPON_UMP:
		case WEAPON_P90:
			return WEAPONTYPE_SUBMACHINEGUN;
		case WEAPON_GALIL:
		case WEAPON_FAMAS:
		case WEAPON_AK47:
		case WEAPON_M4A1:
		case WEAPON_SG552:
		case WEAPON_AUG:
			return WEAPONTYPE_RIFLE;
		case WEAPON_M249:
			return WEAPONTYPE_MACHINEGUN;
		case WEAPON_SCOUT:
		case WEAPON_SG550:
		case WEAPON_AWP:
		case WEAPON_G3SG1:
			return WEAPONTYPE_SNIPER;
		case WEAPON_HE_GRENADE:
		case WEAPON_FLASHBANG:
		case WEAPON_SMOKE_GRENADE:
			return WEAPONTYPE_GRENADE;
		}

		return WEAPONTYPE_UNKNOWN;
	}
};