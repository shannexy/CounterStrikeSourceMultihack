#pragma once
#include <cstdint>

#include "cvector.hpp"
class C_BaseEntity;

#define	TE_BEAMPOINTS		0		// beam effect between two points
#define TE_SPRITE			1	// additive sprite, plays 1 cycle
#define TE_BEAMDISK			2	// disk that expands to max radius over lifetime
#define TE_BEAMCYLINDER		3		// cylinder that expands to max radius over lifetime
#define TE_BEAMFOLLOW		4		// create a line of decaying beam segments until entity stops moving
#define TE_BEAMRING			5		// connect a beam ring to two entities
#define TE_BEAMSPLINE		6		
#define TE_BEAMRINGPOINT	7
#define	TE_BEAMLASER		8		// Fades according to viewpoint
#define TE_BEAMTESLA		9

#define FBEAM_STARTENTITY 0x00000001
#define FBEAM_ENDENTITY 0x00000002
#define FBEAM_FADEIN 0x00000004
#define FBEAM_FADEOUT 0x00000008
#define FBEAM_SINENOISE 0x00000010
#define FBEAM_SOLID 0x00000020
#define FBEAM_SHADEIN 0x00000040
#define FBEAM_SHADEOUT 0x00000080
#define FBEAM_ONLYNOISEONCE 0x00000100		// Only calculate our noise once
#define FBEAM_NOTILE 0x00000200
#define FBEAM_USE_HITBOXES 0x00000400		// Attachment indices represent hitbox indices instead when this is set.
#define FBEAM_STARTVISIBLE 0x00000800		// Has this client actually seen this beam's start entity yet?
#define FBEAM_ENDVISIBLE 0x00001000		// Has this client actually seen this beam's end entity yet?
#define FBEAM_ISACTIVE 0x00002000
#define FBEAM_FOREVER 0x00004000
#define FBEAM_HALOBEAM 0x00008000		// When drawing a beam with a halo, don't ignore the segments and endwidth
#define FBEAM_REVERSED 0x00010000
#define NUM_BEAM_FLAGS 17	// KEEP THIS UPDATED!

class bf_read;
class bf_write;

class INetChannel;
class INetMessage {
public:
	virtual	~INetMessage() {};

	// Use these to setup who can hear whose voice.
	// Pass in client indices (which are their ent indices - 1).

	virtual void	SetNetChannel(INetChannel* netchan) = 0; // netchannel this message is from/for
	virtual void	SetReliable(bool state) = 0;	// set to true if it's a reliable message

	virtual bool	Process(void) = 0; // calles the recently set handler to process this message

	virtual	bool	ReadFromBuffer(bf_read& buffer) = 0; // returns true if parsing was OK
	virtual	bool	WriteToBuffer(bf_write& buffer) = 0;	// returns true if writing was OK

	virtual bool	IsReliable(void) const = 0;  // true, if message needs reliable handling

	virtual int				GetType(void) const = 0; // returns module specific header tag eg svc_serverinfo
	virtual int				GetGroup(void) const = 0;	// returns net message group of this message
	virtual const char* GetName(void) const = 0;	// returns network message name, eg "svc_serverinfo"
	virtual INetChannel* GetNetChannel(void) const = 0;
	virtual const char* ToString(void) const = 0; // returns a human readable string about message content
};

enum ClientFrameStage_t
{
	FRAME_UNDEFINED = -1,			// (haven't run any frames yet)
	FRAME_START,

	// A network packet is being recieved
	FRAME_NET_UPDATE_START,
	// Data has been received and we're going to start calling PostDataUpdate
	FRAME_NET_UPDATE_POSTDATAUPDATE_START,
	// Data has been received and we've called PostDataUpdate on all data recipients
	FRAME_NET_UPDATE_POSTDATAUPDATE_END,
	// We've received all packets, we can now do interpolation, prediction, etc..
	FRAME_NET_UPDATE_END,

	// We're about to start rendering the scene
	FRAME_RENDER_START,
	// We've finished rendering the scene.
	FRAME_RENDER_END
};

struct BeamInfo_t
{
	int			m_nType;

	// Entities
	C_BaseEntity* m_pStartEnt;
	int			m_nStartAttachment;
	C_BaseEntity* m_pEndEnt;
	int			m_nEndAttachment;

	// Points
	CVector		m_vecStart;
	CVector		m_vecEnd;

	int			m_nModelIndex;
	const char* m_pszModelName;

	int			m_nHaloIndex;
	const char* m_pszHaloName;
	float		m_flHaloScale;

	float		m_flLife;
	float		m_flWidth;
	float		m_flEndWidth;
	float		m_flFadeLength;
	float		m_flAmplitude;

	float		m_flBrightness;
	float		m_flSpeed;

	int			m_nStartFrame;
	float		m_flFrameRate;

	float		m_flRed;
	float		m_flGreen;
	float		m_flBlue;

	bool		m_bRenderable;

	int			m_nSegments;

	int			m_nFlags;

	// Rings
	CVector		m_vecCenter;
	float		m_flStartRadius;
	float		m_flEndRadius;
};

enum BhopType {
	DISABLED = 0,
	NORMAL,
	PERFECT,
};