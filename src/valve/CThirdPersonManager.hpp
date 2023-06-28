#pragma once

#include "cvector.hpp"

class CThirdPersonManager 
{
public:
	CThirdPersonManager() {}
	virtual void	SetCameraOffsetAngles(const CVector& vecOffset) = 0;
	virtual const CVector& GetCameraOffsetAngles(void) const = 0;

	virtual void	SetDesiredCameraOffset(const CVector& vecOffset) = 0;
	virtual const CVector& GetDesiredCameraOffset(void) const = 0;

	virtual CVector	GetFinalCameraOffset(void) = 0;

	virtual void	SetCameraOrigin(const CVector& vecOffset) = 0;
	virtual const CVector& GetCameraOrigin(void) const = 0;

	virtual void	Update(void) = 0;

	virtual void	PositionCamera(C_BaseEntity* pPlayer, const CVector& angles) = 0;

	virtual void	UseCameraOffsets(bool bUse) = 0;
	virtual bool	UsingCameraOffsets(void) = 0;

	virtual const CVector& GetCameraViewAngles(void) const = 0;

	virtual CVector	GetDistanceFraction(void) = 0;

	virtual bool	WantToUseGameThirdPerson(void) = 0;
	 
	virtual void	SetOverridingThirdPerson(bool bOverride) = 0;
	virtual bool	IsOverridingThirdPerson(void) = 0;
	 
	virtual void	Init(void) = 0;
	 
	virtual void	SetForcedThirdPerson(bool bForced) = 0;
	virtual bool	GetForcedThirdPerson() const = 0;
public:
	// What is the current camera offset from the view origin?
	CVector		m_vecCameraOffset;
	// Distances from the center
	CVector		m_vecDesiredCameraOffset;

	CVector m_vecCameraOrigin;

	bool	m_bUseCameraOffsets;

	CVector	m_ViewAngles;

	float	m_flFraction;
	float	m_flUpFraction;

	float	m_flTargetFraction;
	float	m_flTargetUpFraction;

	bool	m_bOverrideThirdPerson;

	bool	m_bForced;

	float	m_flUpOffset;

	float	m_flLerpTime;
	float	m_flUpLerpTime;
};