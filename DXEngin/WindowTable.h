#pragma once

#include "Window.h"

#include <vector>

template <class T>
class WindowTable
{
public:
	WindowTable();
	~WindowTable();
	int operator += (const T*);
	void operator -= (const size_t);
	T* operator [] (const size_t);
private:
	std::vector<T> wnds;
};

