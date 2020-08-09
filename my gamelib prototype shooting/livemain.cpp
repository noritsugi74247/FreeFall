#include "mylib.h"
#include "scene.h"
#include "game.h"
#include "title.h"


int APIENTRY wWinMain(HINSTANCE, HINSTANCE, LPWSTR, int)// 使用しない変数は記述しない
{
    SceneManager sceneManager;
    sceneManager.execute(Game::instance());

    return 0;
}