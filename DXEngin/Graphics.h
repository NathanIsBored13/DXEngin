#pragma once

#include "Includes.h"
#include "ExtendedException.h"

#include <d3d11.h>

#pragma comment(lib, "d3d11.lib")

class Graphics
{
public:
	Graphics(HWND);
	~Graphics();
	Graphics(const Graphics&) = delete;
	Graphics& operator=(const Graphics&) = delete;
	void EndFrame();
	void ClearBuffer(float, float, float);
private:
	ID3D11Device* device;
	IDXGISwapChain* swap;
	ID3D11DeviceContext* context;
	ID3D11RenderTargetView* target;
};

