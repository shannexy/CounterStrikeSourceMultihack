#pragma once

#include "iclientnetworkable.hpp"
#include "iclientrenderable.hpp"
#include "iclientunknown.hpp"
#include "iclientthinkable.hpp"

struct SpatializationInfo_t;

class CAudioSource;

class CVoiceData {
public:
	virtual void SetElapsedTime(float t) = 0;
	virtual float GetElapsedTime() const = 0;
	virtual void SetSource(CAudioSource* source, bool bIgnorePhonemes) = 0;
	virtual bool ShouldIgnorePhonemes() const = 0;
	virtual CAudioSource* GetSource() = 0;
};

class CMouthInfo {
public:
	enum
	{
		MAX_VOICE_DATA = 4
	};
public:
	virtual int GetNumVoiceSources(void) = 0;
	virtual CVoiceData* GetVoiceSource(int number) = 0;

	virtual void ClearVoiceSources(void) = 0;
	virtual int GetIndexForSource(CAudioSource* source) = 0;
	virtual bool IsSourceReferenced(CAudioSource* source) = 0;
	virtual CVoiceData* AddSource(CAudioSource* source, bool bIgnorePhonemes) = 0;
	virtual void RemoveSource(CAudioSource* source) = 0;
	virtual void RemoveSourceByIndex(int index) = 0;
	virtual bool IsActive(void) = 0;
	virtual bool NeedsEnvelope() = 0;
	virtual void ActivateEnvelope() = 0;
};

class IClientEntity : public IClientUnknown, public IClientRenderable, public IClientNetworkable, public IClientThinkable
{
public:
	virtual ~IClientEntity() {}
	virtual void Release(void) = 0;
	virtual const CVector& GetAbsOrigin(void) const = 0;
	virtual const CVector& GetAbsAngles(void) const = 0;
	virtual CMouthInfo* GetMouth(void) = 0;
	virtual bool GetSoundSpatialization(SpatializationInfo_t& info) = 0;

	template<typename T>
	inline T As() {
#ifdef _DEBUG
		return dynamic_cast<T*>(this);
#else
		return static_cast<T*>(this);
#endif
	}
};
