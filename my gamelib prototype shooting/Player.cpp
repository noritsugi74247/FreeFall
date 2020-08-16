#include "Player.h"
#include "mylib.h"
#include "user.h"
#include "sprite_data.h"
#include "game.h"
using namespace mylib;
//******************************************************************************
//
//      プレイヤー移動処理
//
//******************************************************************************
extern bool damaged;

typedef struct _SCORE
{
    int little = 100;       // 弱ブースト時に入手するスコア
    int middle = 150;       // 中ブースト時に入手するスコア
    int strong = 200;       // 強ブースト時に入手するスコア
}SCORE;

typedef struct _AVOIDSPD_X
{
    float little = 8.0f;    // 弱ブースト時にX方向に加速する際の変化量
    float middle = 10.0f;   // 中ブースト時にX方向に加速する際の変化量
    float strong = 12.0f;   // 強ブースト時にX方向に加速する際の変化量
}AVOIDSPD_X;

typedef struct _AVOIDSPD_Y
{
    float little = 11.0f;  // 弱ブースト時にY方向に加速する際の変化量
    float middle = 13.0f;  // 中ブースト時にY方向に加速する際の変化量
    float strong = 16.0f;  // 強ブースト時にY方向に加速する際の変化量
}AVOIDSPD_Y;

SCORE score;
AVOIDSPD_X avoidX;
AVOIDSPD_Y avoidY;

const float stick = 0.125f;
//--------------------------------
//  Y方向移動
//--------------------------------
void Player::moveY(OBJ2D* obj)
{
    using namespace input;


    if (STATE(0) & PAD_UP || getPadState(0)->leftY < -stick)
    {
        obj->speed.y -= KASOKU;





    }

    else if (STATE(0) & PAD_DOWN || getPadState(0)->leftY > stick)
    { // 右だけが押されている場合
        obj->speed.y += KASOKU;

    }
    // 通常時
          // 速度に加速度を加える

  /*  else
    {


        obj->speed.y = 0.0f;
    }*/

    // y方向の抗力の計算
    //obj->speed.y += Game::instance()->bgManager()->calcResistance(obj, obj->speed.y);


    float maxSpeed = SPEED_MAX_Y;

   /* obj->speed.y = (std::max)(obj->speed.y, -maxSpeed);
    obj->speed.y = (std::min)(obj->speed.y, maxSpeed);*/
    if (obj->speed.y > maxSpeed)
    {
        obj->speed.y -= KASOKU;
        obj->speed.y = (std::min)(obj->speed.y,BOOST_MAX);
    }
    else
    {
      /*  for (int i = 0; i < 2; i++)
        {
            obj->flags &= ~static_cast<int>(1 << i);
        }*/
        obj->speed.y = clamp(obj->speed.y, -maxSpeed, maxSpeed);
    }

    // 位置更新
    float oldY = obj->position.y;           // 移動前の位置を保持
    obj->position.y += obj->speed.y;
    float deltaY = obj->position.y - oldY;  // 移動後の位置から移動前の位置を引く

    obj->onGround = false;





}

//--------------------------------
//  回避時横ブースト
//--------------------------------
void Player::avoidance(OBJ2D* obj)
{
    using namespace input;

    if (GetAsyncKeyState(VK_SPACE) & 1 && avoidance_timer == 0)
    {
        // まずブースト範囲内のフラグが立ってたら実行しなくていいのでリターンする
        //if (obj->flags & static_cast<int>(Player::BOOSTER::INSIDE_BOOST))
        //{
        //     return;
        //}
        // ブースト範囲内のフラグが立ってなかったら、
        // 残ったフラグに応じてブースト（加速）させる


        switch (obj->flags)
        {
            // 残ったフラグが...ならスピードを設定する。
            case static_cast<int>(Player::BOOSTER::LITTLE_BOOST) + static_cast<int>(Player::BOOSTER::INSIDE_BOOST) :
              avoidance_timer = 30;       //回避ブーストのタイマーを設定する(30フレーム)
              obj->scale.x > 0.0f ? obj->speed.x = avoidX.little : obj->speed.x = -avoidX.little;
              break;
            case static_cast<int>(Player::BOOSTER::MIDDLE_BOOST) + static_cast<int>(Player::BOOSTER::INSIDE_BOOST) :
              avoidance_timer = 30;       //回避ブーストのタイマーを設定する(30フレーム)
              obj->scale.x > 0.0f ? obj->speed.x = avoidX.middle : obj->speed.x = -avoidX.middle;
              break;
            case static_cast<int>(Player::BOOSTER::STRONG_BOOST) + static_cast<int>(Player::BOOSTER::INSIDE_BOOST) :
              avoidance_timer = 30;       //回避ブーストのタイマーを設定する(30フレーム)
              obj->scale.x > 0.0f ? obj->speed.x = avoidX.strong : obj->speed.x = -avoidX.strong;
              break;
            default:
              break;
        }
        // switch文が終わったら、プレイヤーのスピードは設定済みなので、全てのフラグを初期化しておく。
        // INSIDE_BOOSTが立ってなかったら、という条件式なので、
        // INSIDE_BOOSTまで消さなくてもいい(立ってないとわかっているフラグを消す必要がない)。
        // なので消すのはLITTLE_BOOST、MIDDLE_BOOST、STRONG_BOOSTの3つ。
        //if (!(obj->flags & static_cast<int>(Player::BOOSTER::INSIDE_BOOST)))
        //{
        //    for (int i = 0; i < 2; i++)
        //    {
        //        obj->flags &= ~static_cast<int>(1 << i);
        //    }
        //}
    }
    
}

//--------------------------------
//  X方向移動
//--------------------------------
void Player::moveX(OBJ2D* obj)
{
    using namespace input;

    /*---------------------------------左移動-----------------------------------*/

    if (STATE(0) & PAD_LEFT || getPadState(0)->leftX < -stick)
    {
        //avoidance(obj);

        obj->position.x -= obj->speed.x;

        obj->scale.x = 1.0f;            //プレイヤーの向き
    }

    /*--------------------------------------------------------------------------*/

    /*---------------------------------右移動-----------------------------------*/

    if (STATE(0) & PAD_RIGHT || getPadState(0)->leftX > stick)
    {
        //avoidance(obj);

        obj->position.x += obj->speed.x;

        obj->scale.x = -1.0f;            //プレイヤーの向き
    }

    /*--------------------------------------------------------------------------*/

    float maxSpeed = SPEED_MAX_X;

    /* obj->speed.y = (std::max)(obj->speed.y, -maxSpeed);
     obj->speed.y = (std::min)(obj->speed.y, maxSpeed);*/
    if (obj->speed.x > maxSpeed)
    {
        obj->speed.x -= KASOKU;
        obj->speed.x = (std::min)(obj->speed.x, BOOST_MAX);
    }
    else if (obj->speed.x < -maxSpeed)
    {
        obj->speed.x += KASOKU;
        obj->speed.x = (std::max)(obj->speed.x, -BOOST_MAX);
    }
    else
    {
        /*  for (int i = 0; i < 2; i++)
          {
              obj->flags &= ~static_cast<int>(1 << i);
          }*/
        obj->speed.x = clamp(obj->speed.x, -maxSpeed, maxSpeed);
    }

 //   // 左右入力の取り出し
 //
 //   if (STATE(0) & PAD_LEFT || getPadState(0)->leftX < -stick)
 //   {
 //       obj->speed.x -= KASOKU;
 //
 //
 //
 //
 //
 //   }
 //
 //   else if (STATE(0) & PAD_RIGHT || getPadState(0)->leftX > stick)
 //   { // 右だけが押されている場合
 //       obj->speed.x += KASOKU;
 //
 //   }
 //
 //   else
 //   {
 //
 //
 //       obj->speed.x = 0.0f;
 //   }
 //
 //   float maxSpeedx = SPEED_MAX_Y;
 //   // X方向移動
 //   obj->speed.x = (std::max)(obj->speed.x, -maxSpeedx);
 //   obj->speed.x = (std::min)(obj->speed.x, maxSpeedx);
 //   obj->speed.x = clamp(obj->speed.x, -maxSpeedx, maxSpeedx);
 //
 //
 //   float oldX = obj->position.x;
 //   obj->position.x += obj->speed.x;
 //   float deltaX = obj->position.x - oldX;
}

//--------------------------------
//  プレイヤージャンプ処理
//--------------------------------

//--------------------------------
//  プレイヤーエリア制限
//--------------------------------
void Player::areaCheck(OBJ2D* obj)
{
    float oldX = obj->position.x;   // x座標を一時的に保存
    obj->position.x = clamp(obj->position.x, obj->size.x, system::SCREEN_WIDTH - obj->size.x);
    if (oldX != obj->position.x)
    {
        obj->speed.x = 0;  // 保存した値と違えば画面端にあたっている
    }
    float oldY = obj->position.y;   // y座標を一時的に保存
    obj->position.y = clamp(obj->position.y, obj->size.y, Game::instance()->bgManager()->getScrollPos().y + system::SCREEN_HEIGHT);
    if (oldY != obj->position.y)    // 保存した値と違えば画面端にあたっている
    {
        obj->speed.y = 0;           // y方向の速度を0にする

    }
}

//--------------------------------
//  プレイヤーの加速処理
//--------------------------------
void Player::booster(OBJ2D* obj)
{
    // まずブースト範囲内のフラグが立ってたら実行しなくていいのでリターンする
    if (obj->flags & static_cast<int>(Player::BOOSTER::INSIDE_BOOST))
    {
        return;
    }
    // ブースト範囲内のフラグが立ってなかったら、
    // 残ったフラグに応じてブースト（加速）させる
    
    switch (obj->flags)
    {
        // 残ったフラグが...ならスピードを設定する。
        case static_cast<int>(Player::BOOSTER::LITTLE_BOOST) :
            obj->speed.y = avoidY.little;
            obj->score += score.little;
            Game::instance()->effectparticleManager()->add(instanceEffect,VECTOR2(obj->position.x,obj->position.y));
            playerfinisher++;
            break;
        case static_cast<int>(Player::BOOSTER::MIDDLE_BOOST) :
            obj->speed.y = avoidY.middle;
            obj->score += score.middle;
            Game::instance()->effectparticleManager()->add(instanceEffect, VECTOR2(obj->position.x, obj->position.y));
            playerfinisher++;
            break;
        case static_cast<int>(Player::BOOSTER::STRONG_BOOST) :
            obj->speed.y = avoidY.strong;
            obj->score += score.strong;
            Game::instance()->effectparticleManager()->add(instanceEffect, VECTOR2(obj->position.x, obj->position.y));
            playerfinisher++;
            break;
        default:
            break;
    }
    // switch文が終わったら、プレイヤーのスピードは設定済みなので、全てのフラグを初期化しておく。
    // INSIDE_BOOSTが立ってなかったら、という条件式なので、
    // INSIDE_BOOSTまで消さなくてもいい(立ってないとわかっているフラグを消す必要がない)。
    // なので消すのはLITTLE_BOOST、MIDDLE_BOOST、STRONG_BOOSTの3つ。
    if (!(obj->flags & static_cast<int>(Player::BOOSTER::INSIDE_BOOST)))
    {
        for (int i = 0; i < 2; i++)
        {
            obj->flags &= ~static_cast<int>(1 << i);
        }
    }
}

//--------------------------------
//  プレイヤーのはしご処理
//--------------------------------

void playerJumpSet(OBJ2D* obj)
{
    obj->speed.y = -15.0f;
    obj->jumpTimer = 16;
}

//--------------------------------
//  プレイヤー通常時の行動
//--------------------------------
void Player::move(OBJ2D* obj)
{
    using namespace input;  // 関数内で入力処理を行うときに記述する

    switch (obj->state)
    {
    case 0:
        //////// 初期設定 ////////

        // アニメの初期設定

        obj->data = &sprPlayer_idle0;
        obj->xFlip = true;                                 // サイズ設定（足元が中心であるため、幅はあたりとして使用する半分・縦はそのままが扱いやすい）
        obj->size = VECTOR2(48 / 2, 96 - 2);
        obj->scale.x = -1;
        obj->flags = 0;
        //obj->isDrawHitRect = true;
        obj->judgeFlag = true;
        obj->speed.x = SPEED_MAX_X;
        avoidance_timer = 0;
        playerfinisher = 0;
     
        obj->state++;

        break;

    case 1:
        //////// 通常時 ////////
        avoidance(obj);

        booster(obj);

        avoidance_timer--;
        if (avoidance_timer == 0)
        {
            obj->speed.x = SPEED_MAX_X;
        }

        if (avoidance_timer < 0)
        {
            avoidance_timer = 0;
        }
       
        // プレイヤー縦方向の移動処理

        moveY(obj);

        if (TRG(0) & PAD_TRG4 && playerfinisher == 3)         //  必殺技の処理
        {
            obj->speed.y = 20.0f;
            finisher_timer = 120;
            playerfinisher = 0;
        }

        if (finisher_timer > 0)
        {
            finisher_timer--;
            if (finisher_timer < 60)
            {
                obj->speed.y -= 0.5f;
            }

            if (obj->speed.y < 0)
            {
                obj->speed.y = 0;
            }
        }

        // プレイヤー横方向の移動処理
        moveX(obj);


        // プレイヤーのエリアチェック
        areaCheck(obj);

        break;
    }

    // アニメ更新
    switch (playerfinisher)
    {
    case 0:
        sprPlayer_idle0 = SPRITE_BOTTOM(TEXNO::PLAYER, 64 * 0, 64 * 0, 64, 64);
        obj->data = &sprPlayer_idle0;
        break;
    case 1:
        sprPlayer_idle0 = SPRITE_BOTTOM(TEXNO::PLAYER, 64 * 1, 64 * 0, 64, 64);
        obj->data = &sprPlayer_idle0;
        break;
    case 2 :
        sprPlayer_idle0 = SPRITE_BOTTOM(TEXNO::PLAYER, 64 * 2, 64 * 0, 64, 64);
        obj->data = &sprPlayer_idle0;
        break;
    case 3 :
        sprPlayer_idle0 = SPRITE_BOTTOM(TEXNO::PLAYER, 64 * 3, 64 * 0, 64, 64);
        obj->data = &sprPlayer_idle0;
        break;
    }
}

void PlayerManager::draw()
{
    for (auto& pl : objList)
    {
        if (pl.data)
        {
            pl.data->draw(pl.position - Game::instance()->bgManager()->getScrollPos(), pl.scale, pl.angle, pl.color);
        }
    }
}

//--------------------------------
//  消去
//--------------------------------
void ErasePlayer::erase(OBJ2D* obj)
{

    obj->clear();           // OBJ2Dを消去する
}