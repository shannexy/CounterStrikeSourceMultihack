#pragma once

#include <cstdint>
class C_BaseEntity;

namespace globals
{
	inline std::int32_t windowWidth, windowHeight;

	inline std::int32_t localPlayerIndex;
	inline C_BaseEntity* localPlayer = nullptr;

	void UpdateLocalPlayer() noexcept;
}