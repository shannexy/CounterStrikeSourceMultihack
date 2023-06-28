#include "bunnyhop.hpp"
#include "../core/configuration.hpp"
#include "../core/globals.hpp"
#include "../valve/centity.hpp"
#include "../valve/cusercmd.hpp"

void hacks::RunBunnyHop(CUserCmd* cmd) noexcept
{
	//static auto bJumped = false;
	//static auto bFake = false;
	if (!(globals::localPlayer->GetFlags() & C_BaseEntity::FL_ONGROUND))
	{
		switch (g.bhop.BhopType)
		{
			case NORMAL:
			case PERFECT:
			{
				if (g.bhop.BhopType == NORMAL) {
					cmd->buttons &= ~IN_JUMP;
				}
				else if (g.bhop.BhopType == PERFECT)
				{
					cmd->buttons &= ~IN_JUMP;
					cmd->buttons &= ~IN_DUCK;
				}

				if (g.bhop.AutoStrafe)
				{
					if (cmd->mousedx < 0)
						cmd->sideMove = -415.f;
					else if (cmd->mousedx > 0)
						cmd->sideMove = 415.f;
				}
				break;
			}
			case DISABLED:
				break;
		}
	}
}
