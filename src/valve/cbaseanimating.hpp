#ifndef CBASEANIMATING_H
#define CBASEANIMATING_H
#ifdef _WIN32
#pragma once
#endif
#include "centity.hpp"

struct animevent_t;
struct LocalFlexController_t;
struct inputdata_t;
struct string_t;

class C_BaseAnimating : public C_BaseEntity {
public:
	enum
	{
		NUM_POSEPAREMETERS = 24,
		NUM_BONECTRLS = 4
	};
	virtual void SetModel(const char* szModelName) = 0;
	virtual void Activate() = 0;
	virtual void Spawn() = 0;
	virtual void Precache() = 0;
	virtual void SetTransmit(void** pInfo, bool bAlways) = 0;
	virtual int Restore(void*& restore) = 0;
	virtual void OnRestore() = 0;
	virtual CStudioHdr* GetModelPtr(void) = 0;
	virtual void InvalidateMdlCache() = 0;
	virtual CStudioHdr* OnNewModel() = 0;
	virtual C_BaseAnimating* GetBaseAnimating() = 0;
	virtual void SetCycle(float flCycle) = 0;
	virtual float GetCycle() const = 0;
	virtual float GetAnimTimeInterval(void) const = 0;
	virtual void UseClientSideAnimation() = 0;
	virtual bool IsUsingClientSideAnimation() = 0;

	virtual float GetIdealSpeed() const = 0;
	virtual float GetIdealAccel() const = 0;
	virtual void StudioFrameAdvance() = 0; // advance animation frame to some time in the future
	virtual void StudioFrameAdvanceManual(float flInterval) = 0;
	virtual bool IsValidSequence(int iSequence) = 0;

	virtual inline float GetPlaybackRate() = 0;
	virtual inline void SetPlaybackRate(float rate) = 0;

	virtual inline int GetSequence() = 0;
	virtual void SetSequence(int nSequence) = 0;
	virtual void ResetSequence(int nSequence) = 0;

	virtual bool IsActivityFinished(void) = 0;
	virtual inline bool IsSequenceFinished(void) = 0;
	virtual inline bool SequenceLoops(void) = 0;

	virtual bool IsSequenceLooping(CStudioHdr* pStudioHdr, int iSequence) = 0;
	virtual inline bool IsSequenceLooping(int iSequence) = 0;
	virtual inline float SequenceDuration(void) = 0;
	virtual float SequenceDuration(CStudioHdr* pStudioHdr, int iSequence) = 0;
	virtual inline float SequenceDuration(int iSequence) = 0;
	virtual float GetSequenceCycleRate(CStudioHdr* pStudioHdr, int iSequence) = 0;
	virtual inline float GetSequenceCycleRate(int iSequence) = 0;
	virtual float GetLastVisibleCycle(CStudioHdr* pStudioHdr, int iSequence) = 0;
	virtual float GetSequenceGroundSpeed(CStudioHdr* pStudioHdr, int iSequence) = 0;
	virtual inline float GetSequenceGroundSpeed(int iSequence) = 0;

	virtual void ResetActivityIndexes(void) = 0;
	virtual void ResetEventIndexes(void) = 0;
	virtual int SelectWeightedSequence(void* activity) = 0;
	virtual int SelectWeightedSequence(void* activity, int curSequence) = 0;
	virtual int SelectWeightedSequenceFromModifiers(void* activity, void** pActivityModifiers, int iModifierCount) = 0;
	virtual int SelectHeaviestSequence(void* activity) = 0;
	virtual int LookupActivity(const char* label) = 0;
	virtual int LookupSequence(const char* label) = 0;
	virtual void* GetSequenceKeyValues(int iSequence) = 0;

	virtual float GetSequenceMoveYaw(int iSequence) = 0;
	virtual float GetSequenceMoveDist(CStudioHdr* pStudioHdr, int iSequence) = 0;
	virtual inline float GetSequenceMoveDist(int iSequence) = 0;
	virtual void GetSequenceLinearMotion(int iSequence, CVector* pVec) = 0;
	virtual const char* GetSequenceName(int iSequence) = 0;
	virtual const char* GetSequenceActivityName(int iSequence) = 0;
	virtual void* GetSequenceActivity(int iSequence) = 0;

	virtual void ResetSequenceInfo() = 0;

	virtual inline void StopAnimation(void) = 0;

	virtual void ClampRagdollForce(const CVector& vecForceIn, CVector* vecForceOut) = 0;

	virtual bool BecomeRagdollOnClient(const CVector& force) = 0;
	virtual bool IsRagdoll() = 0;
	virtual bool CanBecomeRagdoll(void) = 0; //Check if this entity will ragdoll when dead.

	virtual void GetSkeleton(CStudioHdr* pStudioHdr, CVector pos[], CVector q[], int boneMask) = 0;

	virtual void GetBoneTransform(int iBone, CMatrix3x4& pBoneToWorld) = 0;
	virtual void SetupBones(CMatrix3x4* pBoneToWorld, int boneMask) = 0;
	virtual void CalculateIKLocks(float currentTime) = 0;
	virtual void Teleport(const CVector* newPosition, const CVector* newAngles, const CVector* newVelocity) = 0;

	virtual bool HasAnimEvent(int nSequence, int nEvent) = 0;
	virtual void DispatchAnimEvents(C_BaseAnimating* eventHandler) = 0;
	// Handle events that have happend since last time called up until X seconds into the future
	virtual void HandleAnimEvent(animevent_t* pEvent) = 0;

	virtual int LookupPoseParameter(CStudioHdr* pStudioHdr, const char* szName) = 0;
	virtual inline int LookupPoseParameter(const char* szName) = 0;


	virtual float SetPoseParameter(CStudioHdr* pStudioHdr, const char* szName, float flValue) = 0;
	virtual inline float SetPoseParameter(const char* szName, float flValue) = 0;
	virtual float SetPoseParameter(CStudioHdr* pStudioHdr, int iParameter, float flValue) = 0;
	virtual inline float SetPoseParameter(int iParameter, float flValue) = 0;

	virtual float GetPoseParameter(const char* szName) = 0;
	virtual float GetPoseParameter(int iParameter) = 0;
	virtual bool GetPoseParameterRange(int index, float& minValue, float& maxValue) = 0;
	virtual bool HasPoseParameter(int iSequence, const char* szName) = 0;
	virtual bool HasPoseParameter(int iSequence, int iParameter) = 0;
	virtual float EdgeLimitPoseParameter(int iParameter, float flValue, float flBase = 0.0f) = 0;

protected:
	virtual void PopulatePoseParameters(void) = 0;

public:
	virtual int LookupBone(const char* szName) = 0;
	virtual void GetBonePosition(const char* szName, CVector& origin, CVector& angles) = 0;
	virtual void GetBonePosition(int iBone, CVector& origin, CVector& angles) = 0;
	virtual int GetPhysicsBone(int boneIndex) = 0;
	virtual int GetNumBones(void) = 0;
	virtual int FindTransitionSequence(int iCurrentSequence, int iGoalSequence, int* piDir) = 0;
	virtual bool GotoSequence(int iCurrentSequence, float flCurrentCycle, float flCurrentRate, int iGoalSequence,
		int& iNextSequence, float& flCycle, int& iDir) = 0;
	virtual int GetEntryNode(int iSequence) = 0;
	virtual int GetExitNode(int iSequence) = 0;
	virtual void GetEyeballs(CVector& origin, CVector& angles) = 0; // ?? remove ??
	virtual int LookupAttachment(const char* szName) = 0;
	virtual bool GetAttachment(const char* szName, CVector& absOrigin, CVector& absAngles) = 0;
	virtual bool GetAttachment(int iAttachment, CVector& absOrigin, CVector& absAngles) = 0;
	virtual int GetAttachmentBone(int iAttachment) = 0;
	virtual bool GetAttachment(int iAttachment, CMatrix3x4& attachmentToWorld) = 0;

	// These return the attachment in the space of the entity
	virtual bool GetAttachmentLocal(const char* szName, CVector& origin, CVector& angles) = 0;
	virtual bool GetAttachmentLocal(int iAttachment, CVector& origin, CVector& angles) = 0;
	virtual bool GetAttachmentLocal(int iAttachment, CMatrix3x4& attachmentToLocal) = 0;

	// Non-angle versions of the attachments in world space
	virtual bool GetAttachment(const char* szName, CVector& absOrigin, CVector* forward = nullptr,
		CVector* right = nullptr, CVector* up = nullptr) = 0;
	virtual bool GetAttachment(int iAttachment, CVector& absOrigin, CVector* forward = nullptr,
		CVector* right = nullptr, CVector* up = nullptr) = 0;

	virtual void SetBodygroup(int iGroup, int iValue) = 0;
	virtual int GetBodygroup(int iGroup) = 0;

	virtual const char* GetBodygroupName(int iGroup) = 0;
	virtual int FindBodygroupByName(const char* name) = 0;
	virtual int GetBodygroupCount(int iGroup) = 0;
	virtual int GetNumBodyGroups(void) = 0;

	virtual void SetHitboxSet(int setnum) = 0;
	virtual void SetHitboxSetByName(const char* setname) = 0;
	virtual int GetHitboxSet(void) = 0;
	virtual const char* GetHitboxSetName(void) = 0;
	virtual int GetHitboxSetCount(void) = 0;
	virtual int GetHitboxBone(int hitboxIndex) = 0;
	virtual bool LookupHitbox(const char* szName, int& outSet, int& outBox) = 0;

	// Computes a box that surrounds all hitboxes
	virtual bool ComputeHitboxSurroundingBox(CVector* pVecWorldMins, CVector* pVecWorldMaxs) = 0;
	virtual bool ComputeEntitySpaceHitboxSurroundingBox(CVector* pVecWorldMins, CVector* pVecWorldMaxs) = 0;

	// Clone a CBaseAnimating from another (copies model & sequence data)
	virtual void CopyAnimationDataFrom(C_BaseAnimating* pSource) = 0;

	virtual int ExtractBbox(int sequence, CVector& mins, CVector& maxs) = 0;
	virtual void SetSequenceBox(void) = 0;
	virtual int RegisterPrivateActivity(const char* pszActivityName) = 0;

	virtual void ResetClientsideFrame(void) = 0;

	// Controllers.
	virtual void InitBoneControllers(void) = 0;

	// Return's the controller's angle/position in bone space.
	virtual float GetBoneController(int iController) = 0;

	// Maps the angle/position value you specify into the bone's start/end and sets the specified controller to the value.
	virtual float SetBoneController(int iController, float flValue) = 0;

	virtual void GetVelocity(CVector* vVelocity, CVector* vAngVelocity) = 0;

	// these two need to move somewhere else
	virtual LocalFlexController_t GetNumFlexControllers(void) = 0;
	virtual const char* GetFlexDescFacs(int iFlexDesc) = 0;
	virtual const char* GetFlexControllerName(LocalFlexController_t iFlexController) = 0;
	virtual const char* GetFlexControllerType(LocalFlexController_t iFlexController) = 0;

	virtual CVector GetGroundSpeedVelocity(void) = 0;

	virtual bool GetIntervalMovement(float flIntervalUsed, bool& bMoveSeqFinished, CVector& newPosition,
		CVector& newAngles) = 0;
	virtual bool GetSequenceMovement(int nSequence, float fromCycle, float toCycle, CVector& deltaPosition,
		CVector& delta) = 0;
	virtual float GetInstantaneousVelocity(float flInterval = 0.0) = 0;
	virtual float GetEntryVelocity(int iSequence) = 0;
	virtual float GetExitVelocity(int iSequence) = 0;
	virtual float GetMovementFrame(float flDist) = 0;
	virtual bool HasMovement(int iSequence) = 0;

	virtual void ReportMissingActivity(int iActivity) = 0;
	virtual bool TestCollision(const Ray_t& ray, unsigned int fContentsMask, trace_t& tr) = 0;
	virtual bool TestHitboxes(const Ray_t& ray, unsigned int fContentsMask, trace_t& tr) = 0;
	virtual class CBoneCache* GetBoneCache(void) = 0;
	virtual void InvalidateBoneCache() = 0;
	virtual void InvalidateBoneCacheIfOlderThan(float deltaTime) = 0;
	virtual int DrawDebugTextOverlays(void) = 0;

	// See note in code re: bandwidth usage!!!
	virtual void DrawServerHitboxes(float duration = 0.0f, bool monocolor = false) = 0;
	virtual void DrawRawSkeleton(CMatrix3x4[], int boneMask, bool noDepthTest = true, float duration = 0.0f,
		bool monocolor = false) = 0;

	virtual void SetModelScale(float scale, float change_duration = 0.0f) = 0;
	virtual float GetModelScale() const = 0;

	virtual void UpdateModelScale() = 0;
	virtual void RefreshCollisionBounds(void) = 0;

	virtual void EnableServerIK() = 0;
	virtual void DisableServerIK() = 0;

	// for ragdoll vs. car
	virtual int GetHitboxesFrontside(int* boxList, int boxMax, const CVector& normal, float dist) = 0;

	virtual void GetInputDispatchEffectPosition(const char* sInputString, CVector& pOrigin, CVector& pAngles) = 0;

	virtual void ModifyOrAppendCriteria(void*& set) = 0;

	// Send a muzzle flash event to the client for this entity.
	virtual void DoMuzzleFlash() = 0;

	// Fire
	virtual void Ignite(float flFlameLifetime, bool bNPCOnly = true, float flSize = 0.0f,
		bool bCalledByLevelDesigner = false) = 0;
	virtual void IgniteLifetime(float flFlameLifetime) = 0;
	virtual void IgniteNumHitboxFires(int iNumHitBoxFires) = 0;
	virtual void IgniteHitboxFireScale(float flHitboxFireScale) = 0;
	virtual void Extinguish() = 0;
	virtual bool IsOnFire() = 0;
	virtual void Scorch(int rate, int floor) = 0;
	virtual void InputIgnite(inputdata_t& inputdata) = 0;
	virtual void InputIgniteLifetime(inputdata_t& inputdata) = 0;
	virtual void InputIgniteNumHitboxFires(inputdata_t& inputdata) = 0;
	virtual void InputIgniteHitboxFireScale(inputdata_t& inputdata) = 0;
	virtual void InputBecomeRagdoll(inputdata_t& inputdata) = 0;

	// Dissolve, returns true if the ragdoll has been created
	virtual bool Dissolve(const char* pMaterialName, float flStartTime, bool bNPCOnly = true, int nDissolveType = 0,
		CVector vDissolverOrigin = CVector(0, 0, 0), int iMagnitude = 0) = 0;
	virtual bool IsDissolving() = 0;
	virtual void TransferDissolveFrom(C_BaseAnimating* pAnim) = 0;


	virtual void SetLightingOriginRelative(C_BaseEntity* pLightingOriginRelative) = 0;
	virtual void SetLightingOriginRelative(string_t strLightingOriginRelative) = 0;
	virtual C_BaseEntity* GetLightingOriginRelative() = 0;

	virtual void SetLightingOrigin(C_BaseEntity* pLightingOrigin) = 0;
	virtual void SetLightingOrigin(string_t strLightingOrigin) = 0;
	virtual C_BaseEntity* GetLightingOrigin() = 0;

	virtual const float* GetPoseParameterArray() = 0;
	virtual const float* GetEncodedControllerArray() = 0;

	virtual void BuildMatricesWithBoneMerge(const CStudioHdr* pStudioHdr, const CVector& angles,
		const CVector& origin, const CVector pos[128],
		const CVector q[128], CMatrix3x4 bonetoworld[128],
		C_BaseAnimating* pParent, CBoneCache* pParentCache) = 0;

	virtual void SetFadeDistance(float minFadeDist, float maxFadeDist) = 0;

	virtual int GetBoneCacheFlags(void) = 0;
	virtual inline void SetBoneCacheFlags(unsigned short fFlag) = 0;
	virtual inline void ClearBoneCacheFlags(unsigned short fFlag) = 0;

	virtual bool PrefetchSequence(int iSequence) = 0;

private:
	virtual void LockStudioHdr() = 0;
	virtual void UnlockStudioHdr() = 0;

	virtual void StudioFrameAdvanceInternal(CStudioHdr* pStudioHdr, float flInterval) = 0;
	virtual void InputSetLightingOriginRelative(inputdata_t& inputdata) = 0;
	virtual void InputSetLightingOrigin(inputdata_t& inputdata) = 0;
	virtual void InputSetModelScale(inputdata_t& inputdata) = 0;

	virtual bool CanSkipAnimation(void) = 0;

public:
	virtual void InitStepHeightAdjust(void) = 0;
	virtual void SetIKGroundContactInfo(float minHeight, float maxHeight) = 0;
	virtual void UpdateStepOrigin(void) = 0;

public:
	virtual CVector GetStepOrigin(void) const = 0;
	virtual CVector GetStepAngles(void) const = 0;
};
#endif
