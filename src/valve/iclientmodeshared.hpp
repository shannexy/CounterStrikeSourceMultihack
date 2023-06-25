#pragma once

#include "cusercmd.hpp"
#include "iclientmode.hpp"
#include "igameventmanager.hpp"
#include "../sdk.hpp"


class CBaseViewport;
class CBaseHudChat;
class CBaseHudWeaponSelection;


class IClientModeShared : public IClientMode, public CGameEventListener
{
public:
	DECLARE_CLASS_NOBASE(IClientModeShared);

	IClientModeShared() {}
	virtual			~IClientModeShared() = 0;

	virtual void	Init() = 0;
	virtual void	InitViewport() = 0;
	virtual void	VGui_Shutdown() = 0;
	virtual void	Shutdown() = 0;

	virtual void	LevelInit(const char* newmap) = 0;
	virtual void	LevelShutdown(void) = 0;

	virtual void	Enable() = 0;
	virtual void	Disable() = 0;
	virtual void	Layout() = 0;

	virtual void	ReloadScheme(void) = 0;
	virtual void	OverrideView(CViewSetup* pSetup) = 0;
	virtual bool	ShouldDrawDetailObjects() = 0;
	virtual bool	ShouldDrawEntity(C_BaseEntity* pEnt) = 0;
	virtual bool	ShouldDrawLocalPlayer(C_BasePlayer* pPlayer) = 0;
	virtual bool	ShouldDrawViewModel() = 0;
	virtual bool	ShouldDrawParticles() = 0;
	virtual bool	ShouldDrawCrosshair(void) = 0;
	virtual void	AdjustEngineViewport(int& x, int& y, int& width, int& height) = 0;
	virtual void	PreRender(CViewSetup* pSetup) = 0;
	virtual void	PostRender() = 0;
	virtual void	PostRenderVGui() = 0;
	virtual void	ProcessInput(bool bActive) = 0;
	virtual bool	CreateMove(float flInputSampleTime, CUserCmd* cmd) = 0;
	virtual void	Update() = 0;

	// Input
	virtual int		KeyInput(int down, ButtonCode_t keynum, const char* pszCurrentBinding) = 0;
	virtual int		HudElementKeyInput(int down, ButtonCode_t keynum, const char* pszCurrentBinding) = 0;
	virtual void	OverrideMouseInput(float* x, float* y) = 0;
	virtual void	StartMessageMode(int iMessageModeType) = 0;
	virtual vgui::Panel* GetMessagePanel() = 0;

	virtual void	ActivateInGameVGuiContext(vgui::Panel* pPanel) = 0;
	virtual void	DeactivateInGameVGuiContext() = 0;

	// The mode can choose to not draw fog
	virtual bool	ShouldDrawFog(void) = 0;

	virtual float	GetViewModelFOV(void) = 0;
	virtual vgui::Panel* GetViewport() = 0;
	// Gets at the viewports vgui panel animation controller, if there is one...
	virtual vgui::AnimationController* GetViewportAnimationController() = 0;
	virtual void FireGameEvent(IGameEvent* event) = 0;

	virtual bool CanRecordDemo(char* errorMsg, int length) const = 0;

	virtual int HandleSpectatorKeyInput(int down, ButtonCode_t keynum, const char* pszCurrentBinding) = 0;

protected:
	CBaseViewport* m_pViewport;

private:
	// Message mode handling
	// All modes share a common chat interface
	CBaseHudChat* m_pChatElement;
	vgui::HCursor			m_CursorNone;
	CBaseHudWeaponSelection* m_pWeaponSelection;
	int						m_nRootSize[2];
};
