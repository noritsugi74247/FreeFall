#pragma once
#include "Obj2D.h"
#include "obj2d_data.h"
class Effectparticle : public MoveAlg
{
public:
    const float STAGE_HALFX = 700.0f;

    void move(OBJ2D* obj);


    static Effectparticle* getInstance()
    {
        static Effectparticle instance;
        return	&instance;
    }


private:
};

#define	instanceEffect	Effectparticle::getInstance()
class EffectparticleManager : public OBJ2DManager
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
    void draw() override;    // 描画
};