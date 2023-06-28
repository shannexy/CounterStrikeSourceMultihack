#include "ThirdPerson.hpp"

#include "../core/globals.hpp"
#include "../core/configuration.hpp"

#include "../valve/centity.hpp"

void ThirdPerson::FrameStageNotify()
{
	if (!interfaces::engine->IsInGame())
		return;

	if (interfaces::engine->IsTakingScreenshot())
		return;

	static CVector vecAngles;
	interfaces::engine->GetViewAngles(vecAngles);

	if (g.misc.thirdperson && globals::localPlayer->IsAlive())
	{
		if (!interfaces::cinput->m_fCameraInThirdPerson)
		{
			interfaces::cinput->m_fCameraInThirdPerson = true;

			//g_ThirdPersonManager
		}
	}
}
