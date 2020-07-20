#ifndef INCLUDED_PRIMITIVE
#define INCLUDED_PRIMITIVE

#include <d3d11.h>
#include <DirectXMath.h>
#include "vector.h"

namespace mylib
{
    const int PRIMITIVE_VERTEX_NUM = 130;
    const int PRIMITIVE_CIRCLE_SIDE_NUM = 32;


    class Primitive
    {

        ID3D11VertexShader* vertexShader=nullptr;
        ID3D11PixelShader* pixelShader = nullptr;
        ID3D11InputLayout* inputLayout = nullptr;
        ID3D11Buffer* buffer = nullptr;
        ID3D11RasterizerState* rasterizerState = nullptr;
        ID3D11DepthStencilState* depthStencilState = nullptr;

        ID3D11Device* device = nullptr;
    public:
        Primitive(ID3D11Device* device);
        ~Primitive();

        //--------------------------------------------------------------
        //  ãÈå`ï`âÊ
        //--------------------------------------------------------------
        void rect(ID3D11DeviceContext*, const VECTOR2&, const VECTOR2&,
            const VECTOR2&, float, const VECTOR4&) const;

        //--------------------------------------------------------------
        //  ê¸ï`âÊ
        //--------------------------------------------------------------
        void line(ID3D11DeviceContext*, const VECTOR2&, const VECTOR2&,
            const VECTOR4&, float) const;

        //--------------------------------------------------------------
        //  â~ï`âÊ
        //--------------------------------------------------------------
        void circle(ID3D11DeviceContext*, const VECTOR2&, float,
            const VECTOR4&, int) const;

        //--------------------------------------------------------------
        //  éläpÉ|ÉäÉSÉìï`âÊÅií∏ì_éwíËÅj
        //--------------------------------------------------------------
        void quad(ID3D11DeviceContext*, const VECTOR2(&)[4],
            const VECTOR4&) const;

        void draw_triangle_fan(ID3D11DeviceContext*,
            const VECTOR2[], const VECTOR4&, int) const;

        //--------------------------------------------------------------
        //  ç\ë¢ëÃíËã`
        //--------------------------------------------------------------
        struct vertex { VECTOR3 pos; VECTOR4 color; };

    };

//    //==========================================================================
//    //
//    //      PrimitiveBatchÉNÉâÉX
//    //
//    //==========================================================================
//    class PrimitiveBatch
//    {
//    private:
//
//        ID3D11VertexShader* vertexShader;
//        ID3D11PixelShader* pixelShader;
//        ID3D11InputLayout* inputLayout;
//        ID3D11Buffer* buffer;
//        ID3D11RasterizerState* rasterizerState;
//        ID3D11DepthStencilState* depthStencilState;
//
//        size_t MAX_INSTANCES;
//        struct instance
//        {
//            DirectX::XMFLOAT4X4 ndcTransform;
//            VECTOR4 color;
//        };
//        ID3D11Buffer* instanceBuffer;
//
//    public:
//
//        struct vertex
//        {
//            VECTOR3 pos;
//        };
//
//        PrimitiveBatch(ID3D11Device*, size_t maxInstance = (256));
//        ~PrimitiveBatch();
//
//  
//        void begin(ID3D11DeviceContext*);
//
//    
//        void rect(const VECTOR2&, const VECTOR2&,
//            const VECTOR2&, float,
//            const VECTOR4&);
//
//    
//        void line(const VECTOR2&, const VECTOR2&,
//            const VECTOR4&, float width);
//
//    
//        void end(ID3D11DeviceContext*);
//
//    private:
//        D3D11_VIEWPORT viewport;
//
//        UINT instanceCount = 0;
//        instance* instances = nullptr;
//    };
//
}



#endif // !INCLUDED_PRIMITIVE
