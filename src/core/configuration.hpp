#pragma once

#include "../common.hpp"

#include "file_manager/file.hpp"
#include "file_manager/folder.hpp"

class configuration
{
public:
	void destroy();
	void init(const file& save_file);

	void attempt_save();
	bool load();

private:
	bool deep_compare(nlohmann::json& current_settings, const nlohmann::json& default_settings, bool compare_values = false);
	bool save();
	bool write_default_config();

private:
	bool m_running;

	std::unique_ptr<file> m_save_file;

	nlohmann::json m_default_options;
	nlohmann::json m_options;
public:
	struct bhop {
		bool enabled = false;

		NLOHMANN_DEFINE_TYPE_INTRUSIVE(bhop, enabled)
	} bhop{};

	struct chams {
		bool enabled;

		float HiddenColor[3] = { 0.f, 1.f, 1.f };
		float VisibleColor[3] = { 1.f, 1.f, 0.f };

		NLOHMANN_DEFINE_TYPE_INTRUSIVE(chams, enabled, HiddenColor, VisibleColor)
	} chams{};

	struct aimbot {
		bool enabled = false;
		int aimBone = -1;
		bool showFov = false;
		float fovColor[4] = { 1, 1, 1, 255 };
		float fov = 5;
		bool smooth = false;
		float smoothCount = 0.5;
		bool silentAim = false;
		bool autoFire = false;

		NLOHMANN_DEFINE_TYPE_INTRUSIVE(aimbot, enabled, aimBone, showFov, fovColor, fov, smooth, silentAim, autoFire)
	} aimbot{};

	struct misc {
		bool norecoil = false;
		float additional_fov = 0.0f;

		NLOHMANN_DEFINE_TYPE_INTRUSIVE(misc, norecoil, additional_fov)
	} misc{};

	struct esp {
		bool enabled = false;
		bool showTeammates = false;
		bool drawBones = false;
		bool drawSnapLines = false;
		bool drawBoneNames = false;
		float drawBoxEnemyColor[4] = { 1, 1, 1, 255 };
		float drawBoxFriendlyColor[4] = { 1, 1, 1, 255 };
		float drawBoneColor[4] = { 1, 1, 1, 255 };
		float drawSnapLineColor[4] = { 0, 0, 0, 255 };

		bool drawHealthBar;

		NLOHMANN_DEFINE_TYPE_INTRUSIVE(esp, enabled, showTeammates, drawBones, drawSnapLines, drawBoneNames, drawBoxEnemyColor, drawBoxFriendlyColor, drawBoneColor, drawSnapLineColor, drawHealthBar)
	} esp{};

	struct visuals {
		bool bullet_impact = false;
		bool others_bullet_impact = false;
		float bullet_impact_color[4] = { 1, 0, 0, 255 };
		float bullet_impact_duration = 0.8f;
		bool show_spectators = false;
		int spectators_location[2] = { 100, 100 };
		bool no_flash = false;
		bool no_smokes = false;

		NLOHMANN_DEFINE_TYPE_INTRUSIVE(visuals, bullet_impact, others_bullet_impact, bullet_impact_color, bullet_impact_duration, show_spectators, spectators_location, no_flash, no_smokes)
	} visuals{};

	NLOHMANN_DEFINE_TYPE_INTRUSIVE(configuration, bhop, chams, aimbot, esp, misc, visuals)
};

inline auto g = configuration();

