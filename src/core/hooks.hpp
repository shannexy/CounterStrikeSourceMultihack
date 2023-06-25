#pragma once

#include <iostream>
#include "minhook/MinHook.h"
#include "../valve/cusercmd.hpp"
#include "interfaces.hpp"

#include <d3d9.h>

namespace hooks 
{
	using CreateMoveFn = void(__thiscall*)(void*, int, float, bool);
	inline CreateMoveFn CreateMoveOriginal = nullptr;
	void __stdcall CreateMove(int sequence_number, float input_sample_frametime, bool active);
	
	using LockCursorFn = void(__thiscall*)(void*);
	inline LockCursorFn LockCursorOriginal = nullptr;
	void __stdcall LockCursor() noexcept;


	using PaintTraverseFn = void(__thiscall*)(IVPanel*, std::uintptr_t, bool, bool) noexcept;
	inline PaintTraverseFn PaintTraverseOriginal = nullptr;
	void __stdcall PaintTraverse(std::uintptr_t vguiPanel, bool forceRepaint, bool allowForce) noexcept;


	using EndSceneFn = long(__thiscall*)(void*, IDirect3DDevice9*) noexcept;
	inline EndSceneFn EndSceneOriginal = nullptr;
	long __stdcall EndScene(IDirect3DDevice9* device) noexcept;

	using ResetFn = HRESULT(__thiscall*)(void*, IDirect3DDevice9*, D3DPRESENT_PARAMETERS*) noexcept;
	inline ResetFn ResetOriginal = nullptr;
	HRESULT __stdcall Reset(IDirect3DDevice9* device, D3DPRESENT_PARAMETERS* params) noexcept;

	void Setup();
	void Destroy();
}