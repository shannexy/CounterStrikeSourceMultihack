#pragma once
#include <d3d9.h>

#include "imgui/imgui.h"
#include "imgui/imgui_impl_dx9.h"
#include "imgui/imgui_impl_win32.h"

namespace gui {
	inline bool open = false;

	inline bool setup = false;

	inline HWND window = nullptr;
	inline WNDCLASSEX windowClass = { };
	inline WNDPROC originalWindowProcess = nullptr;

	inline LPDIRECT3DDEVICE9 device = nullptr;
	inline LPDIRECT3D9 d3d9 = nullptr;

	bool SetupWindowClass(const char* windowClassName) noexcept;
	void DestroyWindowClass() noexcept;

	bool SetupWindow(const char* windowName) noexcept;
	void DestroyWindow() noexcept;

	bool SetupDirectX() noexcept;
	void DestroyDirectX() noexcept;

	void Setup();

	void SetupMenu(LPDIRECT3DDEVICE9 device) noexcept;
	void Destroy() noexcept;

	void Render() noexcept;
	void DoTabs() noexcept;

	enum MenuTabs {
		ESP,
		AIMBOT,
		CHAMS,
		BHOP,
		MISC,
		VISUALS
	};
	inline MenuTabs currentTab = ESP;

	#define MakeMenuTab(tab, label, size) \
	{ \
		if(ImGui::Button(label, size)) { \
			currentTab = tab; \
		} \
	}
}