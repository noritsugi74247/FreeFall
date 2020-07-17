#pragma once
// UNIT.04
#include <d3d11.h>
#include "sprite.h"
#include "Vector.h"

HRESULT load_texture_from_file(ID3D11Device* device,
    const wchar_t* file_name,
    ID3D11ShaderResourceView** shader_resource_view, 
    D3D11_TEXTURE2D_DESC* texture2d_desc);



namespace mylib
{

    // テクスチャロードデータ
    struct LoadTexture
    {
        int             texNum;         // テクスチャ番号
        const wchar_t* fileName;       // ファイル名
        UINT            maxInstance;    // 最大数（SpriteBatch使用時）
    };

    //==========================================================================
    //
    //      テクスチャクラス
    //
    //==========================================================================
    class Texture
    {
    private:
        sprite_batch* sprBat;    // スプライトバッチ

    public:
        Texture();
        ~Texture();

        bool load(ID3D11Device*, const wchar_t*, size_t);
        void release();

        void begin(ID3D11DeviceContext*);
        void end(ID3D11DeviceContext*);

        void draw(
            float, float, float, float,
            float, float, float, float,
            float, float, float,
            float, float, float, float,
            bool);

        void draw(
            const VECTOR2&, const VECTOR2&,
            const VECTOR2&, const VECTOR2&,
            const VECTOR2&, float,
            const VECTOR4&, bool);

        bool isEmpty();	// テクスチャデータの有無（true : 無, false : 有）
    };

    //==========================================================================
    //
    //		テクスチャマネージャ
    //
    //==========================================================================
    class TextureManager
    {
    private:
        size_t      maxNum;
        Texture* texture;
        UINT        current;

    public:
        TextureManager();
        ~TextureManager();

    public:
        void init(size_t max);
        void uninit();

        void load(ID3D11Device*, int, const wchar_t*, size_t);  // 番号を指定してロード
        int load(ID3D11Device*, const wchar_t*, size_t);        // 空き番号を探してロード（戻り値：テクスチャ番号、-1：ロードエラー）
        void load(ID3D11Device*, const LoadTexture*);           // ロードデータによるロード

        void release(int);
        void release(const LoadTexture*);
        void releaseAll();

        void begin(ID3D11DeviceContext*, int);
        void end(ID3D11DeviceContext*, int);

        void draw(int,
            float, float, float, float,
            float, float, float, float,
            float, float, float,
            float, float, float, float, bool);
        void draw(int,
            const VECTOR2&, const VECTOR2&,
            const VECTOR2&, const VECTOR2&,
            const VECTOR2&, float angle,
            const VECTOR4&, bool);
    };

}