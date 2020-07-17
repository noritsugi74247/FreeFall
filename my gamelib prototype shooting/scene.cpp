#include "scene.h"
#include "mylib.h"
Scene* Scene::execute()
{
    using namespace mylib;

    // 初期化処理
    init();

    // ゲームループ
    while (mylib::gameLoop(false))    // falseをtrueにするとタイトルバーにフレームレート表示
    {
        // 入力処理
        input::update();


        // 更新処理
        update();

        // 描画処理
        draw();

        // デバッグ文字列の描画
       /* debug::display(1, 0, 0, 1, 1);*/
        //debug::setString("GP2_03");

        // 画面フリップ
        mylib::present(1, 0);

        // 終了チェック
        if (nextScene) break;
    }

    // 終了処理
    uninit();

    return nextScene;	// 次のシーンを返す
}

//******************************************************************************
//
//      SceneManagerクラス
//
//******************************************************************************

//--------------------------------
//  実行
//--------------------------------
void SceneManager::execute(Scene* scene)
{
    using namespace mylib;

    bool isFullScreen = true;	// フルスクリーンにするならtrueに変える
                                //（Releaseモードのみ）

    // ゲームライブラリの初期化処理
    mylib::init(L"shooting",
        static_cast<int>(system::SCREEN_WIDTH),
        static_cast<int>(system::SCREEN_HEIGHT),
        isFullScreen);

    //#ifndef _DEBUG
    ShowCursor(1/*!isFullScreen*/);	// フルスクリーン時はカーソルを消す
//#endif

    // メインループ
    while (scene)
    {
        scene = scene->execute();
    }

    // ゲームライブラリの終了処理
    mylib::uninit();
}