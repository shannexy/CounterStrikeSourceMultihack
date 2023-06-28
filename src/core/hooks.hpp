#pragma once

#include <iostream>
#include "minhook/MinHook.h"
#include "../valve/cusercmd.hpp"
#include "interfaces.hpp"

#include <d3d9.h>

namespace hooks 
{

	using AllocKeyValuesMemoryFn = void* (__thiscall*)(void*, const std::int32_t) noexcept;
	inline AllocKeyValuesMemoryFn AllocKeyValuesMemoryOriginal = nullptr;
	void* __stdcall AllocKeyValuesMemory(const std::int32_t size) noexcept;

	using LockCursorFn = void(__thiscall*)(void*);
	inline LockCursorFn LockCursorOriginal = nullptr;
	void __stdcall LockCursor() noexcept;

	using PaintTraverseFn = void(__thiscall*)(IVPanel*, std::uintptr_t, bool, bool) noexcept;
	inline PaintTraverseFn PaintTraverseOriginal = nullptr;
	void __stdcall PaintTraverse(std::uintptr_t vguiPanel, bool forceRepaint, bool allowForce) noexcept;

	using CreateMoveFn = void(__thiscall*)(void*, int, float, bool);
	inline CreateMoveFn CreateMoveOriginal = nullptr;
	void __stdcall CreateMove(int sequence_number, float input_sample_frametime, bool active);

	using EndSceneFn = long(__thiscall*)(void*, IDirect3DDevice9*) noexcept;
	inline EndSceneFn EndSceneOriginal = nullptr;
	long __stdcall EndScene(IDirect3DDevice9* device) noexcept;

	using ResetFn = HRESULT(__thiscall*)(void*, IDirect3DDevice9*, D3DPRESENT_PARAMETERS*) noexcept;
	inline ResetFn ResetOriginal = nullptr;
	HRESULT __stdcall Reset(IDirect3DDevice9* device, D3DPRESENT_PARAMETERS* params) noexcept;

	using DrawModelFn = void(__thiscall*)(void*, void*, const CDrawModelInfo&, CMatrix3x4*, float*, float*, const CVector&, const std::int32_t) noexcept;
	inline DrawModelFn DrawModelOriginal = nullptr;
	void __stdcall DrawModel(void* results, const CDrawModelInfo& info, CMatrix3x4* bones, float* flexWeights, float* flexDelayedWeights, const CVector& modelOrigin, const std::int32_t flags) noexcept;

	using StartDrawingFn = void(__thiscall*)(void*) noexcept;
	inline StartDrawingFn StartDrawing = nullptr;
	using FinishDrawingFn = void(__thiscall*)(void*) noexcept;
	inline FinishDrawingFn FinishDrawing = nullptr;

	using BeginFrameFn = void(__thiscall*)(void*, float) noexcept;
	inline BeginFrameFn BeginFrameOriginal = nullptr;
	void __stdcall BeginFrame(float frameTime);

	using GetViewModelFovFn = float(__thiscall*)(void*) noexcept;
	inline GetViewModelFovFn GetViewModelOriginal = nullptr;
	float __stdcall GetViewModelFov() noexcept;

	using FrameStageNotifyFn = void(__thiscall*)(void*, ClientFrameStage_t) noexcept;
	inline FrameStageNotifyFn FrameStageNotifyOriginal = nullptr;
	void __stdcall FrameStageNotify(ClientFrameStage_t curState) noexcept;

	void Setup();
	void Destroy();
}