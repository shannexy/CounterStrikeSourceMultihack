#pragma once

#include "../utils/memory.hpp"

#include "ckeyvalues.hpp"

#define TEXTURE_GROUP_LIGHTMAP						"Lightmaps"
#define TEXTURE_GROUP_WORLD							"World textures"
#define TEXTURE_GROUP_MODEL							"Model textures"
#define TEXTURE_GROUP_VGUI							"VGUI textures"
#define TEXTURE_GROUP_PARTICLE						"Particle textures"
#define TEXTURE_GROUP_DECAL							"Decal textures"
#define TEXTURE_GROUP_SKYBOX						"SkyBox textures"
#define TEXTURE_GROUP_CLIENT_EFFECTS				"ClientEffect textures"
#define TEXTURE_GROUP_OTHER							"Other textures"
#define TEXTURE_GROUP_PRECACHED						"Precached"				// TODO: assign texture groups to the precached materials
#define TEXTURE_GROUP_CUBE_MAP						"CubeMap textures"
#define TEXTURE_GROUP_RENDER_TARGET					"RenderTargets"
#define TEXTURE_GROUP_RUNTIME_COMPOSITE				"Runtime Composite"
#define TEXTURE_GROUP_UNACCOUNTED					"Unaccounted textures"	// Textures that weren't assigned a texture group.
//#define TEXTURE_GROUP_STATIC_VERTEX_BUFFER		"Static Vertex"
#define TEXTURE_GROUP_STATIC_INDEX_BUFFER			"Static Indices"
#define TEXTURE_GROUP_STATIC_VERTEX_BUFFER_DISP		"Displacement Verts"
#define TEXTURE_GROUP_STATIC_VERTEX_BUFFER_COLOR	"Lighting Verts"
#define TEXTURE_GROUP_STATIC_VERTEX_BUFFER_WORLD	"World Verts"
#define TEXTURE_GROUP_STATIC_VERTEX_BUFFER_MODELS	"Model Verts"
#define TEXTURE_GROUP_STATIC_VERTEX_BUFFER_OTHER	"Other Verts"
#define TEXTURE_GROUP_DYNAMIC_INDEX_BUFFER			"Dynamic Indices"
#define TEXTURE_GROUP_DYNAMIC_VERTEX_BUFFER			"Dynamic Verts"
#define TEXTURE_GROUP_DEPTH_BUFFER					"DepthBuffer"
#define TEXTURE_GROUP_VIEW_MODEL					"ViewModel"
#define TEXTURE_GROUP_PIXEL_SHADERS					"Pixel Shaders"
#define TEXTURE_GROUP_VERTEX_SHADERS				"Vertex Shaders"
#define TEXTURE_GROUP_RENDER_TARGET_SURFACE			"RenderTarget Surfaces"
#define TEXTURE_GROUP_MORPH_TARGETS					"Morph Targets"

class IMaterial
{
public:
	enum EMaterialVarFlag : std::int32_t
	{
		DEBUG = (1 << 0),
		NO_DEBUG_OVERRIDE = (1 << 1),
		NO_DRAW = (1 << 2),
		USE_IN_FILLRATE_MODE = (1 << 3),

		VERTEXCOLOR = (1 << 4),
		VERTEXALPHA = (1 << 5),
		SELFILLUM = (1 << 6),
		ADDITIVE = (1 << 7),
		ALPHATEST = (1 << 8),
		MULTIPASS = (1 << 9),
		ZNEARER = (1 << 10),
		MODEL = (1 << 11),
		FLAT = (1 << 12),
		NOCULL = (1 << 13),
		NOFOG = (1 << 14),
		IGNOREZ = (1 << 15),
		DECAL = (1 << 16),
		ENVMAPSPHERE = (1 << 17),
		NOALPHAMOD = (1 << 18),
		ENVMAPCAMERASPACE = (1 << 19),
		BASEALPHAENVMAPMASK = (1 << 20),
		TRANSLUCENT = (1 << 21),
		NORMALMAPALPHAENVMAPMASK = (1 << 22),
		NEEDS_SOFTWARE_SKINNING = (1 << 23),
		OPAQUETEXTURE = (1 << 24),
		ENVMAPMODE = (1 << 25),
		SUPPRESS_DECALS = (1 << 26),
		HALFLAMBERT = (1 << 27),
		WIREFRAME = (1 << 28),
		ALLOWALPHATOCOVERAGE = (1 << 29),
		IGNORE_ALPHA_MODULATION = (1 << 30),
	};

	constexpr void SetMaterialVarFlag(const std::int32_t flag, const bool on) noexcept
	{
		memory::Call<void>(this, 29, flag, on);
	}
};

class IMaterialSystem
{
public:
	constexpr IMaterial* CreateMaterial(const char* name, CKeyValues* kv) noexcept
	{
		return memory::Call<IMaterial*>(this, 70, name, kv);
	}

	constexpr IMaterial* FindMaterial(const char* name, const char* pTextureGroupName) noexcept {
		return memory::Call<IMaterial*>(this, 71, name, pTextureGroupName, true, nullptr);
	}

	constexpr IMaterial* FindMaterial(const char* name) noexcept
	{
		return memory::Call<IMaterial*>(this, 71, name, nullptr, true, nullptr);
	}
};