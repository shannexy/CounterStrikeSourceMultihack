#include "hooks.hpp"

#include "console.hpp"
#include "../core/gui.hpp"
#include "../core/globals.hpp"
#include "../valve/centity.hpp"
#include "../valve/cbasecombatweapon.hpp"
#include <fmt/core.h>
#include <fmt/format.h>

#include "../features/bunnyhop.hpp"
#include "../features/aimbot.hpp"
#include "../features/BulletImpact.hpp"
#include "../features/Spectators.hpp"
#include "../features/ThirdPerson.hpp"

std::string join(std::vector<std::string> const& strings, std::string delim)
{
	std::stringstream ss;
	std::copy(strings.begin(), strings.end(),
		std::ostream_iterator<std::string>(ss, delim.c_str()));
	return ss.str();
}

void __stdcall hooks::CreateMove(int sequence_number, float input_sample_frametime, bool active)
{
	CreateMoveOriginal(interfaces::cinput, sequence_number, input_sample_frametime, active);

	auto cmd = interfaces::cinput->GetUserCmd(sequence_number);
	auto verified = interfaces::cinput->GetVerifiedCmd(sequence_number);


	if (!cmd || !verified)
		return;

	globals::UpdateLocalPlayer();
	if (globals::localPlayer && globals::localPlayer->IsAlive())
	{
		hacks::RunAimbot(cmd);
		hacks::RunBunnyHop(cmd);
	}

	verified->m_cmd = *cmd;
	verified->m_crc = cmd->GetChecksum();
}

void* __stdcall hooks::AllocKeyValuesMemory(const std::int32_t size) noexcept
{
	void* returnAddress = _ReturnAddress();

	return AllocKeyValuesMemoryOriginal(interfaces::keyValuesSystem, size);
}

void __stdcall hooks::LockCursor() noexcept
{
	if (!gui::open)
		return LockCursorOriginal(interfaces::surface);

	return interfaces::surface->UnlockCursor();
}

void __stdcall hooks::PaintTraverse(std::uintptr_t vguiPanel, bool forceRepaint, bool allowForce) noexcept
{
	static bool hadInitFonts = false;
	static std::uintptr_t EspPanel = NULL;

	const char* panelName = interfaces::panel->GetName(vguiPanel);

	if (strcmp(panelName, "FocusOverlayPanel") == 0 && !EspPanel) {
		EspPanel = vguiPanel;
		if (!hadInitFonts) {
			Fonts::initializeFonts();
			hadInitFonts = true;
		}
	}

	if (EspPanel == vguiPanel) {
		if (interfaces::engine->IsInGame() && interfaces::engine->IsConnected())
		{
			interfaces::engine->GetScreenSize(globals::windowWidth, globals::windowHeight);

			if (!globals::localPlayer)
				return;

			if (!globals::localPlayer->IsAlive())
				return;


			if(StartDrawing)
				StartDrawing(interfaces::surface);

			Spectators::Get().Draw();

			if (g.aimbot.showFov) {
				interfaces::surface->DrawSetColor(static_cast<int>(g.aimbot.fovColor[0] * 255), static_cast<int>(g.aimbot.fovColor[1] * 255), static_cast<int>(g.aimbot.fovColor[2] * 255), static_cast<int>(g.aimbot.fovColor[3]));
				interfaces::surface->DrawOutlinedCircle((int)globals::windowWidth / 2, (int)globals::windowHeight / 2, static_cast<int>(g.aimbot.fov * 2), static_cast<int>(g.aimbot.fov) * 2);
			}
			const CVector local_head_position = globals::localPlayer->GetBonePosition(14);
			if (g.esp.enabled)
			{
				for (int i = 1; i < interfaces::globals->maxClients; ++i) {
					auto* player = C_BaseEntity::GetEntityByIndex(i);

					if (!player)
						continue;

					CHandle<C_BaseCombatWeapon>* weapons = player->MyWeapons();
					C_BaseCombatWeapon* player_weapon = player->GetActiveWeapon();

					if (!player_weapon)
						continue;

					const FileWeaponInfo_t& weapon_data = player_weapon->GetWeaponData();

					player_info_t player_info;

					if (!player->GetPlayerInfo(&player_info))
						continue;

					if (i == interfaces::engine->GetLocalPlayerIndex())
						continue;

					if (player->IsDormant() || !player->IsAlive())
						continue;

					if (player->GetTeamNumber() == globals::localPlayer->GetTeamNumber() && !g.esp.showTeammates)
						continue;

					if (!globals::localPlayer->IsAlive())
						if (globals::localPlayer->GetObserverTarget() == player)
							continue;

					CMatrix3x4 bones[128];
					if (!player->SetupBones(bones, 128, 0x7FF00, interfaces::globals->curtime))
						continue;

					CVector top;
					if (interfaces::debugOverlay->ScreenPosition(bones[14].Origin() + CVector{ 0.f, 0.f, 11.f }, top))
						continue;

					CVector bottom;
					if (interfaces::debugOverlay->ScreenPosition(player->GetAbsOrigin() - CVector{ 0.f, 0.f, 9.f }, bottom))
						continue;

					const float distance = (bones[14].Origin() - local_head_position).Length();

					const float h = bottom.y - top.y;

					const float w = h * 0.3f;

					const auto left = static_cast<int>(top.x - w);
					const auto right = static_cast<int>(top.x + w);

					const float* box_color = (player->GetTeamNumber() != globals::localPlayer->GetTeamNumber())
						? g.esp.drawBoxEnemyColor
						: g.esp.drawBoxFriendlyColor;

					const vector<std::string> esp_strings = {
						std::format("Weapon: {}", player_weapon->GetWeaponName()),
						(player_info.fakeplayer ? std::format("Name: {} (BOT)", player->GetPlayerName()) : std::format("Name: {}", player_info.name)),
						std::format("Distance: {}", distance),
						std::format("Health: {}", player->GetHealth()),
						std::format("Velocity: {}", player->GetVelocity().Length())
					};

					for (auto esp_data : esp_strings)
					{
						auto it = std::find(esp_strings.begin(), esp_strings.end(), esp_data);
						if (it != esp_strings.end())
						{
							int index = it - esp_strings.begin();
							Draw::String(right + 2, static_cast<int>(top.y) + (index * 13), Fonts::espFont, CColor(255, 255, 255, 255), esp_data.c_str(), false);
						}
					}


					//if (HasC4(player))
					//{
					//	Draw::String(right + 2, (static_cast<int>(top.y) + 52), Fonts::espFont, CColor(255, 255, 255, 255), "Has C4", false);
					//}

					interfaces::surface->DrawSetColor(static_cast<int>(box_color[0] * 255), static_cast<int>(box_color[1] * 255), static_cast<int>(box_color[2] * 255), static_cast<int>(box_color[3]));

					interfaces::surface->DrawOutlinedRect(left, static_cast<int>(top.y), right, static_cast<int>(bottom.y));

					interfaces::surface->DrawSetColor(0, 0, 0, 255);

					interfaces::surface->DrawOutlinedRect(left - 1, static_cast<int>(top.y) - 1, right + 1, static_cast<int>(bottom.y) + 1);
					interfaces::surface->DrawOutlinedRect(left + 1, static_cast<int>(top.y) + 1, right - 1, static_cast<int>(bottom.y) - 1);

					if (g.esp.drawHealthBar) {
						interfaces::surface->DrawOutlinedRect(left - 6, static_cast<int>(top.y) - 1, left - 3, static_cast<int>(bottom.y) + 1);

						const float healthFraction = player->GetHealth() * 0.01f;

						interfaces::surface->DrawSetColor(static_cast<int>((1.0f - healthFraction) * 255), static_cast<int>(255 * healthFraction), 0, 255);
						interfaces::surface->DrawFilledRect(left - 5, static_cast<int>(bottom.y - (h * healthFraction)), left - 4, static_cast<int>(bottom.y));
					}

					if (g.esp.drawBones) {
						const studiohdr_t* StudioModel = interfaces::modelInfo->GetStudioModel(player->GetModel());
						for (int j = 0; j < StudioModel->numbones; j++) {
							const mstudiobone_t* pBone = StudioModel->pBone(j);

							if (pBone && (pBone->flags & BONE_USED_BY_HITBOX | BONE_USED_BY_ATTACHMENT) && (pBone->parent != -1)) {
								CVector vChild;
								CVector vParent;

								if (interfaces::debugOverlay->ScreenPosition(player->GetBonePosition(j), vChild))
									continue;

								if (interfaces::debugOverlay->ScreenPosition(player->GetBonePosition(pBone->parent), vParent))
									continue;

								interfaces::surface->DrawSetColor(static_cast<int>(g.esp.drawBoneColor[0] * 255), static_cast<int>(g.esp.drawBoneColor[1] * 255), static_cast<int>(g.esp.drawBoneColor[2] * 255), static_cast<int>(g.esp.drawBoneColor[3]));
								interfaces::surface->DrawLine(static_cast<int>(vParent.x), static_cast<int>(vParent.y), static_cast<int>(vChild.x), static_cast<int>(vChild.y));

								if (g.esp.drawBoneNames) {
									Draw::String(static_cast<int>(vChild.x), static_cast<int>(vChild.y), Fonts::espFont, CColor(255, 255, 255, 255), pBone->GetName(), false);
								}
							}
						}
					}

					if (g.esp.drawSnapLines) {
						CVector entPos{};
						if (interfaces::debugOverlay->ScreenPosition(player->GetAbsOrigin(), entPos))
							continue;

						interfaces::surface->DrawSetColor(static_cast<int>(g.esp.drawSnapLineColor[0] * 255), static_cast<int>(g.esp.drawSnapLineColor[1] * 255), static_cast<int>(g.esp.drawSnapLineColor[2] * 255), static_cast<int>(g.esp.drawSnapLineColor[3]));
						interfaces::surface->DrawLine(static_cast<int>(entPos.x), static_cast<int>(entPos.y), globals::windowWidth / 2, globals::windowHeight);
					}
				}
			}
			if(FinishDrawing)
				FinishDrawing(interfaces::surface);
		}
	}

	PaintTraverseOriginal(interfaces::panel, vguiPanel, forceRepaint, allowForce);
}

long __stdcall hooks::EndScene(IDirect3DDevice9* device) noexcept
{
	HRESULT result;

	DWORD dwReturnAddress;

	static DWORD dwAllowedReturn = 0;

	__asm
	{
		push eax
		mov eax, [ebp + 4]
		mov dwReturnAddress, eax
		pop eax
	}

	result = EndSceneOriginal(device, device);

	if (dwAllowedReturn == 0 || dwAllowedReturn == dwReturnAddress) 
	{
		if (!gui::setup)
		{
			gui::SetupMenu(device);
		}

		if (gui::open)
			gui::Render();
		dwAllowedReturn = dwReturnAddress;
	}

	return result;
}

HRESULT __stdcall hooks::Reset(IDirect3DDevice9* device, D3DPRESENT_PARAMETERS* params) noexcept
{
	ImGui_ImplDX9_InvalidateDeviceObjects();
	const auto result = ResetOriginal(device, device, params);
	ImGui_ImplDX9_CreateDeviceObjects();
	return result;
}

void __stdcall hooks::DrawModel(void* results, const CDrawModelInfo& info, CMatrix3x4* bones, float* flexWeights, float* flexDelayedWeights, const CVector& modelOrigin, const std::int32_t flags) noexcept
{
	if (globals::localPlayer && globals::localPlayer->IsAlive() && globals::localPlayer->GetTeamNumber() != 1 && info.renderable && interfaces::engine->IsInGame() && interfaces::engine->IsConnected()) {
		C_BaseEntity* entity = info.renderable->GetIClientUnknown()->GetBaseEntity();

		if (entity && entity->IsPlayer() && entity->GetTeamNumber() != globals::localPlayer->GetTeamNumber() && g.chams.enabled) {
			static IMaterial* material = interfaces::materialSystem->FindMaterial("debug/debugambientcube");

			interfaces::studioRender->SetAlphaModulation(1.f);
			material->SetMaterialVarFlag(IMaterial::IGNOREZ, true);
			interfaces::studioRender->SetColorModulation(g.chams.HiddenColor);
			interfaces::studioRender->ForcedMaterialOverride(material);
			DrawModelOriginal(interfaces::studioRender, results, info, bones, flexWeights, flexDelayedWeights, modelOrigin, flags);

			material->SetMaterialVarFlag(IMaterial::IGNOREZ, false);
			interfaces::studioRender->SetColorModulation(g.chams.VisibleColor);
			interfaces::studioRender->ForcedMaterialOverride(material);
			DrawModelOriginal(interfaces::studioRender, results, info, bones, flexWeights, flexDelayedWeights, modelOrigin, flags);

			return interfaces::studioRender->ForcedMaterialOverride(nullptr);
		}

		const char* pszModelName = interfaces::modelInfo->GetModelName(entity->GetModel());

		C_BaseEntity* parent = entity->GetOwnerEntity().Get();

		if (strstr(pszModelName, "flash") && g.visuals.no_flash) {

			IMaterial* FlashWhite = interfaces::materialSystem->FindMaterial("effects\\flashbang_white", TEXTURE_GROUP_CLIENT_EFFECTS);
			FlashWhite->SetMaterialVarFlag(IMaterial::NO_DRAW, true);
			interfaces::studioRender->ForcedMaterialOverride(FlashWhite);
			DrawModelOriginal(interfaces::studioRender, results, info, bones, flexWeights, flexDelayedWeights, modelOrigin, flags);
			return interfaces::studioRender->ForcedMaterialOverride(nullptr);

		}
		else if (strstr(pszModelName, "smoke") && g.visuals.no_smokes && (entity != globals::localPlayer->GetActiveWeapon()))
		{
			C_BaseEntity* parent = entity->GetOwnerEntity().Get();

			IMaterial* dust_motes = interfaces::materialSystem->FindMaterial("particle\\smoke1\\dust_motes", TEXTURE_GROUP_PARTICLE);
			dust_motes->SetMaterialVarFlag(IMaterial::NO_DRAW, true);

			IMaterial* dust_motes_alpha = interfaces::materialSystem->FindMaterial("particle\\smoke1\\dust_motes_alpha", TEXTURE_GROUP_PARTICLE);
			dust_motes_alpha->SetMaterialVarFlag(IMaterial::NO_DRAW, true);

			IMaterial* smoke1 = interfaces::materialSystem->FindMaterial("particle\\smoke1\\smoke1", TEXTURE_GROUP_PARTICLE);
			smoke1->SetMaterialVarFlag(IMaterial::NO_DRAW, true);

			IMaterial* smoke1_additive = interfaces::materialSystem->FindMaterial("particle\\smoke1\\smoke1_additive", TEXTURE_GROUP_PARTICLE);
			smoke1_additive->SetMaterialVarFlag(IMaterial::NO_DRAW, true);

			IMaterial* smoke1_nearcull = interfaces::materialSystem->FindMaterial("particle\\smoke1\\smoke1_nearcull", TEXTURE_GROUP_PARTICLE);
			smoke1_nearcull->SetMaterialVarFlag(IMaterial::NO_DRAW, true);

			IMaterial* smoke1_nearcull2 = interfaces::materialSystem->FindMaterial("particle\\smoke1\\smoke1_nearcull2", TEXTURE_GROUP_PARTICLE);
			smoke1_nearcull2->SetMaterialVarFlag(IMaterial::NO_DRAW, true);

			IMaterial* particle_smoke_dust = interfaces::materialSystem->FindMaterial("particle\\particle_smoke_dust", TEXTURE_GROUP_PARTICLE);
			particle_smoke_dust->SetMaterialVarFlag(IMaterial::NO_DRAW, true);

			IMaterial* particle_smokegrenade = interfaces::materialSystem->FindMaterial("particle\\particle_smokegrenade", TEXTURE_GROUP_PARTICLE);
			particle_smokegrenade->SetMaterialVarFlag(IMaterial::NO_DRAW, true);

			IMaterial* particle_smokegrenade1 = interfaces::materialSystem->FindMaterial("particle\\particle_smokegrenade1", TEXTURE_GROUP_PARTICLE);
			particle_smokegrenade1->SetMaterialVarFlag(IMaterial::NO_DRAW, true);

			interfaces::studioRender->ForcedMaterialOverride(dust_motes);
			interfaces::studioRender->ForcedMaterialOverride(dust_motes_alpha);
			interfaces::studioRender->ForcedMaterialOverride(smoke1);
			interfaces::studioRender->ForcedMaterialOverride(smoke1_additive);
			interfaces::studioRender->ForcedMaterialOverride(smoke1_nearcull);
			interfaces::studioRender->ForcedMaterialOverride(smoke1_nearcull2);
			interfaces::studioRender->ForcedMaterialOverride(particle_smoke_dust);
			interfaces::studioRender->ForcedMaterialOverride(particle_smokegrenade);
			interfaces::studioRender->ForcedMaterialOverride(particle_smokegrenade1);

			DrawModelOriginal(interfaces::studioRender, results, info, bones, flexWeights, flexDelayedWeights, modelOrigin, flags);

			return interfaces::studioRender->ForcedMaterialOverride(nullptr);
		}
	}
	DrawModelOriginal(interfaces::studioRender, results, info, bones, flexWeights, flexDelayedWeights, modelOrigin, flags);
}

void __stdcall hooks::BeginFrame(float frameTime)
{
	BeginFrameOriginal(interfaces::materialSystem, frameTime);

	if (interfaces::engine->IsInGame() && interfaces::engine->IsConnected())
	{
		if (globals::localPlayer)
		{
			BulletImpactEvent::Get().Paint();
		}
	}
}

float __stdcall hooks::GetViewModelFov() noexcept
{
	if (interfaces::engine->IsInGame() && interfaces::engine->IsConnected())
	{
		if (globals::localPlayer && globals::localPlayer->IsAlive())
		{
			if (g.misc.additional_fov != 0.0f)
			{
				return GetViewModelOriginal(interfaces::clientMode) + g.misc.additional_fov;
			}
		}
	}

	return GetViewModelOriginal(interfaces::clientMode);
}

void __stdcall hooks::FrameStageNotify(ClientFrameStage_t curState) noexcept
{
	if (!globals::localPlayer) return FrameStageNotifyOriginal(interfaces::client, curState);

	if(interfaces::engine->IsTakingScreenshot())
		return FrameStageNotifyOriginal(interfaces::client, curState);

	ThirdPerson::FrameStageNotify();
}

void hooks::Setup()
{
	if (MH_Initialize())
		throw std::runtime_error("Unable to initialize minhook");

	StartDrawing = reinterpret_cast<StartDrawingFn>(memory::PatternScan("vguimatsurface.dll", "55 8B EC 64 A1 ? ? ? ? 6A FF 68 ? ? ? ? 50 64 89 25 ? ? ? ? 83 EC 14"));
	if (!StartDrawing)
		throw std::runtime_error("Unable to find StartDrawing()");
	FinishDrawing = reinterpret_cast<FinishDrawingFn>(memory::PatternScan("vguimatsurface.dll", "55 8B EC 6A FF 68 ? ? ? ? 64 A1 ? ? ? ? 50 64 89 25 ? ? ? ? 51 56 6A 00"));
	if (!FinishDrawing)
		throw std::runtime_error("Unable to find FinishDrawing()");

	if (MH_CreateHook(
		memory::Get(interfaces::keyValuesSystem, 1),
		&AllocKeyValuesMemory,
		reinterpret_cast<void**>(&AllocKeyValuesMemoryOriginal)
	)) throw std::runtime_error("Unable to hook AllocKeyValuesMemory()");
	
	if (MH_CreateHook(
		memory::Get(gui::device, 42),
		&EndScene,
		reinterpret_cast<void**>(&EndSceneOriginal)
	)) throw std::runtime_error("Unable to hook EndScene()");

	if (MH_CreateHook(
		memory::Get(gui::device, 16),
		&Reset,
		reinterpret_cast<void**>(&ResetOriginal)
	)) throw std::runtime_error("Unable to hook Reset()");

	if (MH_CreateHook(
		memory::Get(interfaces::cinput, 3),
		&CreateMove,
		reinterpret_cast<void**>(&CreateMoveOriginal)
	)) throw std::runtime_error("Unable to hook CreateMove()");

	if (MH_CreateHook(
		memory::Get(interfaces::panel, 41),
		&PaintTraverse,
		reinterpret_cast<void**>(&PaintTraverseOriginal)
	)) throw std::runtime_error("Unable to hook PaintTraverse()");

	if (MH_CreateHook(
		memory::Get(interfaces::clientMode, 32),
		&GetViewModelFov,
		reinterpret_cast<void**>(&GetViewModelOriginal)
	)) throw std::runtime_error("Unable to hook GetViewModelFov()");

	if (MH_CreateHook(
		memory::Get(interfaces::studioRender, 29),
		&DrawModel,
		reinterpret_cast<void**>(&DrawModelOriginal)
	)) throw std::runtime_error("Unable to hook DrawModel()");

	if (MH_CreateHook(
		memory::Get(interfaces::surface, 62),
		&LockCursor,
		reinterpret_cast<void**>(&LockCursorOriginal)
	)) throw std::runtime_error("Unable to hook LockCursor()");

	if (MH_CreateHook(
		memory::Get(interfaces::materialSystem, 37),
		&BeginFrame,
		reinterpret_cast<void**>(&BeginFrameOriginal)
	)) throw std::runtime_error("Unable to hook BeginFrame()");

	if (MH_EnableHook(MH_ALL_HOOKS))
		throw std::runtime_error("Unable to enable hooks");

	gui::DestroyDirectX();

	BulletImpactEvent::Get().RegisterSelf();
	Spectators::Get().RegisterSelf();
}

void hooks::Destroy()
{
	MH_DisableHook(MH_ALL_HOOKS);
	MH_RemoveHook(MH_ALL_HOOKS);
	MH_Uninitialize();

	BulletImpactEvent::Get().UnregisterSelf();
	Spectators::Get().UnregisterSelf();

	AllocKeyValuesMemoryOriginal = nullptr;
	LockCursorOriginal = nullptr;
	PaintTraverseOriginal = nullptr;
	CreateMoveOriginal = nullptr;
	EndSceneOriginal = nullptr;
	ResetOriginal = nullptr;
	DrawModelOriginal = nullptr;
	StartDrawing = nullptr;
	FinishDrawing = nullptr;
	BeginFrameOriginal = nullptr;
	GetViewModelOriginal = nullptr;
	g_entityList = nullptr;

	globals::localPlayer = nullptr;
	globals::localPlayerIndex = 0;
	globals::windowHeight = globals::windowWidth = 0;
}
