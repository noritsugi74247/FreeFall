#include "primitive2d.h"
#include "shader.h"
#include "misc.h"
#include "util.h"
#include "mylib.h"
namespace mylib 
{
    Primitive::Primitive(ID3D11Device* device) :
        vertexShader(nullptr), pixelShader(nullptr), inputLayout(nullptr), buffer(nullptr), rasterizerState(nullptr)
    {
        //VertexBufferの作成
        vertex vertices[PRIMITIVE_VERTEX_NUM] = { VECTOR3(0, 0, 0), VECTOR4(0, 0, 0, 0) };
        D3D11_BUFFER_DESC bd = {};
        bd.Usage = D3D11_USAGE_DYNAMIC;
        bd.ByteWidth = sizeof(vertices);			// 頂点バッファのサイズ
        bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;	// 頂点バッファ
        bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;	// GPU→読み取りのみ　CPU→書き込みのみ
        bd.MiscFlags = 0;
        bd.StructureByteStride = 0;
        if (FAILED(device->CreateBuffer(&bd, nullptr, &buffer)))
        {
            assert(!"頂点バッファの作成に失敗(Primitive)");
            return;
        }

        // 頂点宣言
        // 入力レイアウトの定義
        D3D11_INPUT_ELEMENT_DESC layout[] = {
            { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT,    0, 0,     D3D11_INPUT_PER_VERTEX_DATA, 0 },
            { "COLOR",    0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 4 * 3, D3D11_INPUT_PER_VERTEX_DATA, 0 },
        };
        UINT numElements = ARRAYSIZE(layout);

        //	頂点シェーダーの読み込み
        create_vs_from_cso(device, "primitive2d_vs.cso", &vertexShader, &inputLayout, layout, ARRAYSIZE(layout));
        create_ps_from_cso(device, "primitive2d_ps.cso", &pixelShader);

        //	ラスタライザステートの設定
        D3D11_RASTERIZER_DESC rsDesc = {};
        rsDesc.FillMode = D3D11_FILL_SOLID;//
        rsDesc.CullMode = D3D11_CULL_NONE;//	カリング
        rsDesc.FrontCounterClockwise = false;
        rsDesc.DepthBias = 0;
        rsDesc.DepthBiasClamp = 0;
        rsDesc.SlopeScaledDepthBias = 0;
        rsDesc.DepthClipEnable = false;
        rsDesc.ScissorEnable = false;
        rsDesc.MultisampleEnable = false;
        rsDesc.AntialiasedLineEnable = false;
        if (FAILED(device->CreateRasterizerState(&rsDesc, &rasterizerState)))
        {
            assert(!"ラスタライザステートの作成に失敗(Primitive)");
            return;
        }

        D3D11_DEPTH_STENCIL_DESC dsDesc;
        dsDesc.DepthEnable = FALSE;
        dsDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO;
        dsDesc.DepthFunc = D3D11_COMPARISON_ALWAYS;
        dsDesc.StencilEnable = FALSE;
        dsDesc.StencilReadMask = 0xFF;
        dsDesc.StencilWriteMask = 0xFF;
        dsDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
        dsDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
        dsDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
        dsDesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
        dsDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
        dsDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR;
        dsDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
        dsDesc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
        if (FAILED(device->CreateDepthStencilState(&dsDesc, &depthStencilState)))
        {
            assert(!"デプスステンシルステートの作成に失敗(Primitive)");
            return;
        }
    }

    //--------------------------------------------------------------
    //  デストラクタ
    //--------------------------------------------------------------
    Primitive::~Primitive()
    {
        safe_release(depthStencilState);
        safe_release(rasterizerState);
        safe_release(buffer);
        safe_release(vertexShader);
        safe_release(pixelShader);
    }

    //--------------------------------------------------------------
    //  四角形の描画
    //--------------------------------------------------------------
    void Primitive::rect(ID3D11DeviceContext* context,
        const VECTOR2& pos, const VECTOR2& size,
        const VECTOR2& center, float angle,
        const VECTOR4& color
    ) const
    {
        if (size.x == 0.0f || size.y == 0.0f) return;

        D3D11_VIEWPORT viewport;
        UINT numViewports = 1;
        context->RSGetViewports(&numViewports, &viewport);

        vertex vertices[] = {
            { VECTOR3(-0.0f, +1.0f, 0), color },
            { VECTOR3(+1.0f, +1.0f, 0), color },
            { VECTOR3(-0.0f, -0.0f, 0), color },
            { VECTOR3(+1.0f, -0.0f, 0), color },
        };

        float sinValue = sinf(angle);
        float cosValue = cosf(angle);
        for (int i = 0; i < 4; i++) {
            vertices[i].pos.x *= size.x;
            vertices[i].pos.y *= size.y;
            vertices[i].pos.x -= center.x;
            vertices[i].pos.y -= center.y;

            float rx = vertices[i].pos.x;
            float ry = vertices[i].pos.y;
            vertices[i].pos.x = rx * cosValue - ry * sinValue;
            vertices[i].pos.y = rx * sinValue + ry * cosValue;
            vertices[i].pos.x += pos.x;
            vertices[i].pos.y += pos.y;

            vertices[i].pos.x = -1.0f + vertices[i].pos.x * 2 / viewport.Width;
#ifdef GAMELIB_PLUS_UP
            vertices[i].pos.y = -1.0f + vertices[i].pos.y * 2 / viewport.Height;
#else
            vertices[i].pos.y = 1.0f - vertices[i].pos.y * 2 / viewport.Height;
#endif
        }

        D3D11_MAPPED_SUBRESOURCE msr;
        context->Map(buffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &msr);
        memcpy(msr.pData, vertices, sizeof(vertices));
        context->Unmap(buffer, 0);

        UINT stride = sizeof(vertex);
        UINT offset = 0;
        context->IASetVertexBuffers(0, 1, &buffer, &stride, &offset);
        context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
        context->IASetInputLayout(inputLayout);
        context->RSSetState(rasterizerState);
        context->VSSetShader(vertexShader, NULL, 0);
        context->PSSetShader(pixelShader, NULL, 0);
        context->OMSetDepthStencilState(depthStencilState, 1);

        context->Draw(4, 0);
    }

    //--------------------------------------------------------------
    //  線の描画
    //--------------------------------------------------------------
    void Primitive::line(ID3D11DeviceContext* context,
        const VECTOR2& from, const VECTOR2& to,
        const VECTOR4& color, float width
    ) const
    {
        if (width <= 0.0f) return;

        VECTOR2 v1(from.x, from.y);
        VECTOR2 v2(to.x, to.y);
        float w = vec2Length(v2 - v1);
        float h = width;
        float cx = w * 0.5f;
        float cy = h * 0.5f;
        float x = (from.x + to.x) * 0.5f;
        float y = (from.y + to.y) * 0.5f;
        float angle = atan2(to.y - from.y, to.x - from.x);

        rect(context, VECTOR2(x, y), VECTOR2(w, h), VECTOR2(cx, cy), angle, color);
    }

    //--------------------------------------------------------------
    //  円の描画
    //--------------------------------------------------------------
    void Primitive::circle(ID3D11DeviceContext* context,
        const VECTOR2& center, float radius,
        const VECTOR4& color, int n
    ) const
    {
        if (n < 3 || radius <= 0.0f) return;
        if (n > 64) n = 64;//最大64角形

        D3D11_VIEWPORT viewport;
        UINT numViewports = 1;
        context->RSGetViewports(&numViewports, &viewport);

        vertex vertices[130] = { VECTOR3(0,0,0) };
        float arc = DirectX::XM_PI * 2 / n;
        vertex* p = &vertices[0];
        for (int i = 0; i <= n; i++)
        {
            p->pos.x = center.x + cosf(arc * i) * radius;
            p->pos.y = center.y + sinf(arc * i) * radius;

            p->pos.x = -1.0f + p->pos.x * 2 / viewport.Width;
#ifdef GAMELIB_PLUS_UP
            p->pos.y = -1.0f + p->pos.y * 2 / viewport.Height;
#else
            p->pos.y = 1.0f - p->pos.y * 2 / viewport.Height;
#endif
            p->color = color;
            p++;

            p->pos.x = center.x;
            p->pos.y = center.y;

            p->pos.x = -1.0f + p->pos.x * 2 / viewport.Width;
#ifdef GAMELIB_PLUS_UP
            p->pos.y = -1.0f + p->pos.y * 2 / viewport.Height;
#else
            p->pos.y = 1.0f - p->pos.y * 2 / viewport.Height;
#endif
            p->color = color;
            p++;
        }

        D3D11_MAPPED_SUBRESOURCE msr;
        context->Map(buffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &msr);
        memcpy(msr.pData, vertices, sizeof(vertex) * (n + 1) * 2);
        context->Unmap(buffer, 0);

        UINT stride = sizeof(vertex);
        UINT offset = 0;
        context->IASetVertexBuffers(0, 1, &buffer, &stride, &offset);
        context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
        context->IASetInputLayout(inputLayout);
        context->RSSetState(rasterizerState);
        context->VSSetShader(vertexShader, NULL, 0);
        context->PSSetShader(pixelShader, NULL, 0);
        context->OMSetDepthStencilState(depthStencilState, 1);

        context->Draw((n + 1) * 2 - 1, 0);
    }

    //--------------------------------------------------------------
    //  四角ポリゴンの描画
    //--------------------------------------------------------------
    void Primitive::quad(ID3D11DeviceContext* context,
        const VECTOR2(&v)[4], const VECTOR4& color) const
    {
        D3D11_VIEWPORT viewport;
        UINT numViewports = 1;
        context->RSGetViewports(&numViewports, &viewport);

        vertex vertices[] = {
            { VECTOR3(v[3].x, v[3].y, 0), color },
            { VECTOR3(v[2].x, v[2].y, 0), color },
            { VECTOR3(v[0].x, v[0].y, 0), color },
            { VECTOR3(v[1].x, v[1].y, 0), color },
        };

        for (int i = 0; i < 4; i++)
        {
            vertices[i].pos.x = -1.0f + vertices[i].pos.x * 2 / viewport.Width;
#ifdef GAMELIB_PLUS_UP
            vertices[i].pos.y = -1.0f + vertices[i].pos.y * 2 / viewport.Height;
#else
            vertices[i].pos.y = 1.0f - vertices[i].pos.y * 2 / viewport.Height;
#endif
        }

        D3D11_MAPPED_SUBRESOURCE msr;
        context->Map(buffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &msr);
        memcpy(msr.pData, vertices, sizeof(vertices));
        context->Unmap(buffer, 0);

        UINT stride = sizeof(vertex);
        UINT offset = 0;
        context->IASetVertexBuffers(0, 1, &buffer, &stride, &offset);
        context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
        context->IASetInputLayout(inputLayout);
        context->RSSetState(rasterizerState);
        context->VSSetShader(vertexShader, NULL, 0);
        context->PSSetShader(pixelShader, NULL, 0);
        context->OMSetDepthStencilState(depthStencilState, 1);

        context->Draw(4, 0);
    }

    void Primitive::draw_triangle_fan(ID3D11DeviceContext* context,
        const VECTOR2 v[], const VECTOR4& color, int nPolygon) const
    {
        D3D11_VIEWPORT viewport;
        UINT numViewports = 1;
        context->RSGetViewports(&numViewports, &viewport);
        vertex vertices[PRIMITIVE_VERTEX_NUM] = {};
        int index = 0;
        for (int i = 0; i < nPolygon; i++)
        {
            vertices[index].pos = v[0];
            vertices[index].pos.x = -1.0f + vertices[index].pos.x * 2 / viewport.Width;
#ifdef GAMELIB_PLUS_UP
            vertices[index].pos.y = -1.0f + vertices[index].pos.y * 2 / viewport.Height;
#else
            vertices[index].pos.y = 1.0f - vertices[index].pos.y * 2 / viewport.Height;
#endif
            vertices[index].color = color;
            index++;

            vertices[index].pos = v[i + 1];
            vertices[index].pos.x = -1.0f + vertices[index].pos.x * 2 / viewport.Width;
#ifdef GAMELIB_PLUS_UP
            vertices[index].pos.y = -1.0f + vertices[index].pos.y * 2 / viewport.Height;
#else
            vertices[index].pos.y = 1.0f - vertices[index].pos.y * 2 / viewport.Height;
#endif
            vertices[index].color = color;
            index++;

            vertices[index].pos = v[i + 2];
            vertices[index].pos.x = -1.0f + vertices[index].pos.x * 2 / viewport.Width;
#ifdef GAMELIB_PLUS_UP
            vertices[index].pos.y = -1.0f + vertices[index].pos.y * 2 / viewport.Height;
#else
            vertices[index].pos.y = 1.0f - vertices[index].pos.y * 2 / viewport.Height;
#endif
            vertices[index].color = color;
            index++;
        }

        D3D11_MAPPED_SUBRESOURCE msr;
        context->Map(buffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &msr);
        memcpy(msr.pData, vertices, sizeof(vertex) * index);
        context->Unmap(buffer, 0);

        UINT stride = sizeof(vertex);
        UINT offset = 0;
        context->IASetVertexBuffers(0, 1, &buffer, &stride, &offset);
        context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
        context->IASetInputLayout(inputLayout);
        context->RSSetState(rasterizerState);
        context->VSSetShader(vertexShader, NULL, 0);
        context->PSSetShader(pixelShader, NULL, 0);
        context->OMSetDepthStencilState(depthStencilState, 1);

        context->Draw(nPolygon * 3, 0);
    }


}

