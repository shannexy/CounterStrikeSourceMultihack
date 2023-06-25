#include "hooks.hpp"

#include "console.hpp"
#include "../core/gui.hpp"
#include "../core/globals.hpp"
#include "../core/interfaces.hpp"
#include "../valve/centity.hpp"
#include "../features/bunnyhop.hpp"

void __stdcall hooks::CreateMove(int sequence_number, float input_sample_frametime, bool active)
{
	CreateMoveOriginal(interfaces::client, sequence_number, input_sample_frametime, active);

	auto cmd = interfaces::cinput->GetUserCmd(sequence_number);
	auto verified = interfaces::cinput->GetVerifiedCmd(sequence_number);

	if (!cmd || !verified)
	{
		return;
	}

	globals::UpdateLocalPlayer();
	if (globals::localPlayer && globals::localPlayer->IsAlive())
	{
		hacks::RunBunnyHop(cmd);
	}

	verified->m_cmd = *cmd;
	verified->m_crc = cmd->GetChecksum();
}

void __stdcall hooks::LockCursor() noexcept
{
	if (!gui::open)
		return LockCursorOriginal(interfaces::surface);

	return interfaces::surface->UnlockCursor();
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

void hooks::Setup()
{
	if (MH_Initialize())
		throw std::runtime_error("Unable to initialize minhook");

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
		memory::Get(interfaces::client, 21),
		&CreateMove,
		reinterpret_cast<void**>(&CreateMoveOriginal)
	)) throw std::runtime_error("Unable to hook CreateMove()");

	if (MH_EnableHook(MH_ALL_HOOKS))
		throw std::runtime_error("Unable to enable hooks");

	gui::DestroyDirectX();
}

void hooks::Destroy()
{
	MH_DisableHook(MH_ALL_HOOKS);
	MH_RemoveHook(MH_ALL_HOOKS);
	MH_Uninitialize();

}
