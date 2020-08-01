#include "result.h"
#include"audio.h"
#include"mylib.h"
#include"audio.h"
#include"game.h"

//------< using >---------------------------------------------------------------
using namespace mylib;

//------< 変数 >----------------------------------------------------------------
Result Result::instance_;


//void Result_spr_road();


void Result::init() {
    Scene::init();


    tutorial = false;
    fade = 5.0;
    wide = 0;
    posX = 0;
    audio_init();
}

//--------------------------------
//  更新処理
//--------------------------------
void Result::update()
{
    using namespace input;

    switch (state)
    {
    case 0:
        //////// 初期設定 ////////
        timer = 0;                                  // タイマーを初期化
        mylib::setBlendMode(blender::BS_ALPHA);   // 通常のアルファ処理
                                     // 初期化処理の終了

       /* Result_spr_road();*/
        state++;

        /*  music::play(0, true);*/
          //Game::instance()->set_tutorial(1);

        break;
    case 1:
        fade -= 1.0f / (float)10.0;

        if (fade <= 0)
        {


            fade = 0;
            state++;

        }

        break;
    case 2:
        //////// 通常時の処理 ////////



        timer++;                            // タイマーを足す

        if (TRG(0) & PAD_TRG1)
        {
            // PAD_TRG1が押されたら
        /*    sound::play(3);*/

            state++;  // ゲームシーンに切り替え
        }
        break;
    case 3:
        fade += 1.0f / 10.0f;
        if (fade > 5.0)
        {
            changeScene(Game::instance());
            music::stop(0);
        }
        break;


    }

}

//--------------------------------
//  描画処理
//--------------------------------
void Result::draw()
{
    // 画面クリア
    mylib::clear(VECTOR4(0.2f, 0.3f, 1.0f, 1));

    if (fade > 0.0f)
    {
        primitive::rect(0, 0, system::SCREEN_WIDTH, system::SCREEN_HEIGHT,
            0, 0, 0, 0, 0, 0, fade);
    }


}

void Result::uninit()
{


    // テクスチャの解放
    /*audio_stop();*/
  /*  texture::releaseAll();*/

    // 音楽のクリア
    /*  music::clear()*/;
}