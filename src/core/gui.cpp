#include "gui.hpp"
#include "interfaces.hpp"
#include "../gui/components.hpp"
#include "globals.hpp"
#include "../valve/centity.hpp"

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

LRESULT CALLBACK WindowProcess(
	HWND window,
	UINT message,
	WPARAM wideParam,
	LPARAM longParam
);

namespace gui {
	bool SetupWindowClass(const char* windowClassName) noexcept {
		windowClass.cbSize = sizeof(WNDCLASSEX);
		windowClass.style = CS_HREDRAW | CS_VREDRAW;
		windowClass.lpfnWndProc = DefWindowProc;
		windowClass.cbClsExtra = 0;
		windowClass.cbWndExtra = 0;
		windowClass.hInstance = GetModuleHandle(NULL);
		windowClass.hIcon = NULL;
		windowClass.hCursor = NULL;
		windowClass.hbrBackground = NULL;
		windowClass.lpszMenuName = NULL;
		windowClass.lpszClassName = windowClassName;
		windowClass.hIconSm = NULL;

		if (!RegisterClassEx(&windowClass)) {
			return false;
		}
		return true;
	}
	void DestroyWindowClass() noexcept {
		UnregisterClass(
			windowClass.lpszClassName,
			windowClass.hInstance
		);
	}

	bool SetupWindow(const char* windowName) noexcept {
		window = CreateWindow(
			windowClass.lpszClassName,
			windowName,
			WS_OVERLAPPEDWINDOW,
			0,
			0,
			100,
			100,
			0,
			0,
			windowClass.hInstance,
			0
		);

		if (!window)
			return false;

		return true;
	}
	void DestroyWindow() noexcept {
		if (window)
			DestroyWindow(window);
	}

	bool SetupDirectX() noexcept {
		const auto handle = GetModuleHandle("d3d9.dll");

		if (!handle)
			return false;

		using CreateFn = LPDIRECT3D9(__stdcall*)(UINT);

		const auto create = reinterpret_cast<CreateFn>(GetProcAddress(
			handle,
			"Direct3DCreate9"
		));

		if (!create)
			return false;

		d3d9 = create(D3D_SDK_VERSION);

		if (!d3d9)
			return false;

		D3DPRESENT_PARAMETERS params = { };
		params.BackBufferWidth = 0;
		params.BackBufferHeight = 0;
		params.BackBufferFormat = D3DFMT_UNKNOWN;
		params.BackBufferCount = 0;
		params.MultiSampleType = D3DMULTISAMPLE_NONE;
		params.MultiSampleQuality = NULL;
		params.SwapEffect = D3DSWAPEFFECT_DISCARD;
		params.hDeviceWindow = window;
		params.Windowed = 1;
		params.EnableAutoDepthStencil = 0;
		params.AutoDepthStencilFormat = D3DFMT_UNKNOWN;
		params.Flags = NULL;
		params.FullScreen_RefreshRateInHz = 0;
		params.PresentationInterval = 0;

		if (d3d9->CreateDevice(
			D3DADAPTER_DEFAULT,
			D3DDEVTYPE_NULLREF,
			window,
			D3DCREATE_SOFTWARE_VERTEXPROCESSING | D3DCREATE_DISABLE_DRIVER_MANAGEMENT,
			&params,
			&device
		) < 0) return false;

		return true;
	}
	void DestroyDirectX() noexcept {
		if (device) {
			device->Release();
			device = NULL;
		}

		if (d3d9) {
			d3d9->Release();
			d3d9 = NULL;
		}
	}

	void Setup() {
		if (!SetupWindowClass("hackClass001"))
			throw std::runtime_error("Failed to create window class.");

		if (!SetupWindow("Hack Window"))
			throw std::runtime_error("Failed to create window.");

		if (!SetupDirectX())
			throw std::runtime_error("Failed to create device.");

		DestroyWindow();
		DestroyWindowClass();
	}

	void SetupMenu(LPDIRECT3DDEVICE9 device) noexcept {
		auto params = D3DDEVICE_CREATION_PARAMETERS{};
		device->GetCreationParameters(&params);
		window = params.hFocusWindow;

		originalWindowProcess = reinterpret_cast<WNDPROC>(
			SetWindowLongPtr(window, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(WindowProcess))
			);

		ImGui::CreateContext();
		ImGui::StyleColorsDark();

		ImGui_ImplWin32_Init(window);
		ImGui_ImplDX9_Init(device);

		setup = true;
	}
	void Destroy() noexcept {
		ImGui_ImplDX9_Shutdown();
		ImGui_ImplWin32_Shutdown();
		ImGui::DestroyContext();

		SetWindowLongPtr(
			window,
			GWLP_WNDPROC,
			reinterpret_cast<LONG_PTR>(originalWindowProcess)
		);

		DestroyDirectX();
	}

	void Render() noexcept {
		ImGui_ImplDX9_NewFrame();
		ImGui_ImplWin32_NewFrame();
		ImGui::NewFrame();

		ImGui::SetNextWindowSize(ImVec2(425, 350));
		ImGui::Begin("Menu", &open, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse);
		DoTabs();
		ImGui::Separator();
		ImGui::SetCursorPos(ImVec2(75, 26));
		if (ImGui::BeginChild("##Options", ImVec2(0, 0), true)) 
		{
			switch ((MenuTabs)g.gui.currentTab)
			{
				case BHOP:
				{
					components::combo_enum<BhopType>(g.bhop.BhopType, "Bhop", std::vector<BhopType> {
						BhopType::DISABLED,
						BhopType::NORMAL,
						BhopType::PERFECT,
					}, [=](BhopType type) 
					{
						if(type == BhopType::NORMAL)
						{
							return "Normal";
						}
						else if (type == BhopType::PERFECT)
						{
							return "Perfect";
						}
						return "Disabled";
					});

					if (g.bhop.BhopType != DISABLED)
					{
						ImGui::Checkbox("AutoStrafe", &g.bhop.AutoStrafe);
					}
				}
				break;
				case AIMBOT:
				{
					if (interfaces::engine->IsInGame() && interfaces::engine->IsConnected())
					{
						ImGui::Checkbox("Enable", &g.aimbot.enabled);
						ImGui::Checkbox("Silent", &g.aimbot.silentAim);
						ImGui::Checkbox("Smooth", &g.aimbot.smooth);
						if (g.aimbot.smooth) {
							ImGui::SliderFloat("Smooth Count", &g.aimbot.smoothCount, 0.1f, 1);
						}

						ImGui::SliderFloat("Aim Fov", &g.aimbot.fov, 5, 60);
						ImGui::Checkbox("Show Fov", &g.aimbot.showFov);
						if (g.aimbot.showFov) {
							ImGui::ColorPicker4("Fov Color", g.aimbot.fovColor, ImGuiColorEditFlags_DisplayHex | ImGuiColorEditFlags_DisplayRGB | ImGuiColorEditFlags_Float);
						}
						components::combo_list<int>(g.aimbot.aimBone, "Selected Bone", "None", std::vector {
							9, 10, 13, 14
						}, [=](int selected_bone) {
							if (globals::localPlayer && globals::localPlayer->IsAlive())
							{
								return globals::localPlayer->GetBoneName(selected_bone);
							}
							return "Join Game First";
						});
					}
				}
				break;
				case CHAMS:
				{
					ImGui::Checkbox("Enable", &g.chams.enabled);
					if (g.chams.enabled) {
						ImGui::ColorPicker3("Visible Color", g.chams.VisibleColor);
						ImGui::ColorPicker3("No Visible Color", g.chams.HiddenColor);
					}
				}
				break;
				case ESP:
				{
					ImGui::Checkbox("Enable", &g.esp.enabled);
					if (g.esp.enabled) {
						ImGui::Checkbox("Show Teammates", &g.esp.showTeammates);
						ImGui::ColorPicker4("Enemy Color", g.esp.drawBoxEnemyColor, ImGuiColorEditFlags_DisplayHex | ImGuiColorEditFlags_DisplayRGB | ImGuiColorEditFlags_Float);
						if (g.esp.showTeammates)
						{
							ImGui::ColorPicker4("Friendly Color", g.esp.drawBoxFriendlyColor, ImGuiColorEditFlags_DisplayHex | ImGuiColorEditFlags_DisplayRGB | ImGuiColorEditFlags_Float);
						}
						ImGui::Checkbox("Draw Bones", &g.esp.drawBones);
						if (g.esp.drawBones) {
							ImGui::Checkbox("Draw Bone Names", &g.esp.drawBoneNames);
							ImGui::ColorPicker4("Bone Color", g.esp.drawBoneColor, ImGuiColorEditFlags_DisplayHex | ImGuiColorEditFlags_DisplayRGB | ImGuiColorEditFlags_Float);
						}
						ImGui::Checkbox("Snap Lines", &g.esp.drawSnapLines);
						if (g.esp.drawSnapLines) {
							ImGui::ColorPicker4("Snap Color", g.esp.drawSnapLineColor, ImGuiColorEditFlags_DisplayHex | ImGuiColorEditFlags_DisplayRGB | ImGuiColorEditFlags_Float);
						}
						ImGui::Checkbox("Draw Health Bar", &g.esp.drawHealthBar);
					}
				}
				break;
				case VISUALS:
				{
					ImGui::Checkbox("Show Bullet Impact", &g.visuals.bullet_impact);
					if (g.visuals.bullet_impact) {
						ImGui::Checkbox("Show Others Players", &g.visuals.others_bullet_impact);
						ImGui::ColorPicker4("Bullet Impact Color", g.visuals.bullet_impact_color, ImGuiColorEditFlags_DisplayHex | ImGuiColorEditFlags_DisplayRGB | ImGuiColorEditFlags_Float);
						ImGui::SliderFloat("Bullet Impact Duration", &g.visuals.bullet_impact_duration, 0.8f, 3.0f);
					}
					ImGui::SliderFloat("Additional Fov", &g.misc.additional_fov, -100, 100);
					ImGui::Checkbox("No Flash", &g.visuals.no_flash);
					ImGui::Checkbox("No Smokes", &g.visuals.no_smokes);
				}
				break;
				case MISC:
				{
					ImGui::Checkbox("Show Spectators", &g.visuals.show_spectators);
					if (g.visuals.show_spectators)
					{
						ImGui::SliderInt("X", &g.visuals.spectators_location[0], -globals::windowWidth, globals::windowWidth);
						ImGui::SliderInt("Y", &g.visuals.spectators_location[1], -globals::windowHeight, globals::windowHeight);
					}
					if (ImGui::Checkbox("No Recoil", &g.misc.norecoil))
					{
						ConVar* cl_predictweapons = interfaces::convar->FindVar("cl_predictweapons");
						if (cl_predictweapons)
						{
							cl_predictweapons->SetValue(g.misc.norecoil ? 1 : 0);
						}
					}
					if (ImGui::Checkbox("Third Person", &g.misc.thirdperson)) {
						ConVar* sv_cheats = interfaces::convar->FindVar("sv_cheats");

						if (g.misc.thirdperson) {
							if (sv_cheats->GetInt() != 1)
								sv_cheats->SetValue(1);
							interfaces::cinput->ToThirdPerson();
						}
						else {
							if (sv_cheats->GetInt() == 1)
								sv_cheats->SetValue(0);

							interfaces::cinput->ToFirstPerson();
						}
					}
				}
				break;
			}
		}
		ImGui::EndChild();
		ImGui::End();

		ImGui::EndFrame();
		ImGui::Render();
		ImGui_ImplDX9_RenderDrawData(ImGui::GetDrawData());
	}
	void DoTabs() noexcept 
	{
		if (ImGui::BeginChild("##Tabs", ImVec2(60, 0), true)) {
			MakeMenuTab(MenuTabs::BHOP, "BHOP", ImVec2(44, 20));
			MakeMenuTab(MenuTabs::AIMBOT, "Aimbot", ImVec2(44, 20));
			MakeMenuTab(MenuTabs::CHAMS, "CHAMS", ImVec2(44, 20));
			MakeMenuTab(MenuTabs::ESP, "ESP", ImVec2(44, 20));
			MakeMenuTab(MenuTabs::VISUALS, "VISUALS", ImVec2(44, 20));
			MakeMenuTab(MenuTabs::MISC, "MISC", ImVec2(44, 20));
		}
		ImGui::EndChild();
	}
}

LRESULT CALLBACK WindowProcess(
	HWND window,
	UINT message,
	WPARAM wideParam,
	LPARAM longParam
) {
	if (GetAsyncKeyState(VK_INSERT) & 1) {
		gui::open = !gui::open;

		if (gui::open) {
			interfaces::surface->SetCursorAlwaysVisible(true);
		}
		else
		{
			interfaces::surface->SetCursorAlwaysVisible(false);
		}
	}

	if (gui::open && ImGui_ImplWin32_WndProcHandler(window, message, wideParam, longParam))
		return 1L;

	return CallWindowProc(
		gui::originalWindowProcess,
		window,
		message,
		wideParam,
		longParam
	);
}