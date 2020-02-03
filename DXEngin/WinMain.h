#pragma once

#include "Windows.h"

#include <sstream>

int CALLBACK WinMain(_In_ HINSTANCE, _In_opt_ HINSTANCE, _In_ LPSTR, _In_ int);
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);