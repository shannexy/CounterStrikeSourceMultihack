#include "BulletImpact.hpp"
#include "../core/interfaces.hpp"
#include "../core/console.hpp"
#include "../core/configuration.hpp"
#include "../core/globals.hpp"
#include "../valve/centity.hpp"
#include "../valve/structs.hpp"
#include "../core/interfaces.hpp"

void BulletImpactEvent::FireGameEvent(IGameEvent* event)
{
	if (!globals::localPlayer || !event)
		return;

	if (g.visuals.bullet_impact)
	{
		int userid = event->GetInt("userid");
		float x = event->GetFloat("x"), y = event->GetFloat("y"), z = event->GetFloat("z");
		if (interfaces::engine->GetPlayerForUserID(userid) == globals::localPlayerIndex &&
			globals::localPlayer && globals::localPlayer->IsAlive() || g.visuals.others_bullet_impact)
		{
			bulletImpactInfo.push_back({
				userid, interfaces::globals->curtime, CVector(x,y,z)
			});
		}
	}
}

void BulletImpactEvent::RegisterSelf()
{
	interfaces::gameeventmanager->AddListener(this, "bullet_impact", false);
}

void BulletImpactEvent::UnregisterSelf()
{
	interfaces::gameeventmanager->RemoveListener(this);
}

void BulletImpactEvent::Paint()
{
	if (!g.visuals.bullet_impact)
		return;

	if (!interfaces::engine->IsInGame() || !globals::localPlayer || !globals::localPlayer->IsAlive()) {
		bulletImpactInfo.clear();
		return;
	}

	std::vector<BulletImpactInfo>& impacts = bulletImpactInfo;

	if (impacts.empty())
		return;

	int red = (int)(g.visuals.bullet_impact_color[0] * 255);
	int green = (int)(g.visuals.bullet_impact_color[1] * 255);
	int blue = (int)(g.visuals.bullet_impact_color[2] * 255);
	int alpha = (int)(g.visuals.bullet_impact_color[3]);

	CColor current_color(red, green, blue, alpha);
	for (size_t i = 0; i < impacts.size(); i++)
	{
		auto& current_impact = impacts.at(i);

		BeamInfo_t beamInfo;

		beamInfo.m_nType = TE_BEAMPOINTS;
		beamInfo.m_pszModelName = "sprites/physbeam.vmt";
		beamInfo.m_nModelIndex = -1;
		beamInfo.m_flHaloScale = 0.0f;
		beamInfo.m_flLife = g.visuals.bullet_impact_duration;
		beamInfo.m_flWidth = 1.5f;
		beamInfo.m_flEndWidth = 1.5f;
		beamInfo.m_flFadeLength = 0.0f;
		beamInfo.m_flAmplitude = 2.0f;
		beamInfo.m_flBrightness = 255.f;
		beamInfo.m_flSpeed = 0.2f;
		beamInfo.m_nStartFrame = 0;
		beamInfo.m_flFrameRate = 0.f;
		beamInfo.m_flRed = current_color.r;
		beamInfo.m_flGreen = current_color.g;
		beamInfo.m_flBlue = current_color.b;
		beamInfo.m_nSegments = 2;
		beamInfo.m_bRenderable = true;
		beamInfo.m_nFlags = FBEAM_ONLYNOISEONCE | FBEAM_NOTILE | FBEAM_HALOBEAM;


		beamInfo.m_vecStart = C_BaseEntity::GetEntityByIndex(interfaces::engine->GetPlayerForUserID(current_impact.fromUserId))->EyePosition();
		beamInfo.m_vecEnd = current_impact.m_vecHitPos;

		Beam_t* beam = interfaces::renderBeams->CreateBeamPoints(beamInfo);
		if (beam)
			interfaces::renderBeams->DrawBeam(beam);

		interfaces::debugOverlay->AddBoxOverlay(current_impact.m_vecHitPos, CVector(-3, -3, -3), CVector(3, 3, 3), CVector(0, 0, 0), current_color.r, current_color.g, current_color.b, current_color.a, g.visuals.bullet_impact_duration);

		impacts.erase(impacts.begin() + i);
	}
}
