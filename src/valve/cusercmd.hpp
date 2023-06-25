#pragma once

#include "cvector.hpp"
#include "checksum/checksum_crc.hpp"

enum ECommandButton : std::int32_t
{
	IN_ATTACK = (1 << 0),
	IN_JUMP = (1 << 1),
	IN_DUCK = (1 << 2),
	IN_FORWARD = (1 << 3),
	IN_BACK = (1 << 4),
	IN_USE = (1 << 5),
	IN_CANCEL = (1 << 6),
	IN_LEFT = (1 << 7),
	IN_RIGHT = (1 << 8),
	IN_MOVELEFT = (1 << 9),
	IN_MOVERIGHT = (1 << 10),
	IN_SECOND_ATTACK = (1 << 11),
	IN_RUN = (1 << 12),
	IN_RELOAD = (1 << 13),
	IN_LEFT_ALT = (1 << 14),
	IN_RIGHT_ALT = (1 << 15),
	IN_SCORE = (1 << 16),
	IN_SPEED = (1 << 17),
	IN_WALK = (1 << 18),
	IN_ZOOM = (1 << 19),
	IN_FIRST_WEAPON = (1 << 20),
	IN_SECOND_WEAPON = (1 << 21),
	IN_BULLRUSH = (1 << 22),
	IN_FIRST_GRENADE = (1 << 23),
	IN_SECOND_GRENADE = (1 << 24),
	IN_MIDDLE_ATTACK = (1 << 25)
};

class CUserCmd
{
public:
	CUserCmd()
	{
		Reset();
	}

	virtual ~CUserCmd() {};

	void Reset()
	{
		commandNumber = 0;
		tickCount = 0;
		viewAngles = { 0.f, 0.f, 0.f };
		forwardMove = 0.0f;
		sideMove = 0.0f;
		upMove = 0.0f;
		buttons = 0;
		impulse = 0;
		weaponselect = 0;
		weaponsubtype = 0;
		random_seed = 0;
		mousedx = 0;
		mousedy = 0;

		hasbeenpredicted = false;
	}

	CUserCmd& operator=(const CUserCmd& src)
	{
		if (this == &src)
		{
			return *this;
		}

		commandNumber = src.commandNumber;
		tickCount = src.tickCount;
		viewAngles = src.viewAngles;
		forwardMove = src.forwardMove;
		sideMove = src.sideMove;
		upMove = src.upMove;
		buttons = src.buttons;
		impulse = src.impulse;
		weaponselect = src.weaponselect;
		weaponsubtype = src.weaponsubtype;
		random_seed = src.random_seed;
		mousedx = src.mousedx;
		mousedy = src.mousedy;

		hasbeenpredicted = src.hasbeenpredicted;

		return *this;
	}

	CUserCmd(const CUserCmd& src)
	{
		*this = src;
	}

	CRC32_t GetChecksum(void) const
	{
		CRC32_t crc;

		CRC32_Init(&crc);
		CRC32_ProcessBuffer(&crc, &commandNumber, sizeof(commandNumber));
		CRC32_ProcessBuffer(&crc, &tickCount, sizeof(tickCount));
		CRC32_ProcessBuffer(&crc, &viewAngles, sizeof(viewAngles));
		CRC32_ProcessBuffer(&crc, &forwardMove, sizeof(forwardMove));
		CRC32_ProcessBuffer(&crc, &sideMove, sizeof(sideMove));
		CRC32_ProcessBuffer(&crc, &upMove, sizeof(upMove));
		CRC32_ProcessBuffer(&crc, &buttons, sizeof(buttons));
		CRC32_ProcessBuffer(&crc, &impulse, sizeof(impulse));
		CRC32_ProcessBuffer(&crc, &weaponselect, sizeof(weaponselect));
		CRC32_ProcessBuffer(&crc, &weaponsubtype, sizeof(weaponsubtype));
		CRC32_ProcessBuffer(&crc, &random_seed, sizeof(random_seed));
		CRC32_ProcessBuffer(&crc, &mousedx, sizeof(mousedx));
		CRC32_ProcessBuffer(&crc, &mousedy, sizeof(mousedy));
		CRC32_Final(&crc);

		return crc;
	}

	void MakeInert(void)
	{
		viewAngles = { 0.f, 0.f, 0.f };
		forwardMove = 0.f;
		sideMove = 0.f;
		upMove = 0.f;
		buttons = 0;
		impulse = 0;
	}

	int commandNumber; //4
	int tickCount; //8
	CVector viewAngles; //C
	float forwardMove; //18
	float sideMove; //1C
	float upMove; //20
	int	buttons; //24
	unsigned int impulse; //28
	int weaponselect; //2C
	int weaponsubtype; //30
	int random_seed; //34
	short mousedx; //38
	short mousedy; //3A
	bool hasbeenpredicted; //3C;
};