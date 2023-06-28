#include "Spectators.hpp"

void Spectators::FireGameEvent(IGameEvent* event)
{
	if (strcmp(event->GetName(), "round_start") == 0)
	{
		m_spectator_count = 0;
		m_spectator_text_opacity = 0;
	}
	else if (strcmp(event->GetName(), "player_death") == 0)
	{
		if (!globals::localPlayer)
			return;

		auto userid = interfaces::engine->GetPlayerForUserID(event->GetInt("userid"));


		m_spectator_count = 0;
		m_spectator_text_opacity = 0;
	}
}

void Spectators::Draw()
{
	if (!interfaces::engine->IsInGame() || !globals::localPlayer)
		return;

	if (!g.visuals.show_spectators)
		return;
	
	auto spectators = globals::localPlayer->GetSpectators();

	//console::print(spectators.size());
}

void Spectators::RegisterSelf()
{
	interfaces::gameeventmanager->AddListener(this, "round_start", false);
	interfaces::gameeventmanager->AddListener(this, "player_death", false);
}

void Spectators::UnregisterSelf()
{
	interfaces::gameeventmanager->RemoveListener(this);
}
