#pragma once

#include "../valve/igameventmanager.hpp"
#include "../core/Singleton.hpp"
#include "../valve/cvector.hpp"
#include <vector>

struct BulletImpactInfo 
{
	int fromUserId;
	float m_flExpTime;
	CVector m_vecHitPos;
};

class BulletImpactEvent : public IGameEventListener2, public Singleton<BulletImpactEvent>
{
public:
	void FireGameEvent(IGameEvent* event);
	void RegisterSelf();
	void UnregisterSelf();
	void Paint();

private:
	std::vector<BulletImpactInfo> bulletImpactInfo;
};

