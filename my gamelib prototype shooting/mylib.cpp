#include "mylib.h"
#include <ctime>
#include <crtdbg.h>
#include "misc.h"
#include "blender.h"
#include "assain.h"
#include <algorithm>
#include <deque>
#include "imgui.h"
#include "mylib.h"
#include "audio.h"
#include "viewsetting.h"
#include "texture.h"
#include "util.h"
#include "high_resolution_timer.h"
#include "frame_rate.h"
#include "primitive2d.h"



namespace mylib
{

    // 内部変数用構造体＆実体宣言
    struct Members
    {
      

        // DirectX11関連
        // ウインドウ関連
        HWND hwnd;  // ウインドウハンドル
        D3D_DRIVER_TYPE   g_driverType = D3D_DRIVER_TYPE_NULL;
        D3D_FEATURE_LEVEL  g_featureLevel = D3D_FEATURE_LEVEL_11_0;
        // DirectX11関連
        //ID3D11Device* device;
        //ID3D11DeviceContext* context;
        //IDXGISwapChain* swapChain;
        //ID3D11RenderTargetView* renderTargetView;
        //ID3D11DepthStencilView* depthStencilView;
        //ID3D11BlendState* blendState;

        Microsoft::WRL::ComPtr<ID3D11Device> device;
        Microsoft::WRL::ComPtr<ID3D11DeviceContext> devicecontext;
        Microsoft::WRL::ComPtr<IDXGISwapChain> swapchain;

        Microsoft::WRL::ComPtr<ID3D11RenderTargetView> rendertargetview;
        Microsoft::WRL::ComPtr<ID3D11DepthStencilView> depthstencilView;

        // その他

        blender* Blender;

        	//Debug* debug;
            Primitive* primitive;
            //PrimitiveBatch* primitiveBatch;
            //HighResolutionTimer     hrTimer;
        high_resolution_timer     hrTimer;
        blender::BLEND_STATE    bs;


       TextureManager* textureManager;
        InputManager* inputManager;
        /*FontManager* fontManager;*/
        audio::DXTKAudio* dxtkAudio;
        ViewSettings* viewSettings;
       
    };

    static Members m;

    //--------------------------------------------------------------
   //  ライブラリ初期設定
   //--------------------------------------------------------------
    void init(LPCTSTR caption, int width, int height, bool isFullscreen, double frameRate)
    {
        // 乱数系列の設定
        srand((unsigned int)time(NULL));

#ifdef _DEBUG
        // フルスクリーンの設定（念のためデバッグモードでは不可）
        isFullscreen = false;
#endif

        // ウインドウの初期設定
        m.hwnd = window::init(caption, width, height);

        // DirectX11の初期化
        DirectX11::init(m.hwnd, width, height, isFullscreen);

        // フレームレートの設定
     /*   m.hrTimer.setFrameRate(frameRate);*/

        // ビューの設定
        m.viewSettings = new ViewSettings;
        view::init();
    }

    void uninit()
    {
        // DirectX11の終了処理
        DirectX11::uninit();

        // ウインドウの終了処理
        window::uninit();
    }


    bool gameLoop(bool isShowFrameRate)
    {
        MSG msg;
        while (PeekMessage(&msg, NULL, 0U, 0U, PM_REMOVE))
        {
            if (msg.message == WM_QUIT) return false;
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }

        VECTOR2 lt = view::getLeftTop();
        VECTOR2 ct = view::getCenter();
        VECTOR2 lb = view::getLeftBottom();

        //if (input::TRG(0) & input::PAD_R3)
        //{
        //    view::setScale(1.0f);
        //    view::setCenter(0, 0);
        //}

        //フレームレート用
        while (!m.hrTimer.tick());  
        if (isShowFrameRate)
        {
            calculateFrameStats(m.hwnd, &m.hrTimer);//フレームレート計算・タイトルバーに表示
        }

        return true;
    }

    void clear(const VECTOR4& color)
    {
        m.devicecontext->ClearRenderTargetView(m.rendertargetview.Get(), (const float*)&color);
        m.devicecontext->ClearDepthStencilView(m.depthstencilView.Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1, 0);
    }

    HRESULT present(UINT SyncInterval, UINT Flags)
    {
        return m.swapchain->Present(SyncInterval, Flags);
    }

    blender::BLEND_STATE getBlendMode()
    {
        return m.bs;
    }

    void setBlendMode(blender::BLEND_STATE blend)
    {
        m.devicecontext->OMSetBlendState(m.Blender->states[blend].Get(), nullptr, 0xFFFFFFFF);
        m.bs = blend;
    }

    void sprite_load(sprite** ppSpr, const wchar_t* fileName, int nBufSize)
    {
        *ppSpr = new sprite(m.device.Get(), fileName, nBufSize);
    }

  

    void sprite_render(sprite* pSpr,
        float x, float y, float sx, float sy,
        float tx, float ty, float tw, float th,
        float cx, float cy, float angle,
        float r, float g, float b, float a,
        bool world)
    {
        if (pSpr)
        {
            if (world)
            {
                float scaleFactor = view::getScale();
                VECTOR2 s = VECTOR2(sx, sy) * scaleFactor;
                VECTOR2 pos = view::worldToScreen(VECTOR2(x, y));
                pSpr->render(m.devicecontext.Get(), pos , s , VECTOR2(tx, ty), VECTOR2(tw, th), VECTOR2(cx, cy), angle, VECTOR4(r, g, b, a));
            }
            else
            {
                pSpr->render(m.devicecontext.Get(), VECTOR2(x, y), VECTOR2(sx, sy), VECTOR2(tx, ty), VECTOR2(tw, th), VECTOR2(cx, cy), angle, VECTOR4(r, g, b, a));
            }
        }
    }
    
    namespace DirectX11
    {
        HRESULT init(HWND hwnd, int width, int height, bool isFullscreen)
        {

            HRESULT hr = S_OK;

            RECT rc;
            GetClientRect(hwnd, &rc);

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
                m.g_driverType = driverTypes[driverTypeIndex];

                hr = D3D11CreateDeviceAndSwapChain(NULL, m.g_driverType, NULL, createDeviceFlags, featureLevels, numFeatureLevels,
                    D3D11_SDK_VERSION, &sd, m.swapchain.GetAddressOf(), m.device.GetAddressOf(), &m.g_featureLevel, m.devicecontext.GetAddressOf());

                if (SUCCEEDED(hr))
                    break;
            }

            if (FAILED(hr))
                return false;
            // Create Render Target View
            D3D11_TEXTURE2D_DESC back_buffer_desc;
            {
                Microsoft::WRL::ComPtr<ID3D11Texture2D> back_buffer;
                hr = m.swapchain->GetBuffer(0,
                    __uuidof(ID3D11Texture2D),
                    reinterpret_cast<void**>(back_buffer.GetAddressOf()));
                _ASSERT_EXPR(SUCCEEDED(hr), hr_trace(hr));

                hr = m.device->CreateRenderTargetView(back_buffer.Get(), NULL, m.rendertargetview.GetAddressOf());
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
                hr = m.device->CreateTexture2D(&depth_stencil_buffer_desc, NULL, depth_stencil_buffer.GetAddressOf());
                _ASSERT_EXPR(SUCCEEDED(hr), hr_trace(hr));

                D3D11_DEPTH_STENCIL_VIEW_DESC depth_stencil_view_desc;
                depth_stencil_view_desc.Format = depth_stencil_buffer_desc.Format;
                depth_stencil_view_desc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
                depth_stencil_view_desc.Flags = 0;
                depth_stencil_view_desc.Texture2D.MipSlice = 0;
                hr = m.device->CreateDepthStencilView(depth_stencil_buffer.Get(), &depth_stencil_view_desc, m.depthstencilView.GetAddressOf());
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
            m.devicecontext->RSSetViewports(1, &vp);
            m.devicecontext->OMSetRenderTargets(1, m.rendertargetview.GetAddressOf(), m.depthstencilView.Get());
            UINT m4xMsaaQuality;
            m.device->CheckMultisampleQualityLevels(DXGI_FORMAT_R8G8B8A8_UNORM, 4, &m4xMsaaQuality);



            m.Blender = new blender(m.device.Get());
            m.primitive = new Primitive(m.device.Get());
            m.textureManager = new TextureManager;

            m.inputManager = new InputManager;

            m.dxtkAudio = new audio::DXTKAudio;
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
            ImGui_ImplDX11_Init(m.device.Get(), m.devicecontext.Get());
            ImGui::StyleColorsDark();
#endif // USE_IMGUI



            return true;
        }

        void uninit()
        {
            m.swapchain->SetFullscreenState(false, nullptr);

            safe_delete(m.viewSettings);
            safe_delete(m.dxtkAudio);

            safe_delete(m.inputManager);
            safe_delete(m.textureManager);
        }
    }

    static const LPCWSTR CLASS_NAME = L"mygp";

    //------< プロトタイプ宣言 >-------------------------------------------------
    LRESULT CALLBACK fnWndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

    namespace window
    {

        HWND init(LPCTSTR caption, int width, int height)
        {
#if defined(DEBUG) | defined(_DEBUG)
            _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif
            WNDCLASSEX wcex;
            wcex.cbSize = sizeof(WNDCLASSEX);
            wcex.style = CS_HREDRAW | CS_VREDRAW;
            wcex.lpfnWndProc = fnWndProc;
            wcex.cbClsExtra = 0;
            wcex.cbWndExtra = 0;
            wcex.hInstance = GetModuleHandle(NULL);
            wcex.hIcon = NULL;
            wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
            wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
            wcex.lpszMenuName = NULL;
            wcex.lpszClassName = CLASS_NAME;
            wcex.hIconSm = 0;
            RegisterClassEx(&wcex);

            RECT rc = { 0, 0, width, height };
            AdjustWindowRect(&rc, WS_OVERLAPPEDWINDOW, FALSE);
            HWND hwnd = CreateWindow(CLASS_NAME, caption, WS_OVERLAPPEDWINDOW ^ WS_MAXIMIZEBOX ^ WS_THICKFRAME | WS_VISIBLE, CW_USEDEFAULT, CW_USEDEFAULT, rc.right - rc.left, rc.bottom - rc.top, NULL, NULL, GetModuleHandle(NULL), NULL);
            ShowWindow(hwnd, SW_SHOWDEFAULT);

            return hwnd;
        }

        //--------------------------------------------------------------
        //  ウインドウの終了処理
        //--------------------------------------------------------------
        void uninit()
        {
            // 終了処理
            UnregisterClass(CLASS_NAME, GetModuleHandle(NULL));
        }
    }
        //--------------------------------------------------------------
        //  ウインドウプロシージャ
        //--------------------------------------------------------------
        LRESULT CALLBACK fnWndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
        {
            using DirectX::Keyboard;

            // マウスホイール用
            static int nWheelFraction = 0;	// 回転量の端数

            // マウス移動用
            static POINT prevPos;

            switch (msg)
            {
            case WM_PAINT:
            {
                PAINTSTRUCT ps;
                HDC hdc;
                hdc = BeginPaint(hwnd, &ps);
                EndPaint(hwnd, &ps);
                break;
            }
            case WM_DESTROY:
                PostQuitMessage(0);
                break;
            case WM_ACTIVATEAPP:
                Keyboard::ProcessMessage(msg, wParam, lParam);
                nWheelFraction = 0;
                break;
            case WM_KEYDOWN:
                if (wParam == VK_ESCAPE)
                {
                    PostMessage(hwnd, WM_CLOSE, 0, 0);
                    break;
                }
            case WM_SYSKEYDOWN:
            case WM_KEYUP:
            case WM_SYSKEYUP:
                Keyboard::ProcessMessage(msg, wParam, lParam);
                break;
            case WM_ENTERSIZEMOVE:
                // WM_EXITSIZEMOVE is sent when the user grabs the resize bars.
               /* m.hrTimer.stop();*/
                break;
            case WM_EXITSIZEMOVE:
                // WM_EXITSIZEMOVE is sent when the user releases the resize bars.
                // Here we reset everything based on the new window dimensions.
              /*  m.hrTimer.start();*/
                break;
            default:
                return DefWindowProc(hwnd, msg, wParam, lParam);
            }

            return 0;
        }
    
        namespace texture
        {

            //--------------------------------------------------------------
            //  テクスチャマネージャ初期化
            //--------------------------------------------------------------
            void init(size_t max)
            {
                m.textureManager->init(max);
            }

            //--------------------------------------------------------------
            //  テクスチャマネージャ終了処理
            //--------------------------------------------------------------
            void uninit()
            {
                m.textureManager->uninit();
            }

            //--------------------------------------------------------------
            //  テクスチャのロード（番号を指定してロード）
            //--------------------------------------------------------------
            void load(int texNo, const wchar_t* fileName, size_t maxInstance)
            {
                m.textureManager->load(m.device.Get(), texNo, fileName, maxInstance);
            }

            //--------------------------------------------------------------
            //  テクスチャのロード（空き番号を探してロード）
            //--------------------------------------------------------------
            int load(const wchar_t* fileName, size_t maxInstance)
            {
                return m.textureManager->load(m.device.Get(), fileName, maxInstance);
            }

            //--------------------------------------------------------------
            //  テクスチャのロード（ロードテクスチャのデータを使ってロード）
            //--------------------------------------------------------------
            void load(const LoadTexture* data)
            {
                m.textureManager->load(m.device.Get(), data);
            }

            //--------------------------------------------------------------
            //  テクスチャの解放（番号を指定して解放）
            //--------------------------------------------------------------
            void release(int texNo)
            {
                m.textureManager->release(texNo);
            }

            //--------------------------------------------------------------
            //  テクスチャの解放（ロードテクスチャのデータを使って解放）
            //--------------------------------------------------------------
            void release(const LoadTexture* data)
            {
                m.textureManager->release(data);
            }

            //--------------------------------------------------------------
            //  テクスチャの解放（全て）
            //--------------------------------------------------------------
            void releaseAll()
            {
                m.textureManager->releaseAll();
            }

            //--------------------------------------------------------------
            //  描画の前処理
            //--------------------------------------------------------------
            void begin(int texNo)
            {
                m.textureManager->begin(m.devicecontext.Get(), texNo);
            }

            //--------------------------------------------------------------
            //  描画の後処理
            //--------------------------------------------------------------
            void end(int texNo)
            {
                m.textureManager->end(m.devicecontext.Get(), texNo);
            }

            //--------------------------------------------------------------
            //  テクスチャの描画
            //--------------------------------------------------------------
            void draw(int texNo,
                const VECTOR2& position, const VECTOR2& scale,
                const VECTOR2& texPos, const VECTOR2& texSize,
                const VECTOR2& center, float angle,
                const VECTOR4& color,
                bool world)
            {
                m.textureManager->draw(texNo,
                    position, scale,
                    texPos, texSize,
                    center, angle,
                    color,
                    world);
            }

            //--------------------------------------------------------------
            //  テクスチャの描画
            //--------------------------------------------------------------
            void draw(int texNo,
                float x, float y, float scaleX, float scaleY,
                float left, float top, float width, float height,
                float centerX, float centerY, float angle,
                float r, float g, float b, float a,
                bool world)
            {
                draw(texNo,
                    VECTOR2(x, y), VECTOR2(scaleX, scaleY),
                    VECTOR2(left, top), VECTOR2(width, height),
                    VECTOR2(centerX, centerY), angle,
                    VECTOR4(r, g, b, a),
                    world);
            }

        }

        namespace primitive
        {

            //--------------------------------------------------------------
            //  矩形描画
            //--------------------------------------------------------------
            void rect(
                float x, float y, float w, float h,
                float cx, float cy, float angle,
                float r, float g, float b, float a,
                bool world)
            {
                rect(VECTOR2(x, y), VECTOR2(w, h), VECTOR2(cx, cy), angle, VECTOR4(r, g, b, a), world);
            }

            //--------------------------------------------------------------
            //  矩形描画
            //--------------------------------------------------------------
            void rect(
                const VECTOR2& pos, const VECTOR2& size,
                const VECTOR2& center, float angle,
                const VECTOR4& color,
                bool world)
            {
                if (world)
                {
                    float scaleFactor = view::getScale();
                    VECTOR2 s = size * scaleFactor;
                    VECTOR2 c = center * scaleFactor;
                    VECTOR2 pos = view::worldToScreen(pos);
                    m.primitive->rect(m.devicecontext.Get(), pos, s, c, angle, color);
                }
                else
                {
                    m.primitive->rect(m.devicecontext.Get(), pos, size, center, angle, color);
                }
            }

            //--------------------------------------------------------------
            //  線描画
            //--------------------------------------------------------------
            void line(
                const VECTOR2& from, const VECTOR2& to,
                const VECTOR4& color, float width,
                bool world)
            {
                if (world)
                {
                    float scaleFactor = view::getScale();
                    float w = (std::max)(width * scaleFactor, 1.0f);
                    VECTOR2 v1 = view::worldToScreen(from);
                    VECTOR2 v2 = view::worldToScreen(to);
                    m.primitive->line(m.devicecontext.Get(), v1, v2, color, w);
                }
                else
                {
                    m.primitive->line(m.devicecontext.Get(), from, to, color, width);
                }
            }

            //--------------------------------------------------------------
            //  線描画
            //--------------------------------------------------------------
            void line(
                float x1, float y1, float x2, float y2,
                float r, float g, float b, float a,
                float width,
                bool world)
            {
                line(VECTOR2(x1, y1), VECTOR2(x2, y2), VECTOR4(r, g, b, a), width, world);
            }

            //--------------------------------------------------------------
            //  円描画
            //--------------------------------------------------------------
            void circle(
                const VECTOR2& pos, float radius,
                const VECTOR4& color,
                int n,
                bool world)
            {
                if (world)
                {
                    float scaleFactor = view::getScale();
                    VECTOR2 v1 = view::worldToScreen(pos);
                    m.primitive->circle(m.devicecontext.Get(), v1, radius * scaleFactor, color, n);
                }
                else
                {
                    m.primitive->circle(m.devicecontext.Get(), pos, radius, color, n);
                }
            }

            //--------------------------------------------------------------
            //  円描画
            //--------------------------------------------------------------
            void circle(
                float x, float y, float radius,
                float r, float g, float b, float a,
                int n,
                bool world)
            {
                circle(VECTOR2(x, y), radius, VECTOR4(r, g, b, a), n, world);
            }

            //--------------------------------------------------------------
            //  四角ポリゴン描画（頂点指定）（旧バージョン）
            //--------------------------------------------------------------
            void quad(const VECTOR2(&v)[4], const VECTOR4& color,
                bool world)
            {
                if (world)
                {
                    VECTOR2 q[4] = {};
                    for (int i = 0; i < 4; i++) q[i] = view::worldToScreen(v[i]);
                    m.primitive->quad(m.devicecontext.Get(), q, color);
                }
                else
                {
                    m.primitive->quad(m.devicecontext.Get(), v, color);
                }
            }

            //--------------------------------------------------------------
            //  四角ポリゴン描画（頂点指定）（旧バージョン）
            //--------------------------------------------------------------
            void quad(const VECTOR2(&v)[4], float r, float g, float b, float a,
                bool world)
            {
                quad(v, VECTOR4(r, g, b, a), world);
            }

            void draw_triangle_fan(VECTOR2 vertex[], const VECTOR4& color, int nPolygon, bool world)
            {
                if (world)
                {
                    for (int i = 0; i < nPolygon + 2; i++) vertex[i] = view::worldToScreen(vertex[i]);
                    m.primitive->draw_triangle_fan(m.devicecontext.Get(), vertex, color, nPolygon);
                }
                else
                {
                    m.primitive->draw_triangle_fan(m.devicecontext.Get(), vertex, color, nPolygon);
                }
            }

        }


    namespace music
    {

        //--------------------------------------------------------------
        //  音楽のロード
        //--------------------------------------------------------------
        void load(int trackNo, const wchar_t* waveFileName, float volume)
        {
            m.dxtkAudio->musicLoad(trackNo, waveFileName, volume);
        }

        //--------------------------------------------------------------
        //  音楽の解放
        //--------------------------------------------------------------
        void unload(int trackNo)
        {
            m.dxtkAudio->musicUnload(trackNo);
        }

        //--------------------------------------------------------------
        //  音楽の再生
        //--------------------------------------------------------------
        void play(int trackNo, bool isLoop)
        {
            m.dxtkAudio->musicPlay(trackNo, isLoop);
        }

        //--------------------------------------------------------------
        //  音楽の停止
        //--------------------------------------------------------------
        void stop(int trackNo)
        {
            m.dxtkAudio->musicStop(trackNo);
        }

        //--------------------------------------------------------------
        //  音楽の一時停止
        //--------------------------------------------------------------
        void pause(int trackNo)
        {
            m.dxtkAudio->musicPause(trackNo);
        }

        //--------------------------------------------------------------
        //  音楽の再開(pauseからの再開)
        //--------------------------------------------------------------
        void resume(int trackNo)
        {
            m.dxtkAudio->musicResume(trackNo);
        }

        //--------------------------------------------------------------
        //  音楽のボリューム設定
        //--------------------------------------------------------------
        void setVolume(int trackNo, float volume)
        {
            m.dxtkAudio->musicSetVolume(trackNo, volume);
        }

        //--------------------------------------------------------------
        //  音楽のクリア
        //--------------------------------------------------------------
        void clear()
        {
            for (int i = 0; i < MUSIC_FILE_MAX; i++)
            {
                stop(i);     // 音楽を止める
                unload(i);   // 音楽を解放する
            }
        }

        //--------------------------------------------------------------
        //  SoundStateの取得
        //--------------------------------------------------------------
        DirectX::SoundState getState(int trackNo)
        {
            return m.dxtkAudio->musicGetState(trackNo);
        }

        //--------------------------------------------------------------
        //  ループの有無の取得
        //--------------------------------------------------------------
        bool isLooped(int trackNo)
        {
            return m.dxtkAudio->musicIsLooped(trackNo);
        }

        //--------------------------------------------------------------
        //  panの設定
        //--------------------------------------------------------------
        void setPan(int trackNo, float pan)
        {
            m.dxtkAudio->musicSetPan(trackNo, pan);
        }

        //--------------------------------------------------------------
        //  pitchの設定
        //--------------------------------------------------------------
        void setPitch(int trackNo, float pitch)
        {
            m.dxtkAudio->musicSetPitch(trackNo, pitch);
        }

        //--------------------------------------------------------------
        //  フォーマットの取得
        //--------------------------------------------------------------
        const WAVEFORMATEX* getFormat(int trackNo)
        {
            return m.dxtkAudio->musicGetFormat(trackNo);
        }

        //--------------------------------------------------------------
        //  音楽が使用中かどうか
        //--------------------------------------------------------------
        bool isInUse(int trackNo)
        {
            return m.dxtkAudio->musicIsInUse(trackNo);
        }
    }

    //==========================================================================
    //
    //      sound
    //
    //==========================================================================

    namespace sound
    {

        //--------------------------------------------------------------
        //  効果音ファイルのロード
        //--------------------------------------------------------------
        void load(const wchar_t* xwbFileName, float volume)
        {
            m.dxtkAudio->soundLoad(xwbFileName, volume);
        }

        //--------------------------------------------------------------
        //  効果音の再生
        //--------------------------------------------------------------
        void play(int trackNo)
        {
            m.dxtkAudio->soundPlay(trackNo);
        }

        //--------------------------------------------------------------
        //  効果音のボリューム設定
        //--------------------------------------------------------------
        void setVolume(int trackNo, float volume)
        {
            m.dxtkAudio->soundSetVolume(trackNo, volume);
        }
    }

    namespace input
    {

        //--------------------------------------------------------------
        //  入力マネージャの初期化
        //--------------------------------------------------------------
        void init()
        {
            m.inputManager->init();
        }

        //--------------------------------------------------------------
        //  入力マネージャの更新
        //--------------------------------------------------------------
        void update()
        {
            m.inputManager->update(m.hwnd);
        }

        //--------------------------------------------------------------
        //  トリガー（押した瞬間）情報の取得
        //--------------------------------------------------------------
        int TRG(int player)
        {
            return m.inputManager->getPadAddress()[player].trigger;
        }

        //--------------------------------------------------------------
        //  トリガー（離した瞬間）情報の取得
        //--------------------------------------------------------------
        int TRG_RELEASE(int player)
        {
            return m.inputManager->getPadAddress()[player].triggerUp;
        }

        //--------------------------------------------------------------
        //  ステート（状態）情報の取得
        //--------------------------------------------------------------
        int STATE(int player)
        {
            return m.inputManager->getPadAddress()[player].state;
        }

        //--------------------------------------------------------------
        //  リピート（繰返し）情報の取得
        //--------------------------------------------------------------
        int REPEAT(int player)
        {
            return m.inputManager->getPadAddress()[player].repeat;
        }

        //--------------------------------------------------------------
        //  マウスカーソルのx座標を取得
        //--------------------------------------------------------------
        int getCursorPosX()
        {
            return m.inputManager->getCursorPosX();
        }

        //--------------------------------------------------------------
        //  マウスカーソルのy座標を取得
        //--------------------------------------------------------------
        int getCursorPosY()
        {
            return m.inputManager->getCursorPosY();
        }

        //--------------------------------------------------------------
        //  コントローラの状態を取得
        //--------------------------------------------------------------
        PadState* getPadState(int player)
        {
            return &m.inputManager->getPadAddress()[player];
        }

    }


    namespace view
    {

        //--------------------------------------------------------------
        //  ViewSettingsの初期化
        //--------------------------------------------------------------
        void init()
        {
            m.viewSettings->initialize();
        }

        //--------------------------------------------------------------
        //  スケールを設定
        //--------------------------------------------------------------
        void setScale(float scale)
        {
            m.viewSettings->setScaleFactor(scale);
        }

        //--------------------------------------------------------------
        //  スケールを取得
        //--------------------------------------------------------------
        float getScale()
        {
            return m.viewSettings->getScaleFactor();
        }

        //--------------------------------------------------------------
        //  中央のワールド座標を設定
        //--------------------------------------------------------------
        void setCenter(float cx, float cy)
        {
            m.viewSettings->setViewCenter(VECTOR2(cx, cy));
        }

        void setCenter(VECTOR2 v)
        {
            m.viewSettings->setViewCenter(v);
        }

        //--------------------------------------------------------------
        //  画面左上のワールド座標を設定
        //--------------------------------------------------------------
        void setLeftTop(float ltx, float lty)
        {
            m.viewSettings->setViewLeftTop(VECTOR2(ltx, lty));
        }

        void setLeftTop(VECTOR2 v)
        {
            m.viewSettings->setViewLeftTop(v);
        }

        //--------------------------------------------------------------
        //  画面左下のワールド座標を設定
        //--------------------------------------------------------------
        void setLeftBottom(float lbx, float lby)
        {
            m.viewSettings->setViewLeftBottom(VECTOR2(lbx, lby));
        }

        void setLeftBottom(VECTOR2 v)
        {
            m.viewSettings->setViewLeftBottom(v);
        }

        //--------------------------------------------------------------
        //  画面中央のワールド座標を取得
        //--------------------------------------------------------------
        VECTOR2 getCenter()
        {
            return m.viewSettings->getViewCenter();
        }

        //--------------------------------------------------------------
        //  画面左上のワールド座標を取得
        //--------------------------------------------------------------
        VECTOR2 getLeftTop()
        {
            return m.viewSettings->getViewLeftTop();
        }

        //--------------------------------------------------------------
        //  画面左下のワールド座標を取得
        //--------------------------------------------------------------
        VECTOR2 getLeftBottom()
        {
            return m.viewSettings->getViewLeftBottom();
        }

        //--------------------------------------------------------------
        //  画面の任意の座標にワールド座標を設定
        //--------------------------------------------------------------
        void setByPosition(VECTOR2 screen, VECTOR2 world)
        {
            m.viewSettings->setViewByPosition(screen, world);
        }

        //--------------------------------------------------------------
        //  Box2Dの座標をワールド座標に変換
        //--------------------------------------------------------------
        //VECTOR2 box2dToWorld(b2Vec2 b2Pos)
        //{
        //    return m.viewSettings->box2dToWorld(b2Pos);
        //}

        //--------------------------------------------------------------
        //  ワールド座標をスクリーン座標へ変換
        //--------------------------------------------------------------
        VECTOR2 worldToScreen(VECTOR2 wPos)
        {
            return m.viewSettings->worldToScreen(wPos);
        }

        VECTOR3 worldToScreen(VECTOR3 wPos)
        {
            return m.viewSettings->worldToScreen(wPos);
        }

        //--------------------------------------------------------------
        //  スクリーン座標をワールド座標に変換
        //--------------------------------------------------------------
        VECTOR2 screenToWorld(VECTOR2 sPos)
        {
            return m.viewSettings->screenToWorld(sPos);
        }

        //--------------------------------------------------------------
        //  ワールド座標をBox2Dの座標に変換
        //--------------------------------------------------------------
        //b2Vec2 worldToBox2d(VECTOR2 wPos)
        //{
        //    return m.viewSettings->worldToBox2d(wPos);
        //}

    }

}