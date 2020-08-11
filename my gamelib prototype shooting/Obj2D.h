#ifndef INCLUDED_OBJ2D
#define	INCLUDED_OBJ2D
//******************************************************************************
//
//
//      OBJ2Dクラス
//
//
//******************************************************************************

//------< インクルード >---------------------------------------------------------
#include <list>
#include "Vector.h"
#include "obj2d_data.h"

using namespace mylib;
// 前方宣言
class OBJ2D;

//==============================================================================

// 移動アルゴリズムクラス（抽象クラス）
class MoveAlg
{
public:
    virtual void move(OBJ2D* obj) = 0;  // 純粋仮想関数を持つので、MoveAlg型のオブジェクトは宣言できない（派生させて使う）
};

// 消去アルゴリズムクラス（抽象クラス）
class EraseAlg
{
public:
    virtual void erase(OBJ2D* obj) = 0; // 上と同様
};

//==============================================================================

//==============================================================================
//
//      OBJ2Dクラス
//
//==============================================================================

class OBJ2D
{
public:
    // メンバ変数
    mylib::SpriteData* data;               // スプライトデータ
    mylib::Anime          anime;              // アニメーションクラス
    mylib::AnimeData* animeData;          // アニメーションデータ

    bool                    onGround;           // 地面フラグ（地上にいる）
    bool                    xFlip;              // x方向反転フラグ
    bool                    kabeFlag;           // 壁フラグ（一歩先が壁）
    bool                    gakeFlag;           // 崖フラグ（一歩先が崖）

    VECTOR2                 position;           // 位置
    VECTOR2                 scale;              // スケール
    float                   angle;              // 角度
    VECTOR4                 color;              // 描画色
    VECTOR2                 size;               // あたり用サイズ（縦横）

    mylib::fRECT          hitRect = {};           // あたり判定RECT
    MoveAlg* mvAlg;              // 移動アルゴリズム
    EraseAlg* eraseAlg;           // 消去アルゴリズム

    float                   velocity;           // 速度
    VECTOR2                 speed;              // 瞬間の移動量ベクトル
    int                     state;              // ステート
    int                     timer;              // タイマー
    int               damageCount;              // ダメージカウント

    int                     score;              // スコア
    int                     param;              // 汎用パラメータ
    int                     jumpTimer;          // 長押しジャンプタイマー
    int                     flags;              // 各種フラグ用(使い方はplayer.hのplayerクラスを参照)
    int                     iWork[16];          // 汎用（使い方は自由）
    float                   fWork[16];          // 汎用（使い方は自由）

    static int              damage;

    bool                    hashigoFlag;        // はしごフラグ
    bool                    drawHitRectFlag;    // あたり判定描画フラグ
    bool                    judgeFlag;          // あたり判定の有無（true:有り / false:無し）
    BYTE                    pad[1];             // 1バイトの空き
public:

    OBJ2D();        // コンストラクタ
    void clear();   // メンバ変数のクリア
    void move();    // 移動
    void draw();    // 描画

    void hoseirect();

    bool animeUpdate();    // アニメーションのアップデート
};

//==============================================================================

// OBJ2DManagerクラス
class OBJ2DManager
{
protected:
    std::list<OBJ2D>  objList; // リスト（OBJ2Dの配列のようなもの）
public:
    // デストラクタ
    ~OBJ2DManager();

   virtual void init();    // 初期化
   virtual void update();  // 更新
   virtual void draw();    // 描画

    OBJ2D* add(MoveAlg* mvAlg, const VECTOR2& pos = VECTOR2(0, 0)); // objListに新たなOBJ2Dを追加する
    std::list<OBJ2D>* getList() { return &objList; }                // objListを取得する
};




//******************************************************************************

#endif// ! INCLUDED_OBJ2D