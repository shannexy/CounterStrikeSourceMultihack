#pragma once

#include "../common.hpp"

using namespace std;

namespace console {
	void open();
	void close();

	template<typename T, typename... Args>
	void print(T first, Args ... args) 
	{
		if (cout.good())
		{
			cout << first;
			(cout << ... << args);
			cout << endl;
		}
	}
}