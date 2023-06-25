#pragma once

#include "../utils/memory.hpp"
#include "../utils/datatable.hpp"

class IBaseClientDLL
{
public:
	constexpr ClientClass* GetAllClasses() noexcept
	{
		return memory::Call<ClientClass*>(this, 8);
	}
};