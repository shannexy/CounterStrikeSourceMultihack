#include "bunnyhop.hpp"
#include "../core/configuration.hpp"
#include "../core/globals.hpp"
#include "../valve/centity.hpp"
#include "../valve/cusercmd.hpp"

void hacks::RunBunnyHop(CUserCmd* cmd) noexcept
{
	//static auto bJumped = false;
	//static auto bFake = false;
	if (g.bhop.enabled) 
	{
		if (!(globals::localPlayer->GetFlags() & C_BaseEntity::FL_ONGROUND))
		{
			cmd->buttons &= ~IN_JUMP;
		}
	}
}
