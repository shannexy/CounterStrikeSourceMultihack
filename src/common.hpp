#pragma once

#define _CRT_SECURE_NO_WARNINGS 1
#define WIN32_LEAN_AND_MEAN             // Excluir itens raramente utilizados dos cabeçalhos do Windows
// Arquivos de Cabeçalho do Windows
#ifndef COMMON_H
#define COMMON_H
#include <windows.h>
#include "nlohmann/json.hpp"
#include "minhook/MinHook.h"
#include "imgui/imgui.h"
#include "imgui/imgui_impl_dx9.h"
#include "imgui/imgui_impl_win32.h"
#include "fmt/core.h"
#include "fmt/format.h"

#include <filesystem>
#include <functional>
#include <source_location>
#include <atomic>
#include <condition_variable>
#include <stack>
#include <string>
#include <fstream>
#include <iostream>

#include "core/hooks.hpp"
#include "core/gui.hpp"
#include "core/netvar.hpp"
#include "core/configuration.hpp"
#include "core/file_manager.hpp"
#include "core/thread_pool.hpp"
#include "core/console.hpp"
#endif

using namespace std;