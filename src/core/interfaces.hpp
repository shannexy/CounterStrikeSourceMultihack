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

namespace interfaces {
	void Setup() noexcept;

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
	inline IGameEventManager2* gameeventmanager = nullptr;
}