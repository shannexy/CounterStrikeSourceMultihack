#pragma once

#include "../utils/memory.hpp"

#include "cvector.hpp"
#include "cmatrix.hpp"
#include "ispatialquery.hpp"
#include "checksum/checksum_crc.hpp"

class C_BaseEntity;

struct player_info_t {
	char			name[32];
	int				userID;
	char			guid[32 + 1];
	std::uint32_t   friendsID;
	char			friendsName[32];
	bool			fakeplayer;
	bool			ishltv;
	CRC32_t			customFiles[4];
	unsigned char	filesDownloaded;
};

class IVEngineClient
{
public:
	constexpr void GetScreenSize(std::int32_t& width, std::int32_t& height) noexcept
	{
		memory::Call<void>(this, 5, std::ref(width), std::ref(height));
	}

	// returns index of local player
	constexpr std::int32_t GetLocalPlayerIndex() noexcept
	{
		return memory::Call<std::int32_t>(this, 12);
	}

	constexpr void GetViewAngles(const CVector& viewAngles) noexcept
	{
		memory::Call<void, const CVector&>(this, 19, viewAngles);
	}

	constexpr void SetViewAngles(const CVector& viewAngles) noexcept
	{
		memory::Call<void, const CVector&>(this, 20, viewAngles);
	}

	// returns true if in-game
	constexpr bool IsInGame() noexcept
	{
		return memory::Call<bool>(this, 26);
	}

	constexpr bool IsTakingScreenshot() noexcept {
		return memory::Call<bool>(this, 85);
	}

	constexpr int GetPlayerForUserID(int userid) noexcept {
		return memory::Call<int>(this, 9, userid);
	}

	constexpr bool IsConnected() noexcept
	{
		return memory::Call<bool>(this, 27);
	}

	constexpr bool GetPlayerInfo(int ent_num, player_info_t* pinfo) {
		return memory::Call<bool>(this, 8, ent_num, pinfo);
	}

	constexpr const CMatrix4x4& WorldToScreenMatrix() noexcept
	{
		return memory::Call<const CMatrix4x4&>(this, 36);
	}

	constexpr void ClientCmd(const char* cmd) noexcept {
		memory::Call<void>(this, 7, cmd);
	}

	constexpr ISpacialQuery* GetBSPTreeQuery() noexcept
	{
		return memory::Call<ISpacialQuery*>(this, 42);
	}
};
