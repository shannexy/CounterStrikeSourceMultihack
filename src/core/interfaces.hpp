#pragma once

#include "../valve/ibaseclientdll.hpp"
#include "../valve/icliententitylist.hpp"
#include "../valve/ivengineclient.hpp"
#include "../valve/cinput.hpp"
#include "../valve/isurface.hpp"
#include "../valve/ivpanel.hpp"
#include "../valve/ivmodelinfo.hpp"
#include "../valve/igameventmanager.hpp"
#include "../valve/iclientmodeshared.hpp"
#include "../valve/icvar.hpp"
#include "../valve/iviewrender_beams.hpp"
#include "../valve/ivdebugoverlay.hpp"
#include "../valve/istudiorender.hpp"
#include "../valve/imaterialsystem.hpp"
#include "../valve/cbaseclientstate.hpp"
#include "../valve/iglobalvars.hpp"
#include "../valve/ienginevgui.hpp"
#include "../valve/CThirdPersonManager.hpp"

namespace interfaces {
	void Setup() noexcept;
	void Destroy() noexcept;
	template<typename Interface>
	Interface* Capture(const char* moduleName, const char* interfaceName) noexcept;

	inline IBaseClientDLL* client = nullptr;
	inline IVEngineClient* engine = nullptr;
	inline CInput* cinput = nullptr;
	inline ISurface* surface = nullptr;
	inline IClientEntityList* entityList = nullptr;
	inline IClientModeShared* clientMode = nullptr;
	inline IVModelInfo* modelInfo = nullptr;
	inline IVPanel* panel = nullptr;
	inline IStudioRender* studioRender = nullptr;
	inline ICVar* convar = nullptr;
	inline IMaterialSystem* materialSystem = nullptr;
	inline CViewRenderBeams* renderBeams = nullptr;
	inline IVDebugOverlay* debugOverlay = nullptr;
	inline CBaseClientState* clientState = nullptr;
	inline IGameEventManager2* gameeventmanager = nullptr;
	inline IEngineVGui* engineVGui = nullptr;
	inline IEngineTrace* engineTrace = nullptr;
	inline IGlobalVars* globals = nullptr;
	inline CThirdPersonManager* thirdpersonmanager = nullptr;

	inline void* keyValuesSystem = nullptr;
}