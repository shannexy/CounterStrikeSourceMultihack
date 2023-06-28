#include "aimbot.hpp"
#include "../core/interfaces.hpp"
#include "../valve/cusercmd.hpp"
#include "../valve/cbasecombatweapon.hpp"
#include "../valve/centity.hpp"
#include "../core/configuration.hpp"
#include "../core/globals.hpp"

void hacks::RunAimbot(CUserCmd* cmd) noexcept {
	if (!g.aimbot.enabled)
		return;

	if (!(cmd->buttons & IN_ATTACK))
		return;

	CVector bestAngle{};
	float bestFov{ g.aimbot.fov };

	C_BaseCombatWeapon* activeWeapon = globals::localPlayer->GetActiveWeapon();

	if (!activeWeapon)
		return;

	const auto weaponType = activeWeapon->GetWeaponType();

	switch (weaponType) {
	case EWeaponType::WEAPONTYPE_MACHINEGUN:
	case EWeaponType::WEAPONTYPE_RIFLE:
	case EWeaponType::WEAPONTYPE_SHOTGUN:
	case EWeaponType::WEAPONTYPE_SNIPER:
	case EWeaponType::WEAPONTYPE_PISTOL:
	{
		if (!activeWeapon->GetClip())
			return;

		if (weaponType == EWeaponType::WEAPONTYPE_SNIPER) {
			if (activeWeapon->WeaponMode() == 0)
				return;
		}

		break;
	}

	default:
		return;
	}

	for (int i = 1; i < interfaces::globals->maxClients; i++) {
		const auto player = C_BaseEntity::GetEntityByIndex(i);

		if (!player)
			continue;

		if (player->IsDormant() || !player->IsAlive())
			continue;


		if (player->GetTeamNumber() == globals::localPlayer->GetTeamNumber())
			continue;

		CMatrix3x4 bones[128];

		if (!player->SetupBones(bones, 128, 256, interfaces::globals->curtime))
			continue;

		CVector localEyePosition = globals::localPlayer->EyePosition();

		CVector aimPunch{};

		switch (weaponType) {
		case WEAPONTYPE_RIFLE:
		case WEAPONTYPE_SUBMACHINEGUN:
		case WEAPONTYPE_MACHINEGUN:
		case WEAPONTYPE_UNKNOWN:
		case WEAPONTYPE_PISTOL:
		case WEAPONTYPE_SHOTGUN:
		case WEAPONTYPE_SNIPER:
			aimPunch = globals::localPlayer->AimPunch();
		default: break;
		}

		CTrace trace;
		Ray_t ray;

		ray.Init(localEyePosition, bones[g.aimbot.aimBone].Origin());
		interfaces::engineTrace->TraceRay(
			ray,
			0x46004003,
			CTraceFilter{ globals::localPlayer },
			trace
		);

		if (!trace.m_pEnt || trace.flFraction < 0.97f)
			continue;

		CVector enemyAngle{
			(bones[14].Origin() - localEyePosition).ToAngle() - (cmd->viewAngles + aimPunch)
		};

		if (const float fov = std::hypot(enemyAngle.x, enemyAngle.y); fov < bestFov) {
			bestFov = fov;
			bestAngle = enemyAngle;
		}
	}
	cmd->viewAngles = cmd->viewAngles + (g.aimbot.smooth ? bestAngle : bestAngle.Scale(g.aimbot.smoothCount));
}