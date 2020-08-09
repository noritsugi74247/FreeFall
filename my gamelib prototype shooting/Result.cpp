#include "result.h"
#include"audio.h"
#include"mylib.h"
#include"sprite_data.h"
#include"audio.h"
#include"game.h"

//------< using >---------------------------------------------------------------
using namespace mylib;

void Result::init() {
    Scene::init();


    tutorial = false;
    fade = 5.0;
    wide = 0;
    posX = 0;
    rfadeManager_ = new RFadeManager;
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
    case RESULTSTATE::RESILT_LOAD:
        //////// 初期設定 ////////
        timer = 0;
        texture::load(loadTexture);
        // タイマーを初期化
        mylib::setBlendMode(blender::BS_ALPHA);   // 通常のアルファ処理
                                     // 初期化処理の終了
        rfadeManager()->init();
        rfadeManager()->add(pRFade, VECTOR2(system::SCREEN_WIDTH / 2, system::SCREEN_HEIGHT));
        /* Result_spr_road();*/
        state++;

        /*  music::play(0, true);*/
          //Game::instance()->set_tutorial(1);

        break;
    case RESULTSTATE::RESILT_FADEOUT:
        timer++;


        rfadeManager()->update();

        break;
    case RESULTSTATE::RESILT_UPDATE:
        //////// 通常時の処理 ////////



        timer++;                            // タイマーを足す

        if (TRG(0) & PAD_TRG1)
        {
            // PAD_TRG1が押されたら
        /*    sound::play(3);*/
            rfadeManager()->add(pRFade, VECTOR2(system::SCREEN_WIDTH / 2, system::SCREEN_HEIGHT));
            state++;  // ゲームシーンに切り替え
        }
        break;
    case RESULTSTATE::RESILT_FADEIN:



        rfadeManager()->update();
        break;
    case RESULTSTATE::CHANGE_GAME_SCENE:

        changeScene(Game::instance());

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



    rfadeManager()->draw();

    if (state == RESULTSTATE::RESILT_LOAD || state == RESULTSTATE::RESILT_FADEOUT && timer < 10
        || state == RESULTSTATE::CHANGE_GAME_SCENE)
    {
        primitive::rect(0, 0, system::SCREEN_WIDTH, system::SCREEN_HEIGHT,
            0, 0, 0, 0, 0, 0, 1);
    }


}

void Result::uninit()
{


    // テクスチャの解放
    /*audio_stop();*/
    texture::releaseAll();

    safe_delete(rfadeManager_);

    // 音楽のクリア
    /*  music::clear()*/;
}