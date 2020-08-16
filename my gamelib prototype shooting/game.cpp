//#include "all.h"
//
#include "sprite_data.h"
#include "mylib.h"
//#include "Player.h"
//#include "enemy.h"
#include "title.h"
#include"result.h"
#include "game.h"
#include "judge.h"
#include"imgui.h"



//------< using >---------------------------------------------------------------
using namespace mylib;

//------< 変数 >----------------------------------------------------------------
Game Game::instance_;

//void game_spr_road();

extern VECTOR2 mouse_pos;
//extern Block block;

//--------------------------------
//  初期化処理
//--------------------------------
void Game::init()
{

    Scene::init();	    // 基底クラスのinitを呼ぶ
    playerManager_ = new PlayerManager;
    blockManager_ = new BlockManager;
    wallManager_ = new WallManager;
    bgManager_ = std::make_unique<MapChip>();
    fadeManager_ = new FadeManager;
    effectparticleManager_ = new EffectparticleManager;
    OBJ2D::damage = 0;

    flg_count = 0;
    count = 0;
    fade = 5.0;
    wide = Title::instance()->wide;
    posX = Title::instance()->posX;
    
    description = false;   // ポーズフラグの初期化
    for (int i = 0; i < 10; i++) {
        stop_flg[i] = { false };
    }
}

//--------------------------------
//  更新処理
//--------------------------------
void Game::update()
{

    using namespace input;
    using std::chrono::system_clock;
    // ソフトリセット
    if ((STATE(0) & PAD_SELECT) &&  // 0コンのセレクトボタンが押されている状態で
        (TRG(0) & PAD_START))       // 0コンのスタートボタンが押された瞬間
    {
        changeScene(Title::instance());   // タイトルシーンに切り替える
        return;
    }

    // デバッグ文字列表示
  /*  debug::setString("state:%d", state);
    debug::setString("timer:%d", timer);*/

    // ポーズ処理
    //if (TRG(0) & PAD_START)
        //isPaused = !isPaused;       // 0コンのスタートボタンが押されたらポーズ状態が反転
    //if (description) return;           // この時点でポーズ中ならリターン

    switch (state)
    {
    case GAMESTATE::LOAD:
        //////// 初期設定 ////////
        //if (!tutorial) { state = 4; }
        //else { state++; }
       // テクスチャの読み込み
        texture::load(loadTexture);

        timer = 0;
        sprite_load(&sprTitle, L"./Data/Images/title.png", 1U);
        sprite_load(&BG, L"./Data/Images/bg.png", 1U);
        mylib::setBlendMode(blender::BS_ALPHA);   // 通常のアルファ処理


       /* music::play(1, true);*/

        

        state++;
        break;
    case GAMESTATE::INIT:

        playerManager()->init();

        playerManager()->add(pPlayer, VECTOR2(50, 640));
        
        // bgManager()->init("./Data/Maps/stage1.csv");
        
        blockManager()->init("./Data/Maps/test.csv");
        effectparticleManager()->init();

        wallManager()->init();

        wallManager()->add(pWallL, VECTOR2(-960 / 2, 1080));
        wallManager()->add(pWallR, VECTOR2(1920 + (960 / 2), 1080));

        fadeManager()->init();
        fadeManager()->add(pFade, VECTOR2(system::SCREEN_WIDTH / 2, system::SCREEN_HEIGHT));
        
        //ctime版。clock_t = int の代入はできるみたい。(そもそもclock_t が longのtypedefだし...)
        //start = end = time = 0;

        start = end = system_clock::now(); // 開始・終了共に現在の時間を取得し疑似的に初期化。
        elapsed_time = 0;    // elapsed_time は double型なのでintの 0 で初期化できる。
        state++;
        break;
    case GAMESTATE::TITLE_FADEOUT:

        timer++;


        
        playerManager()->update();
        fadeManager()->update();
        effectparticleManager()->update();
        break;

    case GAMESTATE::TITLE:
        playerManager()->update();
        effectparticleManager()->update();

        if (TRG(0) & PAD_TRG1
            ||
            TRG(0) & PAD_TRG2
            ||
            TRG(0) & PAD_TRG3
            ||
            TRG(0) & PAD_TRG4
            )
        {
            fadeManager()->add(pFade, VECTOR2(system::SCREEN_WIDTH / 2, system::SCREEN_HEIGHT));
            timer = 0;
            state++;
        }


        break;

    case GAMESTATE::TITLE_FADEIN:
        fadeManager()->update();


        break;

    case GAMESTATE::STAY:
        timer++;

        if (timer >= CHANGE_FRAME * 2)
        {

            state++;
            fadeManager()->add(pFade, VECTOR2(system::SCREEN_WIDTH / 2, system::SCREEN_HEIGHT));
            timer = 0;
        }

        break;



    case GAMESTATE::FADEOUT:
        timer++;
        fadeManager()->update();
        // ctime版。startが0以下なら現在時間を取得。
      /*  if(start <= 0 )
            start = clock();*/
        start = system_clock::now(); // 現在時間を取得してstartに代入する。
        break;

    case GAMESTATE::UPDATE:

        //　当たり判定の有効化
        judge();
       
        wallManager()->update();


        playerManager()->update();

        blockManager()->update();

        bgManager()->update();

        boostjudge();

        effectparticleManager()->update();

        if (OBJ2D::damage >= 5)
        {
            state++;
        }

        break;

    case GAMESTATE::FADEINSTAY:
       /* if (end <= 0)
        {
            // ctime版。endに現在時間を取得して代入。
            //timeにはendからstartを引いた時間(経過時間)を代入する。
            
            end = clock();
            time = end - start;

            // timeにはミリ秒(1/1000秒)単位で数値が入っているため、
            // CLOCKS_PER_SEC で秒単位に変換する。
            // (例えば、8.301秒が経過したときtimeには "8301"という数値が入っている)
            // (CLOCKS_PER_SEC は #defineで 1000 と定義されている)

            time /= CLOCKS_PER_SEC; 
        }*/
        if (elapsed_time <= 0) // elapsed_timeが一回も更新されてなかったら…
        {
            end = system_clock::now(); // 現在時間を取得してendに代入。
            // 経過時間をマイクロ秒に変換(直接1秒単位に変換してしまうと小数点以下が取れなくなるため)
            elapsed_time = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();

            elapsed_time /= MICRO_TO_SECOND; // マイクロ秒から1秒単位に変換する(100万で割ると1秒単位に変換できる)

            // これ以降、elapsed_timeは次のステージなどに移行するまで常に0以外の数値が入っているため更新されない
            // (最低でも180フレームはこのブロックにいるので、その間過剰に時間が計測されるのを防ぐため)
        }
        timer++;

        if (timer >= CHANGE_FRAME * 3)
        {

            state++;
            fadeManager()->add(pFade, VECTOR2(system::SCREEN_WIDTH / 2, system::SCREEN_HEIGHT));
        }
       

        break;


    case GAMESTATE::FADEIN:
        fadeManager()->update();


        break;

    case  GAMESTATE::CHANGE_SCENE:
        Scene::changeScene(Result::instance());

        break;


    }
}

//--------------------------------
//  描画処理
//--------------------------------
void Game::draw()
{
    // 画面クリア
    mylib::clear(VECTOR4(1, 0, 0, 1));
    // プレイヤーの描画
     // プレイヤーの描画
    if (state >= GAMESTATE::TITLE_FADEOUT)
    {
        sprite_render(BG, 0, 0);
    }

    if (state == GAMESTATE::TITLE_FADEOUT || state == GAMESTATE::TITLE)
    {
        sprite_render(sprTitle, 300, 300);

    }

    wallManager()->draw();

    bgManager()->drawchips();

    blockManager()->draw();

    effectparticleManager()->draw();

    playerManager()->draw();


    fadeManager()->draw();

    if (state == GAMESTATE::INIT || state == GAMESTATE::STAY || state == GAMESTATE::TITLE_FADEOUT && timer < 10
        || state == GAMESTATE::FADEOUT && timer < 10 || state == GAMESTATE::CHANGE_SCENE)
    {
        primitive::rect(0, 0, system::SCREEN_WIDTH, system::SCREEN_HEIGHT,
            0, 0, 0, 0, 0, 0, 1);
    }

    //邪魔だったんでコメント
    ImGui_ImplDX11_NewFrame();
    ImGui_ImplWin32_NewFrame();
    ImGui::NewFrame();


    ImGui::DragFloat("fade", &fade);
    if (state == GAMESTATE::UPDATE)
    {
        ImGui::DragFloat("player.speed.x", &(playerManager()->getList()->begin()->speed.x));
        ImGui::DragFloat("player.speed.y", &(playerManager()->getList()->begin()->speed.y));
        ImGui::DragInt("score", &(playerManager()->getList()->begin()->score));
    }
    ImGui::Render();
    ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

   
}

//--------------------------------
//  終了処理
//--------------------------------
void Game::uninit()
{
    // 各マネージャの解放
    safe_delete(playerManager_);
    safe_delete(blockManager_);
    safe_delete(fadeManager_);
    safe_delete(effectparticleManager_);
    // テクスチャの解放
    texture::releaseAll();
    // 音楽のクリア
    /*music::clear();*/
}

float Sine::easeIn(float t, float b, float c, float d) {
    // t: 時間(進行度)
    // b : 開始の値(開始時の座標やスケールなど)
    // c : 開始と終了の値の差分
    // d : Tween(トゥイーン)の合計時間
    return -c * cosf(t / d * (DirectX::XM_PI / 2)) + c + b;
}

float Sine::easeOut(float t, float b, float c, float d) {
    return c * sinf(t / d * (DirectX::XM_PI / 2)) + b;
}

float Sine::easeInOut(float t, float b, float c, float d) {
    return -c / 2 * (cosf(DirectX::XM_PI * t / d) - 1) + b;
}
