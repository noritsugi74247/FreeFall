#ifndef INCLUDED_PLAYER
#define INCLUDED_PLAYER

//******************************************************************************
//
//
//      Player.h
//
//
//******************************************************************************

//==============================================================================
//
//      移動アルゴリズム
//
//==============================================================================

#include "Obj2D.h"
#include "obj2d_data.h"
#include "Singleton.h"
// プレイヤー操作クラス
class Player : public MoveAlg
{
public:
    static constexpr float KASOKU = 0.5f;         // 横方向・縦方向の加速度
    static constexpr float SPEED_MAX_X = 4.0f;         // 横方向の最大速度
    static constexpr float SPEED_MAX_Y = 8.0f;         // 最大の落下速度
    static constexpr float SPEED_JUMP_Y = -6.0f;        // ジャンプ中の上昇スピード

    static constexpr float HASHIGO_KASOKU = 0.2f;       // はしご昇降中の加速度
    static constexpr float HASHIGO_SPEED_MAX_X = 4.0f;  // 横方向の最大速度
    static constexpr float HASHIGO_SPEED_MAX_Y = 2.0f;  // 最大の落下速度

    void move(OBJ2D* obj);

    static Player* getInstance()
    {
        static Player instance;
        return	&instance;
    }


private:

    //TODO_02
    void moveY(OBJ2D*);
    void moveX(OBJ2D*);
    void areaCheck(OBJ2D*);
    ;
};

// 移動アルゴリズムの実体
#define	pPlayer	Player::getInstance()
void playerJumpSet(OBJ2D*);


// 消去アルゴリズム
class ErasePlayer : public EraseAlg
{
    void erase(OBJ2D* obj);
};

// 消去アルゴリズムの実体
//EXTERN ErasePlayer      erasePlayer;


class PlayerManager : public OBJ2DManager
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
