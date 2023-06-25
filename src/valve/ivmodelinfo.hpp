#pragma once

#include "../utils/memory.hpp"
#include "../sdk.hpp"
#include "cvector.hpp"

#define BONE_USED_MASK				0x0007FF00
#define BONE_USED_BY_ANYTHING		0x0007FF00
#define BONE_USED_BY_HITBOX			0x00000100	// bone (or child) is used by a hit box
#define BONE_USED_BY_ATTACHMENT		0x00000200	// bone (or child) is used by an attachment point
#define BONE_USED_BY_VERTEX_MASK	0x0003FC00
#define BONE_USED_BY_VERTEX_LOD0	0x00000400	// bone (or child) is used by the toplevel model via skinned vertex
#define BONE_USED_BY_VERTEX_LOD1	0x00000800	
#define BONE_USED_BY_VERTEX_LOD2	0x00001000  
#define BONE_USED_BY_VERTEX_LOD3	0x00002000
#define BONE_USED_BY_VERTEX_LOD4	0x00004000
#define BONE_USED_BY_VERTEX_LOD5	0x00008000
#define BONE_USED_BY_VERTEX_LOD6	0x00010000
#define BONE_USED_BY_VERTEX_LOD7	0x00020000
#define BONE_USED_BY_BONE_MERGE		0x00040000	// bone is available for bone merge to occur against it

#define BONE_USED_BY_VERTEX_AT_LOD(lod) ( BONE_USED_BY_VERTEX_LOD0 << (lod) )
#define BONE_USED_BY_ANYTHING_AT_LOD(lod) ( ( BONE_USED_BY_ANYTHING & ~BONE_USED_BY_VERTEX_MASK ) | BONE_USED_BY_VERTEX_AT_LOD(lod) )


struct model_t;
struct mstudiobbox_t
{
	int					bone;
	int					group;
	CVector				bbmin;
	CVector				bbmax;
	int					szhitboxnameindex;
	int					pad00[3];
	float				pillradius;
	int					pad01[4];
};

struct mstudiohitboxset_t
{
	int					sznameindex;
	inline char* const	pszName(void) const { return ((char*)this) + sznameindex; }
	int					numhitboxes;
	int					hitboxindex;
	inline mstudiobbox_t* pHitbox(int i) const { return (mstudiobbox_t*)(((unsigned char*)this) + hitboxindex) + i; };
};

struct mstudiobone_t
{
	int                                     sznameindex;
	inline char* const GetName(void) const { return ((char*)this) + sznameindex; }
	int                                     parent;
	int                                     bonecontroller[6];

	CVector                          pos;
	float                           quat[4];
	CVector                          rot;
	CVector                          posscale;
	CVector                          rotscale;

	CMatrix3x4						poseToBone;
	float                           qAlignment[4];
	int                                     flags;
	int                                     proctype;
	int                                     procindex;              // procedural rule
	mutable int                     physicsbone;    // index into physically simulated bone
	inline void* GetProcedure() const { if (procindex == 0) return NULL; else return (void*)(((unsigned char*)this) + procindex); };
	int                                     surfacepropidx; // index into string tablefor property name
	inline char* const GetSurfaceProps(void) const { return ((char*)this) + surfacepropidx; }
	int                                     contents;               // See BSPFlags.h for the contents flags

	int                                     unused[8];              // remove as appropriate
};

struct mstudiobonecontroller_t {
	int					bone;	// -1 == 0
	int					type;	// X, Y, Z, XR, YR, ZR, M
	float				start;
	float				end;
	int					rest;	// byte index value at rest
	int					inputfield;	// 0-3 user set controller, 4 mouth
	int					unused[8];
};

struct studiohdr_t
{
	int					id;
	int					version;
	long				checksum;		// this has to be the same in the phy and vtx files to load!
	char				name[64];
	int					length;


	CVector				eyeposition;	// ideal eye position

	CVector				illumposition;	// illumination center

	CVector				hull_min;		// ideal movement hull size
	CVector				hull_max;

	CVector				view_bbmin;		// clipping bounding box
	CVector				view_bbmax;

	int					flags;

	int					numbones;			// bones
	int					boneindex;
	inline mstudiobone_t* pBone(int i) const
	{
		assert(i >= 0 && i < numbones);
		return (mstudiobone_t*)(((unsigned char*)this) + boneindex) + i;
	};

	int					numbonecontrollers;		// bone controllers
	int					bonecontrollerindex;

	int					numhitboxsets;
	int					hitboxsetindex;

	// Look up hitbox set by index
	mstudiohitboxset_t* pHitboxSet(int i) const
	{
		return (mstudiohitboxset_t*)(((unsigned char*)this) + hitboxsetindex) + i;
	};

	// Calls through to hitbox to determine size of specified set
	inline mstudiobbox_t* pHitbox(int i, int set) const
	{
		mstudiohitboxset_t const* s = pHitboxSet(set);
		if (!s)
			return NULL;

		return s->pHitbox(i);
	};

	// Calls through to set to get hitbox count for set
	inline int			iHitboxCount(int set) const
	{
		mstudiohitboxset_t const* s = pHitboxSet(set);
		if (!s)
			return 0;

		return s->numhitboxes;
	};
};

class IVModelInfo
{
public:
	enum ERenderFlags : std::uint32_t
	{
		RENDER_FLAGS_DISABLE_RENDERING = 0x01,
		RENDER_FLAGS_HASCHANGED = 0x02,
		RENDER_FLAGS_ALTERNATE_SORTING = 0x04,
		RENDER_FLAGS_RENDER_WITH_VIEWMODELS = 0x08,
		RENDER_FLAGS_BLOAT_BOUNDS = 0x10,
		RENDER_FLAGS_BOUNDS_VALID = 0x20,
		RENDER_FLAGS_BOUNDS_ALWAYS_RECOMPUTE = 0x40,
		RENDER_FLAGS_IS_SPRITE = 0x80,
		RENDER_FLAGS_FORCE_OPAQUE_PASS = 0x100,
	};

	const char* GetModelName(const model_t* model) noexcept
	{
		return memory::Call<const char*>(this, 3, model);
	}

	studiohdr_t* GetStudioModel(const model_t* model) noexcept
	{
		return memory::Call<studiohdr_t*>(this, 28, model);
	}
};