#pragma once
#include "../valve/igameventmanager.hpp"
#include "../valve/centity.hpp"
#include "../core/globals.hpp"
#include "../core/Singleton.hpp"
#include "../core/interfaces.hpp"
#include "../core/configuration.hpp"
#include <type_traits>
#include <vector>
#include <memory>

class Spectators : public IGameEventListener2, public Singleton<Spectators>
{
private:
	int m_spectator_count;
	float m_spectator_text_opacity;
	bool m_observed;
	float m_opacities[32] = { 0 };
public:
	void FireGameEvent(IGameEvent* event);
	void Draw();
	void RegisterSelf();
	void UnregisterSelf();
};