#pragma once
#include "ienginetrace.hpp"
#include "structs.hpp"
#include "cvector.hpp"

#define NOISE_DIVISIONS		128
#define MAX_BEAM_ENTS		10

class C_Beam;
typedef int pixelvis_handle_t;

struct BeamTrail_t
{
	// NOTE:  Don't add user defined fields except after these four fields.
	BeamTrail_t* next;
	float			die;
	CVector			org;
	CVector			vel;
};

class Beam_t
{
public:
	Beam_t() {}

	// Methods of IClientRenderable
	virtual const CVector& GetRenderOrigin(void) = 0;
	virtual const CVector& GetRenderAngles(void) = 0;
	virtual const CMatrix3x4& RenderableToWorldTransform() = 0;
	virtual void			GetRenderBounds(CVector& mins, CVector& maxs) = 0;
	virtual bool			ShouldDraw(void) = 0;
	virtual bool			IsTransparent(void) = 0;
	virtual int				DrawModel(int flags) = 0;
	virtual void			ComputeFxBlend() = 0;
	virtual int				GetFxBlend() = 0;

	// Resets the beam state
	virtual void			Reset() = 0;

	// Method to computing the bounding box
	virtual void			ComputeBounds() = 0;

	// Bounding box...
	CVector			m_Mins;
	CVector			m_Maxs;
	pixelvis_handle_t* m_queryHandleHalo;
	float			m_haloProxySize;

	// Data is below..

	// Next beam in list
	Beam_t* next;

	// Type of beam
	int				type;
	int				flags;

	// Control points for the beam
	int				numAttachments;
	CVector			attachment[MAX_BEAM_ENTS];
	CVector			delta;

	// 0 .. 1 over lifetime of beam
	float			t;
	float			freq;

	// Time when beam should die
	float			die;
	float			width;
	float			endWidth;
	float			fadeLength;
	float			amplitude;
	float			life;

	// Color
	float			r, g, b;
	float			brightness;

	// Speed
	float			speed;

	// Animation
	float			frameRate;
	float			frame;
	int				segments;

	// Attachment entities for the beam
	void* entity[MAX_BEAM_ENTS];
	int				attachmentIndex[MAX_BEAM_ENTS];

	// Model info
	int				modelIndex;
	int				haloIndex;

	float			haloScale;
	int				frameCount;

	float			rgNoise[NOISE_DIVISIONS + 1];

	// Popcorn trail for beam follows to use
	BeamTrail_t* trail;

	// for TE_BEAMRINGPOINT
	float			start_radius;
	float			end_radius;

	// for FBEAM_ONLYNOISEONCE
	bool			m_bCalculatedNoise;

	float			m_flHDRColorScale;

};

class IViewRenderBeams
{
public:
	virtual void	InitBeams(void) = 0;
	virtual void	ShutdownBeams(void) = 0;
	virtual void	ClearBeams(void) = 0;

	// Updates the state of the temp ent beams
	virtual void	UpdateTempEntBeams() = 0;

	virtual void	DrawBeam(C_Beam* pbeam, ITraceFilter* pEntityBeamTraceFilter = NULL) = 0;
	virtual void	DrawBeam(Beam_t* pbeam) = 0;

	virtual void	KillDeadBeams(C_BaseEntity* pEnt) = 0;

	// New interfaces!
	virtual Beam_t* CreateBeamEnts(BeamInfo_t& beamInfo) = 0;
	virtual Beam_t* CreateBeamEntPoint(BeamInfo_t& beamInfo) = 0;
	virtual	Beam_t* CreateBeamPoints(BeamInfo_t& beamInfo) = 0;
	virtual Beam_t* CreateBeamRing(BeamInfo_t& beamInfo) = 0;
	virtual Beam_t* CreateBeamRingPoint(BeamInfo_t& beamInfo) = 0;
	virtual Beam_t* CreateBeamCirclePoints(BeamInfo_t& beamInfo) = 0;
	virtual Beam_t* CreateBeamFollow(BeamInfo_t& beamInfo) = 0;

	virtual void	FreeBeam(Beam_t* pBeam) = 0;
	virtual void	UpdateBeamInfo(Beam_t* pBeam, BeamInfo_t& beamInfo) = 0;

	// These will go away!
	virtual void	CreateBeamEnts(int startEnt, int endEnt, int modelIndex, int haloIndex, float haloScale,
		float life, float width, float m_nEndWidth, float m_nFadeLength, float amplitude,
		float brightness, float speed, int startFrame,
		float framerate, float r, float g, float b, int type = -1) = 0;
	virtual void	CreateBeamEntPoint(int	nStartEntity, const CVector* pStart, int nEndEntity, const CVector* pEnd,
		int modelIndex, int haloIndex, float haloScale,
		float life, float width, float m_nEndWidth, float m_nFadeLength, float amplitude,
		float brightness, float speed, int startFrame,
		float framerate, float r, float g, float b) = 0;
	virtual void	CreateBeamPoints(CVector& start, CVector& end, int modelIndex, int haloIndex, float haloScale,
		float life, float width, float m_nEndWidth, float m_nFadeLength, float amplitude,
		float brightness, float speed, int startFrame,
		float framerate, float r, float g, float b) = 0;
	virtual void	CreateBeamRing(int startEnt, int endEnt, int modelIndex, int haloIndex, float haloScale,
		float life, float width, float m_nEndWidth, float m_nFadeLength, float amplitude,
		float brightness, float speed, int startFrame,
		float framerate, float r, float g, float b, int flags = 0) = 0;
	virtual void	CreateBeamRingPoint(const CVector& center, float start_radius, float end_radius, int modelIndex, int haloIndex, float haloScale,
		float life, float width, float m_nEndWidth, float m_nFadeLength, float amplitude,
		float brightness, float speed, int startFrame,
		float framerate, float r, float g, float b, int flags = 0) = 0;
	virtual void	CreateBeamCirclePoints(int type, CVector& start, CVector& end,
		int modelIndex, int haloIndex, float haloScale, float life, float width,
		float m_nEndWidth, float m_nFadeLength, float amplitude, float brightness, float speed,
		int startFrame, float framerate, float r, float g, float b) = 0;
	virtual void	CreateBeamFollow(int startEnt, int modelIndex, int haloIndex, float haloScale,
		float life, float width, float m_nEndWidth, float m_nFadeLength, float r, float g, float b,
		float brightness) = 0;
};

class CViewRenderBeams : public IViewRenderBeams
{
	// Construction
public:
	CViewRenderBeams(void) {};
	virtual				~CViewRenderBeams(void) = 0;

	// Implement IViewRenderBeams
public:
	virtual	void		InitBeams(void) = 0;
	virtual	void		ShutdownBeams(void) = 0;
	virtual	void		ClearBeams(void) = 0;

	// Updates the state of the temp ent beams
	virtual void		UpdateTempEntBeams() = 0;

	virtual void		DrawBeam(C_Beam* pbeam, ITraceFilter* pEntityBeamTraceFilter = NULL) = 0;
	virtual void		DrawBeam(Beam_t* pbeam) = 0;

	virtual	void		KillDeadBeams(C_BaseEntity* pDeadEntity) = 0;

	virtual Beam_t* CreateBeamEnts(BeamInfo_t& beamInfo) = 0;
	virtual Beam_t* CreateBeamEntPoint(BeamInfo_t& beamInfo) = 0;
	virtual	Beam_t* CreateBeamPoints(BeamInfo_t& beamInfo) = 0;
	virtual Beam_t* CreateBeamRing(BeamInfo_t& beamInfo) = 0;
	virtual Beam_t* CreateBeamRingPoint(BeamInfo_t& beamInfo) = 0;
	virtual Beam_t* CreateBeamCirclePoints(BeamInfo_t& beamInfo) = 0;
	virtual Beam_t* CreateBeamFollow(BeamInfo_t& beamInfo) = 0;

	virtual	void		CreateBeamEnts(int startEnt, int endEnt, int modelIndex, int haloIndex, float haloScale,
		float life, float width, float endWidth, float fadeLength, float amplitude,
		float brightness, float speed, int startFrame,
		float framerate, float r, float g, float b, int type = -1) = 0;
	virtual	void		CreateBeamEntPoint(int	nStartEntity, const CVector* pStart, int nEndEntity, const CVector* pEnd,
		int modelIndex, int haloIndex, float haloScale,
		float life, float width, float endWidth, float fadeLength, float amplitude,
		float brightness, float speed, int startFrame,
		float framerate, float r, float g, float b) = 0;
	virtual	void		CreateBeamPoints(CVector& start, CVector& end, int modelIndex, int haloIndex, float haloScale,
		float life, float width, float endWidth, float fadeLength, float amplitude,
		float brightness, float speed, int startFrame,
		float framerate, float r, float g, float b) = 0;
	virtual	void		CreateBeamRing(int startEnt, int endEnt, int modelIndex, int haloIndex, float haloScale,
		float life, float width, float endWidth, float fadeLength, float amplitude,
		float brightness, float speed, int startFrame,
		float framerate, float r, float g, float b, int flags) = 0;
	virtual void		CreateBeamRingPoint(const CVector& center, float start_radius, float end_radius, int modelIndex, int haloIndex, float haloScale,
		float life, float width, float m_nEndWidth, float m_nFadeLength, float amplitude,
		float brightness, float speed, int startFrame,
		float framerate, float r, float g, float b, int flags) = 0;
	virtual	void		CreateBeamCirclePoints(int type, CVector& start, CVector& end,
		int modelIndex, int haloIndex, float haloScale, float life, float width,
		float endWidth, float fadeLength, float amplitude, float brightness, float speed,
		int startFrame, float framerate, float r, float g, float b) = 0;
	virtual	void		CreateBeamFollow(int startEnt, int modelIndex, int haloIndex, float haloScale,
		float life, float width, float endWidth, float fadeLength, float r, float g, float b,
		float brightness) = 0;

	virtual void		FreeBeam(Beam_t* pBeam) { BeamFree(pBeam); }
	virtual void		UpdateBeamInfo(Beam_t* pBeam, BeamInfo_t& beamInfo) = 0;

private:
	virtual void					FreeDeadTrails(BeamTrail_t** trail) = 0;
	virtual void					UpdateBeam(Beam_t* pbeam, float frametime) = 0;
	virtual void					DrawBeamWithHalo(Beam_t* pbeam, int frame, int rendermode, float* color, float* srcColor, const model_t* sprite, const model_t* halosprite, float flHDRColorScale) = 0;
	virtual void					DrawBeamFollow(const model_t* pSprite, Beam_t* pbeam, int frame, int rendermode, float frametime, const float* color, float flHDRColorScale = 1.0f) = 0;
	virtual void					DrawLaser(Beam_t* pBeam, int frame, int rendermode, float* color, model_t const* sprite, model_t const* halosprite, float flHDRColorScale = 1.0f) = 0;
	virtual void					DrawTesla(Beam_t* pBeam, int frame, int rendermode, float* color, model_t const* sprite, float flHDRColorScale = 1.0f) = 0;

	virtual bool					RecomputeBeamEndpoints(Beam_t* pbeam) = 0;

	virtual int						CullBeam(const CVector& start, const CVector& end, int pvsOnly) = 0;

	// Creation
	virtual Beam_t* CreateGenericBeam(BeamInfo_t& beamInfo) = 0;
	virtual void					SetupBeam(Beam_t* pBeam, const BeamInfo_t& beamInfo) = 0;
	virtual void					SetBeamAttributes(Beam_t* pBeam, const BeamInfo_t& beamInfo) = 0;

	// Memory Alloc/Free
	virtual Beam_t* BeamAlloc(bool bRenderable) = 0;
	virtual void					BeamFree(Beam_t* pBeam) = 0;

	// DATA
private:
	enum
	{

#ifndef _XBOX
		// default max # of particles at one time
		DEFAULT_PARTICLES = 2048,
#else
		DEFAULT_PARTICLES = 1024,
#endif

		// no fewer than this no matter what's on the command line
		MIN_PARTICLES = 512,

#ifndef _XBOX
		// Maximum length of the free list.
		BEAM_FREELIST_MAX = 32
#else
		BEAM_FREELIST_MAX = 4
#endif

	};

	Beam_t* m_pActiveBeams;
	Beam_t* m_pFreeBeams;
	int						m_nBeamFreeListLength;

	BeamTrail_t* m_pBeamTrails;
	BeamTrail_t* m_pActiveTrails;
	BeamTrail_t* m_pFreeTrails;
	int						m_nNumBeamTrails;
};
