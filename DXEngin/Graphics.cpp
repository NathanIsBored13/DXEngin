#include "Graphics.h"

Graphics::Graphics(HWND hWnd) : device(nullptr), swap(nullptr), context(nullptr), target(nullptr)
{
	DXGI_SWAP_CHAIN_DESC sd =
	{
		.BufferDesc = DXGI_MODE_DESC
		{
			.Width = 0,
			.Height = 0,
			.RefreshRate = DXGI_RATIONAL
			{
				.Numerator = 0,
				.Denominator = 0
			},
			.Format = DXGI_FORMAT_B8G8R8A8_UNORM,
			.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED,
			.Scaling = DXGI_MODE_SCALING_UNSPECIFIED
		},
		.SampleDesc = DXGI_SAMPLE_DESC
		{
			.Count = 1,
			.Quality = 0
		},
		.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT,
		.BufferCount = 1,
		.OutputWindow = hWnd,
		.Windowed = true,
		.SwapEffect = DXGI_SWAP_EFFECT_DISCARD,
		.Flags = 0
	};
	D3D11CreateDeviceAndSwapChain(
		nullptr,
		D3D_DRIVER_TYPE_HARDWARE,
		nullptr,
		0,
		nullptr,
		0,
		D3D11_SDK_VERSION,
		&sd,
		&swap,
		&device,
		nullptr,
		&context
	);
	ID3D11Resource* BBuffer = nullptr;
	swap->GetBuffer(0, __uuidof(ID3D11Resource), reinterpret_cast<void**>(&BBuffer));
	if (BBuffer != nullptr)
	{
		device->CreateRenderTargetView(BBuffer, nullptr, &target);
		BBuffer->Release();
	}
}

Graphics::~Graphics()
{
	if (target != nullptr)
	{
		target->Release();
	}
	if (context != nullptr)
	{
		context->Release();
	}
	if (swap != nullptr)
	{
		swap->Release();
	}
	if (device != nullptr)
	{
		device->Release();
	}
}

void Graphics::EndFrame()
{
	swap->Present(1u, 0u);
}

void Graphics::ClearBuffer(float red, float green, float blue)
{
	context->ClearRenderTargetView(target, new float[4]{ red, green, blue, 1 });
}