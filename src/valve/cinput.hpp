#ifndef CINPUT_H
#define CINPUT_H
#ifdef _WIN32
#pragma once
#endif
#include "cusercmd.hpp"
#include "chandle.hpp"
#include "../utils/memory.hpp"

class C_BaseCombatWeapon;

struct CameraThirdData_t
{
	float	m_flPitch;
	float	m_flYaw;
	float	m_flDist;
	float	m_flLag;
	CVector	m_vecHullMin;
	CVector	m_vecHullMax;
};

class bf_write;
class bf_read;
typedef unsigned int CRC32_t;

#define MULTIPLAYER_BACKUP 90

struct kbutton_t
{
	// key nums holding it down
	int		down[2];
	// low bit is down state
	int		state;
};

class CKeyboardKey
{
public:
	// Name for key
	char				name[32];
	// Pointer to the underlying structure
	kbutton_t* pkey;
	// Next key in key list.
	CKeyboardKey* next;
};

typedef struct
{
	unsigned int AxisFlags;
	unsigned int AxisMap;
	unsigned int ControlMap;
} joy_axis_t;

enum
{
	GAME_AXIS_NONE = 0,
	GAME_AXIS_FORWARD,
	GAME_AXIS_PITCH,
	GAME_AXIS_SIDE,
	GAME_AXIS_YAW,
	MAX_GAME_AXES
};

enum
{
	CAM_COMMAND_NONE = 0,
	CAM_COMMAND_TOTHIRDPERSON = 1,
	CAM_COMMAND_TOFIRSTPERSON = 2
};

enum
{
	MOUSE_ACCEL_THRESHHOLD1 = 0,	// if mouse moves > this many mickey's double it
	MOUSE_ACCEL_THRESHHOLD2,		// if mouse moves > this many mickey's double it a second time
	MOUSE_SPEED_FACTOR,				// 0 = disabled, 1 = threshold 1 enabled, 2 = threshold 2 enabled

	NUM_MOUSE_PARAMS,
};

enum JoystickAxis_t
{
	JOY_AXIS_X = 0,
	JOY_AXIS_Y,
	JOY_AXIS_Z,
	JOY_AXIS_R,
	JOY_AXIS_U,
	JOY_AXIS_V,
	MAX_JOYSTICK_AXES,
};

class CVerifiedUserCmd
{
public:
	CUserCmd	m_cmd;
	CRC32_t		m_crc;
};

class CInput {
public:
	virtual void  Init_All(void) = 0;
	virtual void  Shutdown_All(void) = 0;
	virtual int   GetButtonBits(int) = 0;
	virtual void  CreateMove(int sequence_number, float input_sample_frametime, bool active) = 0;
	virtual void  ExtraMouseSample(float frametime, bool active) = 0;
	virtual bool  WriteUsercmdDeltaToBuffer(bf_write* buf, int from, int to, bool isnewcommand) = 0;
	virtual void  EncodeUserCmdToBuffer(bf_write& buf, int slot) = 0;
	virtual void  DecodeUserCmdFromBuffer(bf_read& buf, int slot) = 0;


	CUserCmd* GetUserCmd(int sequence_number)
	{
		CUserCmd* usercmd = &m_pCommands[sequence_number % MULTIPLAYER_BACKUP];

		return usercmd;
	}

	CVerifiedUserCmd* GetVerifiedCmd(int sequence_number)
	{
		return &m_pVerifiedCommands[sequence_number % MULTIPLAYER_BACKUP];
	}
public:

	// Has the mouse been initialized?
	bool		m_fMouseInitialized;
	// Is the mosue active?
	bool		m_fMouseActive;
	// Has the joystick advanced initialization been run?
	bool		m_fJoystickAdvancedInit;
	// Used to support hotplugging by reinitializing the advanced joystick system when we toggle between some/none joysticks.
	bool		m_fHadJoysticks;

	// Accumulated mouse deltas
	float		m_flAccumulatedMouseXMovement;
	float		m_flAccumulatedMouseYMovement;
	float		m_flPreviousMouseXPosition;
	float		m_flPreviousMouseYPosition;
	float		m_flRemainingJoystickSampleTime;
	float		m_flKeyboardSampleTime;

	// Flag to restore systemparameters when exiting
	bool		m_fRestoreSPI;
	// Original mouse parameters
	int			m_rgOrigMouseParms[NUM_MOUSE_PARAMS];
	// Current mouse parameters.
	int			m_rgNewMouseParms[NUM_MOUSE_PARAMS];
	bool		m_rgCheckMouseParam[NUM_MOUSE_PARAMS];
	// Are the parameters valid
	bool		m_fMouseParmsValid;
	// Joystick Axis data
	joy_axis_t m_rgAxes[MAX_JOYSTICK_AXES];
	// List of queryable keys
	CKeyboardKey* m_pKeys;

	// Is the 3rd person camera using the mouse?
	bool		m_fCameraInterceptingMouse;
	// Are we in 3rd person view?
	bool		m_fCameraInThirdPerson;
	// Should we move view along with mouse?
	bool		m_fCameraMovingWithMouse;


	// Is the camera in distance moving mode?
	bool		m_fCameraDistanceMove;
	// Old and current mouse position readings.
	int			m_nCameraOldX;
	int			m_nCameraOldY;
	int			m_nCameraX;
	int			m_nCameraY;

	// orthographic camera settings
	bool		m_CameraIsOrthographic;

	CVector		m_angPreviousViewAngles;

	float		m_flLastForwardMove;

	float m_flPreviousJoystickForward;
	float m_flPreviousJoystickSide;
	float m_flPreviousJoystickPitch;
	float m_flPreviousJoystickYaw;

	CUserCmd* m_pCommands;
	CVerifiedUserCmd* m_pVerifiedCommands;

	CameraThirdData_t* m_pCameraThirdData;

	// Set until polled by CreateMove and cleared
	CHandle<C_BaseCombatWeapon>* m_hSelectedWeapon;

	constexpr void ToThirdPerson() noexcept {
		memory::Call<void>(this, 32);
	}

	constexpr void ToFirstPerson() noexcept {
		memory::Call<void>(this, 33);
	}
};
#endif