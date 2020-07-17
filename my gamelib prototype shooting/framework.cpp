#include "framework.h"
#include <memory>
#include "shader.h"

#include "blender.h"

#include <algorithm>
#include <deque>
#include "imgui.h"
#include "mylib.h"
#include "audio.h"

using namespace mylib;
using namespace input;

bool framework::initialize()
{
	HRESULT hr=S_OK;

	RECT rc;
	GetClientRect(hwnd,&rc);

	UINT screen_width = rc.right - rc.left;
	UINT screen_height = rc.bottom - rc.top;
	// Create Device


		UINT createDeviceFlags = 0;

#ifdef _DEBUG
		createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif
		D3D_DRIVER_TYPE driverTypes[] =
		{
			D3D_DRIVER_TYPE_HARDWARE,
			D3D_DRIVER_TYPE_WARP,
			D3D_DRIVER_TYPE_REFERENCE,
		};
		UINT numDriverTypes = ARRAYSIZE(driverTypes);

		D3D_FEATURE_LEVEL featureLevels[] =
		{
			D3D_FEATURE_LEVEL_11_0,
			D3D_FEATURE_LEVEL_10_1,
			D3D_FEATURE_LEVEL_10_0,
		};
		UINT numFeatureLevels = ARRAYSIZE(featureLevels);

		DXGI_SWAP_CHAIN_DESC sd = {};
		
		sd.BufferCount = 1;
		sd.BufferDesc.Width = screen_width;
		sd.BufferDesc.Height = screen_height;
		sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		sd.BufferDesc.RefreshRate.Numerator = 60;
		sd.BufferDesc.RefreshRate.Denominator = 1;
		sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
		sd.OutputWindow = hwnd;
		sd.SampleDesc.Count = 1;
		sd.SampleDesc.Quality = 0;
		sd.Windowed = TRUE;

		for (UINT driverTypeIndex = 0; driverTypeIndex < numDriverTypes; driverTypeIndex++)
		{
			g_driverType = driverTypes[driverTypeIndex];

			hr= D3D11CreateDeviceAndSwapChain(NULL, g_driverType, NULL, createDeviceFlags, featureLevels, numFeatureLevels,
				D3D11_SDK_VERSION, &sd, swapchain.GetAddressOf(), device.GetAddressOf(), &g_featureLevel, devicecontext.GetAddressOf());

			if (SUCCEEDED(hr))
				break;
		}

		if (FAILED(hr))
			return false;
		// Create Render Target View
	D3D11_TEXTURE2D_DESC back_buffer_desc;
	{
		Microsoft::WRL::ComPtr<ID3D11Texture2D> back_buffer;
		hr = swapchain->GetBuffer(0,
			__uuidof(ID3D11Texture2D), 
			reinterpret_cast<void**>(back_buffer.GetAddressOf()));
		_ASSERT_EXPR(SUCCEEDED(hr), hr_trace(hr));

		hr = device->CreateRenderTargetView(back_buffer.Get(), NULL, rendertargetview.GetAddressOf());
		_ASSERT_EXPR(SUCCEEDED(hr), hr_trace(hr));
	
		back_buffer->GetDesc(&back_buffer_desc);
	}


	// Create Depth Stencil View
	D3D11_TEXTURE2D_DESC depth_stencil_buffer_desc = back_buffer_desc;
	{
		Microsoft::WRL::ComPtr<ID3D11Texture2D> depth_stencil_buffer;
		depth_stencil_buffer_desc.MipLevels = 1;
		depth_stencil_buffer_desc.ArraySize = 1;
		depth_stencil_buffer_desc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
		depth_stencil_buffer_desc.Usage = D3D11_USAGE_DEFAULT;
		depth_stencil_buffer_desc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
		depth_stencil_buffer_desc.CPUAccessFlags = 0;
		depth_stencil_buffer_desc.MiscFlags = 0;
		hr = device->CreateTexture2D(&depth_stencil_buffer_desc, NULL, depth_stencil_buffer.GetAddressOf());
		_ASSERT_EXPR(SUCCEEDED(hr), hr_trace(hr));

		D3D11_DEPTH_STENCIL_VIEW_DESC depth_stencil_view_desc;
		depth_stencil_view_desc.Format = depth_stencil_buffer_desc.Format;
		depth_stencil_view_desc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
		depth_stencil_view_desc.Flags = 0;
		depth_stencil_view_desc.Texture2D.MipSlice = 0;
		hr = device->CreateDepthStencilView(depth_stencil_buffer.Get(), &depth_stencil_view_desc, depthstencilView.GetAddressOf());
		_ASSERT_EXPR(SUCCEEDED(hr), hr_trace(hr));
	}

		/*devicecontext->OMSetRenderTargets(1, rendertargetview.GetAddressOf(), depthstencilView.GetAddressOf());*/

		// Setup the viewport
		D3D11_VIEWPORT vp;
		vp.Width = (FLOAT)screen_width;
		vp.Height = (FLOAT)screen_height;
		vp.MinDepth = 0.0f;
		vp.MaxDepth = 1.0f;
		vp.TopLeftX = 0;
		vp.TopLeftY = 0;
		devicecontext->RSSetViewports(1, &vp);

		UINT m4xMsaaQuality;
		device->CheckMultisampleQualityLevels(DXGI_FORMAT_R8G8B8A8_UNORM, 4, &m4xMsaaQuality);
	/*	sprites[0] = std::make_unique<sprite>(device.Get());*/
	/*	sprites[0] = std::make_unique<sprite>(device.Get(), L"player-sprites.png");*/
		//for (auto& p : sprites)
		//{

		//	p = std::make_unique<sprite>(device.Get(), L"player-sprites.png");
		//}

		//sprites[0] = std::make_unique<sprite>(device.Get(), L"logos.jpg");
		//sprites[1] = std::make_unique<sprite>(device.Get(), L"n64.png");

#ifdef USE_IMGUI
// setup imgui
		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		ImGuiIO& io = ImGui::GetIO();
		//日本語用フォントの設定
		io.Fonts->AddFontFromFileTTF("c:\\Windows\\Fonts\\meiryo.ttc", 13.0f, nullptr, glyphRangesJapanese);
		// setup platform/renderer
		ImGui_ImplWin32_Init(hwnd);
		ImGui_ImplDX11_Init(device.Get(), devicecontext.Get());
		ImGui::StyleColorsDark();
#endif // USE_IMGUI
		

		particle = std::make_unique<sprite>(device.Get(), L"particle-smoke.png");
		font = std::make_unique<sprite>(device.Get(), L"./fonts/font0.png");

		particle_batch = std::make_unique<sprite_batch>(device.Get(), L"particle-smoke.png", 1024);

	return true;
}
void framework::update(float elapsed_time/*Elapsed seconds from last frame*/)
{

}
void framework::render(float elapsed_time/*Elapsed seconds from last frame*/)
{

#ifdef USE_IMGUI
	// imgui new frame
	ImGui_ImplDX11_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();
#endif // USE_IMGUI

	HRESULT hr = S_OK;
	static float angle = 0;
	angle += 6.0f * elapsed_time;

	FLOAT color[] = { 0.2f, 0.2f, 0.2f, 1.0f };
	devicecontext->ClearRenderTargetView(rendertargetview.Get(), color);
	devicecontext->ClearDepthStencilView(depthstencilView.Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
	devicecontext->OMSetRenderTargets(1, rendertargetview.GetAddressOf(), depthstencilView.Get());


	//float ClearColor[4] = { 0.0f, 0.125f, 0.3f, 1.0f }; //red,green,blue,alpha
	//devicecontext->ClearRenderTargetView(rendertargetview, ClearColor);
	//static float angle = 0;
	//angle += 6.0f * elapsed_time; // 1 round per 60 seconds
	/*sprites[0]->render(devicecontext.Get(),DirectX::XMFLOAT2(200,200), DirectX::XMFLOAT2(200, 200), angle, DirectX::XMFLOAT4(1,0,0,1));*/
	/*sprites[0]->render(devicecontext.Get(), DirectX::XMFLOAT2(200, 200), DirectX::XMFLOAT2(200, 200), DirectX::XMFLOAT2(140 * 0, 240 * 0) , DirectX::XMFLOAT2(140, 240), angle, DirectX::XMFLOAT4(1, 0, 0, 1));*/

	/*float x = 0;
	float y = 0;
	for (auto &p : sprites)
	{
		p->render(devicecontext.Get(), DirectX::XMFLOAT2(x, static_cast<int>(y) % 720) , DirectX::XMFLOAT2(64, 64), DirectX::XMFLOAT2(140 * 0, 240 * 0), DirectX::XMFLOAT2(140, 240), 0, DirectX::XMFLOAT4(1, 0, 0, 1));
		x += 32;
		if (x > 1280 - 64)
		{
			x = 0;
			y += 24;
		}
	}*/


	
	/*devicecontext->OMSetBlendState(blender.states[blender::BS_NONE].Get(), nullptr, 0xFFFFFFFF);
	sprites[0]->render(devicecontext.Get(),
		DirectX::XMFLOAT2(0,0),
		DirectX::XMFLOAT2(1280, 720),
		DirectX::XMFLOAT2(0, 0),
		DirectX::XMFLOAT2(1920, 1080) ,
		0,
		DirectX::XMFLOAT4(1, 1, 1, 1));

	devicecontext->OMSetBlendState(blender.states[blender::BS_SUBTRACT].Get(), nullptr, 0xFFFFFFFF);
	
	sprites[1]->render(devicecontext.Get(), 
		DirectX::XMFLOAT2(8, 8), 
		DirectX::XMFLOAT2(1280, 720),
		DirectX::XMFLOAT2(0, 0), 
		DirectX::XMFLOAT2(900, 877),
		0,
		DirectX::XMFLOAT4(1, 1, 1, 1));*/
	static blender blender(device.Get());


	static DirectX::XMFLOAT2 sprite_position[1024] = {}; // screen space
	static float timer = 0; // 0 - 4(sec)
	timer += elapsed_time;
	if (timer > 4.0f) // update positions once in four seconds
	{
		for (auto& p : sprite_position)
		{
			float a = static_cast<float>(rand()) / RAND_MAX * 360.0f; // angle(degree) : 0 - 360 
			float r = static_cast<float>(rand()) / RAND_MAX * 256.0f; // radius(screen space) : 0 - 64
			p.x = cosf(a * 0.01745f) * r;
			p.y = sinf(a * 0.01745f) * r;
		}
		timer = 0;
	}

	static benchmark bm;
	bm.begin();

	devicecontext->OMSetBlendState(blender.states[blender::BS_ADD].Get(), nullptr, 0xFFFFFFFF);

#if 0
	for (auto& p : sprite_position)
	{
		particle->render(devicecontext.Get(),
			DirectX::XMFLOAT2(p.x + 256, p.y + 256),
			DirectX::XMFLOAT2(128, 128),
			DirectX::XMFLOAT2(0, 0),
			DirectX::XMFLOAT2(420, 420),
			DirectX::XMFLOAT2(210, 210),
			DirectX::XMFLOAT2(0.5f, 0.5f),
			angle * 4,
			DirectX::XMFLOAT4(0.2f, 0.05f * timer,
				0.01f * timer, fabsf(sinf(3.141592f * timer * 0.5f * 0.5f))));
	}
#else

	particle_batch->begin(devicecontext.Get());
	for (auto& p : sprite_position)
	{
		particle_batch->render(
			VECTOR2(p.x + 256, p.y + 256),
			VECTOR2(0.5f, 0.5f),
			VECTOR2(0, 0),
			VECTOR2(420, 420),
			VECTOR2(210, 210),
			angle * 4,
			VECTOR4(0.2f, 0.05f * timer,
				0.01f * timer, fabsf(sinf(3.141592f * timer * 0.5f * 0.5f))));
	}
	particle_batch->end(devicecontext.Get());
#endif

#ifdef USE_IMGUI
	//sample code
	static bool f_open = true;
	ImGui::Begin("test", &f_open);
	ImGui::Text("Hello World");
	ImGui::End();
#endif

#ifdef USE_IMGUI
	//imgui render
	ImGui::Render();
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
#endif

	swapchain->Present(0, 0);
}

