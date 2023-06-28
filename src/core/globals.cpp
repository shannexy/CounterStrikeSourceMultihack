#include "globals.hpp"
#include "../valve/centity.hpp"
#include "../core/interfaces.hpp"

void globals::UpdateLocalPlayer() noexcept
{
	localPlayerIndex = interfaces::engine->GetLocalPlayerIndex();
	localPlayer = (C_BaseEntity*)interfaces::entityList->GetClientEntity(localPlayerIndex);
}
