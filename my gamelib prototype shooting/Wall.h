
#ifndef INCLUDED_WALL
#define INCLUDED_WALL


//==============================================================================
//
//      移動アルゴリズム
//
//==============================================================================

#include "Obj2D.h"
#include "obj2d_data.h"

// プレイヤー操作クラス
class WallR : public MoveAlg
{
public:
    const float STAGE_HALFX = 700.0f;

    void move(OBJ2D* obj);

    static WallR* getInstance()
    {
        static WallR instance;
        return	&instance;
    }


private:
};

class WallL : public MoveAlg
{
public:
    const float STAGE_HALFX = 700.0f;

    void move(OBJ2D* obj);

    static WallL* getInstance()
    {
        static WallL instance;
        return	&instance;
    }


private:
};

// 移動アルゴリズムの実体
#define	pWallR	WallR::getInstance()
#define	pWallL	WallL::getInstance()
void playerJumpSet(OBJ2D*);


// 消去アルゴリズム
class EraseWall : public EraseAlg
{
    void erase(OBJ2D* obj);
};

// 消去アルゴリズムの実体
//EXTERN ErasePlayer      erasePlayer;


class WallManager : public OBJ2DManager
{
public:
    static constexpr float PLAYER_MOVE = 4.0f;
    enum IWORK {        // 当教材では15から逆に使っていく（0から順番に好きな用途で使ってよい）
        PAD_STATE = 13, // 13 キーの状態保持用
        PAD_TRG,        // 14 キーのトリガー保持用
        ATTACK_TIMER,   // 15 攻撃可能になるまでのタイマー
    };
public:
    //OBJ2DManagerクラスのものが呼び出されれば良いので、下記オーバーライドは不要である

    //void init();    // 初期化
    //void update();  // 更新
    //void draw();    // 描画
};

//------< ワーク用 >-------------------------------------------------------------

#endif // !INCLUDED_PLAYER