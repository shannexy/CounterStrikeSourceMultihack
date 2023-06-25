#pragma once

#include "../core/interfaces.hpp"
#include "../core/netvar.hpp"
#include "../utils/memory.hpp"
#include "icliententity.hpp"
#include "chandle.hpp"
#include "cutlvector.hpp"

#include <string>

class C_BaseCombatWeapon;

enum e_ClassDefinitions
{
	CTestTraceline = 156,
	CTEWorldDecal = 157,
	CTESpriteSpray = 154,
	CTESprite = 153,
	CTESparks = 152,
	CTESmoke = 151,
	CTEShowLine = 149,
	CTEProjectedDecal = 146,
	CTEPlayerDecal = 145,
	CTEPhysicsProp = 142,
	CTEParticleSystem = 141,
	CTEMuzzleFlash = 140,
	CTELargeFunnel = 138,
	CTEKillPlayerAttachments = 137,
	CTEImpact = 136,
	CTEGlowSprite = 135,
	CTEShatterSurface = 148,
	CTEFootprintDecal = 133,
	CTEFizz = 132,
	CTEExplosion = 130,
	CTEEnergySplash = 129,
	CTEEffectDispatch = 128,
	CTEDynamicLight = 127,
	CTEDecal = 125,
	CTEClientProjectile = 124,
	CTEBubbleTrail = 123,
	CTEBubbles = 122,
	CTEBSPDecal = 121,
	CTEBreakModel = 120,
	CTEBloodStream = 119,
	CTEBloodSprite = 118,
	CTEBeamSpline = 117,
	CTEBeamRingPoint = 116,
	CTEBeamRing = 115,
	CTEBeamPoints = 114,
	CTEBeamLaser = 113,
	CTEBeamFollow = 112,
	CTEBeamEnts = 111,
	CTEBeamEntPoint = 110,
	CTEBaseBeam = 109,
	CTEArmorRicochet = 108,
	CTEMetalSparks = 139,
	CSteamJet = 104,
	CSmokeStack = 99,
	DustTrail = 188,
	CFireTrail = 47,
	SporeTrail = 195,
	SporeExplosion = 194,
	RocketTrail = 192,
	SmokeTrail = 193,
	CPropVehicleDriveable = 90,
	ParticleSmokeGrenade = 191,
	CParticleFire = 73,
	MovieExplosion = 189,
	CTEGaussExplosion = 134,
	CEnvQuadraticBeam = 41,
	CEmbers = 34,
	CEnvWind = 45,
	CPrecipitation = 87,
	CBaseTempEntity = 17,
	CWeaponXM1014 = 186,
	CWeaponUSP = 185,
	CWeaponUMP45 = 184,
	CWeaponTMP = 183,
	CSmokeGrenade = 98,
	CWeaponSG552 = 182,
	CWeaponSG550 = 181,
	CWeaponScout = 180,
	CWeaponP90 = 179,
	CWeaponP228 = 178,
	CWeaponMP5Navy = 177,
	CWeaponMAC10 = 176,
	CWeaponM4A1 = 175,
	CWeaponM3 = 174,
	CWeaponM249 = 173,
	CKnife = 70,
	CHEGrenade = 65,
	CWeaponGlock = 172,
	CWeaponGalil = 171,
	CWeaponG3SG1 = 170,
	CFlashbang = 49,
	CWeaponFiveSeven = 169,
	CWeaponFamas = 168,
	CWeaponElite = 167,
	CDEagle = 31,
	CWeaponCSBaseGun = 165,
	CWeaponCSBase = 164,
	CC4 = 23,
	CBaseCSGrenade = 6,
	CWeaponAWP = 163,
	CWeaponAug = 162,
	CAK47 = 1,
	NextBotCombatCharacter = 190,
	CFootstepControl = 51,
	CCSGameRulesProxy = 26,
	CWeaponCycler = 166,
	CTEPlantBomb = 143,
	CTEFireBullets = 131,
	CTERadioIcon = 147,
	CPlantedC4 = 81,
	CCSTeam = 30,
	CCSPlayerResource = 28,
	CCSPlayer = 27,
	CCSRagdoll = 29,
	CTEPlayerAnimEvent = 144,
	CHostage = 66,
	CBaseCSGrenadeProjectile = 7,
	CHandleTest = 64,
	CTeamplayRoundBasedRulesProxy = 107,
	CSpriteTrail = 103,
	CSpriteOriented = 102,
	CSprite = 101,
	CRagdollPropAttached = 93,
	CRagdollProp = 92,
	CPredictedViewModel = 88,
	CPoseController = 86,
	CGameRulesProxy = 63,
	CInfoLadderDismount = 67,
	CFuncLadder = 56,
	CEnvDetailController = 38,
	CWorld = 187,
	CWaterLODControl = 161,
	CWaterBullet = 160,
	CVoteController = 159,
	CVGuiScreen = 158,
	CPropJeep = 89,
	CTest_ProxyToggle_Networkable = 155,
	CTesla = 150,
	CBaseTeamObjectiveResource = 16,
	CTeam = 106,
	CSun = 105,
	CParticlePerformanceMonitor = 74,
	CSpotlightEnd = 100,
	CSlideshowDisplay = 97,
	CShadowControl = 96,
	CSceneEntity = 95,
	CRopeKeyframe = 94,
	CRagdollManager = 91,
	CPhysicsPropMultiplayer = 79,
	CPhysBoxMultiplayer = 77,
	CBasePropDoor = 15,
	CDynamicProp = 33,
	CPointCommentaryNode = 85,
	CPointCamera = 84,
	CPlayerResource = 83,
	CPlasma = 82,
	CPhysMagnet = 80,
	CPhysicsProp = 78,
	CPhysBox = 76,
	CParticleSystem = 75,
	CMaterialModifyControl = 72,
	CLightGlow = 71,
	CInfoOverlayAccessor = 69,
	CFuncTrackTrain = 62,
	CFuncSmokeVolume = 61,
	CFuncRotating = 60,
	CFuncReflectiveGlass = 59,
	CFuncOccluder = 58,
	CFuncMonitor = 57,
	CFunc_LOD = 53,
	CTEDust = 126,
	CFunc_Dust = 52,
	CFuncConveyor = 55,
	CBreakableSurface = 22,
	CFuncAreaPortalWindow = 54,
	CFish = 48,
	CEntityFlame = 36,
	CFireSmoke = 46,
	CEnvTonemapController = 44,
	CEnvScreenEffect = 42,
	CEnvScreenOverlay = 43,
	CEnvProjectedTexture = 40,
	CEnvParticleScript = 39,
	CFogController = 50,
	CEntityParticleTrail = 37,
	CEntityDissolve = 35,
	CDynamicLight = 32,
	CColorCorrectionVolume = 25,
	CColorCorrection = 24,
	CBreakableProp = 21,
	CBasePlayer = 13,
	CBaseFlex = 10,
	CBaseEntity = 9,
	CBaseDoor = 8,
	CBaseCombatCharacter = 4,
	CBaseAnimatingOverlay = 3,
	CBoneFollower = 20,
	CBaseAnimating = 2,
	CInfoLightingRelative = 68,
	CBeam = 19,
	CBaseViewModel = 18,
	CBaseProjectile = 14,
	CBaseParticleEntity = 12,
	CBaseGrenade = 11,
	CBaseCombatWeapon = 5
};


class C_BaseEntity : public IClientEntity
{
public:
	enum EFlags : std::int32_t
	{
		FL_ONGROUND = (1 << 0),
		FL_DUCKING = (1 << 1),
		FL_ANIMDUCKING = (1 << 2),
		FL_WATERJUMP = (1 << 3),
		FL_ONTRAIN = (1 << 4),
		FL_INRAIN = (1 << 5),
		FL_FROZEN = (1 << 6),
		FL_ATCONTROLS = (1 << 7),
		FL_CLIENT = (1 << 8),
		FL_FAKECLIENT = (1 << 9),
		FL_INWATER = (1 << 10),
		FL_FLY = (1 << 11),
		FL_SWIM = (1 << 12),
		FL_CONVEYOR = (1 << 13),
		FL_NPC = (1 << 14),
		FL_GODMODE = (1 << 15),
		FL_NOTARGET = (1 << 16),
		FL_AIMTARGET = (1 << 17),
		FL_PARTIALGROUND = (1 << 18),
		FL_STATICPROP = (1 << 19),
		FL_GRAPHED = (1 << 20),
		FL_GRENADE = (1 << 21),
		FL_STEPMOVEMENT = (1 << 22),
		FL_DONTTOUCH = (1 << 23),
		FL_BASEVELOCITY = (1 << 24),
		FL_WORLDBRUSH = (1 << 25),
		FL_OBJECT = (1 << 26),
		FL_KILLME = (1 << 27),
		FL_ONFIRE = (1 << 28),
		FL_DISSOLVING = (1 << 29),
		FL_TRANSRAGDOLL = (1 << 30),
		FL_UNBLOCKABLE_BY_PLAYER = (1 << 31)
	};
public:
	NETVAR(GetFlags, "CBasePlayer->m_fFlags", int);
	NETVAR(Money, "CCSPlayer->m_iAccount", int);
	NETVAR(GetHealth, "CBasePlayer->m_iHealth", int);
	NETVAR(GetArmor, "CCSPlayer->m_ArmorValue", int);
	NETVAR(Fov, "CBasePlayer->m_iFOV", int);
	NETVAR(FlashDuration, "CCSPlayer->m_flFlashDuration", float);
	NETVAR(FlashMaxAlpha, "CCSPlayer->m_flFlashMaxAlpha", float);
	NETVAR(GetVecOrigin, "CCSPlayer->m_vecOrigin", CVector);
	NETVAR(GetVelocity, "CBasePlayer->m_vecVelocity[0]", CVector);
	NETVAR(GetOwnerEntity, "CBaseEntity->m_hOwnerEntity", CHandle<C_BaseEntity>);
	NETVAR(GetObserverMode, "CBasePlayer->m_iObserverMode", int);
	NETVAR(GetObserverTarget, "CBasePlayer->m_hObserverTarget", CHandle<C_BaseEntity>);

private:
	NETVAR(ActiveWeapon, "CBaseCombatCharacter->m_hActiveWeapon", CHandle<C_BaseCombatWeapon>);
	NETVAR(ModelIndex, "CBaseEntity->m_nModelIndex", int);
public:
	CHandle<C_BaseCombatWeapon>* MyWeapons() const
	{
		static int _m_hMyWeapons = netvars::GetOffset("CBaseCombatCharacter", "m_hMyWeapons");
		return reinterpret_cast<CHandle<C_BaseCombatWeapon>*>(reinterpret_cast<uintptr_t>(this) + (_m_hMyWeapons));
	}

	constexpr const CVector& GetAbsOrigin() noexcept
	{
		return memory::Call<const CVector&>(this, 9);
	}

	constexpr const CVector& GetEyeAngles() noexcept
	{
		return memory::Call<const CVector&>(this, 193);
	}

	constexpr CVector GetBonePosition(int bone) noexcept
	{
		if (this == nullptr)
		{
			return CVector(0, 0, 0);
		}
		CMatrix3x4 boneMatrix[128];

		if (this->SetupBones(boneMatrix, 128, 0x00000100, 0))
			return CVector(boneMatrix[bone][0][3], boneMatrix[bone][1][3], boneMatrix[bone][2][3]);

		return CVector(0, 0, 0);
	}

	const char* GetBoneName(int bone, bool keepOnlyBoneName = true) noexcept
	{
		studiohdr_t* studioModel = interfaces::modelInfo->GetStudioModel(this->GetModel());

		if (!studioModel)
			return "unknown";

		mstudiobone_t* pBone = studioModel->pBone(bone);

		if (!pBone)
			return "unknown";


		std::string boneName(pBone->GetName());

		if (keepOnlyBoneName)
		{
			boneName.replace(boneName.find("ValveBiped.Bip01_"), std::string("ValveBiped.Bip01_").size(), "");
		}

		auto result = new char[boneName.size() + 1];
		strcpy_s(result, boneName.size() + 1, boneName.c_str());

		return result;
	}

	constexpr int GetTeamNumber() noexcept
	{
		return memory::Call<bool>(this, 74);
	}

	constexpr bool IsAlive() noexcept
	{
		return memory::Call<bool>(this, 130);
	}

	bool IsWeapon() noexcept
	{
		switch (const auto ClassId = static_cast<e_ClassDefinitions>(GetClientClass()->m_ClassID))
		{
		case CWeaponXM1014: return true;
			break;
		case CWeaponUSP: return true;
			break;
		case CWeaponUMP45: return true;
			break;
		case CWeaponTMP: return true;
			break;
		case CWeaponSG552: return true;
			break;
		case CWeaponSG550: return true;
			break;
		case CWeaponScout: return true;
			break;
		case CWeaponP90: return true;
			break;
		case CWeaponP228: return true;
			break;
		case CWeaponMP5Navy: return true;
			break;
		case CWeaponMAC10: return true;
			break;
		case CWeaponM4A1: return true;
			break;
		case CWeaponM3: return true;
			break;
		case CWeaponM249: return true;
			break;
		case CWeaponGlock: return true;
			break;
		case CWeaponGalil: return true;
			break;
		case CWeaponG3SG1: return true;
			break;
		case CWeaponFiveSeven: return true;
			break;
		case CWeaponFamas: return true;
			break;
		case CWeaponElite: return true;
			break;
		case CWeaponAWP: return true;
			break;
		case CWeaponAug: return true;
			break;
		case CDEagle: return true;
			break;
		case CKnife: return true;
			break;
		case CHEGrenade: return true;
			break;
		case CFlashbang: return true;
			break;
		case CSmokeGrenade: return true;
			break;
		default: return false;
			break;
		}
		return false;
	}

	//bool GetPlayerInfo(player_info_t info)
	//{
	//	return interfaces::engine->GetPlayerInfo(this->entindex(), &info);
	//}

	constexpr bool IsPlayer() noexcept
	{
		return memory::Call<bool>(this, 131);
	}

	constexpr bool IsNPC() noexcept
	{
		if (this == nullptr)
		{
			return false;
		}
		player_info_t p_info;
		if (interfaces::engine->GetPlayerInfo(entindex(), &p_info))
		{
			return p_info.fakeplayer;
		}
		return false;
	}

	bool IsObject() noexcept
	{
		return false;
	}

	CVector EyePosition() noexcept
	{
		return this->GetAbsOrigin() + *reinterpret_cast<CVector*>(uintptr_t(this) + 0x0E8);
	}

	CVector AimPunch() noexcept
	{
		return *reinterpret_cast<CVector*>(uintptr_t(this) + 0x0DDC + 0x6C);
	}

	constexpr bool IsDormant() noexcept
	{
		return memory::Call<bool>(this + 0x8, 9);
	}


	static __forceinline C_BaseEntity* GetEntityByIndex(int index)
	{
		return static_cast<C_BaseEntity*>(interfaces::entityList->GetClientEntity(index));
	}

	static __forceinline C_BaseEntity* GetEntityFromHandle(CBaseHandle h)
	{
		return static_cast<C_BaseEntity*>(interfaces::entityList->GetClientEntityFromHandle(h));
	}

	C_BaseCombatWeapon* GetActiveWeapon() noexcept
	{
		return reinterpret_cast<C_BaseCombatWeapon*>(C_BaseEntity::GetEntityFromHandle(ActiveWeapon()));
	}
};