#include "interfaces.hpp"

#include <Windows.h>

void interfaces::Setup() noexcept 
{
	client = Capture<IBaseClientDLL>("client.dll", "VClient017");
	//cinput = **reinterpret_cast<CInput***>((*reinterpret_cast<uintptr_t**>(client))[15] + 2);
	cinput = **reinterpret_cast<CInput***>(memory::PatternScan("client.dll", "8B 0D ? ? ? ? 8B 01 FF 60 44") + 2);
	entityList = Capture<IClientEntityList>("client.dll", "VClientEntityList003");
	clientMode = **reinterpret_cast<IClientModeShared***>((*reinterpret_cast<unsigned int**>(client))[10] + 5);
	//engineTrace = Capture<IEngineTrace>("engine.dll", "EngineTraceClient003");
	//engineVGui = Capture<IEngineVGui>("engine.dll", "VEngineVGui001");
	//globals = **reinterpret_cast<IGlobalVars***>((*reinterpret_cast<DWORD**>(client))[11] + 0x5);
	//materialSystem = Capture<IMaterialSystem>("materialsystem.dll", "VMaterialSystem080");
	//studioRender = Capture<IStudioRender>("studiorender.dll", "VStudioRender025");
	surface = Capture<ISurface>("vguimatsurface.dll", "VGUI_Surface030");
	//debugOverlay = Capture<IVDebugOverlay>("engine.dll", "VDebugOverlay003");
	engine = Capture<IVEngineClient>("engine.dll", "VEngineClient014");
	//clientState = *reinterpret_cast<CBaseClientState**>(memory::PatternScan("engine.dll", "B9 ? ? ? ? E8 ? ? ? ? 83 3D ? ? ? ? ? 75 4A") + 1);
	modelInfo = Capture<IVModelInfo>("engine.dll", "VModelInfoClient006");
	panel = Capture<IVPanel>("vgui2.dll", "VGUI_Panel009");
	gameeventmanager = Capture<IGameEventManager2>("engine.dll", "GAMEEVENTSMANAGER002");
	//renderBeams = **reinterpret_cast<CViewRenderBeams***>(memory::PatternScan("client.dll", "8B 0D ? ? ? ? 8B 01 FF 10 8B 0D ? ? ? ? 8B 01 FF 50 04 6A 01 68 ? ? ? ? 68 ? ? ? ? 8D 8E ? ? ? ? E8 ? ? ? ? 6A 01 68 ? ? ? ?") + 2);
	//convar = Capture<ICVar>("vstdlib.dll", "VEngineCvar004");
}

template<typename Interface>
Interface* interfaces::Capture(const char* moduleName, const char* interfaceName) noexcept
{
	const HINSTANCE handle = GetModuleHandle(moduleName);

	if (!handle)
		return nullptr;

	using CreateInterfaceFn = Interface * (__cdecl*)(const char*, int*);
	const CreateInterfaceFn createInterface = reinterpret_cast<CreateInterfaceFn>(GetProcAddress(handle, "CreateInterface"));

	return createInterface(interfaceName, nullptr);
}